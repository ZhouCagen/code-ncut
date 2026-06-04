import matplotlib.pyplot as plt
import numpy as np
cross_table=pd.crosstab(diamonds['cut'],diamonds['clarity']) # type: ignore
N_cut=cross_table.shape[0]
N_clarity=cross_table.shape[1]
size=3
cmap=plt.get_cmap("tab20c")
outer_colors=cmap(list(np.arange(N_clarity)+10)*N_cut)
inner_colors=cmap(np.arange(N_cut))
labels_inner=cross_table.index
labels_outer=list(cross_table.columns)*N_cut
plt.pie(cross_table.sum(axis=1),radius=10-size,colors=inner_colors,labels=labels_inner,wedgeprops=dict(width=size,edgecolor='w'),labeldistance=0.3)
plt.pie(np.array(cross_table).ravel(),radius=10,colors=outer_colors,labels=labels_outer,wedgeprops=dict(width=size,edgecolor='w',labeldistance=0.8))
plt.axis('equal')