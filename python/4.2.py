import matplotlib.pyplot as plt
names=['group_a','group_b','group_c']
values=[1,10,100]
plt.figure(figsize=(9,3))
plt.subplot(131)
plt.bar(names,values)
plt.text(0,50,90,'Bar Graph')
plt.subplot(132)
plt.scatter(names,values)
plt.annotate('important point',xy(1,10),xytext=(1,40),arrowprops=dict(facecolor='black',shrink=0.05))  # type: ignore
plt.subplot(133)
plt.plot(names,values)
plt.grid()
plt.suptitle('Categorical Plotting')
plt.show()