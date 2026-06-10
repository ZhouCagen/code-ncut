////1、编写一个基于对象的程序：计算圆的周长和面积并输出。
////实现提示：
////①数据成员包括圆的半径 radius；
////②成员函数包括构造函数，计算周长： length()，计算面积：area()。
////③main()函数中以半径值 10.0 创建一个对象，分别输出该对象的周长和面积。
// 
//include<iostream>
//#include<cmath>
//#include<iomanip>
//using namespace std;
//#define Pi acos(-1);
//class Circle
//{
//public:
//	Circle(double radius) :m_radius(radius) {}
//	double length()
//	{
//		return 2 * m_radius * Pi;
//	}
//	double area()
//	{
//		return pow(m_radius, 2) * Pi;
//	}
//private:
//	double m_radius;
//};
//int main()
//{
//	Circle circle(10.0);
//	cout << "area = " << setiosflags(ios::fixed) << setprecision(2) << circle.area() << endl;
//	cout << "length = " << setiosflags(ios::fixed) << setprecision(2) << circle.length() << endl;
//}

////2、需要求 3 个长方柱的体积，请编写一个基于对象的程序。数据成员包括 length(长)、
////width(宽)、height(高)。要求实现以下功能：
////①构造函数；
////②计算长方柱的体积；
////③输出 3 个长方柱的体积。
//
//#include<iostream>
//#include<iomanip>
//using namespace std;
//class Cuboid
//{
//public:
//	Cuboid(double l, double w, double h) :length(l), width(w), height(h) {}
//	double volume()
//	{
//		return length * width * height;
//	}
//	void print()
//	{
//		cout << setiosflags(ios::fixed) << setprecision(2) << volume() << endl;
//	}
//private:
//	double length, width, height;
//};
//int main()
//{
//	double a, b, c;
//	for (int i = 1; i <= 3; i++)
//	{
//		cin >> a >> b >> c;
//		Cuboid cuboid(a, b, c);
//		cuboid.print();
//	}
//	return 0;
//}

////3、声明一个人员类(Person)，包括 3 个数据成员：name(姓名)、age(年龄)、sex(性别)；2 个成员函数：构造函数和输出相关信息的函数 display()。
////利用单继承的方式声明一个学生(Student)派生类，其中增加 2 个数据成员：grade(年级)、score(总学分)；3 个成员函数：构造函数、输出函数 show()
////和增加学分的函数 add()。在定义派生类对象时给出初始化的数据，然后输出这些数据。
// 
//#include<iostream>
//#include<iomanip>
//#include<string>
//using namespace std;
//class Person
//{
//public:
//	Person(string m_name, int m_age, string m_sex) :name(m_name), age(m_age), sex(m_sex) {}
//	void display()
//	{
//		cout << "name = " << name << endl << "age = " << age << endl << "sex = " << sex << endl;
//	}
//private:
//	string name;
//	int age;
//	string sex;
//};
//class Student :public Person
//{
//public:
//	Student(string m_name, int m_age, string m_sex, int m_grade, double m_score,double m_addscore) :Person(m_name, m_age, m_sex), grade(m_grade), score(m_score),addscore(m_addscore) {}
//	void show()
//	{
//		display();
//		cout << "grade = " << grade << endl << "score = " << Addscore() << endl;
//	}
//	double Addscore()
//	{
//		return score += addscore;
//	}
//private:
//	int grade;
//	double score;
//	double addscore;
//};
//
//int main()
//{
//	Student student("XiaoMing", 18, "man", 5, 20, 2.8);
//	student.show();
//	return 0;
//}

