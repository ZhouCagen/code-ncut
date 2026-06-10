import matplotlib.pyplot as plt  
  
# 定义类别和对应的金额  
categories = ['Ideal', 'Premium', 'Very Good', 'Good', 'Fair']  
values = [20000, 15000, 10000, 5000, 2500]  # 假设的金额值  
  
# 创建一个新的图形  
fig, ax = plt.subplots()  
  
# 绘制条形图  
ax.bar(categories, values, color='skyblue')  
  
# 设置x轴和y轴的标签  
ax.set_xlabel('Cut Quality')  
ax.set_ylabel('Amount')  
  
# 设置标题  
ax.set_title('Cut Quality vs Amount')  
  
# 显示网格线（可选）  
ax.grid(axis='y', alpha=0.75)  
  
# 显示图形  
plt.show()

import matplotlib.pyplot as plt  
  
# 定义数据和标签  
ratings = ['Fair', 'Good', 'Very Good', 'Premium', 'Ideal']  
values = [1000, 3000, 6000, 10000, 20000]  # 假设的数值，您可以根据实际情况修改  
  
# 创建一个新的图形  
plt.figure(figsize=(10, 6))  
  
# 绘制竖直条形图  
plt.bar(ratings, values, color='red', edgecolor='black')  
  
# 设置x轴和y轴的标签以及标题  
plt.xlabel('Product Ratings')  
plt.ylabel('Amount')  
plt.title('Product Ratings Distribution')  
  
# 显示图形  
plt.show()