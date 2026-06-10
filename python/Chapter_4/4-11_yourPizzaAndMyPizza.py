my_pizzas=["chicago style pizza","california style pizza","thick pizza"]
friend_pizzas=my_pizzas[:]
friend_pizzas.append("pepperoni pizza")
print("My favorite pizzas are:")
for pizza in my_pizzas:
    print(pizza)

print("\nMy friend's favorite pizzas are:")
for pizza in friend_pizzas:
    print(pizza)
