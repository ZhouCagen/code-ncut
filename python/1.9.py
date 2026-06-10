a=[1,2,2,3,5,8,13,21,34,55,89]
b=[]
for i in range(0,10):
    if a[i]<=5:
        b.append(a[i])
print(b)
b.remove(5)
print(b)