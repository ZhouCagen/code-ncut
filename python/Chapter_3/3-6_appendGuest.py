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