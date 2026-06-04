str=str(input("请输入一个密码字符串:"))
str_length=len(str)
a=0
b=0
for i in range(0,str_length):
    if str[i]>='A'and str[i]<='Z':
        a=1
    elif str[i]>='a'and str[i]<='z':
        a=1
    elif str[i]>='0'and str[i]<='9':
        b=1
if a==1 and b==1:
    print("字母和数字都不缺")
elif a==1 and b==0:
    print("缺数字")
elif a==0 and b==1:
    print("缺2字母")