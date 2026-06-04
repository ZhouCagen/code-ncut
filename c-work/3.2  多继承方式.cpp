////【问题描述】分别定义Teacher(教师)类和Cadre(干部)类，采用多重继承方式由这两个类派生出新类Teacher_Cadre(教师兼干部)。要求：
////① 在两个基类中都包含姓名、出生日期(日期类Date的子对象)、性别等数据成员。
////② 在Teacher类中还包含数据成员title(职称)，在Cadre类中还包含数据成员post(职务)， 在Teacher_Cadre类中还包含数据成员wages(工资)。
////③ 对两个基类中的姓名、出生日期、性别等数据成员用相同的名字，在引用这些数据成员时，指定作用域。
////④ 在类体中声明成员函数，在类外定义成员函数。
////⑤ 在派生类Teacher_Cadre的成员函数show()中调用Teacher类中的display()函数
////   输出姓名、出生日期、性别、职称，然后再用cout语句输出职务与工资。设计main()完成测试。
////【输入形式】无
////【输出形式】派生类对象的各个数据成员的值，彼此以空格间隔。
////【样例输入】无
////【样例输出】LiHong 1967 / 10 / 12 f  professor Header 8000.00
////【样例说明】以数据："LiHong", 1967, 10, 12, 'f', "professor", "Header", 8000.00初始化Teacher_Cadre类对象，在main()中测试show()函数输出有关数据。
//
//#include<iostream>
//#include<string>
//using namespace std;
//class Date
//{
//public:
//	Date(int year, int month, int day): m_year(year), m_month(month), m_day(day) {}
//	void show()
//	{
//		cout << m_year << "/" << m_month << "/" << m_day;
//	}
//	int m_year;
//	int m_month;
//	int m_day;
//};
//class Teacher
//{
//public:
//	int m_Name;
//	Date date_Teacher;
//	string m_title;
//	void display();
//};
//
//class Cadre
//{
//public:
//	int m_Name;
//	Date date_Cadre;
//	string m_post;
//	void display();
//};
//
//class Teacher_Cadre :public Teacher, public Cadre
//{
//public:
//	Teacher_Cadre();
//	void show()
//	{
//		Teacher display();
//	}
//	double wages;
//};ss
//void Teacher::display()
//{
//	cout << "1" << endl;
//}
//
//void Cadre::display()
//{
//
//}
//
//int main()
//{
//	Teacher_Cadre teacher_cadre;
//	return 0;
//}