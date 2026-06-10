//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int main()
//{
//	int m, n, p, i, j, h, k, q;
//	scanf("%d%d%d", &m, &n, &p);
//	int arr1[10][10] = { {0} }, arr2[10][10] = { {0} };
//	for (i = 0; i < m; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			scanf("%d", &arr1[i][j]);
//		}
//	}
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < p; j++)
//		{
//			scanf("%d", &arr2[i][j]);
//		}
//	}
//	int arr3[10][10] = { {0} };
//	i = 0, j = 0;
//	for (h = 0; h < m; h++)
//	{
//		for (k = 0; k < p; k++)
//		{
//			for (q = 0; q < n; q++)
//			{
//				arr3[h][k] = arr3[h][k] + arr1[h][q] * arr2[q][k];
//			}
//		}
//	}
//	for (h = 0; h < m; h++)
//	{
//		for (k = 0; k < p; k++)
//		{
//			if (arr3[h][k] != 0)
//				printf("%d ", arr3[h][k]);
//			else break;
//		}
//		printf("\n");
//	}
//
//
//
//}