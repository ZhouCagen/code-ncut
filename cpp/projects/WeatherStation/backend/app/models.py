# Models 模块

# 定义数据库表结构，例如：
# `sensor_data` 表：
#   - `id` → 主键
#   - `temperature` → 温度
#   - `humidity` → 湿度
#   - `pressure` → 气压
#   - `created_at` → 创建时间，带时区，默认数据库当前时间

from sqlalchemy import Table, Column, Integer, Float, String, DateTime
# Table Column 
# 用法: sensor_data = Table( 
#           "sensor_data",
#           metadata,
#           Column("temperature", Float, nullable=False),
#           ...
#       )
# 使用场景：models.py 定义表结构

# Integer Float String
# 作用: 定义表字段的数据类型
# 使用场景: models.py 定义字段类型

from sqlalchemy.sql import func

# func
# 用法：Column("created_at", DateTime(timezone=True), server_default=func.now())
# 作用: 设置默认值/函数，例如自动生成时间戳
# 使用场景: 在 models.py 给 created_at 字段自动生成当前时间

from .database import metadata
# 在 database.py 中
# metadata =  MetaData()   # 存放表信息、用于建表

sensor_data = Table(
    "sensor_data",
    metadata,
    Column("id", Integer, primary_key=True),
    Column("temperature", Float, nullable=False),
    Column("humidity", Float, nullable=False),
    Column("pressure", Float, nullable=False),
    Column("created_at", DateTime(timezone=True), server_default=func.now())
)

