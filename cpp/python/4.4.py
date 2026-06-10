import matplotlib.pyplot as plt
import numpy as np
clarity_table=diamonds['clarity'].value_counts() # type: ignore
N=len(clarity_table)
sizes=clarity_table
cmap=plt.get_cmap('tab20c')
colors=cmap(np.arange(N))
labels=clarity_table.index
explode=[0.1]+[0]*(N-1)
plt.pie(sizes,explode=explode,colors=colors,labels=labels,shadow=True,startangle=140,radius=1,wedgeprops=dict(width=0.3,edgecolor='w'))
plt.axit('equal')
plt.text(-0.2,0,'clarity')