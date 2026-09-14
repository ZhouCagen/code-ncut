#!/usr/bin/env bash
set -euo pipefail
museflow_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
cmake -S "$museflow_root/backend" -B "$museflow_root/build" -DCMAKE_BUILD_TYPE=Release -DMUSEFLOW_BUILD_DESKTOP=ON
cmake --build "$museflow_root/build" --parallel "${MUSEFLOW_BUILD_JOBS:-2}"
cd "$museflow_root/frontend"
npm ci --no-audit --no-fund
npm run build
echo '构建完成。运行 scripts/run.sh 启动桌面应用。'
