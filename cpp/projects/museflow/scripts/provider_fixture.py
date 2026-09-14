"""Local Chat Completions contract fixture. No real model or external network access."""
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import threading
import time

def blueprint():
    return {
        'id': 'model-draft', 'title': '契约测试配乐', 'sceneDescription': '旅人走进雨后的森林', 'purpose': 'game',
        'genre': '电影配乐', 'subGenre': 'Ambient', 'mood': ['希望'], 'bpm': 96, 'key': 'D minor',
        'instruments': ['钢琴', '弦乐'], 'vocalMode': 'instrumental', 'vocalGender': '', 'language': '中文', 'lyrics': '',
        'targetDurationSeconds': 60, 'strictDuration': False,
        'structure': [{'id': 'opening', 'name': '序章', 'startSeconds': 0, 'endSeconds': 30, 'emotion': '静谧', 'intensity': 3, 'instruments': ['钢琴'], 'description': '钢琴渐入'},
                      {'id': 'ending', 'name': '尾声', 'startSeconds': 30, 'endSeconds': 60, 'emotion': '希望', 'intensity': 6, 'instruments': ['弦乐'], 'description': '弦乐展开并收尾'}],
        'negativePrompt': '刺耳高频', 'finalLyriaPrompt': 'A hopeful cinematic forest cue with piano and strings, 96 BPM in D minor. Target 60 seconds: sparse piano at 0–30s, hopeful strings at 30–60s. Instrumental; avoid harsh high frequencies.'
    }

class TextFixture:
    def __init__(self, port=0):
        fixture = self
        self.counts = {}
        self.paths = []
        self.expected_key = 'fixture-session-text'
        self.lock = threading.Lock()
        class Handler(BaseHTTPRequestHandler):
            def log_message(self, *_): pass
            def respond(self, status, data, headers=None):
                content = data if isinstance(data, bytes) else json.dumps(data).encode()
                try:
                    self.send_response(status)
                    self.send_header('Content-Type', 'application/json')
                    self.send_header('Content-Length', str(len(content)))
                    for name, value in (headers or {}).items(): self.send_header(name, value)
                    self.end_headers(); self.wfile.write(content)
                except (BrokenPipeError, ConnectionResetError): pass
            def do_POST(self):
                payload = json.loads(self.rfile.read(int(self.headers['Content-Length'])))
                model = payload['model']
                with fixture.lock:
                    fixture.counts[model] = fixture.counts.get(model, 0) + 1
                    count = fixture.counts[model]
                    fixture.paths.append(self.path)
                if self.headers.get('Authorization') != 'Bearer ' + fixture.expected_key:
                    self.respond(401, {'error': 'bad fixture key'}); return
                if payload.get('response_format') != {'type': 'json_object'} or payload.get('stream') is not False:
                    self.respond(400, {'error': 'request contract mismatch'}); return
                if model.startswith('http-'):
                    self.respond(int(model[5:]), {'error': fixture.expected_key}); return
                if model == 'redirect':
                    self.respond(302, {}, {'Location': fixture.endpoint + '/redirect-target'}); return
                if model == 'invalid-envelope': self.respond(200, b'not-json'); return
                if model == 'missing-choices': self.respond(200, {}); return
                if model == 'too-deep': self.respond(200, b'[' * 70 + b'0' + b']' * 70); return
                if model == 'oversized': self.respond(200, b'x' * (2 * 1024 * 1024 + 10)); return
                if model in ('slow', 'timeout'): time.sleep(3)
                messages = payload['messages']
                test = 'Connection test.' in messages[-1]['content']
                data = {'ok': True} if test else blueprint()
                if not test and '请修改当前设计稿' in messages[1]['content']:
                    data['bpm'] = 112; data['vocalMode'] = 'vocal'; data['vocalGender'] = '轻柔女声'; data['lyrics'] = '雨后的光，带我回家'
                    data['finalLyriaPrompt'] = 'Revised cinematic song with soft female vocals in Chinese at 112 BPM. Lyrics: 雨后的光，带我回家'
                if model in ('invalid-schema', 'retry-once') and (model == 'invalid-schema' or count == 1): data['bpm'] = 'fast'
                if model == 'overlap': data['structure'][1]['startSeconds'] = 20
                if model == 'unknown-property': data['notAllowed'] = 'value'
                if model in ('echo-key', 'echo-escaped-key'): data['sceneDescription'] = fixture.expected_key
                content = json.dumps(data, ensure_ascii=False)
                if model == 'echo-escaped-key': content = content.replace(fixture.expected_key, '\\u0066' + fixture.expected_key[1:])
                if model == 'blank-once' and count == 1: content = ''
                reason = 'content_filter' if model == 'refusal' else 'length' if model == 'truncated' else 'stop'
                self.respond(200, {'choices': [{'finish_reason': reason, 'message': {'role': 'assistant', 'content': content}}]})
        self.server = ThreadingHTTPServer(('127.0.0.1', port), Handler)
        self.endpoint = f'http://127.0.0.1:{self.server.server_port}'
        self.thread = threading.Thread(target=self.server.serve_forever, daemon=True)
    def __enter__(self): self.thread.start(); return self
    def __exit__(self, *_): self.server.shutdown(); self.server.server_close(); self.thread.join()
