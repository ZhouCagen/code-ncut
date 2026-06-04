////问题描述
////
////A number sequence is defined as follows :
////
////f(1) = 1， f(2) = 1， f(n) = (A * f(n - 1) + B * f(n - 2)) mod 7.
////
////Given A， B， and n， you are to calculate the value of f(n).
////
////输入形式
////
////The input consists of multiple test cases.Each test case contains 3 integers A， B and n on a single line(1 <= A， B <= 1000， 1 <= n <= 100，000，000).Three zeros signal the end of input and this test case is not to be processed.
////
////输出形式
////
////For each test case， print the value of f(n) on a single line.
////
////样例输入
////
////1 1 3
////
////1 2 10
////
////0 0 0
////
////样例输出
////
////2
////
////5

//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int NumberSequence(int a, int b, int n)
//{
//	if (n == 1 || n == 2)
//	{
//		return 1;
//	}
//	else
//	{
//		return (a * NumberSequence(a, b, n - 1) + b * NumberSequence(a, b, n - 2)) % 7;
//	}
//	
//}
//int main()
//{
//	int A, B, n, i;
//	scanf("%d %d %d", &A, &B, &n);
//	for (i = 1;; i++)
//	{
//		if (A == 0 && B == 0 && n == 0)
//		{
//			break;
//		}
//		printf("%d\n", NumberSequence(A, B, n));
//		scanf("%d %d %d", &A, &B, &n);
//	}
//	return 0;
//}