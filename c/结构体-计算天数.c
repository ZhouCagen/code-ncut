//问题描述
//
//定义一个结构体变量（包括年、月、日）。输入某年某月某日，计算该日在本年中是第几天，注意闰年问题。
//
//输入形式
//
//输入一行，包括三个正整数，表示某年某月某日。
// 
//输出形式
// 
//输出一行，输出这一天是这一年的第几天。
//样例输入
//2013 3 5
//样例输出
//64


//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//struct date
//{
//	int year;
//	int month;
//	int day;
//};
//int main()
//{
//	struct date date1 = { 0,0,0 };
//	scanf("%d %d %d", &date1.year, &date1.month, &date1.day);
//	int month1[12] = { 0,31,28,31,30,31,30,31,31,30,31,30 };
//	int i, sum1 = 0;
//	if ((date1.year % 4 == 0 && date1.year % 100 != 0) || date1.year % 400 == 0)
//	{
//		month1[2] = 29;
//	}
//	for (i = 0; i < date1.month; i++)
//	{
//		sum1 += month1[i];
//	}
//	int num = sum1 + date1.day; 
//	//if (((date1.year % 4 == 0 && date1.year % 100 != 0) || date1.year % 400 == 0) && date1.month > 2)
//	//{
//	//	num++;
//	//}
//	printf("%d", num);
//	return 0;
//}