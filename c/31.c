//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<math.h>
//int split(int x, int n)
//{
//	return x % (int)pow(10, n) / (int)pow(10, n - 1);
//}
//void number(int m, int n)
//{
//	int min = m, max = n;
//	min = min > 100 ? min : 100;
//	max = max < 999 ? max : 999;
//	int have = 0;
//	for (int i = min; i <= max; i++)
//	{
//		int sum = 0;
//		for (int j = 1; j <= 3; j++)
//		{
//			sum += (int)(pow(split(i, j), 3));
//		}
//		if (sum == i)
//		{
//			printf("%d ", i);
//			have = 1;
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
//	int m, n;
//	scanf("%d %d", &m, &n);
//	number(m, n);
//	return 0;
//}