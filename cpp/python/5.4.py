import matplotlib.pyplot as plt  
import numpy as np  
  
# 假设的数据  
categories = ['clarity', 'IF', 'S1', 'S2', 'V51', 'V52', 'WS1', 'WS2']  
n_groups = len(categories)  
  
# 假设每个类别都有3个子类别（例如：'Blue', 'Orange', 'Green'）  
subcategories = ['Blue', 'Orange', 'Green']  
n_bars = len(subcategories)  
  
# 假设的数据值  
values = np.random.randint(1000, 4000, size=(n_groups, n_bars))  # 随机生成数据  
  
# 条形图的索引位置  
index = np.arange(n_groups)  
bar_width = 0.25  # 可以调整条形之间的间隔  
  
# 绘制分组条形图  
fig, ax = plt.subplots()  
for i in range(n_bars):  
    ax.bar(index + i * bar_width, values[:, i], bar_width, label=subcategories[i])  
  
# 设置x轴标签  
ax.set_xlabel('Categories')  
  
# 设置y轴标签  
ax.set_ylabel('Amount')  
  
# 设置x轴刻度标签  
ax.set_xticks(index + bar_width / 2)  
ax.set_xticklabels(categories)  
  
# 添加图例  
ax.legend()  
  
# 显示图形  
plt.tight_layout()  
plt.show()