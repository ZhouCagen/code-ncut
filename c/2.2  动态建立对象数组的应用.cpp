////【问题描述】动态建立一个对象数组，包含5个学生的数据(学号、1门成绩)。利用指向数组的指针，统计平均成绩并输出第1、3、5个学生的数据。
////【输入形式】输入由成员函数完成。依次输入5个学生的学号和成绩。各数据用空格间隔。
////【输出形式】输出占4行。第一行为平均成绩；以下三行为第1、3、5个学生的数据。保留小数点后2位。
////【样例输入】1001 90.5 1002 86.0 1003 92.5 1004 98.0 1005 80
////【样例输出】89.40
////
////1001 90.50
////
////1003 92.50
////
////1005 80.00
////【样例说明】5个学生的学号和成绩分别为：1001、90.5；1002、86；1003、92.5；1004、98.0；1005、80。平均成绩为89.40；第1、3、5个学生的数据为：1001、90.50；1003、92.50；1005、80.00。
////【评分标准】本题共10分，2个测试点，每个测试点5分。
////
////【编程说明】本题必须使用类，在main()函数中创建对象。非基于对象的程序不得分。

//#include<iostream>
//#include<iomanip>
//using namespace std;
//class Student
//{
//public:
//	int id;
//	double score;
//	void cinstudent()
//	{
//		cin >> id >> score;
//	}
//};
//int main()
//{
//	Student* student = new Student[5];
//	double sum = 0;
//	for (int i = 0; i < 5; i++)
//	{
//		student[i].cinstudent();
//		sum += student[i].score;
//	}
//	double average = sum / 5;
//	cout << setiosflags(ios::fixed) << setprecision(2) << average << endl;
//	cout << student[0].id << " " << setiosflags(ios::fixed) << setprecision(2) << student[0].score << endl;
//	cout << student[2].id << " " << setiosflags(ios::fixed) << setprecision(2) << student[2].score << endl;
//	cout << student[4].id << " " << setiosflags(ios::fixed) << setprecision(2) << student[4].score << endl;
//	return 0;
//}