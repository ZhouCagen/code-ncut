# Database 模块

# 负责数据库配置与连接，定义：
# - `database` → 异步数据库对象，用于 CRUD
# - `metadata` → SQLAlchemy MetaData 实例，用于建表
# - `engine` → 同步 Engine，用于 `metadata.create_all()` 创建表


from sqlalchemy import create_engine, MetaData
# sqlalchemy.create_engine
# 创建同步数据库引擎，用于建表或一些同步操作
# 使用场景：create_tables.py 调用 metadata.create_all(engine) 来实际生成表

# sqlalchemy.MetaData
# 来自 SQLAlchemy
# 用法：metadata = MetaData()
# 作用：存储数据库表结构信息，所有 Table 都挂在这个 metadata 上
# 使用场景：
# models.py 定义表时：sensor_data = Table(..., metadata, ...)
# create_tables.py 初始化表时：metadata.create_all(engine)

from databases import Database
# databases.Database
# 来自第三方库 databases
# 用法：database = Database(DATABASE_URL)
# 作用：创建异步数据库对象，FastAPI 路由中执行 CRUD 时用它异步访问数据库
# 使用场景：在 crud.py 和 main.py 中，异步执行增删改查

DATABASE_URL = "sqlite:///./weather.db"
database =  Database(DATABASE_URL)  # 异步数据库对象
metadata =  MetaData()              # 存放表信息、用于建表
engine = create_engine(DATABASE_URL)  # 同步 Engine，用于 create_all()