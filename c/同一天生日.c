//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//struct Student
//{
//	char xuehao[11];
//	int month;
//	int day;
//};
//struct Student stu[200];
//int main()
//{
//	int n,i,j;
//	scanf("%d", &n);
//	for (i = 0; i < n; i++)
//	{
//		scanf("%s %d %d",stu[i].xuehao,&stu[i].month,&stu[i].day);
//	}
//	for (i = 0; i < n - 1; i++)
//	{
//		for (j = 0; j < n - i - 1; j++)
//		{
//			if (stu[j].month > stu[j + 1].month || stu[j].month == stu[j + 1].month && stu[j].day > stu[j + 1].day)
//			{
//				struct Student temp = stu[j];
//				stu[j] = stu[j + 1];
//				stu[j + 1] = temp;
//			}
//
//		}
//	}
//	for (i = 0; i < n; i++)
//	{
//		if (stu[i].month == stu[i + 1].month && stu[i].day == stu[i + 1].day)
//		{
//			printf("%d %d ", stu[i].month, stu[i].day);
//			for (j = i; j < n && stu[i].month == stu[j].month && stu[i].day == stu[j].day;j++)
//			{
//				printf("%s ", stu[j].xuehao);
//			}
//			i = j - 1;
//			printf("\n");
//		}
//		else
//		{
//			printf("%d %d %s\n", stu[i].month, stu[i].day, stu[i].xuehao);
//		}
//	}
//	return 0;
//
//}