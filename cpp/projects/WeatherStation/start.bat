@echo off
REM -------------------------------
REM 一键启动 Weather_Station 项目
REM -------------------------------

REM 切换到脚本所在目录（项目根目录）
cd /d %~dp0

REM 激活虚拟环境
call venv\Scripts\activate.bat

REM 启动后端 FastAPI（新窗口）
start "" cmd /k "cd backend && uvicorn app.main:app --reload"

REM 启动前端 HTTP 服务器（新窗口）
start "" cmd /k "cd frontend\web && python -m http.server 5500"

REM 等几秒再打开浏览器，保证前端服务器已经启动
timeout /t 3

REM 自动打开浏览器访问前端
start http://127.0.0.1:5500
