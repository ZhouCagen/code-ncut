//问题描述
//
//把一个R进制正整数转化成十进制
//
//输入形式
//
//一行，包含一个R进制表示的正整数a和进制数R，a的十进制表示的范围是(0 - 65536)，1 < R <= 9。
//
//	输出形式
//
//	一行，a的十进制表示
//
//	样例输入
//
//	11 2
//
//	样例输出
//
//	3

//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<math.h>
//int digit(int x, int n)
//{
//	return x % (int)pow(10, n) / (int)pow(10, n - 1);
//}
//int main()
//{
//	int a, R;
//	scanf("%d %d", &a, &R);
//	int sum = 0;
//	int i;
//	int num = (int)log10(a) + 1;
//	for (i = 1; i <= num; i++)
//	{
//		sum += digit(a, i) * (int)pow(R, i - 1);
//	}
//	printf("%d", sum);
//	return 0;
//}