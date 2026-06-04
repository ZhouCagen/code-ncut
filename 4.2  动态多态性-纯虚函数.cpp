//#include<iostream>
//#include<iomanip>
//#include<cmath>
//using namespace std;
//class Container
//{
//public:
//	Container(double r) :radius(r) {}
//	virtual double area() = 0;
//	virtual void printarea()
//	{
//		cout << setiosflags(ios::fixed) << setprecision(2) << area() << " ";
//	}
//	double radius;
//};
//class Cube :public Container
//{
//public:
//	Cube(double side) :Container(side) {}
//	virtual double area()
//	{
//		return 6 * pow(radius, 2);
//	}
//};
//class Global:public Container
//{
//public:
//	Global(double r) :Container(r) {}
//	virtual double area()
//	{
//		return 4 * 3.14 * pow(radius, 2);
//	}
//};
//class Cylinder :public Container
//{
//public:
//	Cylinder(double r, double h) :Container(r), height(h) {}
//	virtual double area()
//	{
//		return 2 * 3.14 * pow(radius, 2) + 2 * 3.14 * radius * height;
//	}
//private:
//	double height;
//};
//
//void printContainer(Container* container)
//{
//	container->printarea();
//}
//int main()
//{
//	Container* cube = new Cube(6);
//	printContainer(cube);
//	Container* global = new Global(5);
//	printContainer(global);
//	Container* cylinder = new Cylinder(5, 16);
//	printContainer(cylinder);
//	return 0;
//}