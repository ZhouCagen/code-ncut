//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//void count(int* N, int n)
//{
//	int max = N[0], min = N[0];
//	float sum = 0, ave = 0;
//	for (int i = 0; i < n; i++)
//	{
//		max = max > N[i] ? max : N[i];
//		min = min < N[i] ? min : N[i];
//		sum += N[i];
//	}
//	ave = sum / n;
//	printf("%d %d %.2f %.0f", max, min, ave, sum);
//	return;
//}
//int main()
//{
//	int n;
//	scanf_s("%d", &n);
//	int* N = (int*)malloc(n * sizeof(int));
//	if (N==NULL)
//	{
//		return 0;
//	}
//	for (int i = 0; i < n; i++)
//	{
//		scanf_s("%d", &N[i]);
//	}
//	count(N,n);
//	return 0;
//}