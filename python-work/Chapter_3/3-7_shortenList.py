guestlists=["周世阳","刘文昊","刘俊琳","刘东昕"]

for guestlist in guestlists:
    print(f"{guestlist},would you like to have dinner together?")
print("First invitation list:",end="")
for guestlist in guestlists:
    print(guestlist,end=" ")

guestlists.insert(0,"胥绍博")
len_guestlists = len(guestlists)
guestlists.insert(round(len_guestlists/2),"李炮")
guestlists.append("张添瑞")

print("\nSecond invitation list:",end="")
for guestlist in guestlists:
    print(guestlist,end=" ")

len_guestlists = len(guestlists)

print("\n\nI just learned that the newly purchased dining table will not be delivered in time, so I can only invite two guests.")
while len_guestlists>2:
    popped_guestlist=guestlists.pop()
    print(f"{popped_guestlist}, I'm sorry, I can't invite you to dinner.")
    len_guestlists-=1

print("\nThird invitation list:",end="")
for guestlist in guestlists:
    print(guestlist,end=" ")

print()
for guestlist in guestlists:
    print(f"{guestlist}, you are still invited.")

del guestlists[1]
del guestlists[0]

for guestlist in guestlists:
    print(guestlist)
len_guestlists = len(guestlists)
if len_guestlists==0:
    print("\nThe list is empty.")