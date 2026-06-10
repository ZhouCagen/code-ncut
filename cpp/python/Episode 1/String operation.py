str1=input("请输入一个字符串：")
reverse_str1=str1[::-1]
print("Slice：{}".format(reverse_str1))

reverse_str1=''.join(reversed(str1))
print("Reversed and join：{}".format(reverse_str1))

reverse_str1 =""
for char in str1:
    reverse_str1 =char+reverse_str1
print("Circular splicing：{}\n".format(reverse_str1))

if str1==reverse_str1:
    print("The string is symmetric strings！")
else:
    print("The string is not symmetric strings！")

from collections import Counter

word_counts_first={}
word_counts_second = Counter()  # 初始化计数器

temp_str=input("请输入单词（输入Exit退出）：")

if temp_str.lower()=="exit" or not temp_str:
    exit()

word_counts_first[temp_str.lower()]= word_counts_first.get(temp_str.lower(), 0) + 1
word_counts_second[temp_str.lower()] +=1

while True:
    temp_str=input("请再次输入单词（输入Exit退出）：")
    if temp_str.lower()=="exit" or not temp_str:
        break
    word_counts_first[temp_str.lower()]= word_counts_first.get(temp_str.lower(), 0) + 1
    word_counts_second[temp_str.lower()] += 1

sorted_word_counts=sorted(word_counts_first.items(), key=lambda x:x[1], reverse=True)
print("\nDictionary and sort：")
for item in sorted_word_counts:
    print(item)

print("\nUse collections.Counter：")
for item in word_counts_second.items():
    print(item)
