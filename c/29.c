////【问题描述】
////从控制台输入整数n（n >= 1），计算并输出从1到n之间（包括n）个位为1的所有素数，若没有符合要求的素数，则输出－1。
////素数判断请通过自定义函数实现。
////【输入形式】
////从控制台输入整数n。
////【输出形式】
////按照从小到大的顺序，在一行上输出1到n之间（包括n）个位为1的所有素数，素数之间用一个空格分隔，若没有符合要求的素数，则输出－1。
////【样例输入】
////120
////【样例输出】
////11 31 41 61 71 101
////【样例说明】
////输入的整数n为120，1到120之间个位为1的所有素数为11、31、41、61、71、101。
//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<math.h>
//#include<stdlib.h>
//int split(int x, int n)
//{
//	return x % (int)pow(10, n) / pow(10, n - 1);
//}
//void prime(int n)
//{
//	int i;
//	int* a = (int*)malloc(n * sizeof(int));
//	for (i = 1; i <=n; i++)
//	{
//		a[i] = i;
//	}
//	a[1] = 0;
//	for (i = 2; i < sqrt(n); i++)
//	{
//		if (a[i] != 0)
//		{
//			for (int j = i + 1; j <=n; j++)
//			{
//				if (a[j] != 0 && a[j] % a[i] == 0)
//				{
//					a[j] = 0;
//				}
//			}
//		}
//	}
//	int have = 0;
//	for (i = 2; i <= n; i++)
//	{
//		if (split(i, 1) == 1)
//		{
//			if (a[i] != 0)
//			{
//				have = 1;
//				printf("%d ", a[i]);
//			}
//		}
//	}
//	if (have == 0)
//	{
//		printf("-1");
//	}
//	return;
//}
//int main()
//{
//	int n;
//	scanf("%d", &n);
//	while (n < 1)
//	{
//		scanf("%d", &n);
//	}
//	prime(n);
//	return 0;
//}