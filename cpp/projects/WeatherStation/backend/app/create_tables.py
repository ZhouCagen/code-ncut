# 数据库初始化脚本
# 运行一次即可创建数据库表：
# cd backend
# python -m app.create_tables
# 生成 weather.db 及 sensor_data 表。


from app.database import engine, metadata
# engine
# 来自 database.py 的同步 Engine
# 用于 create_all() 实际创建数据库表

# metadata
# 挂载了所有表
# metadata.create_all(engine) → 把 metadata 里所有表在数据库中生成

from app.models import sensor_data
# sensor_data
# 导入是为了确保表已经定义，否则 metadata 不会包含表

metadata.create_all(engine)
