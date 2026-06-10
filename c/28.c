//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<math.h>
//int prime(int n)
//{
//	int have = 1;
//	if (n == 1)
//	{
//		return 0;
//	}
//	for (int i = 2; i <= sqrt(n); i++)
//	{
//		if (n % i == 0)
//		{
//			have = 0;
//			break;
//		}
//	}
//	if (have == 1)
//	{
//		return 1;
//	}
//	if (have == 0)
//	{
//		return 0;
//	}
//	return -1;
//}
//void factor(int N)
//{
//	int have = 0;
//	for (int i = 1; i <= N; i++)
//	{
//		if (N % i == 0)
//		{
//			if (prime(i) == 1)
//			{
//				printf("%d ", i);
//				have = 1;
//			}
//		}
//	}
//	if (have == 0)
//	{
//		printf("No Answer");
//	}
//	return;
//}
//int main()
//{
//	int N;
//	scanf_s("%d", &N);
//	factor(N);
//	return 0;
//}