import matplotlib.pyplot as plt  
  
# 定义数据和类别  
categories = ['IF', 'WS1', 'WS2', 'V51', 'SI2', 'V52']  
values = [11000, 9000, 7000, 6000, 4000, 3000]  # 假设的值，因为您提供的图片中的数值是模糊的  
  
# 创建一个新的图形  
fig, ax = plt.subplots()  
  
# 绘制水平条形图  
ax.barh(categories, values, align='center', color='skyblue')  
  
# 设置x轴和y轴的标签  
ax.set_xlabel('Amount')  
ax.set_ylabel('Category')  
  
# 设置x轴的刻度标签，以便更好地显示数值  
ax.set_xticks(range(0, max(values) + 2000, 2000))  # 设置刻度间隔为2000  
ax.set_xticklabels(['0', '2000', '4000', '6000', '8000', '10000', '12000'])  # 设置刻度标签  
  
# 添加标题  
ax.set_title('Cut Horizontal Bar Chart')  

# 显示图形  
plt.show()