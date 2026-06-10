# CRUD 模块
# 封装数据库操作函数，所有操作异步：
# - `insert_sensor_data(data)` → 插入一条传感器数据
# - `get_all_sensor_data()` → 查询全部传感器数据
# - `get_sensor_data_by_id(record_id: int)` → 根据主键查询某条数据

from app.database import database
# database：来自 databases.Database 的异步数据库对象，用于执行 SQL
from app.models import sensor_data
# sensor_data：SQLAlchemy 定义的表对象，存储字段信息（temperature、humidity、pressure、created_at）

# 如果你用 from app.database import database
# app 表示 整个包（就是 backend/app/ 这个目录）
# Python 会从 项目根目录（也就是 backend/）开始搜索模块
# 如果你用 from database import database
# Python 会在当前工作目录和系统路径查找 database.py
# 如果你直接在 backend/ 下面执行，可能找不到 database.py（因为它在 app/ 里）

# 插入传感器数据
async def insert_sensor_data(tem: float, hum: float, pre: float):
    query = sensor_data.insert().values(
        temperature = tem,
        humidity = hum,
        pressure = pre
    )
    last_record_id = await database.execute(query)
    return last_record_id
# async def → 异步函数，可以配合 await 非阻塞执行数据库操作
# sensor_data.insert().values(...) → 生成插入语句对象，这里还没执行
# await database.execute(query) → 真正执行 SQL，把数据写入数据库
# 返回 last_record_id → 新插入行的主键 ID
# 这个函数就是把前端或传感器传来的温湿压数据插入数据库，并返回该条记录 ID


# 获取所有数据
async def get_all_sensor_data():
    query = sensor_data.select()
    return await database.fetch_all(query)
# sensor_data.select() → 生成 SELECT * FROM sensor_data
# await database.fetch_all(query) → 异步执行查询，返回结果列表
# 拿到表里所有数据，返回给前端或其他逻辑

# 根据ID获取单条数据
async def get_sensor_data_by_id(record_id: int):
    query = sensor_data.select().where(sensor_data.c.id == record_id) # .c = columns的缩写 用来访问列对象
    return await database.fetch_one(query)
    # fetch_one 是 databases.Database 对象的方法 
    # 作用：执行传入的 SQL 查询，只取 一条记录
    # 返回类型：
    # 默认是 Record 对象（类似字典，字段名可以索引）
    # 如果没查到记录 → 返回 None
# .where(sensor_data.c.id == record_id) → 给查询加条件，只取指定 id 的记录
# await database.fetch_one(query) → 异步执行，返回单条结果（dict 或 Row对象）
# 根据主键查找某一条传感器数据

# database.execute / database.fetch_all / database.fetch_one 都是 异步操作
# 异步好处：不会阻塞 FastAPI 的请求，支持高并发