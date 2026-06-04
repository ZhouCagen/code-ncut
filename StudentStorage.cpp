#include"StudentStorage.h"

void StudentStorage()
{
	std::string studentid;
	std::string studentname;
	std::string studentgender;
	std::string studentage;
	std::string studentsystem;
	std::vector<StudentInformation>students;

	StudentInformation studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

	std::cout << "请输入学生的基本信息（不支持输入空格）" << std::endl;

	while (1)
	{
		std::cout << "学号：（输入End返回上一级）" << std::endl;

		std::getline(std::cin, studentid);

		RefreshScreenBuffer();

		if (studentid == "end" || studentid == "End")
		{
			std::cout << "返回成功" << std::endl;
			std::cout << "------------------------------------" << std::endl;

			break;
		}

		if (FindSpace(studentid) == 1)
		{
			std::cout << "输入学号格式错误 不支持输入空格" << std::endl;
			continue;
		}

		studentinformation.LoadStudentsFromFile(students);

		if (StudentInformation::isIDExist(studentid,students) == true)
		{
			std::cout << "错误：学号已存在，不能重复保存信息" << std::endl;
			std::cout << "请重新输入学生的基本信息" << std::endl;
			std::cout << "------------------------------------" << std::endl;

			continue;
		}

		std::cout << "姓名：" << std::endl;
		std::getline(std::cin, studentname);

		RefreshScreenBuffer();

		if (FindSpace(studentname) == 1)
		{
			std::cout << "输入姓名格式错误 不支持输入空格" << std::endl;
			continue;
		}

		std::cout << "性别：" << std::endl;
		std::getline(std::cin, studentgender);

		RefreshScreenBuffer();

		if (FindSpace(studentgender) == 1)
		{
			std::cout << "输入性别格式错误 不支持输入空格" << std::endl;
			continue;
		}

		while (1)
		{
			std::cout << "年龄：" << std::endl;
			std::getline(std::cin, studentage);

			RefreshScreenBuffer();

			if (FindSpace(studentage) == 1)
			{
				std::cout << "输入年龄格式错误 不支持输入空格" << std::endl;
				continue;
			}

			/*正则表达式：^ [1 - 9] * $
			  ^表示字符串的开始。
	          [1 - 9] 表示匹配任何一个在1到9之间的数字。
			  * 表示前面的字符（在这个例子中是[1 - 9]中的任何一个数字）可以出现0次或多次。
	          $ 表示字符串的结束。
			*/

			std::regex pattern("^[1-9]*$");

			if (std::regex_match(studentage, pattern) == 0)
			{
				std::cout << "输入年龄格式错误 请重新输入（只能输入数字）" << std::endl;
				std::cout << "--------------------------------------------------" << std::endl;
			}
			else
			{
				break;
			}
		}

		std::cout << "所在系：" << std::endl;
		std::getline(std::cin, studentsystem);

		RefreshScreenBuffer();

		if (FindSpace(studentsystem) == 1)
		{
			std::cout << "输入所在系格式错误 不支持输入空格" << std::endl;
			continue;
		}

		students.emplace_back(studentid, studentname, studentgender, studentage, studentsystem);

		std::cout << "该学生信息已添加完毕" << std::endl;
		std::cout << "------------------------------------" << std::endl;

		StudentInformation::SaveToFile(students);

		students.clear();

	}
}


void StudentStorageSearch()
{

	std::cout << "确认成功，正在读取学生基本信息" << std::endl;
	std::cout << "------------------------------------" << std::endl;

	StudentInformation::SearchForFile();

	std::cout << "------------------------------------" << std::endl;

}