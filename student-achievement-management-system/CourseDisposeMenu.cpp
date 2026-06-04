#include "CourseDisposeMenu.h"

void CourseDisposeMenu()
{
	while (1)
	{
		std::cout << "确认是否选择修改/删除课程基本信息内容（Yes/No）" << std::endl;
		std::string Coursedetermine;
		std::getline(std::cin, Coursedetermine);

		RefreshScreenBuffer();

		if (Coursedetermine == "yes" || Coursedetermine == "Yes")
		{
			while (1)
			{
				std::cout << "将查找需修改/删除的课程基本信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
				std::cout << "请输入课程编号" << std::endl;

				std::string Coursenumber;
				std::getline(std::cin, Coursenumber);

				RefreshScreenBuffer();

				std::cout << "请输入课名" << std::endl;

				std::string Coursename;
				std::getline(std::cin, Coursename);

				RefreshScreenBuffer();

				std::cout << "请输入任课教师" << std::endl;
				
				std::string Courseteacher;
				std::getline(std::cin, Courseteacher);

				RefreshScreenBuffer();

				std::vector<CourseInformation>Courses;

				CourseInformation Courseinformation(Coursenumber, Coursename,Courseteacher);

				Courseinformation.LoadCoursesFromFile(Courses);

				Courseinformation.isIDNameExist(Coursenumber, Coursename, Courseteacher, Courses);

				CourseInformation::SaveToFile(Courses);

				break;
			}
		}
		else if (Coursedetermine == "no" || Coursedetermine == "No")
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