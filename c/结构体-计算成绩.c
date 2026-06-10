//问题描述
//
//有n（不大于20）个学生，每个学生的数据包括学号（字符串长度不超过5）、姓名（字符串长度不超过10）、3门课的成绩（整数），
//从键盘输入n个学生的数据，要求输出3门课的总平均成绩，以及每个学生的数据（包括学号、姓名、3门课成绩、平均成绩）
//
//输入形式
//
//输入n + 1行，第1行输入n，表示有n个学生。从第2行到第n + 1行输入每个学生的数据（包括学号、姓名、3门课的成绩）
//
//输出形式
//
//输出n + 1行。
//
//第1行输出3门课的总平均成绩，保留小数点后两位。
//
//从第2行到第n + 1行输入每个学生的数据，包括学号、姓名、3门课的成绩、平均成绩（保留小数点后两位），数据输出所占格式为"%5s%10s%5d%5d%5d%8.2f";
//
//样例输入
//3
//001 zhang 70 80 90
//002 li 89 90 100
//003 zhe 60 70 80
// 
//样例输出
//81.00
//001     zhang   70   80   90   80.00
//002        li   89   90 100   93.00
//003       zhe   60   70   80   70.00

//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//struct people
//{
//	char xh[5];
//	char xm[10];
//	int score[3];
//};
//int main()
//{
//	struct people *s;
//	int n, i, j;
//	scanf("%d", &n);
//	while (n > 20 && n <= 0)
//	{
//		scanf("%d", &n);
//	}
//	s = (struct people*)malloc(n * sizeof(struct people));
//	for (i = 0; i < n; i++)
//	{
//		scanf("%s %s", s[i].xh,s[i].xm);
//		for (j = 0; j < 3; j++)
//		{
//			scanf("%d", &s[i].score[j]);
//		}
//	}
//	float* ave = (float*)malloc(n * sizeof(float));
//	for (i = 0; i < n; i++)
//	{
//		float sum = 0;
//		for (j = 0; j < 3; j++)
//		{
//			sum += s[i].score[j];
//		}
//		ave[i] = sum / 3;
//	}
//	float sum1 = 0;
//	for (i = 0; i < n; i++)
//	{
//		sum1 += ave[i];
//	}
//	float AVE = sum1 / n;
//	printf("%.2f\n", AVE);
//	for (i = 0; i < n; i++)
//	{
//		printf("%5s%10s", s[i].xh, s[i].xm);
//		for (j = 0; j < 3; j++)
//		{
//			printf("%5d", s[i].score[j]);
//		}
//		printf("%8.2f\n", ave[i]);
//	}
//	return 0;
//}