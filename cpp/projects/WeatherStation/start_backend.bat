@echo off
REM -------------------------------
REM 启动 Weather_Station 后端
REM -------------------------------

REM 切换到脚本所在目录（项目根目录）
cd /d %~dp0

REM 激活虚拟环境
call venv\Scripts\activate.bat

REM 启动 FastAPI 后端
uvicorn backend.app.main:app --reload
