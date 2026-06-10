guestlists=["周世阳","刘文昊","刘俊琳","刘东昕"]

for guestlist in guestlists:
    print(f"{guestlist},would you like to have dinner together?")
print("First invitation list:",end="")
for guestlist in guestlists:
    print(guestlist,end=" ")

guestlists.remove("周世阳")
guestlists.append("胥绍博")
print("\nSecond invitation list:",end="")
for guestlist in guestlists:
    print(guestlist,end=" ")

guestlists.pop(0)
guestlists.append("李炮")
print("\nThrid invitation list:",end="")
for guestlist in guestlists:
    print(guestlist,end=" ")

del guestlists[0]
guestlists.insert(3,"陈飞海")
print("\nFinally invitation list:",end="")
for guestlist in guestlists:
    print(guestlist,end=" ")

len_guestlists = len(guestlists)
print(f"\nA total of {len_guestlists} guests came to have dinner.")