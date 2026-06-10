//#include<iostream>
//#include<cmath>
//#include<iomanip>
//using namespace std;
//class Shape
//{
//public:
//	virtual double Area() = 0;
//	virtual double Length() = 0;
//	virtual void printArea()
//	{
//		cout << setiosflags(ios::fixed) << setprecision(2) << Area() << endl;
//	}
//	virtual void printLength()
//	{
//		cout << setiosflags(ios::fixed) << setprecision(2) << Length() << " ";
//	}
//};
//class Circle :public Shape
//{
//public:
//	Circle(double radius) :m_radius(radius) {}
//	virtual double Area()
//	{
//		return 3.14 * pow(m_radius, 2);
//	}
//	virtual double Length()
//	{
//		return 2 * 3.14 * m_radius;
//	}
//private:
//	double m_radius;
//};
//
//class Square :public Shape
//{
//public:
//	Square(double side):m_side(side){}
//	virtual double Area()
//	{
//		return pow(m_side, 2);
//	}
//	virtual double Length()
//	{
//		return 4 * m_side;
//	}
//private:
//	double m_side;
//};
//
//class Rectangle :public Shape
//{
//public:
//	Rectangle(double width,double length):m_width(width),m_length(length){}
//	virtual double Area()
//	{
//		return m_width * m_length;
//	}
//	virtual double Length()
//	{
//		return 2 * (m_width + m_length);
//	}
//private:
//	double m_width;
//	double m_length;
//};
//
//void printShape(Shape* shape)
//{
//	shape->printLength();
//	shape->printArea();
//	return;
//}
//int main()
//{
//	Circle cir(4);
//	printShape(&cir);
//	Square squa(5);
//	printShape(&squa);
//	Rectangle rect(2, 5);
//	printShape(&rect);
//	return 0;
//}