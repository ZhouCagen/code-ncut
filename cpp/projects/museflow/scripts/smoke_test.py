#!/usr/bin/env python3
"""HTTP integration tests; runs a real backend against isolated XDG directories."""
from pathlib import Path
from urllib import request, error
import io
import json
import os
import socket
import subprocess
import tempfile
import time
import wave

ROOT = Path(__file__).resolve().parents[1]
BASE = 'http://127.0.0.1:18080'
OPENER = request.build_opener(request.ProxyHandler({}))

def call(path, method='GET', payload=None, headers=None, expected=200, raw=False):
    data = None if payload is None else json.dumps(payload).encode()
    req = request.Request(BASE + path, data=data, method=method, headers={'Content-Type': 'application/json', **(headers or {})})
    try: response = OPENER.open(req, timeout=10)
    except error.HTTPError as caught: response = caught
    with response:
        content = response.read()
        assert response.status == expected, (path, response.status, content[:300])
        return content if raw else json.loads(content)

def wait_ready(process):
    for _ in range(100):
        if process.poll() is not None: raise RuntimeError('后端提前退出')
        try:
            if call('/api/health')['status'] == 'ok': return
        except (OSError, error.URLError): pass
        time.sleep(.1)
    raise RuntimeError('后端启动超时')

def stop(process):
    if process.poll() is None:
        process.terminate()
        try: process.wait(5)
        except subprocess.TimeoutExpired:
            process.kill(); process.wait()

def main():
    with socket.socket() as probe:
        probe.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try: probe.bind(('127.0.0.1', 18080))
        except OSError: raise SystemExit('集成测试需要独占 18080 端口，请先停止开发服务器。')
    with tempfile.TemporaryDirectory(prefix='museflow-http-test-') as directory:
        env = dict(os.environ, XDG_DATA_HOME=directory + '/data', XDG_CONFIG_HOME=directory + '/config', XDG_CACHE_HOME=directory + '/cache')
        process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env)
        try:
            wait_ready(process)
            assert call('/api/health')['mode'] == 'mock'
            assert call('/api/settings')['capabilities']['strictDuration'] is True
            call('/api/health', headers={'Origin': 'https://evil.example'}, expected=403)
            call('/api/health', headers={'Host': 'evil.example:18080'}, expected=403)
            call('/api/projects/not-a-uuid', expected=400)
            call('/api/unknown', expected=404)
            blueprint = call('/api/blueprints/generate', 'POST', {'userInput': '雨后的森林，温暖的希望', 'purpose': 'game'})['blueprint']
            broken = json.loads(json.dumps(blueprint)); broken['structure'][1]['startSeconds'] = 10
            call('/api/blueprints/validate', 'POST', {'blueprint': broken}, expected=400)
            project = call('/api/projects', 'POST', {'name': '集成测试项目', 'blueprint': blueprint}, expected=201)
            pid = project['id']
            task = call('/api/generations', 'POST', {'projectId': pid, 'blueprint': blueprint, 'mode': 'preview'}, expected=202)
            tid = task['id']
            call(f'/api/generations/{tid}/audio', expected=409)
            call(f'/api/projects/{pid}', 'DELETE', expected=400)
            duplicate = subprocess.run([str(ROOT / 'build/museflow')], env=env, capture_output=True, timeout=5)
            assert duplicate.returncode != 0, 'Duplicate instance must not open the live database'
            assert call(f'/api/generations/{tid}')['status'] != 'failed', 'Duplicate startup corrupted an active task'
            states = {'queued'}
            for _ in range(100):
                task = call(f'/api/generations/{tid}')
                states.add(task['status'])
                start = time.monotonic(); call('/api/health')
                assert time.monotonic() - start < 1, 'HTTP was blocked by generation'
                if task['status'] in ('completed', 'failed'): break
                time.sleep(.08)
            assert task['status'] == 'completed', task
            assert {'preparing', 'generating', 'post_processing', 'completed'} <= states, states
            content = call(f'/api/generations/{tid}/audio', raw=True)
            with wave.open(io.BytesIO(content)) as audio:
                assert audio.getnframes() / audio.getframerate() == 8
            assert call(f'/api/generations/{tid}/audio', headers={'Range': 'bytes=0-43'}, expected=206, raw=True) == content[:44]
            assert call(f'/api/generations/{tid}/audio?download=1', raw=True) == content
            version = call(f'/api/projects/{pid}/versions')[0]
            assert version['blueprint']['bpm'] == 88
            blueprint['bpm'] = 112
            call(f'/api/projects/{pid}', 'PUT', {'name': '修改后的项目', 'blueprint': blueprint})
            assert call(f'/api/projects/{pid}/versions')[0]['blueprint']['bpm'] == 88
            stop(process)
            process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env)
            wait_ready(process)
            assert call(f'/api/projects/{pid}')['blueprint']['bpm'] == 112
            assert call(f'/api/generations/{tid}')['status'] == 'completed'
            assert len(call('/api/versions')) == 1
            assert call(f'/api/generations/{tid}/audio', raw=True) == content
            html = call('/studio', raw=True)
            assert b'<html' in html and b'MuseFlow' in html, 'Production SPA was not served'
            call(f'/api/projects/{pid}', 'DELETE')
            assert not call('/api/projects') and not call('/api/versions')
            call(f'/api/generations/{tid}/audio', expected=404)
            assert not (Path(directory) / 'data/museflow/projects' / pid).exists()
            print('PASS: HTTP workflow, all progress states, nonblocking health, Range/audio/download, validation, local-origin guards, single-instance lock, immutable snapshots, restart, SPA serving, cascade deletion')
        finally: stop(process)

if __name__ == '__main__': main()
