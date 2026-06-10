//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//int main()
//{
//	int a[3] = { 0 };
//	int i,j;
//	int* p = a;
//	for (i = 0; i < 3; i++)
//	{
//		scanf("%d", &a[i]);
//	}
//	for (i = 0; i < 2; i++)
//	{
//		for (j = 0; j < 2 - i; j++)
//		{
//			if (*(p + j) > *(p + j + 1))
//			{
//				int temp = *(p + j);
//				*(p + j) = *(p + j + 1);
//				*(p + j + 1) = temp;
//			}
//		}
//	}
//	for (p = a; p < (a + 3); p++)
//	{
//		printf("%d ", *p); 
//	}
//	return 0;
//}