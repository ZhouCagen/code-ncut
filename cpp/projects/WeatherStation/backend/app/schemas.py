# Schemas 模块
# 使用 pydantic 定义请求/响应模型：
# - `SensorDataIn` → 前端 POST 请求体
# - `SensorDataOut` → GET 返回数据，包含 `id` 和 `created_at`

from pydantic import BaseModel
from datetime import datetime

# 前端读取数据
class SensorDataOut(BaseModel):
# BaseModel 是 Pydantic 提供的基类
# 功能：
#   1.数据验证：确保输入的数据类型正确（比如 temperature 必须是 float）
#   2.数据序列化/反序列化：可以把 Python 对象 → JSON，或者 JSON → Python 对象
# FastAPI 路由接收请求体时，就会用 Pydantic 自动把 JSON 解析成 BaseModel 对象
    id: int
    temperature: float
    humidity: float
    pressure: float
    created_at: datetime

    class Config:
        from_attributes = True # Pydantic 2.x 推荐写法，支持 ORM 对象直接序列化
    # 默认情况下，Pydantic 只能处理字典类型的数据，比如：
    # {"temperature": 25.5, "humidity": 60.0, "pressure": 1013.2}
    #
    # 数据库查询返回的是 ORM 对象，例如：
    # sensor = <SensorData(id=1, temperature=25.5, humidity=60.0, pressure=1013.2, created_at=...)>
    #
    # 如果不设置 from_attributes=True（V2 中替代 orm_mode=True），
    # Pydantic 无法直接把 ORM 对象转换为 JSON。
    #
    # 设置 from_attributes=True 后：
    # - Pydantic 会自动读取 ORM 对象的属性
    # - 将其序列化为字典/JSON
    # - FastAPI 返回给前端时就能直接输出 JSON

# 前端提交数据
class SensorDataIn(BaseModel):
    temperature: float
    humidity: float
    pressure: float
