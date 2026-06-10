import random

specials_allowed={'\n','\r','*','&','^','$'}

count=0
with open('test.txt','w') as file:
    while count<1000:
        num = random.randint(1, 122)
        print("随机生成的数字：", num)
        ch=chr(num)
        print("此数字对应的字符：", ch)
        if ('a'<=ch<='z') or ('A'<=ch<='Z') or('0'<=ch<='9') or (ch in specials_allowed):
            file.write(ch)
            count+=1

file.close()

with open('test.txt','r') as file:
    string =file.read()
    count=len(string)
    count_upper=count_lower=count_digit=count_specials=0
    for ch in string:
        if ch.isupper():
            count_upper+=1
        elif ch.islower():
            count_lower+=1
        elif ch.isdigit():
            count_digit+=1
        else:
            count_specials+=1

    print("方法一统计结果：")
    print("大写字母数量：",count_upper," 百分比：",round(count_lower/count*100,2),"%")
    print("小写字母数量：",count_lower," 百分比：",round(count_digit/count*100,2),"%")
    print("数字数量：",count_digit," 百分比：",round(count_upper/count*100,2),"%")
    print("其他字母数量",count_specials," 百分比：",round(count_specials/count*100,2),"%")

file.close()

with open('test.txt','r') as file:
    string_second =file.read()
    count_second=len(string_second)
    count_upper_second=count_lower_second=count_digit_second=count_specials_second=0
    for ch in string_second:
        asc_code=ord(ch)
        if 65<=asc_code<=90:
            count_upper_second+=1
        elif 97<=asc_code<=122:
            count_lower_second+=1
        elif 48<=asc_code<=57:
            count_digit_second+=1
        else:
            count_specials_second+=1

    print("\n方法二统计结果：")
    print("大写字母数量：", count_upper_second, " 百分比：", round(count_lower_second / count_second * 100, 2), "%")
    print("小写字母数量：", count_lower_second, " 百分比：", round(count_digit_second/ count_second * 100, 2), "%")
    print("数字数量：", count_digit_second, " 百分比：", round(count_upper_second / count_second * 100, 2), "%")
    print("其他字母数量", count_specials_second, " 百分比：", round(count_specials_second / count_second * 100, 2), "%")

file.close()

with open('test.txt','r') as file:
    string=file.read()
    string_upper=string.upper()
    with open('test_copy.txt','w') as file_copy:
        file_copy.write(string_upper)
    file_copy.close()

file.close()