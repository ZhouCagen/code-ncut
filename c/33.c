//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int approximate(int n)
//{
//	int sum = 0;
//	for (int i = 1; i < n; i++)
//	{
//		if (n % i == 0)
//		{
//			sum += i;
//		}
//	}
//	return sum;
//}
//void number(int m, int n)
//{
//	int i, have = 0, x;
//	for (i = m; i <= n; i++)
//	{
//		x = approximate(i);
//		if (i == approximate(x) && /*approximate(x) <= n &&*/ x > i)
//		{
//			printf("%d %d\n", i, x);
//			have = 1;
//		}
//	}	
//	if (have == 0)
//	{
//		printf("NONE");
//	}
//	return;
//}
//int main()
//{
//	int m = 0, n = 0;
//	scanf_s("%d %d", &m, &n);
//	number(m, n);
//	return 0;
//}