/*
	BasicInformationStorage函数为管理员对基本信息的管理
	其中包括对学生基本信息的录入、查看、修改 对课程基本信息的录入、查看、修改
*/


/*
    任务（2）基本数据信息的录入（包括学生与课程基本信息录入）
	1、学生基本信息录入：录入信息保存在student文件中
	注意：不同学生的学号不能相同，若出现相同，应给出不能保存信息的提示
	2、课程信息的录入：录入信息保存在course文件中
	课号S开头表示专业课，P开头表示公共基础课

	注意：不同课程的课号不能相同，若出现相同，应给出不能保存信息的提示

	任务（4）维护基本信息（包括修改、删除功能）
	1、学生与课程基本信息修改
	修改指定基本信息时，可以根据学生编号与姓名、课程课号与课名查找并修改。
	2、学生与课程基本信息删除
	删除指定基本信息时，可以根据学生编号与姓名、课程课号与课名查找并删除。

	注意：课程与学生如果已经有录入成绩，则课程的课号与学生的编号都不能被修改、不能被删除，应给出不能修改与删除信息的提示。课程的课名和学生姓名可以修改。

	任务（5）学生、课程及成绩显示：	输出所有的学生信息及成绩信息
	输出学生、课程及成绩的全部信息。

	注意：显示信息时，应该排列有序、整齐。

*/

#include "BasicInformationStorage.h"

void BasicInformationStorage()
{
	while (1)
	{
		std::cout << "基本信息管理菜单（输入对应序号即可选择）：" << std::endl;
		std::cout << "1--学生基本信息录入" << std::endl;
		std::cout << "2--查看学生基本信息内容" << std::endl;
		std::cout << "3--修改/删除学生基本信息内容" << std::endl;
		std::cout << "4--课程基本信息录用" << std::endl;
		std::cout << "5--查看课程基本信息内容" << std::endl;
		std::cout << "6--修改/删除课程基本信息内容" << std::endl;
		std::cout << "7--退出菜单" << std::endl;

		std::string solution;
		std::getline(std::cin, solution);

		RefreshScreenBuffer();

		//管理员对学生基本信息的录入
		if (solution == "1")
		{
			StudentStorage();
		}
		//管理员对学生基本信息的查看
		else if (solution == "2")
		{
			StudentStorageSearch();
		}
		//管理员对学生基本信息的修改
		else if (solution == "3")
		{
			StudentDisposeMenu();
		}
		//管理员对课程基本信息的录入
		else if (solution == "4")
		{
			CourseStorage();
		}
		//管理员对课程基本信息的查看
		else if (solution == "5")
		{
			CourseStorageSearch();
		}
		//管理员对课程基本信息的修改
		else if (solution == "6")
		{
			CourseDisposeMenu();
		}
		else if (solution == "7")
		{
			break;
		}
		else
		{	
			std::cout << "选择错误，请重新选择" << std::endl;
			std::cout << "------------------------------------" << std::endl;
		}
	}
}