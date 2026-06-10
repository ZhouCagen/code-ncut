////【问题描述】人员类是抽象程度更高的类，它是所有人的共性的抽象。现声明一个人员类（Person ），
//// 包括3个数据成员：name（姓名）、age（年龄）、sex（性别）；2个成员函数：构造函数和输出相关信息的函数display()。
//// 利用单继承的方式声明一个学生（Student）派生类，其中增加2个数据成员：grade（年级）、score（总学分）；
//// 3个成员函数：构造函数、输出函数show()和增加学分的函数add()。add()原型为：void add(float s)，其中，s表示某门课的学分，
//// 将s加入到总学分score中。要求：在main()中定义派生类对象，
//// 初始化的数据为：姓名"Xu Li"，年龄20，性别'f'，年级2，总学分25.4，某门课的学分为2.6，按照要求测试show()、add()函数。
////【输入形式】无
////【输出形式】派生类对象的各个数据成员的值，彼此以空格间隔。
////
////【样例输入】无
////【样例输出】Xu Li 20 f 2 28
////
////【样例说明】以给定的值初始化派生类对象，调用add()和show()，调整学分并输出姓名、年龄、性别、年级、总学分。
////【评分标准】本题50分，1个测试点。

//#include<iostream>
//#include<cstring>
//#include<iomanip>
//using namespace std;
//class Person
//{
//public:
//	Person(string name, int age, char sex) :m_Name(name), m_Age(age), m_Sex(sex) {}
//	Person() {}
//	void display()
//	{
//		cout << m_Name << " " << m_Age << " " << m_Sex << " ";
//	}
//	string m_Name;
//	int m_Age;
//	char m_Sex ;
//};
//class Student :public Person
//{
//public:
//	Student(string name,int age,char sex,int grade, float score,float addscore) :Person(name,age,sex),m_Grade(grade), m_Score(score) ,m_AddScore(addscore){}
//	void add()
//	{
//		m_AllScore = m_Score + m_AddScore;
//	}
//	void show()
//	{
//		Person::display();
//		cout << m_Grade << " " << setiosflags(ios::fixed) << setprecision(1) << m_AllScore << " " << endl;
//	}
//	int m_Grade;
//	float m_Score;
//	float m_AllScore;
//	float m_AddScore;
//};
//
//int main()
//{
//	Student student1("Xu Li", 20, 'f', 2, 25.4f, 2.6f);
//	student1.add();
//	student1.show();
//	return 0;
//}