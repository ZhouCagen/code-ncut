a = [1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89]
b = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]
c=[]
m=0
for i in range(1,10):
    for j in range(0,13):
        if(a[i]==b[j]):
            c.append(a[i])
print(c,end='\n')
print()
for i in b:
    if (i in a):
        a.remove(i)
print(a)
