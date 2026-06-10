//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//int main()
//{
//	int n;
//	scanf("%d", &n);
//	int i, j, k, m = 0;
//	int* a = (int*)malloc(n * sizeof(int));
//	for (i = 0; i < n; i++)
//	{
//		scanf("%d", &a[i]);
//	}
//	if (n == 1)
//	{
//		printf("%d\n", a[0]);
//	}
//	else
//	{
//		for (k = n; k >= 2; k--)
//		{
//			for (i = 0; i < k - 1; i++)
//			{
//				for (j = 0; j < k - 1 - i; j++)
//				{
//					if (a[j] < a[j + 1])
//					{
//						int temp = a[j];
//						a[j] = a[j + 1];
//						a[j + 1] = temp;
//					}
//				}
//			}
//			a[k - 2] += a[k - 1]; 
//			m = m + a[k - 2];
//		}
//	}
//	printf("%d", m);
//	return 0;
//}
