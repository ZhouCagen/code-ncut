#!/usr/bin/env bash
set -euo pipefail
museflow_root="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
for dependency in cmake c++ npm python3 curl setsid; do
  command -v "$dependency" >/dev/null || { echo "缺少依赖：$dependency，请查看 README。" >&2; exit 1; }
done
python3 - <<'PY'
import socket
for port in (18080, 5173):
    with socket.socket() as sock:
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try: sock.bind(('127.0.0.1', port))
        except OSError: raise SystemExit(f'端口 {port} 已被占用，请先关闭占用该端口的程序。')
PY
cmake -S "$museflow_root/backend" -B "$museflow_root/build" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$museflow_root/build" --parallel "${MUSEFLOW_BUILD_JOBS:-2}"
cd "$museflow_root/frontend"
if [[ ! -d node_modules ]]; then npm ci --no-audit --no-fund; fi
museflow_backend_pid=''
museflow_frontend_pid=''
cleanup() {
  trap - EXIT INT TERM
  if [[ -n "$museflow_frontend_pid" ]]; then kill -- "-$museflow_frontend_pid" 2>/dev/null || true; fi
  if [[ -n "$museflow_backend_pid" ]]; then kill "$museflow_backend_pid" 2>/dev/null || true; fi
  wait 2>/dev/null || true
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM
"$museflow_root/build/museflow" &
museflow_backend_pid=$!
museflow_ready=0
for ((attempt=0; attempt<60; attempt++)); do
  kill -0 "$museflow_backend_pid" 2>/dev/null || { echo '后端启动失败。' >&2; exit 1; }
  if curl --noproxy '*' --fail --silent http://127.0.0.1:18080/api/health >/dev/null; then museflow_ready=1; break; fi
  sleep 0.1
done
[[ "$museflow_ready" == 1 ]] || { echo '等待后端启动超时。' >&2; exit 1; }
setsid npm run dev &
museflow_frontend_pid=$!
echo 'MuseFlow 开发模式：http://127.0.0.1:5173（Ctrl+C 同时关闭前后端）'
wait -n "$museflow_backend_pid" "$museflow_frontend_pid"
