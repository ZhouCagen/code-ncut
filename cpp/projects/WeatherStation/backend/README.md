# Weather Station Backend

这是天气站项目的后端部分，基于 **Python + FastAPI**，负责接收传感器数据、存储数据库、并提供 API 给前端访问。

---

## 目录结构

```text
backend/
├─ README.md
├─ requirements.txt
├─ .gitignore
│
├─ app/
│ ├─ __init__.py
│ ├─ main.py
│ ├─ models.py
│ ├─ schemas.py
│ ├─ crud.py
│ ├─ database.py
│ └─ fake_data.py
│
├─ tests/
│ ├─ __init__.py
│ └─ test_api.py
│
└─ scripts/
  └─ populate_fake_data.py

```

---

## 环境依赖

建议使用 Python 3.10+。推荐在虚拟环境中安装和管理依赖。

1. 创建并激活虚拟环境
```Bash

# 创建虚拟环境
python -m venv venv

# 激活虚拟环境 (Windows)
venv\Scripts\activate

# 激活虚拟环境 (Linux / Mac)
source venv/bin/activate

```

2. 安装依赖
确保虚拟环境已激活，然后运行以下命令安装所有必需的库：

```Bash

pip install -r requirements.txt

```

requirements.txt 
    文件示例: fastapi
             uvicorn[standard]
             sqlalchemy
             pydantic
             databases[sqlite]

---

## 启动服务
在激活虚拟环境后，执行以下命令来启动开发服务器：

```Bash
# 激活虚拟环境
# Windows
venv\Scripts\activate

# Linux / Mac
source venv/bin/activate

# 启动 FastAPI 服务
uvicorn app.main:app --reload

```

服务启动后，可以通过浏览器访问以下地址：
API 主页: http://127.0.0.1:8000
API 交互文档 (Swagger UI): http://127.0.0.1:8000/docs
备用 API 文档 (ReDoc): http://127.0.0.1:8000/redoc

---

## 数据库
配置文件: app/database.py

包含数据库连接 URL 等相关设置。

---

## 数据模型
硬件未到时，可使用以下脚本生成测试数据：

app/fake_data.py

scripts/populate_fake_data.py

---

## 测试
```Bash
pytest tests/test_api.py
```

