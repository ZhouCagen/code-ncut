#!/usr/bin/env python3
"""C++ Lyria + FFmpeg integration: local fixture only, never a paid API request."""
import io
import json
import os
from pathlib import Path
import subprocess
import tempfile
import time
import wave
from music_fixture import MusicFixture
from smoke_test import ROOT, call, wait_ready, stop
from provider_test import text_job

def duration_and_samples(data):
    with wave.open(io.BytesIO(data)) as audio:
        return audio.getnframes() / audio.getframerate(), audio.readframes(audio.getnframes())

def main():
    with tempfile.TemporaryDirectory(prefix='museflow-music-test-') as directory, MusicFixture() as fixture:
        env = {k: v for k, v in os.environ.items() if k not in ('DEEPSEEK_API_KEY', 'GEMINI_API_KEY') and k.lower() not in ('http_proxy', 'https_proxy', 'all_proxy', 'no_proxy')}
        env.update(XDG_DATA_HOME=directory + '/data', XDG_CONFIG_HOME=directory + '/config', XDG_CACHE_HOME=directory + '/cache', NO_PROXY='127.0.0.1', MUSEFLOW_GOOGLE_TEST_BASE=fixture.endpoint)
        process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        try:
            wait_ready(process)
            call('/api/settings', 'PUT', {'music': {'provider': 'lyria', 'apiKey': fixture.expected_key, 'outputFormat': 'wav'}})
            assert text_job('/api/providers/music/test', {})['ok']
            assert not fixture.requests, 'Key test must not generate music'
            assert call('/api/settings')['music']['status'] == 'connected'
            blueprint = call('/api/blueprints/generate', 'POST', {'userInput': '音乐测试', 'purpose': 'game'})['blueprint']
            project = call('/api/projects', 'POST', {'name': 'Lyria 契约测试', 'blueprint': blueprint}, expected=201)
            def generate(mode='full', target=12, strict=False, failure=None):
                b = json.loads(json.dumps(blueprint)); b['targetDurationSeconds'] = target; b['strictDuration'] = strict
                b['structure'] = [dict(b['structure'][0], startSeconds=0, endSeconds=target)]
                started = time.monotonic()
                task = call('/api/generations', 'POST', {'projectId': project['id'], 'blueprint': b, 'mode': mode}, expected=202)
                assert time.monotonic() - started < 1
                for _ in range(400):
                    task = call('/api/generations/' + task['id'])
                    started = time.monotonic(); call('/api/health'); assert time.monotonic() - started < 1
                    if task['status'] in ('completed', 'failed'): break
                    time.sleep(.04)
                assert fixture.expected_key not in json.dumps(task)
                if failure:
                    assert task['status'] == 'failed' and failure in task['error'], task
                    call('/api/generations/' + task['id'] + '/audio', expected=409)
                else: assert task['status'] == 'completed', task
                return task
            first = generate()
            assert first['sourceDurationSeconds'] == 8 and first['actualDurationSeconds'] == 8 and first['provider'] == 'lyria'
            assert first['songText'][0] == '温暖的回家旋律' and first['songStructure'][0]['sections'][0]['name'] == 'intro'
            assert fixture.requests[-1]['model'] == 'lyria-3.5' and fixture.requests[-1]['response_format'] == {'type': 'audio'}
            for target in (5, 12, 5.123):
                task = generate(target=target, strict=True)
                duration, samples = duration_and_samples(call('/api/generations/' + task['id'] + '/audio', raw=True))
                assert abs(duration - target) <= 1 / 44100
                assert task['strictDurationApplied'] and task['sourceDurationSeconds'] == 8
                if target == 12: assert samples[-44100:] == bytes(44100), 'Padded ending must be silent'
                if target == 5:
                    import array
                    values = array.array('h', samples[-400:]); assert max(map(abs, values)) < 100, 'Trimmed ending must fade'
            call('/api/settings', 'PUT', {'music': {'outputFormat': 'mp3'}})
            mp3 = generate('preview')
            assert mp3['format'] == 'mp3' and mp3['modelTargetDurationSeconds'] == 30
            assert fixture.requests[-1]['model'] == 'lyria-3-clip-preview' and 'response_format' not in fixture.requests[-1]
            assert '30' in fixture.requests[-1]['input']
            data = call('/api/generations/' + mp3['id'] + '/audio', raw=True)
            assert data[:3] == b'ID3' or data[:1] == b'\xff'
            assert call('/api/generations/' + mp3['id'] + '/audio', headers={'Range': 'bytes=0-9'}, expected=206, raw=True) == data[:10]
            exact = generate('preview', strict=True)
            assert exact['format'] == 'wav' and exact['actualDurationSeconds'] == 30, 'Strict MP3 preference must select exact WAV'
            fixture.scenario = 'mp3-source'; source_mp3 = generate(target=5, strict=True)
            assert source_mp3['actualDurationSeconds'] == 5 and source_mp3['sourceDurationSeconds'] > 8
            fixture.scenario = 'async'; generate(); assert fixture.polls
            count = len(call('/api/versions'))
            for scenario, message in [('invalid-base64', 'Base64'), ('invalid-padding', 'Base64'), ('invalid-wav', 'FFmpeg'), ('unsupported-mime', 'MIME'), ('no-audio', '未返回音频'), ('refused', '未完成'), ('unsafe-id', '任务 ID'), ('echo-key', '敏感凭据'), ('escaped-key', '敏感凭据'), ('http-401', 'API Key'), ('http-403', '权限'), ('http-429', '额度'), ('http-503', '不可用')]:
                fixture.scenario = scenario; before = len(fixture.requests); generate(failure=message)
                assert len(fixture.requests) == before + 1, 'Paid generation must never automatically retry'
                assert len(call('/api/versions')) == count, 'Failed generation must not save a version'
            fixture.scenario = 'success'
            call('/api/settings', 'PUT', {'music': {'clearApiKey': True}})
            call('/api/generations', 'POST', {'projectId': project['id'], 'blueprint': blueprint, 'mode': 'full'}, expected=400)
            call('/api/settings', 'PUT', {'music': {'provider': 'mock', 'outputFormat': 'wav'}})
            mock = generate(target=5, strict=True)
            assert mock['provider'] == 'mock' and mock['actualDurationSeconds'] == 5
            stop(process); assert fixture.expected_key.encode() not in process.stdout.read(); process.stdout.close()
            for file in Path(directory).rglob('*'):
                if file.is_file(): assert fixture.expected_key.encode() not in file.read_bytes(), file
            process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            wait_ready(process)
            assert not call('/api/settings')['music']['hasApiKey']
            assert call('/api/generations/' + first['id'])['songStructure']
            assert call('/api/generations/' + mp3['id'] + '/audio', raw=True) == data
            print('PASS: Lyria Interactions POST/GET and key metadata test, MP3/WAV, lyrics/structure/Base64, strict trim/pad/fade/sample precision, preview timeline, async health, failure/no-paid-retry, key secrecy, restart')
        finally:
            stop(process)
            if process.stdout: process.stdout.close()
if __name__ == '__main__': main()
