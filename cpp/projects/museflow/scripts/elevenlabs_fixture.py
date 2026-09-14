"""Local ElevenLabs contract fixture. Never contacts the real provider."""
import json
import subprocess
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

class ElevenLabsFixture:
    expected_key = 'fixture-elevenlabs-secret'
    def __init__(self, port=0):
        self.requests = []
        self.scenario = 'success'
        self.mp3 = subprocess.run(['ffmpeg', '-nostdin', '-v', 'error', '-i', str(Path(__file__).resolve().parents[1] / 'assets/mock-demo.wav'), '-f', 'mp3', 'pipe:1'], capture_output=True, check=True).stdout
        owner = self
        class Handler(BaseHTTPRequestHandler):
            def log_message(self, *_): pass
            def respond(self, status, body, mime='application/json'):
                if not isinstance(body, bytes): body = json.dumps(body).encode()
                self.send_response(status)
                self.send_header('Content-Type', mime)
                self.send_header('Content-Length', str(len(body)))
                self.end_headers()
                self.wfile.write(body)
            def do_POST(self):
                body = json.loads(self.rfile.read(int(self.headers['Content-Length'])))
                owner.requests.append((self.path, body))
                assert not self.headers.get('Authorization') and not self.headers.get('x-goog-api-key')
                assert owner.expected_key not in self.path and owner.expected_key not in json.dumps(body)
                if self.headers.get('xi-api-key') != owner.expected_key:
                    self.respond(401, {'detail': {'status': 'invalid_api_key'}}); return
                scenario = owner.scenario
                if scenario.startswith('http-'):
                    self.respond(int(scenario[5:]), {'detail': {'message': owner.expected_key}}); return
                if scenario == 'paid-plan':
                    self.respond(402, {'detail': {'code': 'paid_plan_required', 'message': owner.expected_key}}); return
                if scenario == 'permissions':
                    self.respond(401, {'detail': {'status': 'missing_permissions', 'message': owner.expected_key}}); return
                if scenario == 'redirect':
                    self.send_response(307); self.send_header('Location', '/must-not-follow'); self.send_header('Content-Length', '0'); self.end_headers(); return
                if self.path == '/v1/music/plan':
                    assert body == {'prompt': 'A gentle solo piano instrumental.', 'music_length_ms': 3000, 'model_id': 'music_v1'}
                    if scenario == 'escaped-key':
                        escaped = ''.join('\\u%04x' % ord(c) for c in owner.expected_key)
                        self.respond(200, ('{"sections":[{"lines":["'+escaped+'"]}]}').encode()); return
                    self.respond(200, {'sections': [{'section_name': 'Intro', 'duration_ms': 3000, 'lines': []}]}); return
                assert self.path == '/v1/music?output_format=mp3_44100_128'
                assert set(body) == {'prompt', 'music_length_ms', 'model_id', 'force_instrumental'}
                assert body['model_id'] == 'music_v1' and isinstance(body['force_instrumental'], bool)
                assert 3000 <= body['music_length_ms'] <= 600000 and len(body['prompt']) <= 4100
                if scenario == 'wrong-mime': self.respond(200, {}, 'text/html'); return
                if scenario == 'invalid-audio': self.respond(200, b'not an mp3', 'audio/mpeg'); return
                if scenario == 'echo-key': self.respond(200, owner.expected_key.encode(), 'audio/mpeg'); return
                self.respond(200, owner.mp3, 'audio/mpeg')
        self.server = ThreadingHTTPServer(('127.0.0.1', port), Handler)
        self.endpoint = f'http://127.0.0.1:{self.server.server_port}'
        self.thread = threading.Thread(target=self.server.serve_forever, daemon=True)
    def __enter__(self): self.thread.start(); return self
    def __exit__(self, *_): self.server.shutdown(); self.server.server_close(); self.thread.join()
