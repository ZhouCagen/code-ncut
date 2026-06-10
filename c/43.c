//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<math.h>
//int main()
//{
//	char arr[100];
//	char ch = '\0';
//	int m = 0, b = 0;
//	int a[100] = { 0 };
//	while ((ch = getchar()) && ch != '\n')
//	{
//		arr[m++] = ch;
//	}
//	arr[m] = '\0';
//	for (int i = 0; arr[i] != '\0'; i++)
//	{
//		if (arr[i] >= '0' && arr[i] <= '9')
//		{
//			int len = 1;
//			int j, sum = 0;
//			for (j = i + 1; arr[j] >= '0' && arr[j] <= '9'; j++)
//			{
//				len++;
//			}
//			for (int n = i; n < j; n++)
//			{
//				sum += arr[n] * (int)pow(10, len + i - n - 1);
//			}
//			a[b++] = sum;
//			i = j;
//		}
//	}
//	printf("%d ", b + 1);
//	for (int i = 0; i < b; i++)
//	{
//		printf("%d ", a[i]);
//	}
//	return 0;
//}