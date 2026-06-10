/*
	
*/

#include "QueryMessage.h"

void QueryMessage()
{
	while (1)
	{
		std::cout << "菜单（输入序号即可选择）：" << std::endl;
		std::cout << "1--查询学生信息" << std::endl;
		std::cout << "2--查询课程信息" << std::endl;
		std::cout << "3--查询成绩信息" << std::endl;
		std::cout << "4--退出登录" << std::endl;

		std::string Resolution;
		std::getline(std::cin, Resolution);

		RefreshScreenBuffer();

		//管理员对学生信息的查询
		if (Resolution == "1")
		{
			QueryStudentFunction();
		}
		//管理员对课程信息的查询
		else if (Resolution == "2")
		{
			QueryCourseFunction();
		}
		//管理员对成绩信息的查询
		else if (Resolution == "3")
		{
			QueryScoreFunction();
		}
		else if (Resolution == "4")
		{
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
