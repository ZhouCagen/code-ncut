#!/usr/bin/env python3
"""Exercise the actual C++ ElevenLabs provider and audio pipeline against localhost."""
import copy
import json
import os
from pathlib import Path
import subprocess
import tempfile
import time
from elevenlabs_fixture import ElevenLabsFixture
from smoke_test import ROOT, call, wait_ready, stop
from provider_test import text_job

def main():
    with tempfile.TemporaryDirectory(prefix='museflow-elevenlabs-test-') as directory, ElevenLabsFixture() as fixture:
        env = {k:v for k,v in os.environ.items() if k not in ('DEEPSEEK_API_KEY', 'GEMINI_API_KEY', 'SONGAPI_KEY', 'ELEVENLABS_API_KEY') and k.lower() not in ('http_proxy','https_proxy','all_proxy','no_proxy')}
        env.update(XDG_DATA_HOME=directory+'/data', XDG_CONFIG_HOME=directory+'/config', XDG_CACHE_HOME=directory+'/cache', NO_PROXY='127.0.0.1', MUSEFLOW_ELEVENLABS_TEST_BASE=fixture.endpoint)
        process = subprocess.Popen([str(ROOT/'build/museflow')], env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        try:
            wait_ready(process)
            def settings(**patch): return call('/api/settings', 'PUT', {'music': patch})['music']
            settings(provider='lyria', apiKey='fixture-google-only')
            settings(provider='songapi', apiKey='fixture-songapi-only')
            assert not settings(provider='elevenlabs')['hasApiKey']
            prefs = settings(apiKey=fixture.expected_key, outputFormat='wav')
            assert prefs['keySource'] == 'session' and prefs['activeModel'] == 'music_v1'
            assert text_job('/api/providers/music/test', {})['ok']
            assert len(fixture.requests) == 1 and fixture.requests[0][0] == '/v1/music/plan'
            assert call('/api/settings')['music']['status'] == 'connected'
            fixture.scenario = 'escaped-key'
            text_job('/api/providers/music/test', {}, 'unsafe_response')
            fixture.scenario = 'success'
            blueprint = call('/api/blueprints/generate', 'POST', {'userInput':'温柔钢琴音乐', 'purpose':'game'})['blueprint']
            project = call('/api/projects', 'POST', {'name':'ElevenLabs test', 'blueprint':blueprint}, expected=201)
            def generate(mode='full', strict=False, vocal=False, target=5.123, failure=None, long_prompt=False):
                b = copy.deepcopy(blueprint)
                b.update(targetDurationSeconds=target, strictDuration=strict, vocalMode='vocal' if vocal else 'instrumental', lyrics='今晚星光温柔' if vocal else '')
                if long_prompt: b['sceneDescription'] = 'a' * 4200
                b['structure'] = [dict(b['structure'][0], startSeconds=0, endSeconds=target)]
                started = time.monotonic()
                task = call('/api/generations', 'POST', {'projectId':project['id'], 'blueprint':b, 'mode':mode}, expected=202)
                assert time.monotonic() - started < 1
                for _ in range(400):
                    task = call('/api/generations/'+task['id'])
                    call('/api/health')
                    if task['status'] in ('completed','failed'): break
                    time.sleep(.03)
                assert fixture.expected_key not in json.dumps(task)
                if failure:
                    assert task['status'] == 'failed' and failure in task['error'], task
                    call('/api/generations/'+task['id']+'/audio', expected=409)
                else:
                    assert task['status'] == 'completed', task
                    assert task['provider'] == 'elevenlabs' and task['model'] == 'music_v1'
                return task
            first = generate(strict=True)
            assert abs(first['actualDurationSeconds'] - 5.123) <= 1/44100 and first['format'] == 'wav' and call('/api/versions')[-1]['sourceFormat'] == 'mp3'
            assert fixture.requests[-1][1]['music_length_ms'] == 5123 and fixture.requests[-1][1]['force_instrumental'] is True
            settings(outputFormat='mp3')
            preview = generate(mode='preview', vocal=True)
            assert preview['format'] == 'mp3' and preview['modelTargetDurationSeconds'] == 30
            sent = fixture.requests[-1][1]
            assert sent['music_length_ms'] == 30000 and sent['force_instrumental'] is False and '今晚星光温柔' in sent['prompt'] and '[0s–30s]' in sent['prompt']
            audio = call('/api/generations/'+preview['id']+'/audio', raw=True)
            assert call('/api/generations/'+preview['id']+'/audio', headers={'Range':'bytes=0-9'}, expected=206, raw=True) == audio[:10]
            count = len(call('/api/versions'))
            for scenario, message in [('paid-plan','付费套餐'), ('permissions','Music 权限'), ('http-401','API Key'), ('http-402','额度'), ('http-403','权限'), ('http-429','额度'), ('http-503','不可用'), ('redirect','重定向'), ('wrong-mime','响应类型'), ('invalid-audio','MP3'), ('echo-key','敏感凭据')]:
                fixture.scenario = scenario
                before = len(fixture.requests)
                generate(failure=message)
                assert len(fixture.requests) == before + 1, 'Never retry a paid request or follow redirects'
                assert len(call('/api/versions')) == count
            fixture.scenario = 'success'
            before = len(fixture.requests); generate(long_prompt=True, failure='4100')
            assert len(fixture.requests) == before
            settings(clearApiKey=True)
            call('/api/generations', 'POST', {'projectId':project['id'], 'blueprint':blueprint, 'mode':'full'}, expected=400)
            assert settings(provider='songapi')['hasApiKey'] and settings(provider='lyria')['hasApiKey']
            assert not settings(provider='elevenlabs')['hasApiKey']
            settings(apiKey=fixture.expected_key)
            stop(process)
            assert fixture.expected_key.encode() not in process.stdout.read(); process.stdout.close()
            for file in Path(directory).rglob('*'):
                if file.is_file(): assert fixture.expected_key.encode() not in file.read_bytes(), file
            process = subprocess.Popen([str(ROOT/'build/museflow')], env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            wait_ready(process)
            assert not call('/api/settings')['music']['hasApiKey']
            assert call('/api/generations/'+preview['id']+'/audio', raw=True) == audio
            stop(process); process.stdout.close()
            env['ELEVENLABS_API_KEY'] = fixture.expected_key
            process = subprocess.Popen([str(ROOT/'build/museflow')], env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            wait_ready(process)
            assert call('/api/settings')['music']['keySource'] == 'environment'
            assert not settings(clearApiKey=True)['hasApiKey']
            assert settings(useEnvironmentKey=True)['keySource'] == 'environment'
            assert text_job('/api/providers/music/test', {})['ok']
            print('PASS: ElevenLabs auth/plan/generation, preview/vocals/ms duration, MP3/WAV/Range, errors/no retries, prompt limits, independent keys, no key persistence, restart and environment')
        finally:
            stop(process)
            if process.stdout: process.stdout.close()
if __name__ == '__main__': main()
