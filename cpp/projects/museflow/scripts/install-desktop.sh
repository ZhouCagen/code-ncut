#!/usr/bin/env bash
set -euo pipefail
museflow_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
python3 "$museflow_root/scripts/install_desktop.py"
