////【问题描述】
////具有下面性质的数a称为同构数：
////设b是a的平方，a与b的低若干位相同。
////例如，5是25的同构数, 25是625的同构数
////
////编程序满足如下要求 :
////输入两个整数a, b(0 <= a, b <= 99), 找出a、b之间全部的同构数。
////【输入形式】
////从键盘输入0 - 99之间的两个整数a和b，以空格隔开。
////【输出形式】
////在屏幕上按照由小到大的顺序输出所有同构数，每一个整数占一行。
////【输入样例】
////0 30
////【输出样例】
////0
////1
////5
////6
////25
////【样例说明】
////在0 - 30之间的同构数有0, 1, 5, 6, 25.



//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<math.h>
//int digit(int x, int n)
//{
//	return x % (int)pow(10, n) / (int)pow(10, n - 1);
//}
//void isomorphism(int n)
//{
//	if (n < 10)
//	{
//		if (digit(n, 1) == digit((int)pow(n, 2), 1))
//		{
//			printf("%d\n", n);
//		}
//	}
//	if (n >= 10)
//	{
//		if (digit(n, 1) == digit((int)pow(n, 2), 1) && digit(n, 2) == digit((int)pow(n, 2), 2))
//		{
//			printf("%d\n", n);
//		}
//	}
//	return;
//}
//int main()
//{
//	int a, b;
//	scanf_s("%d %d", &a, &b);
//	for (int i = a; i <= b; i++)
//	{
//		isomorphism(i);
//	}
//	return 0;
//}
