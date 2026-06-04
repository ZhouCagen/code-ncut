#include"StudentLoginSystem.h"

void StudentLoginProcedure(std::string& StudentUserName, int& judgestudent)
{
	while (1)
	{
		std::cout << "菜单（输入对应序号即可选择）：" << std::endl;
		std::cout << "1--查询个人信息" << std::endl;
		std::cout << "2--查询课程内容" << std::endl;
		std::cout << "3--查询个人成绩" << std::endl;
		std::cout << "4--退出登录" << std::endl;

		std::string resolution;
		std::getline(std::cin, resolution);

		RefreshScreenBuffer();

		//查询个人信息
		if (resolution == "1")
		{
			std::string studentid;
			std::string studentname;
			std::string studentgender;
			std::string studentage;
			std::string studentsystem;
			std::vector<StudentInformation>Students;

			StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

			Studentinformation.LoadStudentsFromFile(Students);

			std::vector<StudentInformation>::iterator it;

			bool isExist = false;
			for (it = Students.begin(); it != Students.end(); it++)
			{
				if (it->StudentID == StudentUserName)
				{
					isExist = true;

					std::cout << "查询成功" << std::endl;
					std::cout << "------------------------------------" << std::endl;
					std::cout << "学号：" << it->StudentID << std::endl;
					std::cout << "姓名：" << it->StudentName << std::endl;
					std::cout << "性别：" << it->StudentGender << std::endl;
					std::cout << "年龄：" << it->StudentAge << std::endl;
					std::cout << "所在系：" << it->StudentSystem << std::endl;
					std::cout << "------------------------------------" << std::endl;

					break;
				}
			}

			if (isExist == 0)
			{
				std::cout << "信息尚未录入 暂无个人信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}

			StudentInformation::SaveToFile(Students);
		}
		else if (resolution == "2")
		{
			CourseStorageSearch();
		}
		else if (resolution == "3")
		{
			std::string Studentid;
			std::string Classid;
			int usualscore = 0;
			int rollscore = 0;
			int score = 0;

			std::vector<ScoreInformation>Scores;

			ScoreInformation Scoreinformation(Studentid, Classid, usualscore, rollscore, score);

			Scoreinformation.LoadScoresFromFile(Scores);

			std::vector<ScoreInformation>::iterator it;

			bool isQuery = false;

			for (it = Scores.begin(); it != Scores.end(); it++)
			{
				if (it->StudentID == StudentUserName)
				{
					if (isQuery == 0)
					{
						std::cout << "查询成功" << std::endl;
						std::cout << "------------------------------------" << std::endl;
					}

					std::cout << "学号：" << it->StudentID << std::endl;
					//利用学号在"Student.txt"中查找姓名并输出

					std::string studentid;
					std::string studentname;
					std::string studentgender;
					std::string studentage;
					std::string studentsystem;
					std::vector<StudentInformation>Students;

					StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

					Studentinformation.LoadStudentsFromFile(Students);

					std::vector<StudentInformation>::iterator It;


					for (It = Students.begin(); It != Students.end(); It++)
					{
						if (It->StudentID == it->StudentID)
						{
							std::cout << "姓名：" << It->StudentName << std::endl;
							break;
						}
					}
					//查找姓名结束

					std::cout << "课号：" << it->ClassID << std::endl;
					//利用课号在"Course.txt"中查找课名并输出

					std::string coursenumber;
					std::string coursename;
					std::string courseteacher;

					std::vector<CourseInformation>Courses;

					CourseInformation courseinformation(coursenumber, coursename, courseteacher);

					courseinformation.LoadCoursesFromFile(Courses);

					std::vector<CourseInformation>::iterator IT;

					for (IT = Courses.begin(); IT != Courses.end(); IT++)
					{
						if (IT->CourseNumber == it->ClassID)
						{
							std::cout << "课名：" << IT->CourseName << std::endl;
							break;
						}
					}

					std::cout << "成绩：" << it->Score << std::endl;
					std::cout << "------------------------------------" << std::endl;

					isQuery = true;
				}
			}

			if (isQuery == 0)
			{
				std::cout << "暂无成绩" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}

			ScoreInformation::SaveToFile(Scores);

		}
		else if (resolution == "4")
		{
			judgestudent = 0;
			std::cout << "退出成功" << std::endl;
			std::cout << "------------------------------------" << std::endl;
			break;
		}
		else
		{
			std::cout << "选择错误，请重新选择" << std::endl;
			std::cout << "------------------------------------" << std::endl;
		}
	}
}

StudentLoginSystem::StudentLoginSystem(const char* filepath) :StudentFilePath(filepath) {}

//注册时数据库中的账号密码
void StudentLoginSystem::SaveUsersToFile()
{
	std::ofstream studentfile;

	studentfile.open(StudentFilePath);

	if (studentfile.is_open())
	{
		for (auto user = users.begin(); user != users.end(); user++)
		{
			studentfile << user->UserName << " " << user->Password << std::endl;
		}
	}
	else
	{
		std::cout << "存储用户登录数据失败" << std::endl;
	}

	users.clear();

}

//登录读取数据库中的账号密码
void StudentLoginSystem::LoadUsersFromFile()
{
	std::ifstream File;

	File.open(StudentFilePath);

	if (File.is_open())
	{
		std::string Line;

		while (std::getline(File, Line))
		{
			size_t pos = Line.find(' ');
			if (pos != std::string::npos) //std::string::npos 是一个静态成员常量，表示字符串中不存在的位置
			{
				std::string username = Line.substr(0, pos);//substr 函数用于从字符串中提取子串
				std::string password = Line.substr(pos + 1);
				users.emplace_back(username, password);//存储在users的vector容器中

			}
			else
			{
				std::cout << "未成功读取数据库中登录数据的账号密码" << std::endl;
			}
		}
		File.close();

	}
	else
	{
		std::cout << "读取用户登录数据失败" << std::endl;
	}
}

//注册账号密码
bool StudentLoginSystem::RegisterUser(const std::string& username, const std::string& password)
{

	//用于遍历容器中的每个元素 const后不能通过user修改容器中的元素。
	for (const auto user : users)
	{
		if (user.UserName == username)
		{
			return false;//重复注册
		}
	}

	users.emplace_back(username, md5(password));

	SaveUsersToFile();

	return true;//注册成功
	
}

//登录判断
int StudentLoginSystem::isLogin(const std::string& username, const std::string& password)
{
	for (const auto& user : users)
	{
		if (user.UserName == username && user.Password == md5(password))
		{
			return 1;//登录成功
		}
		else if (user.UserName == username && user.Password != md5(password))
		{
			return 2;//密码错误
		}
	}
	return 3;//用户名不存在
}
