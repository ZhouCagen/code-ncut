import sys,os

# 获取项目根目录（backend）
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# 添加到 Python 模块搜索路径
if BASE_DIR not in sys.path:
    sys.path.append(BASE_DIR)