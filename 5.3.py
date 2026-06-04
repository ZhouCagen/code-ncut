import matplotlib.pyplot as plt  
  
# 类别名称  
categories = ['Fair', 'Good', 'Ideal', 'Cut', 'Premium', 'Very Good']  
  
# 每个类别的不同颜色级别的数量和对应的颜色  
# 假设每个类别都有六个颜色级别，从左到右依次代表蓝色、橙色、绿色、红色、棕色和紫色  
levels = {  
    'Blue': 20000, 'Orange': 15000, 'Green': 10000, 'Red': 5000, 'Brown': 3000, 'Purple': 2000  
}  
colors = ['b', 'orange', 'g', 'r', 'brown', 'purple']  # 对应matplotlib的颜色代码  
  
# 初始化堆叠条形图的数据列表  
stacked_data = [[] for _ in range(len(categories))]  
  
# 根据类别和颜色级别填充数据  
for i, category in enumerate(categories):  
    for color, level in levels.items():  
        stacked_data[i].append(level)  
  
# 计算每个类别的总数量，用于水平条形图表示比例  
total_values = [sum(data) for data in stacked_data]  
if not total_values or max(total_values) == 0:  
    raise ValueError("total_values is empty or all values are zero.")  
  
# 绘制堆叠条形图  
fig, ax1 = plt.subplots()  
  
bottoms = [0] * len(categories)  
for i, color in enumerate(colors):  
    ax1.bar(categories, [data[i] for data in stacked_data], bottom=bottoms, color=color, label=color.capitalize())  
    bottoms = [bottoms[j] + data[i] for j, data in enumerate(stacked_data)]  
  
# 添加水平条形图以表示比例  

  
plt.show()

ax2 = ax1.twinx()  
max_value = max(total_values)  
ax2.barh(categories, [v / max_value for v in total_values], height=0.3, color='gray', alpha=0.5, edgecolor='black', label='Proportion')  # 注意这里使用barh而不是bar   
ax2.set_ylim(ax1.get_ylim())  
ax2.invert_yaxis()  # 反转y轴以便水平条形图正确显示  

  
# 设置图例  
ax1.legend(title='Color Levels')  
ax2.legend(title='Proportion', loc='upper left')  
  
# 设置x轴和y轴标签  
ax1.set_xlabel('Categories')  
ax1.set_ylabel('Quantity')  
ax2.set_ylabel('Proportion')  
  
# 设置标题  
plt.title('Stacked Bar Chart')  
  
# 显示图形  
plt.tight_layout()  # 调整布局以避免重叠  
plt.show()