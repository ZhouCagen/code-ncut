n=int(input())
for num in range(100,n+1):
    m=len(str(num))
    total=0
    for i in str(abs(num)):
        total+=int(i)**m
    if num==total:
        print(num)
