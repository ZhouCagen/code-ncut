//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<math.h>
//int main()
//{
//	int i = 0, b, c, j = -1, h = 0, have = 1;
//	int a[16] = { 0 }, d[17] = { 0 };
//	scanf("%d ", &b);
//	while (b != 0)
//	{
//		a[i++] = b;
//		scanf("%d ", &b);
//	}
//	scanf("%d", &c);
//	while (j < i-1)
//	{
//		j++;
//		d[j] = a[j] < c ? a[j] : c;
//		if (d[j] == c)
//		{
//			have = 0;
//			break;
//		}
//
//	}
//	if (have == 1)
//	{
//		d[i] = c;
//	}
//	else
//	{
//		while (j < i + 1)
//			d[++j] = a[j - 1];
//	}
//	while (h < i + 1)
//		printf("%d ", d[h++]);
//	return 0;
//
//
//
//
//}