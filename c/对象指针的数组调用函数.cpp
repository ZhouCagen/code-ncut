////解释：Test temp[2]用于创建含有两个 Test 对象的数组，每个对象在释放时调用一次析构函数，
////而 Test* p[2]只建立了两个 Test 对象指针的数组，并没有创建 Test 对象，所以不会调用构造
////函数和析构函数。
//
//#include <iostream>
//using namespace std;
//class Test
//{
//public:
//	Test() { cout << "*"; }
//	~Test() { cout << "#"; }
//};
//int main()
//{
//	Test temp[2], * p[2];
//	return 0;
//}

//#include<iostream>
//using namespace std;
//class Student
//{
//public:
//	virtual void study() { cout << "Student::study()" << endl; } //study()是虚函数
//	void study1() { cout << "Student::study1()" << endl; } //study1()不是虚函数
//protected:
//	int num;
//};
//class Student1 : public Student
//{
//public:
//	virtual void study() { cout << "Student1::study()" << endl; }
//	void study1() { cout << "Student1::study1()" << endl; }
//protected:
//	int score;
//};
//class Student2 : public Student
//{
//public:
//	virtual void study() { cout << "Student2::study()" << endl; }
//	void study1() { cout << "Student2::study1()" << endl; }
//protected:
//	int score;
//};
////void f2()
////{
////	Student* p; //p 为基类指针
////	Student s;
////	Student1 s1;
////	Student2 s2;
////	p = &s;
////	p->study1();
////	p = &s1;
////	p->study1();
////	p = &s2;
////	p->study1();
////}
////void f3()
////{
////	Student s;
////	Student1 s1;
////	Student2 s2;
////	Student&r = s; //r 为基类对象引用
////	r.study1();
////	Student& r1 = s1; //r1 为基类对象引用
////	r1.study1();
////	Student& r2 = s2; //r2 为基类对象引用
////	r2.study1();
////}
//void f4()
//{
//	Student* p; //p 为基类指针
//	Student s;
//	Student1 s1;
//	p = &s;
//	p->study();
//	p = &s1;
//	p->study();
//	p = new Student2;
//	p->study();
//}
//void study(Student& s)
//{
//	s.study();
//}
//void f5()
//{
//	Student s;
//	Student1 s1;
//	Student2 s2;
//	study(s);
//	study(s1);
//	study(s2);
//}
//int main()
//{
//	//f2();
//	//f3();
//	f4();
//	cout << endl;
//	f5();
//	return 0;
//}

//#include<iostream>
//using namespace std;
//class Test
//{
//public:
//	Test()
//	{
//		n += 2;
//	}
//	~Test()
//	{
//		n -= 3;
//	}
//	static int getNum()
//	{
//		return n;
//	}
//private:
//	static int n;
//};
//int Test::n = 1;
//int main()
//{
//	Test* p = new Test;
//	delete p;
//	cout << "n = " << Test::getNum() << endl;
//	return 0;
//}
//

////1）说明并定义一个长方形类CRectangle。其数据成员定义为私有，包括左下角点坐标x，y；x方向长度a与y方向长度b。
////定义公有成员函数作为CRectangle的外部接口，包括输入或修改长方形的坐标；输出该长方形坐标、长宽，周长与面积等若干个。
////必须包括以下函数：
////①带参数的构造函数：实现在对象的创建时完成长方形坐标与尺寸的初始化。
////②析构函数：使对象在被构造和析构时向用户发出相应提示信息。
////③length()函数：计算长方形周长。
////④area()函数：计算长方形面积。
////⑤求长方形重心与原点（0, 0）距离的函数distance()将该函数声明为类CRectangle的友元函数通过调用该函数计算长方形类的对象的重心与原点距离。
////⑥重载运算符“ << ”的函数：能输出CRectangle类对象的信息。
//
//#include<iostream>
//#include<cmath>
//#include<iomanip>
//using namespace std;
//class CRectangle
//{
//	friend double distance(CRectangle& crectangle);
//public:
//	CRectangle(double m_x, double m_y, double m_a, double m_b) :x(m_x), y(m_y), a(m_a), b(m_b) {}
//	double length()
//	{
//		return 2 * (a + b);
//	}
//	double area()
//	{
//		return a * b;
//	}
//	~CRectangle(){}
//private:
//	double a, b;
//	double x, y;
//};
//double distance(CRectangle& crectangle)
//{
//	return sqrt(pow(crectangle.x + crectangle.a / 2, 2) + pow(crectangle.y + crectangle.b / 2, 2));
//}
//const ostream& operator << (ostream& cout, CRectangle& p)
//{
//	cout << "length = " << setiosflags(ios::fixed) << setprecision(2) << p.length() << endl << "area = " << p.area() << endl << "distance = " << distance(p) << endl;
//	return cout;
//}
//int main()
//{
//	CRectangle crectangle(200, 150, 2000, 800);
//	cout << crectangle;
//}
////2）创建CRectangle的对象rect，左下角点（200, 150），a＝2000，b＝800。调用类的成员函数或友元函数求该矩形周长、面积、矩形重心与原点距离