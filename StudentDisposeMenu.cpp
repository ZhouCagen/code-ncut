#include "StudentDisposeMenu.h"

void StudentDisposeMenu()
{
	while (1)
	{
		std::cout << "确认是否选择修改/删除学生基本信息内容（Yes/No）" << std::endl;
		std::string Studentdetermine;
		std::getline(std::cin, Studentdetermine);

		RefreshScreenBuffer();

		if (Studentdetermine == "yes" || Studentdetermine == "Yes")
		{
			while (1)
			{
				std::cout << "将查找需修改/删除的课程基本信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
				std::cout << "请输入学号" << std::endl;

				std::string Studentid;
				std::getline(std::cin, Studentid);

				RefreshScreenBuffer();

				std::cout << "请输入姓名" << std::endl;

				std::string Studentname;
				std::getline(std::cin, Studentname);

				RefreshScreenBuffer();

				std::string Studentgender;
				std::string Studentage;
				std::string Studentsystem;

				std::vector<StudentInformation>Students;

				StudentInformation Studentinformation(Studentid, Studentname, Studentgender, Studentage, Studentsystem);

				Studentinformation.LoadStudentsFromFile(Students);

				Studentinformation.isIDNameExist(Studentid, Studentname, Students);

				StudentInformation::SaveToFile(Students);

				break;
			}
		}
		else if (Studentdetermine == "no" || Studentdetermine == "No")
		{
			std::cout << "确认成功，已返回菜单" << std::endl;
			std::cout << "------------------------------------" << std::endl;
			break;
		}
		else
		{
			std::cout << "确认失败，请重新确认" << std::endl;
			std::cout << "------------------------------------" << std::endl;
		}
	}
}
