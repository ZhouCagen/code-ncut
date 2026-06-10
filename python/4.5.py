import matplotlib.pyplot as plt
import numpy as np
clarity_table=diamonds['clarity'].value_counts() # type: ignore
N=len(clarity_table)
theta=np.linspace(0,2*np.pi,N,endpoint=False)
radii=clarity_table
width=2*np.pi/N*0.8
colors=['b','g','r','y','c','k','m','tan']
plt.subplot(111,projection='polar')
plt.bar(theta,radii,width=width,bottom=0,colors=colors)
plt.yticks(np.linspace(0,12000,4))