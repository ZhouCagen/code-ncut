import matplotlib.pyplot as plt
import numpy as np
y=np.array([40,30,20,10])
label=['Monetary','Food Items','Clothing','Books']
color=["#d5695d", "#5d8ca8", "#65a479", "#a564c9"]
plt.title("Donation Percentages at Community Center")
plt.pie(y,labels=label,colors=color,autopct='%0.1f%%',startangle=90)
plt.axis('equal')
plt.show()