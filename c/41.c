//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//int main()
//{
//	int m, n;
//	scanf_s("%d %d", &m, &n);
//	int* integer1 = (int*)malloc(m * sizeof(int));
//	int* integer2 = (int*)malloc(n * sizeof(int));
//	for (int i = 0; i < m; i++)
//	{
//		scanf_s("%d", &integer1[i]);
//	}
//	for (int j = 0; j < n; j++)
//	{
//		scanf_s("%d", &integer2[j]);
//	}
//	int* integer = (int*)malloc((m + n) * sizeof(int));
//	if (integer == NULL)
//	{
//		return;
//	}
//	int c;
//	for (c = 0; c < m; c++)
//	{
//		integer[c] = integer1[c];
//	}
//	for (int j = 0; j < n; j++, c++)
//	{
//		integer[c] = integer2[j];
//	}
//	for (int i = 0; i < m + n - 1; i++)
//	{
//		for (int j = 0; j < m + n - i - 1; j++)
//		{
//			if (integer[j] > integer[j + 1])
//			{
//				int temp = integer[j + 1];
//				integer[j + 1] = integer[j];
//				integer[j] = temp;
//			}
//		}
//	}
//	printf("%d ", integer[0]);
//	for (int i = 1; i < m + n; i++)
//	{
//		if (integer[i] != integer[i - 1])
//		{
//			printf("%d " ,integer[i]);
//		}
//	}
//	return 0;
//}