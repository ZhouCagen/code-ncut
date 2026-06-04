//问题描述
//写一函数，输入一行字符，将此字符串中最长的单词输出。在主函数中输入和输出字符串。
//输入形式
//输入一行，包括一个不大于80的字符串。
//输出形式
//输出一行，输出字符串中最长单词，对于最长长度相同的单词，输出第一个出现单词。
//样例输入
//There is a nice mouse.
//样例输出
//There

//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<string.h>
//void charge(char a[], char b[])
//{
//	int i;
//	for (i = 0; b[i] != '\0'; i++)
//	{
//		b[i] = a[i];
//	}
//}
//void maxword(char* arr)
//{
//	int i = 0, j;
//	char a[80];
//	char b[80];
//	int c1 = 0;
//	while (arr[i] != '\0')
//	{
//		int count = 0;
//		for (j = i; (arr[j]>='a'&&arr[j]<='z')||(arr[j]>='A'&&arr[j]<='Z'); j++)
//		{
//			b[j] = arr[j];
//			count++;
//		}
//		if (count > c1)
//			{
//				for (j = 0; j < 80; j++)
//				{
//					a[j] = '\0';
//				}
//				charge(a, b);
//			}
//		c1 = c1 > count ? c1 : count;
//		for (j = 0; j < 80; j++)
//		{
//			b[j] = '\0';
//		}
//	}
//	printf("%s", a);
//	return;
//}
//int main()
//{
//	char arr[80] = { '\0' };
//	gets(arr);
//	maxword(arr);
//	return 0;
//}