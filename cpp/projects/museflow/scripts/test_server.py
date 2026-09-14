#!/usr/bin/env python3
"""Isolated backend lifecycle for browser tests; never touches user projects."""
from pathlib import Path
import os
import signal
import subprocess
import tempfile
from provider_fixture import TextFixture
from music_fixture import MusicFixture
from elevenlabs_fixture import ElevenLabsFixture

ROOT = Path(__file__).resolve().parents[1]
def shutdown(*_):
    raise SystemExit(0)

signal.signal(signal.SIGTERM, shutdown)
with tempfile.TemporaryDirectory(prefix='museflow-browser-test-') as directory, TextFixture(18081), MusicFixture(18082) as music, ElevenLabsFixture(18083) as elevenlabs:
    env = dict(os.environ, XDG_DATA_HOME=directory + '/data', XDG_CONFIG_HOME=directory + '/config', XDG_CACHE_HOME=directory + '/cache')
    env.pop('DEEPSEEK_API_KEY', None); env.pop('GEMINI_API_KEY', None)
    env.pop('ELEVENLABS_API_KEY', None); env.pop('SONGAPI_KEY', None)
    env['MUSEFLOW_ELEVENLABS_TEST_BASE'] = elevenlabs.endpoint
    env['MUSEFLOW_GOOGLE_TEST_BASE'] = music.endpoint
    env['NO_PROXY'] = '127.0.0.1'; env['no_proxy'] = '127.0.0.1'
    process = subprocess.Popen([str(ROOT / 'build/museflow')], env=env)
    try: process.wait()
    finally:
        process.terminate()
        try: process.wait(5)
        except subprocess.TimeoutExpired: process.kill(); process.wait()
