#!/usr/bin/env bash
set -euo pipefail
museflow_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$museflow_root"
if [[ "${1:-}" != "--native" ]]; then python3 scripts/desktop_test.py; fi
./build/museflow_desktop_tests
