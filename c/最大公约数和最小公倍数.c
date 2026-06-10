//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int gys(int a, int b)
//{
//	if (b == 0)
//		return a;
//	else
//		return gys(b, a % b);
//
//}
//int gbs(int a, int b)
//{
//	int i = 1;
//	while ((a * i) % b != 0)
//	{
//		i++;
//	}
//	return a*i;
//}
//int main()
//{
//	int m, n, c, d;
//	scanf("%d%d", &m, &n);
//	scanf("%d%d", &c, &d);
//	int sum1, sum2;
//	sum1 = gbs(m, n) + gbs(c, d);
//	sum2 = gys(m, n) + gys(c, d);
//	printf("%d\n%d\n", sum2,sum1);
//	return 0;
//}