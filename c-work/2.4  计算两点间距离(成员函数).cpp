////【问题描述】在平面直角坐标系中，点类Point包括两个数据成员：x(横坐标)，y(纵坐标)；若干成员函数，要求使用成员函数计算两点间距离。
//// 设计main()函数，以点(0，0)和(3，4)作为测试数据，测试程序。
////【输入形式】本题没有输入，初始化工作由构造函数完成。
////【输出形式】输出占一行，为给定的两点间距离。保留2位小数。
////【样例输入】无
////【样例输出】5.00
////【样例说明】两点(0, 0)和(3, 4)之间的距离为5.00。
////【评分标准】本题共15分，1个测试点。
////【编程说明】本题必须使用类，在main()函数中按照给定的数值创建对象，通过调用函数完成所需计算。 非基于对象的程序且未按照要求完成不得分。
//
//#include<iostream>
//#include<cmath>
//#include<iomanip>
//using namespace std;
//class Point
//{
//	friend const double operator-(const Point& a, const Point& b);
//private:
//	double x, y;
//public:
//	Point(double x = 0, double y = 0) :x(x), y(y) {}
//};
//const double operator-(const Point& a, const Point& b)
//{
//	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
//}
//int main()
//{
//	Point a(0, 0), b(3, 4);
//	cout << setiosflags(ios::fixed) << setprecision(2) << a - b << endl;
//	return 0;
//}