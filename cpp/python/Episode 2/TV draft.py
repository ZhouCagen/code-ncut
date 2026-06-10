import random

win_shift=0
win_stay=0
trials=1000000
for i in range(trials):
    door_prize=random.randint(0,2)
    door_choice=random.randint(0,2)

    all_doors={0,1,2}
    host_choice=list(all_doors-{door_choice,door_prize})
    host_open=random.choice(host_choice)

    door_shift = list(all_doors - {door_choice, host_open})[0]

    win_stay+=(door_choice==door_prize)
    win_shift+=(door_shift==door_prize)

print("不换门胜率：{:.3f}".format(win_stay / trials))
print("换门胜率：{:.3f}".format(win_shift / trials))

