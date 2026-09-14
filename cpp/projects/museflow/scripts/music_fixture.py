"""Official Interactions REST shape, served locally with synthetic audio and dummy credentials."""
import base64
import copy
import json
import threading
import time
import subprocess
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

class MusicFixture:
    expected_key = 'fixture-session-google'
    def __init__(self, port=0):
        self.scenario = 'success'; self.requests = []; self.polls = 0
        self.wav = (Path(__file__).resolve().parents[1] / 'assets/mock-demo.wav').read_bytes()
        self.mp3 = subprocess.run(['ffmpeg', '-nostdin', '-v', 'error', '-f', 'wav', '-i', 'pipe:0', '-f', 'mp3', 'pipe:1'], input=self.wav, capture_output=True, check=True).stdout
        owner = self
        class Handler(BaseHTTPRequestHandler):
            def log_message(self, *_): pass
            def respond(self, status, payload):
                content = json.dumps(payload).encode()
                self.send_response(status); self.send_header('Content-Type', 'application/json'); self.send_header('Content-Length', str(len(content))); self.end_headers()
                try: self.wfile.write(content)
                except (BrokenPipeError, ConnectionResetError): pass
            def authorized(self):
                assert owner.expected_key not in self.path and not self.headers.get('Authorization')
                if self.headers.get('x-goog-api-key') != owner.expected_key:
                    self.respond(401, {'error': {'message': 'invalid dummy key'}}); return False
                return True
            def do_GET(self):
                if not self.authorized(): return
                if self.path.startswith('/v1beta/models/'):
                    self.respond(200, {'name': self.path.removeprefix('/v1beta/')}); return
                if self.path.startswith('/v1beta/interactions/'):
                    owner.polls += 1; self.respond(200, owner.output()); return
                self.respond(404, {})
            def do_POST(self):
                if not self.authorized(): return
                body = json.loads(self.rfile.read(int(self.headers['Content-Length'])))
                owner.requests.append(copy.deepcopy(body))
                assert self.path == '/v1beta/interactions', self.path
                assert set(body) <= {'model', 'input', 'response_format'}
                assert body['model'] in ('lyria-3-clip-preview', 'lyria-3.5') and isinstance(body['input'], str)
                if 'response_format' in body:
                    assert body['model'] == 'lyria-3.5' and body['response_format'] == {'type': 'audio'}
                time.sleep(.2)
                scenario = owner.scenario
                if scenario.startswith('http-'):
                    self.respond(int(scenario[5:]), {'error': {'message': owner.expected_key}}); return
                if scenario == 'async':
                    self.respond(200, {'id': 'v1_fixture-task', 'status': 'in_progress', 'steps': []}); return
                if scenario == 'unsafe-id':
                    self.respond(200, {'id': '../../secret', 'status': 'queued'}); return
                self.respond(200, owner.output())
        self.server = ThreadingHTTPServer(('127.0.0.1', port), Handler)
        self.endpoint = f'http://127.0.0.1:{self.server.server_port}/v1beta'
        self.thread = threading.Thread(target=self.server.serve_forever, daemon=True)
    def output(self):
        content = [{'type': 'text', 'text': '温暖的回家旋律'}, {'type': 'text', 'text': json.dumps({'sections': [{'name': 'intro', 'start': 0, 'end': 8}]})}, {'type': 'audio', 'mime_type': 'audio/wav', 'data': base64.b64encode(self.wav).decode()}]
        scenario = self.scenario
        if scenario == 'mp3-source': content[-1].update(mime_type='audio/mpeg', data=base64.b64encode(self.mp3).decode())
        if scenario == 'invalid-base64': content[-1]['data'] = '***='
        if scenario == 'invalid-padding': content[-1]['data'] = 'AB=='
        if scenario == 'invalid-wav': content[-1]['data'] = base64.b64encode(b'not-a-wav-file').decode()
        if scenario == 'unsupported-mime': content[-1]['mime_type'] = 'text/html'
        if scenario == 'no-audio': content = content[:-1]
        if scenario == 'echo-key': content[0]['text'] = self.expected_key
        if scenario == 'escaped-key': content[0]['text'] = json.dumps({'key': ''.join('\\u%04x' % ord(c) for c in self.expected_key)}).replace('\\\\u', '\\u')
        return {'id': 'v1_fixture-task', 'status': 'failed' if scenario == 'refused' else 'completed', 'steps': [{'type': 'user_input', 'content': []}, {'type': 'model_output', 'content': content}]}
    def __enter__(self): self.thread.start(); return self
    def __exit__(self, *_): self.server.shutdown(); self.server.server_close(); self.thread.join()
