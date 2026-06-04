////#define _CRT_SECURE_NO_WARNINGS
////#include<stdio.h>
////#include<stdlib.h>
////void count(int a[], int n, int* max, int* min, float* ave)
////{
////	*max = a[0];
////	*min = a[0];
////	int i;
////	float sum = 0;
////	for (i = 0; i < n; i++)
////	{
////		*max = *max > a[i] ? *max : a[i];
////		*min = *min < a[i] ? *min : a[i];
////		sum += a[i];
////	}
////	*ave = sum / n;
////	return;
////}
////int main()
////{
////	int n, i;
////	scanf("%d", &n);
////	int* a = (int*)malloc(n * sizeof(int));
////	for (i = 0; i < n; i++)
////	{
////		scanf("%d", &a[i]);
////	}
////	int max = 0, min = 0;
////	float ave = 0;
////	count(a, n, &max, &min, &ave);
////	printf("%d %d %.2f", max, min, ave);
////	return 0;
////}
//
//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//struct student
//{
//	char number[10];
//	int a[4];
//	int sum;
//};
//int main()
//{
//	int n, i, j;
//	scanf("%d", &n);
//	struct student* s = (struct student*)malloc(n * sizeof(struct student));
//	for (i = 0; i < n; i++)
//	{
//		scanf("%s", s[i].number);
//		for (j = 0; j < 4; j++)
//		{
//			scanf("%d", &s[i].a[j]);
//		}
//	}
//	for (int z = 0; z < n; z++)
//	{
//		for (i = 0; i < 3; i++)
//		{
//			for (j = 0; j < 3 - i; j++)
//			{
//				if (s[z].a[j] < s[z].a[j + 1])
//				{
//					int temp = s[z].a[j];
//					s[z].a[j] = s[z].a[j + 1];
//					s[z].a[j + 1] = temp;
//				}
//			}
//		}
//	}
//	for (i = 0; i < n; i++)
//	{
//		s[i].sum = 0;
//		for (j = 0; j < 3; j++)
//		{
//			s[i].sum += s[i].a[j];
//		}
//	}
//	for (i = 0; i < n - 1; i++)
//	{
//		for (j = 0; j < n - 1 - i; j++)
//		{
//			if (s[j].sum < s[j + 1].sum)
//			{ 
//				struct student temp = s[j];
//				s[j] = s[j + 1];
//				s[j + 1] = temp;
//			}
//		}
//	}
//	for (i = 0; i < n; i++)
//	{
//		printf("%-10s%-6d\n", s[i].number, s[i].sum);
//	}
//	return 0;
//	
//
//}