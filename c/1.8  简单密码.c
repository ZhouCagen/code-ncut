////问题描述
////
////Julius Caesar曾经使用过一种很简单的密码。对于明文中的每个字符，将它用它字母表中后5位对应的字符来代替，这样就得到了密文。比如字符A用F来代替。如下是密文和明文中字符的对应关系。
////
////密文
////
////A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
////
////明文
////
////V W X Y Z A B C D E F G H I J K L M N O P Q R S T U
////
////你的任务是对给定的密文进行解密得到明文。
////
////你需要注意的是，密文中出现的字母都是大写字母。密文中也包括非字母的字符，对这些字符不用进行解码。
////
////输入形式
////
////输入中的测试数据不超过100组。每组数据都有如下的形式，而且各组测试数据之间没有空白的行。
////
////一组测试数据包括三部分
////
////起始行：一行，包括字符串 "START"
////
////密文：一行，给出密文，密文不为空，而且其中的字符数不超过200
////
////结束行：一行，包括字符串 "END"
////
////在最后一组测试数据之后有一行，包括字符串 "ENDOFINPUT"。
////
////输出形式
////
////对每组数据，都有一行输出，给出密文对应的明文。
////
////样例输入
////
////START
////
////NS BFW， JAJSYX TK NRUTWYFSHJ FWJ YMJ WJXZQY TK YWNANFQ HFZXJX
////
////END
////
////START
////
////N BTZQI WFYMJW GJ KNWXY NS F QNYYQJ NGJWNFS ANQQFLJ YMFS XJHTSI NS WTRJ
////
////END
////
////START
////
////IFSLJW PSTBX KZQQ BJQQ YMFY HFJXFW NX RTWJ IFSLJWTZX YMFS MJ
////
////END
////
////ENDOFINPUT
////
////样例输出
////
////IN WAR， EVENTS OF IMPORTANCE ARE THE RESULT OF TRIVIAL CAUSES
////
////I WOULD RATHER BE FIRST IN A LITTLE IBERIAN VILLAGE THAN SECOND IN ROME
////
////DANGER KNOWS FULL WELL THAT CAESAR IS MORE DANGEROUS THAN HE

//#define _CRT_SECURE_NO_WARNINGS	
//#include<stdio.h>
//#include<string.h>
//int main()
//{
//	char ch[26] = { 'V','W','X','Y','Z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U' };
//	char start[15], end[5], arr[100];
//	int i;
//	while (1)
//	{
//		gets(start);
//		if (strcmp(start,"ENDOFINPUT") == 0)
//		{
//			break;
//		}
//		gets(arr);
//		gets(end);
//		for (i = 0; arr[i] != '\0'; i++)
//		{
//			if (arr[i] >= 'A' && arr[i] <= 'Z')
//			{
//				printf("%c", ch[arr[i] - 'A']);
//			}
//			else
//			{
//				printf("%c", arr[i]);
//			}
//		}
//		printf("\n");
//	}
//	return 0;
//}
