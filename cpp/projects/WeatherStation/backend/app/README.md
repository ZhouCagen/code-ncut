# App 包

这是 Weather Station 项目的后端核心包，包含数据库连接、表模型、Pydantic 模型、CRUD 函数以及 FastAPI 服务入口。

目录说明：

- `database.py` → 数据库连接与 MetaData 实例
- `models.py` → 数据表定义
- `schemas.py` → 请求/响应数据模型
- `crud.py` → 数据库操作函数
- `main.py` → FastAPI 服务入口
- `create_tables.py` → 初始化数据库表
