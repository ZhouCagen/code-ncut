//#include<iostream>
//#include<cmath>
//using namespace std;
//class Point
//{
//public:
//	Point(double x, double y) :x_Point(x), y_Point(y) {}
//	double x_Point, y_Point;
//};
//class Circle
//{
//public:
//	Circle(Point circle, double radius) :m_Circle(circle), m_Radius(radius){}
//	Point m_Circle;
//	double m_Radius;
//};
//
//const double operator -(Point& p1, Point& p2)
//{
//	return sqrt(pow(p2.x_Point - p1.x_Point, 2) + pow(p2.y_Point - p1.y_Point, 2));
//}
//
//bool isOnCircle(Point& p, Circle& circle)
//{
//	if ((p - circle.m_Circle) == circle.m_Radius)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//int main()
//{
//	Circle circle(Point(1, 2), 5);
//	Point point(6, 2);
//	cout << isOnCircle(point, circle) << endl;
//	return 0;
//}