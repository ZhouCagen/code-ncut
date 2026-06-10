////问题描述
////
////You have devised a new encryption technique which encodes a message by inserting between its characters randomly generated strings in a clever way.Because of pending patent issues we will not discuss in detail how the strings are generated and inserted into the original message.To validate your method， however， it is necessary to write a program that checks if the message is really encoded in the final string.
////
////Given two strings s and t， you have to decide whether s is a subsequence of t，i.e. if you can remove characters from t such that the concatenation of the remaining characters is s.
////
////输入形式
////
////The input contains several test cases.Each is specified by two strings s, t of alphanumeric ASCII characters separated by white space.The length of s and t will no more than 100000.
////
////输出形式
////
////For each test case output "Yes"， if s is a subsequence of t，otherwise output "No".
////
////样例输入
////
////4
////
////sequence subsequence
////
////person compression
////
////VERDI vivaVittorioEmanueleReDiItalia
////
////caseDoesMatter CaseDoesMatter
////
////样例输出
////
////Yes
////
////No
////
////Yes
////
////No
//
////您设计了一种新的加密技术，该技术通过在字符之间插入随机生成的字符串来对消息进行编码。由于未决的专利问题，
//// 我们不会详细讨论如何生成字符串并将其插入到原始消息中。但是，要验证您的方法，
//// 有必要编写一个程序来检查消息是否真的编码在最终字符串中。
////
////给定两个字符串 s 和 t，您必须确定 s 是否是 t 的子序列，即如果可以从 T 中删除字符，以便剩余字符的串联为 S。
////
////输入形式
////
////输入包含多个测试用例。每个字符由两个字符串 s、t 指定，这些字符串由空格分隔的字母数字 ASCII 字符。
//// s 和 t 的长度不会超过 100000。
////
////输出形式
////
////对于每个测试用例，输出“是”，如果 s 是 t 的子序列，则输出“否”
//
//
//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<string.h>
//int main()
//{
//	char s[100] = { '\0' }, t[100] = { '\0' };
//	int n, i, j, q = 0;
//	scanf("%d", &n);
//	for (i = 1; i <= n; i++)
//	{
//		scanf("%s %s", s, t);
//		int len1 = (int)strlen(s);
//		int len2 = (int)strlen(t);
//		q = 0;
//		for (j = 0; j < len2; j++)
//		{
//			if (s[q] == t[j])
//			{
//				q++;
//			}
//		}
//		if (q == len1)
//		{
//			printf("Yes\n");
//		}
//		else
//		{
//			printf("No\n");
//		}
//	}
//	return 0;
//}
