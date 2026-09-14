#!/usr/bin/env bash
set -euo pipefail
museflow_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$museflow_root"
ctest --test-dir build --output-on-failure
npm --prefix frontend test
python3 scripts/smoke_test.py
python3 scripts/provider_test.py
python3 scripts/music_test.py
python3 scripts/elevenlabs_test.py
python3 scripts/versions_test.py
if [[ "${1:-}" == '--e2e' ]]; then
  cd frontend
  npm run test:e2e
fi
