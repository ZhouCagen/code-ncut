//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//double Fibon1(int n)
//{
//	static double fb[201];
//	if (n == 1 || n == 2)
//		return 1;
//	if (fb[n] != 0) return fb[n];
//	return fb[n] = Fibon1(n - 1) + Fibon1(n - 2);
//}
//int main()
//{
//
//	int n, i;
//	double sum = 0;
//	scanf("%d", &n);
//	for (i = 1; i <= n; i++)
//		sum = sum + (double)Fibon1(i + 2) / (double)Fibon1(i + 1);
//	printf("%.2f", sum);
//	return 0;
//}