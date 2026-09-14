#!/usr/bin/env bash
set -euo pipefail
museflow_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
if [[ ! -x "$museflow_root/build/museflow-desktop" || ! -x "$museflow_root/build/museflow" || ! -f "$museflow_root/frontend/dist/index.html" ]]; then
  echo '请先运行 scripts/build.sh 构建桌面应用。' >&2
  exit 1
fi
exec "$museflow_root/build/museflow-desktop" --backend "$museflow_root/build/museflow" --assets "$museflow_root/assets" --web "$museflow_root/frontend/dist" "$@"
