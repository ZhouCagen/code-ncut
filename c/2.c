//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//int year, month, day, sum;
//int main()
//{
// 	scanf("%d%d%d", &year, &month, &day);
//	switch (month)
//	{
//	case 1:
//		sum = 0 + day; break;
//	case 2:
//		sum = 31 + day; break;
//	case 3:
//		sum = 31 + 28 + day; break;
//	case 4:
//		sum = 31 + 28 + 31 + day; break;
//	case 5:
//		sum = 31 + 28 + 31 + 30 + day; break;
//	case 6:
//		sum = 31 + 28 + 31 + 30 + 31 + day; break;
//	case 7:
//		sum = 31 + 28 + 31 + 30 + 31 + 30 + day; break;
//	case 8:
//		sum = 31 + 28 + 31 + 30 + 31 + 30 + 31 + day; break;
//	case 9:
//		sum = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + day; break;
//	case 10:
//		sum = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + day; break;
//	case 11:
//		sum = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 30 + 31 + day; break;
//	case 12:
//		sum = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 30 + 31 + 30 + day; break;
//	}
//	if (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) && month > 2)
//		sum = sum + 1;
//	printf("%d\n", sum);
//	return 0;
//}