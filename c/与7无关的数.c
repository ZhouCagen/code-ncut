//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<math.h>
//int digit(int x, int n)
//{
//	return x % (int)pow(10, n) / pow(10, n - 1);
//}
//
//int main()
//{
//	int n,i,sum=0;
//	scanf("%d", &n);
//	while (n >= 100 || n <= 0)
//		scanf("%d", &n);
//	for (i = 1; i <= n; i++)
//	{
//		if (i % 7 != 0 && digit(i, 1) != 7 && digit(i, 2) != 7)
//			sum = sum + i * i;
//	}
//	printf("%d", sum);
//	return 0;
//}