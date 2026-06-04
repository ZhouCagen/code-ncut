////【问题描述】编写一个基于对象的程序，求长方体的体积。实现提示：类中数据成员包括length(长)、width(宽)、height(高)；成员函数至少完成以下功能：①初始化3个长方体；②计算长方柱的体积；
////
////③输出3个长方柱的体积。设计main()函数，测试程序。已知的三个长方体长宽高分别为：10.2、3.1、5.6；11.21、9.22、13.5；5.1、3.4、6.5。
////【输入形式】本题没有输入，初始化工作由构造函数完成。
////【输出形式】输出占一行，共3个数据，分别为三个长方体的体积。保留3位小数(以小数形式输出的控制符为setiosflags(ios::fixed)，输出n位小数的控制符为setprecision(n)，需要打开“iomanip”头文件)。
////【样例输入】无
////
////【样例输出】177.072  1395.309  112.710
////
////【样例说明】按照给定的长宽高创建对象，其中长宽高为10.2、3.1、5.6的长方体体积是177.072；长宽高为11.21、9.22、13.5的长方体体积是1395.309；长宽高为5.1、3.4、6.5的长方体体积是112.710。
////
////【评分标准】本题10分，共1个测试点。
////【编程说明】本题必须使用类，在main()函数中按照给定的数值创建对象，通过调用函数完成体积的计算并输出结果。非基于对象的程序不得分。
//
//#include<iostream>
//#include<iomanip>
//using namespace std;
//class Cuboid
//{
//private:
//	double length, width, height;
//public:
//	Cuboid(double l, double w, double h) :length(l), width(w), height(h){}
//	double volume() const
//	{
//		return length * width * height;
//	}
//	void coutvolume() const
//	{
//		cout << setiosflags(ios::fixed) << setprecision(3) << volume() << " ";
//	}
//};
//int main()
//{
//	Cuboid cuboid1(10.2, 3.1, 5.6);
//	Cuboid cuboid2(11.21, 9.22, 13.5);
//	Cuboid cuboid3(5.1, 3.4, 6.5);
//	cuboid1.coutvolume();
//	cuboid2.coutvolume();
//	cuboid3.coutvolume();
//	return 0;
//}