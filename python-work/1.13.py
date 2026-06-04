import random
num=random.randint(0,100)
play1=[str(input("请输入名字：")),int(input("请猜测数字："))]
play2=[str(input("请输入名字：")),int(input("请猜测数字："))]
print(num)
if play1[1]<0 or play1[1]>100:
    play1=[str(input("请输入名字：")),int(input("请猜测数字："))]
    play2=[str(input("请输入名字：")),int(input("请猜测数字："))]
elif (abs(play1[1]-num)>abs(play2[1]-num)):
    print('{}胜利'.format(play2[0]))
elif (abs(play1[1]-num)<abs(play2[1]-num)):
    print('{}胜利'.format(play1[0]))
else:
    print('平局')