import matplotlib.pyplot as plt  
  
# 角度转换为占比（假设总和为360°）  
labels = ['90°', '135°', '45°', '180°', '0°', '225°', '315°', '270°']  
angles = [90, 135, 45, 180, 0, 225, 315, 270]  
sizes = [a / sum(angles) for a in angles]  # 转换为占比  
  
# 如果想要显示每个扇形的具体数值，可以使用explode参数来分离扇形，以及autopct参数来显示数值  
explode = (0.1, 0, 0, 0, 0, 0, 0, 0)  # 只有第一个扇形被稍微分离  
  
# 创建一个饼图  
fig, ax = plt.subplots()  
ax.pie(sizes, explode=explode, labels=labels, autopct='%1.1f%%',  
       shadow=True, startangle=90)  # startangle设置起始角度为90°，即正上方  
ax.axis('equal')  # 保证饼图是圆的，而不是椭圆的  
  
# 隐藏x和y轴  
plt.axis('off')  
  
# 显示图形  
plt.show()