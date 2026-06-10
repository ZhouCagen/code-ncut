import matplotlib.pyplot as plt
from matplotlib_venn import venn2 # type: ignore
set1=set(['A','B','C','D'])
set2=set(['E','F','G','H'])
plt.figure(figsize=(8,6))
venn2(subsets=(set1,set2),set_labels=('one','two'))
plt.title('Venn diagram of two sets')
plt.show()