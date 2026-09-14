#!/usr/bin/env python3
"""Version lineage, immutable snapshots, deletion, export and restart on an isolated backend."""
import copy
import json
import os
from pathlib import Path
import subprocess
import tempfile
import time
from smoke_test import ROOT, call, wait_ready, stop

def main():
    with tempfile.TemporaryDirectory(prefix='museflow-versions-') as directory:
        env = dict(os.environ, XDG_DATA_HOME=directory + '/data', XDG_CONFIG_HOME=directory + '/config', XDG_CACHE_HOME=directory + '/cache')
        for name in ('DEEPSEEK_API_KEY', 'GEMINI_API_KEY', 'MUSEFLOW_GOOGLE_TEST_BASE'): env.pop(name, None)
        process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env)
        try:
            wait_ready(process)
            b = call('/api/blueprints/generate', 'POST', {'userInput': '分支测试森林', 'purpose': 'game'})['blueprint']
            p = call('/api/projects', 'POST', {'name': '原项目', 'blueprint': b}, expected=201)
            other = call('/api/projects', 'POST', {'name': '另一个项目', 'blueprint': b}, expected=201)
            def generate(blueprint, parent=''):
                return call('/api/generations', 'POST', {'projectId': p['id'], 'blueprint': blueprint, 'mode': 'preview', 'parentVersionId': parent}, expected=202)
            def wait(task):
                for _ in range(160):
                    result = call('/api/generations/' + task['id'])
                    if result['status'] in ('completed', 'failed'): break
                    time.sleep(.05)
                assert result['status'] == 'completed', result
                return call('/api/versions/' + result['versionId'])
            root = wait(generate(b)); root_audio = call('/api/generations/' + root['generationId'] + '/audio', raw=True)
            draft = call('/api/versions/' + root['id'] + '/branch', 'POST', {}, expected=201)['project']
            assert draft['draftParentVersionId'] == root['id'] and draft['blueprint']['id'] != b['id']
            assert len(call('/api/versions')) == 1, 'Branch draft cannot invent a completed audio version'
            b2 = copy.deepcopy(draft['blueprint']); b2['bpm'] = 112
            call('/api/projects/' + p['id'], 'PUT', {'name': '改名项目', 'blueprint': b2, 'draftParentVersionId': root['id']})
            task = generate(b2, root['id'])
            assert call('/api/activity')['musicTasks'] == 1
            call('/api/versions/' + root['id'], 'DELETE', expected=400)
            call('/api/versions/' + root['id'] + '/branch', 'POST', {}, expected=400)
            child = wait(task)
            assert child['parentVersionId'] == root['id'] and child['blueprint']['bpm'] == 112
            assert call('/api/versions/' + root['id'])['blueprint']['bpm'] == 88
            assert call('/api/generations/' + root['generationId'] + '/audio', raw=True) == root_audio
            call('/api/generations', 'POST', {'projectId': other['id'], 'blueprint': b, 'mode': 'full', 'parentVersionId': root['id']}, expected=400)
            call('/api/projects/' + other['id'], 'PUT', {'name': 'invalid parent', 'blueprint': b, 'draftParentVersionId': root['id']}, expected=400)
            call('/api/versions/' + root['id'], 'DELETE', expected=400)
            grandchild = wait(generate(b2, child['id']))
            sibling = wait(generate(b, root['id']))
            assert len(call('/api/projects/' + p['id'] + '/versions')) == 4
            rated = call('/api/versions/' + child['id'] + '/metadata', 'PUT', {'rating': 5, 'notes': '更适合结尾，保留。'})
            assert rated['rating'] == 5 and rated['notes'] and rated['parentVersionId'] == root['id']
            for patch in ({'rating': 6}, {'rating': 1.5}, {'rating': 4294967296}, {'notes': 42}, {'notes': 'a' * 6001}, {'blueprint': b}, {'apiKey': 'fixture-metadata-key'}):
                call('/api/versions/' + child['id'] + '/metadata', 'PUT', patch, expected=400)
            call('/api/settings', 'PUT', {'text': {'apiKey': 'fixture-export-secret'}, 'music': {'apiKey': 'fixture-export-google'}})
            export = call('/api/projects/' + p['id'] + '/export')
            assert export['format'] == 'museflow-project' and not export['includesAudio'] and len(export['versions']) == 4
            assert 'fixture-' not in json.dumps(export) and 'apiKey' not in json.dumps(export)
            stop(process); process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env); wait_ready(process)
            assert call('/api/versions/' + child['id'])['rating'] == 5
            assert call('/api/projects/' + p['id'])['draftParentVersionId'] == root['id']
            assert call('/api/versions/' + grandchild['id'])['parentVersionId'] == child['id']
            for version in (grandchild, sibling, child, root):
                call('/api/versions/' + version['id'], 'DELETE')
                call('/api/versions/' + version['id'], expected=404)
                call('/api/generations/' + version['generationId'] + '/audio', expected=404)
                assert call('/api/generations/' + version['generationId'])['audioDeleted']
                for suffix in ('.wav', '.mp3', '.source.wav', '.source.mp3'):
                    assert not (Path(directory) / 'data/museflow/projects' / p['id'] / (version['generationId'] + suffix)).exists()
            assert call('/api/projects/' + p['id'])['draftParentVersionId'] is None
            assert not call('/api/versions') and call('/api/activity')['musicTasks'] == 0
            assert len(call('/api/projects/' + p['id'])['tasks']) == 4
            print('PASS: root/child/grandchild/sibling lineage, persistent drafts, immutable audio, cross-project guard, active/parent deletion guards, rating validation, export secrecy, restart, leaf deletion and history')
        finally: stop(process)
if __name__ == '__main__': main()
