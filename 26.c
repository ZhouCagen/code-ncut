	//#define _CRT_SECURE_NO_WARNINGS

	//#include <stdio.h>
	//#include <math.h>

	//int pArr[10000000] = { -1 };
	//int isP(int x)
	//{
	//	if (pArr[x] != -1) return pArr[x];
	//	if (x <= 1) return 0;
	//	int i;
	//	for (i = 2; i <= sqrt(x); i++)
	//	{
	//		if (x % i == 0) return pArr[x] = 0;
	//	}
	//	return pArr[x] = 1;
	//}

	//int have(int x, int* arr, int len)
	//{
	//	int i;
	//	for (i = 0; i < len; i++) if (x == arr[i]) return 1;
	//	return 0;
	//}

	//void popSort(int* arr, int len)
	//{
	//	int i, j;
	//	for (i = 0; i < len - 1; i++)
	//	{
	//		for (j = 0; j < len - 1 - i; j++)
	//		{
	//			if (arr[j] > arr[j + 1])
	//			{
	//				int temp = arr[j];
	//				arr[j] = arr[j + 1];
	//				arr[j + 1] = temp;
	//			}
	//		} 
	//	}
	//}

	//int main()
	//{
	//	int n;
	//	scanf("%d", &n);
	//	int arr[20];
	//	int ans[100];
	//	int idx = 0;
	//	int i, j;
	//	for (i = 0; i < 10000000; i++) pArr[i] = -1;
	//	for (i = 0; i < n; i++) scanf("%d", &arr[i]);
	//	for (i = 0; i < n; i++)
	//	{
	//		for (j = 2; j <= arr[i]; j++)
	//		{
	//			//printf("%d ", j);
	//			if (isP(j) && (arr[i] % j == 0) && !have(j, ans, idx + 1))
	//			{
	//				ans[idx++] = j;
	//				//printf("[%d] ", j);
	//			}

	//		}
	//	}

	//	popSort(ans, idx);
	//	for (i = 0; i < idx; i++) printf("%d ", ans[i]);

	//	return 0;
	//}



	//#define _CRT_SECURE_NO_WARNINGS
	//#include<stdio.h>
	//#include<math.h>

	////筛选素数的prime number函数
	//void prime(int *a,int *b)
	//{
	//	int i, j = 0;
	//	for (i = 2; i <= 1000; i++)
	//	{
	//		a[i] = i;
	//	}
	//	for (i = 2; i < sqrt(1000); i++)
	//	{
	//		if (a[i] != 0)
	//		{
	//			for (j = i + 1; j <= 1000; j++)
	//			{
	//				if (a[j] != 0 && a[j] % a[i] == 0)
	//				{
	//					a[j] = 0;
	//				}
	//			}
	//		}
	//	}
	//	j = 0;
	//	for (i = 2; i <= 1000; i++)
	//	{
	//		if (a[i] != 0)
	//		{
	//			b[j++] = a[i];
	//		}
	//	}
	//}

	//int main()
	//{
	//	int a[1001] = { 0 }, b[1001] = { 0 };
	//	prime(a, b);
	//	int n, i, j, m = 0;
	//	scanf("%d", &n);
	//	int c[20] = { 0 };
	//	int d[40] = { 0 };
	//	for (i = 0; i < n; i++)
	//	{
	//		scanf("%d", &c[i]);
	//	}
	//	for (j = 0; j < n; j++)
	//	{ 
	//		for (i = 1; b[i] != 0; i++)
	//		{
	//			if (c[j] == b[i])
	//			{
	//				d[m++] = c[j];
	//				c[j] = 0;
	//				break;
	//			}
	//		}
	//	}
	//	for (i = 0; i < n; i++)
	//	{
	//		if (c[i] != 0)
	//		{
	//			for (j = 2; b[j] != 0;)
	//			{
	//				if (c[i] % b[j] == 0)
	//				{
	//					c[i] %= b[j];
	//					d[m++] = j;
	//				}
	//				else
	//				{
	//					j++;
	//				}
	//			}
	//		}
	//	}
	//	int count = 0;
	//	for (m = 0; d[m] != 0; m++)
	//	{
	//		count++;
	//	}
	//	for (m = 0; m < count - 1; m++)
	//	{
	//		for (j = 0; j < count - 1 - m; j++)
	//		{
	//			if (d[j] > d[j + 1])
	//			{
	//				int temp = d[j];
	//				d[j] = d[j + 1];
	//				d[j + 1] = temp;
	//			}
	//		}
	//	}
	//	for (j = 0; j < count - 1; j++)
	//	{
	//		if (d[j] != d[j + 1])
	//		{
	//			printf("%d ", d[j]);
	//		}
	//	}
	//	printf("%d", d[j]);
	//	return 0;
	//}