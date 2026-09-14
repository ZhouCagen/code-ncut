#!/usr/bin/env python3
"""Install only this user's launcher and icon; keep all project data unchanged."""
import os
from pathlib import Path
import shlex
import shutil
import subprocess
ROOT = Path(__file__).resolve().parents[1]
if not (ROOT / 'build/museflow-desktop').is_file() or not (ROOT / 'frontend/dist/index.html').is_file():
    raise SystemExit('请先运行 scripts/build.sh')
home = Path.home()
xdg = Path(os.environ.get('XDG_DATA_HOME', str(home / '.local/share')))
if not xdg.is_absolute(): xdg = home / '.local/share'
bin_dir = home / '.local/bin'
applications = xdg / 'applications'
icons = xdg / 'icons/hicolor/scalable/apps'
for directory in (bin_dir, applications, icons): directory.mkdir(parents=True, exist_ok=True)
launcher = bin_dir / 'museflow'
marker = '# MuseFlow managed launcher'
if launcher.exists() and marker not in launcher.read_text(): raise SystemExit('已有同名启动程序，已保留；可直接使用项目 scripts/run.sh')
entry = applications / 'museflow.desktop'
if entry.exists() and 'X-MuseFlow-Managed=true' not in entry.read_text(): raise SystemExit('已有同名桌面入口，已保留')
icon = icons / 'museflow.svg'
if icon.exists() and not entry.exists() and icon.read_bytes() != (ROOT / 'desktop/resources/museflow.svg').read_bytes(): raise SystemExit('已有同名图标，已保留')
launcher.write_text('#!/usr/bin/env bash\n' + marker + '\nexec ' + shlex.quote(str(ROOT / 'scripts/run.sh')) + ' "$@"\n')
launcher.chmod(0o755)
# Desktop Entry has both string escaping and Exec argument escaping.
def argument(value):
    escaped = ''.join('\\' + c if c in '\\"`$' else c for c in value)
    return '"' + escaped.replace('\\', '\\\\').replace('%', '%%') + '"'
entry.write_text('[Desktop Entry]\nType=Application\nName=MuseFlow\nName[zh_CN]=MuseFlow 音乐创作\nComment=本地 AI 场景配乐工作台\nExec=' + argument(str(launcher)) + '\nIcon=museflow\nTerminal=false\nCategories=AudioVideo;Audio;\nStartupNotify=true\nStartupWMClass=museflow-desktop\nX-MuseFlow-Managed=true\n')
shutil.copyfile(ROOT / 'desktop/resources/museflow.svg', icons / 'museflow.svg')
if shutil.which('update-desktop-database'): subprocess.run(['update-desktop-database', str(applications)], check=False)
print('已安装应用菜单入口：' + str(entry))
print('启动命令：' + str(launcher))
