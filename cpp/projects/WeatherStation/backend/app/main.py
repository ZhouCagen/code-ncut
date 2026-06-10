# FastAPI 服务入口
# 定义 API 路由并启动服务：
# - `@app.post("/sensor/")` → 接收传感器数据
# - `@app.get("/sensor/")` → 查询全部传感器数据

#  POST → 代表“提交数据”   GET → 代表“读取数据”
from fastapi import FastAPI, HTTPException
# FastAPI：框架的主类，用它创建应用实例 app = FastAPI()。负责路由注册、请求分发、自动生成 API 文档等。
# HTTPException：用于在路由处理中抛出 HTTP 错误（例如找不到资源时抛 404）。FastAPI 会把它转成对应的 HTTP 响应。
from app.schemas import SensorDataIn, SensorDataOut
# SensorDataIn / SensorDataOut：在 schemas.py 用 Pydantic 定义的模型。
# SensorDataIn：表示客户端（前端或硬件）发送给后端的数据结构（请求体），负责 类型校验。
# SensorDataOut：表示后端返回给客户端的数据结构（响应体）。FastAPI 会根据这个模型把返回值自动序列化成 JSON 并校验格式。
from app.crud import insert_sensor_data, get_all_sensor_data, get_sensor_data_by_id
# 在 crud.py（数据库操作层）：
# insert_sensor_data(...)：把一条传感器数据插入数据库（异步），返回新记录的 id。
# get_all_sensor_data()：查询并返回表中所有记录（异步）。
# get_sensor_data_by_id(id)：根据主键 id 查询单条记录（异步）。
# 为什么把 DB 操作放在 crud.py：分离关注点（路由只调用 CRUD，不直接写 SQL），便于测试与维护。
from app.database import database
# database 来自 database.py，它是 databases.Database(DATABASE_URL) 的实例。
# await database.connect()：建立数据库连接（通常在应用启动时执行）。
# await database.disconnect()：断开数据库连接（应用关闭时执行）。
from contextlib import asynccontextmanager
# asynccontextmanager：Python 标准库提供的装饰器，用来方便定义异步上下文管理器（async with 可用的那种）。
# 在 FastAPI 新推荐的生命周期（lifespan）里，我们用它来包装“应用启动到关闭”这段代码，确保 connect() 在启动前执行，disconnect() 在关闭后执行。

from fastapi.middleware.cors import CORSMiddleware

@asynccontextmanager   
async def lifespan(app: FastAPI):
    await database.connect()
    yield        #  暂停，开始运行 FastAPI 主程序
    await database.disconnect()
# 异步上下文管理器 lifespan，它接收 app（FastAPI 实例）作为参数。
# await database.connect()：进入上下文时（应用启动）先建立 DB 连接。
# yield：控制权交回给 FastAPI，应用在 yield 之后的这段时间里正常处理请求。
# await database.disconnect()：当应用退出时（关闭、重启等），yield 后的代码执行，断开 DB 连接。

app = FastAPI(lifespan= lifespan)
# app = FastAPI(lifespan=lifespan)：把这个生命周期管理器注册给 FastAPI。这是推荐替代 @app.on_event("startup")/("shutdown") 的新写法。

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # 允许所有前端访问（开发阶段可以这样）
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# 根路由测试
@app.get("/")
# @app.get("/")：注册 HTTP GET 方法、路径 /（称为“根路由”或首页）。 
async def root():
    return {"message": "Hello Weather_Station"} # 测试服务器是否启动
# async def root()：异步函数作为请求处理器；当有人访问 GET / 时 FastAPI 调用它。
# 返回一个字典，FastAPI 自动把它转成 JSON 响应：{"message":"Hello Weather_Station"}

# 插入传感器数据
@app.post("/sensor", response_model = SensorDataOut)
# 定义 POST /sensor 路由；
# response_model = SensorDataOut：指定返回数据应该符合 SensorDataOut 模型（FastAPI 会验证并自动序列化返回值）。
async def insert_sensor_data_route(data: SensorDataIn): #FastAPI 会自动把请求体 JSON 解析成 SensorDataIn 对象并验证类型（Pydantic）。
    record_id = await insert_sensor_data(data.temperature, data.humidity, data.pressure)
    record = await get_sensor_data_by_id(record_id)
    if not record:
        raise HTTPException(status_code = 404, detail = "Record not found after insertion")
    
    return record # 返回数据库查询到的完整记录，FastAPI 用 SensorDataOut 把记录序列化成 JSON。

# 获取所有传感器数据
@app.get("/sensor", response_model = list[SensorDataOut])
async def read_all_sensor_data():
    return await get_all_sensor_data() # 异步查询所有记录

# 根据id获取单条数据
@app.get("/sensor/{record_id}", response_model = SensorDataOut) 
async def read_sensor_data(record_id: int):
    record = await get_sensor_data_by_id(record_id) # 异步根据ID查询记录
    if not record:
        raise HTTPException(status_code = 404, detail = "Record not found")
    return record