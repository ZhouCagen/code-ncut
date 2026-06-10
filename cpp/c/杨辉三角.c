//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int main()
//{
//	int n,i,j,h, arr[20][20] = { {0} };
//	scanf("%d", &n);
//	for ( i = 0; i < n; i++)
//	{
//		arr[i][0] = 1;
//		arr[i][i] = 1;
//	}
//	for (j = 2; j < n; j++)
//	{
//		for (h = 1; h < j; h++)
//		{
//			arr[j][h] = arr[j - 1][h - 1] + arr[j - 1][h];
//		}
//	}
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j <= i; j++)
//		{
//				printf("%d ", arr[i][j]);
//		}
//		printf("\n");
//	}
//	return 0;
//}