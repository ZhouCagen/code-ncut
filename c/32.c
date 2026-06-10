//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>

//void charge(int* a)
//{
//	int min = a[0], m=0, n = 0;
//	for (int i = 0; i < 10; i++)
//	{
//
//		if (a[i] < min)
//		{
//			min = a[i];
//			n = i;
//		}
//	}
//	int temp2 = a[n];
//	a[n] = a[0];
//	a[0] = temp2;
//	int max = a[0];
//	for (int i = 0; i < 10; i++)
//	{
//	    if (a[i] > max)
//	    {
//		max = a[i];
//		m = i;
//	    }
//	}
//	int temp1 = a[m];
//	a[m] = a[9];
//	a[9] = temp1;
//	for (int j = 0; j < 10; j++)
//	{
//		printf("%d ", a[j]);
//	}
//
//}
//
//int main()
//{
//	int a[10] = { 0 };
//	for (int i = 0; i < 10; i++)
//	{
//		scanf("%d", &a[i]);
//	}
//	charge(a);
//	return 0;
//}