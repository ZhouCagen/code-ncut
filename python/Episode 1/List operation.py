list1=['a','b','c','d','e','f','a','b']
print("Original：{0}".format(list1))
list1=set(list1)
print("Set：{0}".format(list1))

list2=['x','y','z','a','b','c','a','b']
print("Original：{0}".format(list2))
temp_dict=dict.fromkeys(list2)
new_list2=list(temp_dict)
print("Dictionary key-value：{0}".format(new_list2))
