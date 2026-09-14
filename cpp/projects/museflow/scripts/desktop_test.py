#!/usr/bin/env python3
"""Exercise the real desktop executable in an offscreen display, with isolated user directories."""
import os
from pathlib import Path
import socket
import subprocess
import tempfile
import time
from urllib import request
from smoke_test import ROOT, call, stop, wait_ready

def main():
    with tempfile.TemporaryDirectory(prefix='museflow-desktop-test-') as directory:
        runtime = Path(directory) / 'runtime'; runtime.mkdir(mode=0o700)
        alsa = Path(directory) / 'alsa.conf'; alsa.write_text('pcm.!default { type null }\n')
        env = dict(os.environ, XDG_DATA_HOME=directory + '/data', XDG_CONFIG_HOME=directory + '/config', XDG_CACHE_HOME=directory + '/cache', XDG_RUNTIME_DIR=str(runtime), QT_QPA_PLATFORM='offscreen', QTWEBENGINE_REMOTE_DEBUGGING='127.0.0.1:18083', QTWEBENGINE_CHROMIUM_FLAGS='--disable-gpu', ALSA_CONFIG_PATH=str(alsa), NO_PROXY='127.0.0.1,localhost', no_proxy='127.0.0.1,localhost')
        env['DEEPSEEK_API_KEY'] = 'fixture-desktop-environment'
        env['ELEVENLABS_API_KEY'] = 'fixture-elevenlabs-desktop-environment'
        env.pop('GEMINI_API_KEY', None); env.pop('MUSEFLOW_GOOGLE_TEST_BASE', None)
        for port in (18080, 18083):
            with socket.socket() as probe:
                probe.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1); probe.bind(('127.0.0.1', port))
        log = Path(directory) / 'desktop.log'
        with log.open('wb') as output:
            process = subprocess.Popen([str(ROOT / 'scripts/run.sh')], env=env, stdout=output, stderr=subprocess.STDOUT)
            try:
                wait_ready(process)
                assert call('/api/health')['desktopSession'], 'Desktop must own its backend'
                assert call('/api/settings')['text']['keySource'] == 'environment', 'Backend lost environment key'
                assert call('/api/settings')['music']['elevenlabsKeySource'] == 'environment', 'Backend lost ElevenLabs environment key'
                opener = request.build_opener(request.ProxyHandler({}))
                for _ in range(100):
                    if process.poll() is not None: raise RuntimeError('Desktop exited early')
                    try:
                        with opener.open('http://127.0.0.1:18083/json/version', timeout=1): break
                    except OSError: time.sleep(.1)
                else: raise RuntimeError('Qt debug endpoint unavailable')
                duplicate = subprocess.run([str(ROOT / 'scripts/run.sh')], env=env, stdout=output, stderr=subprocess.STDOUT, timeout=8)
                assert duplicate.returncode == 0 and process.poll() is None, 'Single instance activation failed'
                (ROOT / 'frontend/test-results').mkdir(exist_ok=True)
                subprocess.run(['node', 'tests/desktop.mjs'], cwd=ROOT / 'frontend', env=env, check=True, timeout=90)
                assert process.poll() is None, 'Disconnecting developer tools must keep the app open'
                stop(process)
                for _ in range(50):
                    try: call('/api/health')
                    except OSError: break
                    time.sleep(.1)
                else: raise AssertionError('Desktop left an orphan backend')
                assert b'fixture-desktop-environment' not in log.read_bytes(), 'Key leaked to desktop log'
                # Restart keeps versions while clearing renderer profile state.
                process = subprocess.Popen([str(ROOT / 'scripts/run.sh')], env=env, stdout=output, stderr=subprocess.STDOUT)
                wait_ready(process); assert len(call('/api/versions')) == 2
                print('PASS: desktop single instance, environment key handoff, backend ownership, shutdown cleanup and restart persistence')
            except Exception:
                print(log.read_text()[-4000:]); raise
            finally: stop(process)
if __name__ == '__main__': main()