////4、写一个程序，定义抽象基类 Shape，由它派生出 3 个派生类：Circle(圆形)、Square(正方形)、Rectangle(矩形)。
////利用指针、虚函数 printArea()、printLength()分别计算以上三者的面积和周长，三个图形的数据在定义对象时给定。
//// 另外实现一个通用的函数输出面积和周长，而不需要考虑是何种图形。
// 
//#include<iostream>
//#include<cmath>
//#include<iomanip>
//using namespace std;
//#define Pi acos(-1)
//class Shape
//{
//public:
//	virtual double printArea() = 0;
//	virtual double printLength() = 0;
//	void showArea()
//	{
//		cout << "Area =" << printArea() << endl;
//	}
//	void showLength()
//	{
//		cout << "Length = " << printLength() << endl;
//	}
//};
//class Circle :public Shape
//{
//public:
//	Circle(double m_radius) :radius(m_radius) {}
//	double printArea()
//	{
//		return Pi * pow(radius, 2);
//	}
//	double printLength()
//	{
//		return Pi * 2 * radius;
//	}
//private:
//	double radius;
//};
//class Square :public Shape
//{
//public:
//	Square(double m_radius) :radius(m_radius) {}
//	double printArea()
//	{
//		return pow(radius, 2);
//	}
//	double printLength()
//	{
//		return 4 * radius;
//	}
//private:
//	double radius;
//};
//class Rectangle :public Shape
//{
//public:
//	Rectangle(double m_length, double m_width) :length(m_length), width(m_width) {}
//	double printArea()
//	{
//		return length * width;
//	}
//	double printLength()
//	{
//		return 2 * (length + width);
//	}
//private:
//	double length, width;
//};
//void doWork(Shape *shape)
//{
//	shape->showArea();
//	shape->showLength();
//}
//int main()
//{
//	Shape* circle = new Circle(2);
//	doWork(circle);
//	Shape* square = new Square(3);
//	doWork(square);
//	Shape* rectangle = new Rectangle(4, 6);
//	doWork(rectangle);
//	return 0;
//}

//////5、要求正方体、球、圆柱的表面积，可以抽象出一个公共的基类 Container 为抽象类在其中定义一个公共的数据成员 radius(此数据可以作为正方形的边长、球的半径、圆柱体底面圆半径)
//////以及求表面积的纯虚函数 area()。由此抽象类派生出要描述的三个类，利用基类指针和虚函数，分别计算三种图形的表面积。
//#include<iostream>
//#include<cmath>
//using namespace std;
//#define Pi acos(-1)
//class Container
//{
//public:
//	Container(double m_radius) :radius(m_radius) {}
//	virtual double area() = 0;
//	void printarea()
//	{
//		cout << "Area = " << area() << endl;
//	}
//	double radius;
//};
//class Cube :public Container
//{
//public:
//	Cube(double m_side) :Container(m_side) {}
//	double area()
//	{
//		return 6 * pow(radius, 2);
//	}
//};
//class Global:public Container
//{
//public:
//	Global(double m_r):Container(m_r){}
//	double area()
//	{
//		return 4 * Pi * pow(radius, 2);
//	}
//};
//class Cylinder :public Container
//{
//public:
//	Cylinder(double m_r,double m_height):Container(m_r),height(m_height){}
//	double area()
//	{
//		return 2 * Pi * pow(radius, 2) + 2 * Pi * radius * height;
//	}
//private:
//	double height;
//};
//void doWork(Container* container)
//{
//	container->printarea();
//	return;
//}
//int main()
//{
//	Container* cube = new Cube(2.0);
//	doWork(cube);
//	Container* global = new Global(3.0);
//	doWork(global);
//	Container* cylinder = new Cylinder(4.0, 2.0);
//	doWork(cylinder);
//	return 0;
//}

////6、定义 Point 类描述点，并加入一个友元函数计算两点间距离。利用 Point 类设计描述圆的Circle 类，包括圆心（Point 类的点）、半径，
////   构造函数初始化圆，成员函数判断某个点 m 是否在圆周上。设计 main()函数，以(1，2)为圆心、5 为半径、m 为(6，2)进行测试。
//
//#include<iostream>
//#include<cmath>
//using namespace std;
//class Point
//{
//public:
//	Point(int x_Point, int y_Point) :x(x_Point), y(y_Point) {}
//	Point() {};
//	int x, y;
//};
//class Circle :public Point
//{
//public:
//	Circle(Point m_circle, int m_radius) :circle(m_circle), radius(m_radius) {}
//	Point circle;
//	int radius;
//};
//const double operator -(Point& a, Point& b)
//{
//	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
//}
//void isOnCircle(Point &p,Circle &c)
//{
//	if (p - c.circle == c.radius)
//	{
//		cout << "该点在圆上" << endl;
//	}
//	else if (p - c.circle > c.radius)
//	{
//		cout << "该点在圆外" << endl;
//	}
//	else
//	{
//		cout << "该点在圆内" << endl;
//	}
//}
//int main()
//{
//	Circle circle(Point(1, 2), 5);
//	Point point(6, 2);
//	isOnCircle(point, circle);
//	return 0;
//}