#!/usr/bin/env python3
"""Exercises the real C++ libcurl provider against a local contract server, with dummy keys only."""
from pathlib import Path
import json
import os
import socket
import subprocess
import tempfile
import time
from provider_fixture import TextFixture
from smoke_test import ROOT, call, wait_ready, stop

def text_job(path, payload, expected_code=None):
    started = time.monotonic()
    accepted = call(path, 'POST', payload, expected=202)
    assert time.monotonic() - started < 1, 'HTTP handler waited on upstream model'
    job_id = accepted['job']['id']
    for _ in range(200):
        job = call('/api/text-jobs/' + job_id)
        started = time.monotonic(); call('/api/health')
        assert time.monotonic() - started < 1, 'Health blocked by model request'
        if job['status'] in ('completed', 'failed'): break
        time.sleep(.04)
    assert 'fixture-session-text' not in json.dumps(job), 'Key leaked in public job'
    if expected_code:
        assert job['status'] == 'failed' and job['code'] == expected_code, job
        return job
    assert job['status'] == 'completed', job
    return job['result']

def main():
    with socket.socket() as probe:
        probe.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        probe.bind(('127.0.0.1', 18080))
    with tempfile.TemporaryDirectory(prefix='museflow-provider-test-') as directory, TextFixture() as fixture:
        env = {k: v for k, v in os.environ.items() if k not in ('DEEPSEEK_API_KEY', 'GEMINI_API_KEY') and k.lower() not in ('http_proxy', 'https_proxy', 'all_proxy', 'no_proxy')}
        env.update(XDG_DATA_HOME=directory + '/data', XDG_CONFIG_HOME=directory + '/config', XDG_CACHE_HOME=directory + '/cache', NO_PROXY='127.0.0.1')
        process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        try:
            wait_ready(process)
            assert call('/api/health')['phase'] == 4
            initial = call('/api/settings')
            assert initial['text']['hasApiKey'] is False
            call('/api/settings', 'PUT', {'text': {'baseUrl': 'https://invalid.example?key=secret'}}, expected=400)
            call('/api/settings', 'PUT', {'text': {'baseUrl': 'file:///etc/passwd'}}, expected=400)
            call('/api/settings', 'PUT', {'text': {'apiKey': 'header\ninjection'}}, expected=400)
            call('/api/settings', 'PUT', {'text': {'apiKey': 'secret'}}, headers={'Origin': 'https://evil.example'}, expected=403)
            prefs = call('/api/settings', 'PUT', {'text': {'provider': 'deepseek', 'baseUrl': fixture.endpoint + '/v1', 'model': 'custom-model', 'apiKey': fixture.expected_key}, 'music': {'apiKey': 'fixture-music-key', 'model': 'lyria-3.5', 'outputFormat': 'mp3'}})
            assert prefs['text']['keySource'] == 'session' and 'apiKey' not in prefs['text']
            assert prefs['music']['provider'] == 'mock' and prefs['music']['activeOutputFormat'] == 'mp3'
            assert 'fixture-' not in json.dumps(prefs)
            assert text_job('/api/providers/text/test', {})['ok'] is True
            assert call('/api/settings')['text']['status'] == 'connected'
            generated = text_job('/api/blueprints/generate', {'userInput': '希望的森林', 'purpose': 'game'})['blueprint']
            assert generated['promptSource'] == 'deepseek' and generated['bpm'] == 96
            assert fixture.paths[-1] == '/v1/chat/completions'
            saved = call('/api/projects', 'POST', {'name': 'AI 草稿', 'blueprint': generated}, expected=201)
            assert saved['blueprint']['finalLyriaPrompt'] == generated['finalLyriaPrompt'], 'Saved AI prompt overwritten'
            revised = text_job('/api/blueprints/revise', {'blueprint': generated, 'instruction': '加入中文人声，BPM 改为112'})['blueprint']
            assert revised['vocalMode'] == 'vocal' and revised['lyrics'] and revised['bpm'] == 112
            assert revised['id'] != generated['id']
            generated['bpm'] = 110
            validated = call('/api/blueprints/validate', 'POST', {'blueprint': generated})['blueprint']
            assert validated['promptSource'] == 'local' and '110 BPM' in validated['finalLyriaPrompt']
            cases = [('retry-once', None, 2), ('blank-once', None, 2), ('invalid-schema', 'invalid_blueprint', 2), ('overlap', 'invalid_blueprint', 2), ('unknown-property', 'invalid_blueprint', 2), ('http-401', 'unauthorized', 1), ('http-402', 'insufficient_balance', 1), ('http-403', 'forbidden', 1), ('http-429', 'rate_limited', 1), ('http-503', 'upstream_error', 1), ('redirect', 'redirect', 1), ('refusal', 'refused', 1), ('truncated', 'truncated', 1), ('invalid-envelope', 'invalid_response', 1), ('missing-choices', 'invalid_response', 1), ('oversized', 'response_too_large', 1), ('echo-key', 'unsafe_response', 1)]
            for model, code, count in cases:
                call('/api/settings', 'PUT', {'text': {'model': model}})
                text_job('/api/blueprints/generate', {'userInput': '测试场景', 'purpose': 'game'}, code)
                assert fixture.counts[model] == count, (model, fixture.counts)
            for model, code in [('http-422', 'bad_request'), ('echo-escaped-key', 'unsafe_response'), ('too-deep', 'invalid_response')]:
                call('/api/settings', 'PUT', {'text': {'model': model}})
                text_job('/api/blueprints/generate', {'userInput': '测试场景', 'purpose': 'game'}, code)
            call('/api/settings', 'PUT', {'text': {'model': 'timeout', 'connectTimeoutSeconds': 1, 'requestTimeoutSeconds': 2}})
            text_job('/api/blueprints/generate', {'userInput': '测试场景', 'purpose': 'game'}, 'timeout')
            call('/api/settings', 'PUT', {'text': {'clearApiKey': True}})
            assert call('/api/settings')['text']['hasApiKey'] is False
            text_job('/api/providers/text/test', {}, 'missing_key')
            call('/api/settings', 'PUT', {'text': {'model': 'custom-model', 'apiKey': fixture.expected_key}})
            stop(process)
            assert b'fixture-' not in process.stdout.read()
            process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            wait_ready(process)
            after = call('/api/settings')
            assert after['text']['model'] == 'custom-model' and not after['text']['hasApiKey'] and not after['music']['hasApiKey']
            assert call('/api/projects')[0]['blueprint']['promptSource'] == 'deepseek'
            for path in Path(directory).rglob('*'):
                if path.is_file(): assert b'fixture-' not in path.read_bytes(), f'Key leaked to {path.name}'
            stop(process)
            env.update(DEEPSEEK_API_KEY=fixture.expected_key, GEMINI_API_KEY='fixture-environment-music', HTTP_PROXY=fixture.endpoint, NO_PROXY='')
            process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            wait_ready(process)
            assert call('/api/settings')['text']['keySource'] == 'environment'
            assert call('/api/settings')['music']['keySource'] == 'environment'
            # Unreachable target succeeds only if uppercase HTTP_PROXY is honored by the C++ transport.
            call('/api/settings', 'PUT', {'text': {'baseUrl': 'http://127.0.0.1:1/v1', 'model': 'proxy-test'}})
            assert text_job('/api/providers/text/test', {})['ok']
            assert fixture.paths[-1] == 'http://127.0.0.1:1/v1/chat/completions'
            print('PASS: real libcurl provider contract, async responsiveness, create/revise/lyrics, schema + bounded retries, 401/402/403/429/503/timeout/refusal/redirect/oversize errors, key redaction and restart, environment keys, HTTP_PROXY')
        finally:
            stop(process)
            if process.stdout: process.stdout.close()

if __name__ == '__main__': main()
