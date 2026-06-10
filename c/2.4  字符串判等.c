//问题描述
//
//判断两个由大小写字母和空格组成的字符串在忽略大小写和压缩掉空格后是否相等
//
//输入形式
//
//第1行是测试数据的组数n，每组测试数据占2行，第1行是第一个字符串s1，第2行是第二个字符串s2。
//
//每组测试数据之间有一个空行，每行数据不超过100个字符（注意字符串的长度可能为0）
//
//输出形式
//
//n行，相等则输出YES，否则输出NO
//
//样例输入
//
//3
//
//
//
//
//
//
//
//a A bb BB ccc CCC
//
//Aa BBbb CCCccc
//
//
//
//a              dfadf              fasdf
//
//adasddfsfsaf
//
//样例输出
//
//YES
//
//YES
//
//NO
//


//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//int main()
//{
//	int i, j, n, len1, len2, k;
//	char a[100] = { '\0' }, b[100] = { '\0' }, c[100] = { '\0' }, d[100] = { '\0' }, space[100] = { '\0' };
//	scanf("%d", &n);
//	getchar();
//	for (i = 0; i < n; i++)
//	{
//		gets(a);
//		gets(b);
//		len1 = (int)strlen(a);
//		k = 0;
//		for (j = 0; j < len1; j++)
//		{
//			if (a[j] >= 'a' && a[j] <= 'z')
//			{
//				c[k++] = a[j] - 'a' + 'A';
//			}
//			if (a[j] >= 'A' && a[j] <= 'Z')
//			{
//				c[k++] = a[j];
//			}
//		}
//		c[k] = '\0';
//		len2 = (int)strlen(b);
//		k = 0;
//		for (j = 0; j < len2; j++)
//		{
//			if (b[j] >= 'a' && b[j] <= 'z')
//			{
//				d[k++] = b[j] - 'a' + 'A';
//			}
//			if (b[j] >= 'A' && b[j] <= 'Z')
//			{
//				d[k++] = b[j];
//			}
//		}
//		d[k] = '\0';
//		if (strcmp(c, d) == 0)
//		{
//			printf("YES\n");
//		}
//		else
//		{
//			printf("NO\n");
//		}
//		if (i < n - 1)
//		{
//			gets(space);
//		}
//	}
//	return 0;
//}