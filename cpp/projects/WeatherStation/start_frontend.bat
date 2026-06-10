@echo off
REM -------------------------------
REM 启动 Weather_Station 前端
REM -------------------------------

REM 切换到脚本所在目录（项目根目录）
cd /d %~dp0

REM 启动前端 HTTP 服务器
cd frontend\web
python -m http.server 5500

REM 打开浏览器访问前端
start http://127.0.0.1:5500
