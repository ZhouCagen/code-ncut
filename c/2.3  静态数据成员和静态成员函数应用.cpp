////【问题描述】某商品定价为30元。现在商店在搞促销活动，所有商品都会有固定的折扣，具体折扣当天宣布。现有3个销售员卖此商品。
//// 请根据销售的数量计算出当日此商品的总销售额及平均售价。
//// 设计main()函数，测试程序。本题已知的3个销售员的工号及销售此商品的数量为：1001、5；1002、20；1003、75。
////【输入形式】输入占一行，为当天折扣。
////【输出形式】输出占一行，有两个数。第1个数为当日此商品的总销售额；第2个数为平均售价。保留两位小数，各数据用空格间隔。
////【样例输入】8
////【样例输出】2760.00  27.60
////【样例说明】当天折扣为8 % ，即按原价92 % 销售。当日此商品的总销售额为2760.00；平均售价为27.60。
////【评分标准】本题共20分，2个测试点，每个测试点10分。
////
////【编程说明】本题必须使用类，在main()函数中按照给定的数值创建对象，通过调用函数完成所需计算。
//// 将商品价格、折扣、总销售款和商品销售总件数声明为静态数据成员。适当使用静态成员函数。非基于对象的程序不得分。

//#include<iostream>
//#include<iomanip>
//#include<vector>
//using namespace std;
//class Commondity
//{
//public:
//	static double price;
//	static double discount;
//	static double totalsales;
//	static int totalquantity;
//	static void Discount(double Discount)
//	{
//		discount = Discount / 100;
//	}
//	static void Price(double Price)
//	{
//		price = Price * (1 - discount);
//	}
//	static void Sales(int Totalquantity)
//	{
//		totalsales += Totalquantity * price;
//		totalquantity += Totalquantity;
//	}
//	static void coutcommondity()
//	{
//		double averageprice = totalsales / totalquantity;
//		cout << setiosflags(ios::fixed) << setprecision(2) << totalsales << " " << setiosflags(ios::fixed) << setprecision(2) << averageprice << endl;
//	}
//};
//double Commondity::price = 30.0;
//double Commondity::discount = 0.0;
//double Commondity::totalsales = 0.0;
//int Commondity::totalquantity = 0;
//int main()
//{
//	cin >> Commondity::discount;
//	Commondity::Discount(Commondity::discount);
//	Commondity::Price(30.0);
//	vector<vector<int>>sale = { {1001,5},{1002,20},{1003,75} };
//	for (int i = 0; i < 3; i++)
//	{
//		Commondity::Sales(sale[i][1]);
//	}
//	Commondity::coutcommondity();
//	return 0;
//}