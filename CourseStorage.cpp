#include"CourseStorage.h"

void CourseStorage()
{
	std::string coursenumber;
	std::string coursename;
	std::string courseteacher;

	std::vector<CourseInformation>courses;

	CourseInformation courseinformation(coursenumber, coursename, courseteacher);

	std::cout << "请输入课程的基本信息（不支持输入空格）" << std::endl;

	while (1)
	{
		std::cout << "课号：（输入End返回上一级）" << std::endl;

		std::getline(std::cin, coursenumber);

		RefreshScreenBuffer();

		if (coursenumber == "end" || coursenumber == "End")
		{
			std::cout << "返回成功" << std::endl;
			std::cout << "------------------------------------" << std::endl;
			break;
		}

		if (coursenumber[0] != 'P' && coursenumber[0] != 'S')
		{
			std::cout << "输入课号格式错误（课号的第一位号码需为S/P 分别代表专业课/公共基础课）" << std::endl;
			std::cout << "------------------------------------------------------------------------" << std::endl;
			continue;
		}

		if (FindSpace(coursenumber) == 1)
		{
			std::cout << "输入课号格式错误 不支持输入空格" << std::endl;
			continue;
		}

		courseinformation.LoadCoursesFromFile(courses);

		std::cout << "课名：" << std::endl;
		std::getline(std::cin, coursename);

		RefreshScreenBuffer();

		if (FindSpace(coursenumber) == 1)
		{
			std::cout << "输入课名格式错误 不支持输入空格" << std::endl;
			continue;
		}

		std::cout << "任课教师：" << std::endl;
		std::getline(std::cin, courseteacher);

		RefreshScreenBuffer();

		if (FindSpace(courseteacher) == 1)
		{
			std::cout << "输入任课教师格式错误 不支持输入空格" << std::endl;
			continue;
		}

		if (CourseInformation::isCourseExist(coursenumber, coursename, courseteacher, courses) == true)
		{
			std::cout << "错误：该任课老师的此课程已存在，不能重复保存信息" << std::endl;
			std::cout << "请重新输入" << std::endl;
			std::cout << "------------------------------------" << std::endl;
			continue;
		}

		courses.emplace_back(coursenumber, coursename, courseteacher);

		std::cout << "该课程信息已添加完毕" << std::endl;

		CourseInformation::SaveToFile(courses);

		courses.clear();
	}
}

void CourseStorageSearch()
{

			std::cout << "确认成功，正在读取课程基本信息" << std::endl;
			std::cout << "------------------------------------" << std::endl;

			CourseInformation::SearchForFile();

			std::cout << "------------------------------------" << std::endl;

}