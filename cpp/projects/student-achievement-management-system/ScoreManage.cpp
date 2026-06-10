
/*
	
*/
	
/*  
	任务（3）学生成绩录入与综合成绩计算
	1、学生成绩录入：成绩信息保存在score文件中
	学生成绩应该是整数，如果录入成绩不是整数应该允许录入，但系统自动向上取整。
	2、综合成绩计算
	录入学生成绩信息完毕，应该设置保存功能，保存功能运行前成绩不保存，保存功能运行后，综合成绩根据设定条件自动运算。综合成绩运算条件：
	如果录入课程为公共课，则 综合成绩 = 平时成绩 * 30 % +卷面成绩 * 70 %
	如果录入课程为专业课，则 综合成绩 = 平时成绩 * 40 % +卷面成绩 * 70 % .

	任务（6）成绩修改及删除（要求先输入密码）
	1、成绩修改
	在修改成绩之前必须再次正确的输入密码才能进入成绩修改菜单进行操作；
	只允许修改学生的平时成绩与卷面成绩，综合成绩不允许修改；
	综合成绩根据修改后的平时成绩与卷面成绩，自动计算更新。
	2、成绩删除
	可以根据学生学号与课程课号删除对应成绩。

*/

#include "ScoreManage.h"

void StudentScoreManage(std::string& AdminUserName, std::string& AdminPassword)
{
	while (1)
	{
		std::cout << "菜单（输入序号即可选择）：" << std::endl;
		std::cout << "1--学生课程成绩信息录入" << std::endl;
		std::cout << "2--查看学生课程成绩信息内容" << std::endl;
		std::cout << "3--修改/删除学生课程成绩信息" << std::endl;
		std::cout << "4--退出菜单" << std::endl;

		std::string decision;
		std::getline(std::cin, decision);

		RefreshScreenBuffer();

		//管理员对学生课程成绩信息的录入
		if (decision == "1")
		{
			ScoreStorage();
		}
		//管理员对学生课程成绩信息的查看
		else if (decision == "2")
		{
			ScoreStorageSearch();
		}
		//管理员对学生课程成绩信息的修改
		else if (decision == "3")
		{
			ScoreDisposeMenu(AdminUserName, AdminPassword);
		}
		else if (decision == "4")
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

/*ScoreManage函数作用：计算总成绩
	课号S开头表示专业课，P开头表示公共基础课。
	如果录入课程为公共课，则 综合成绩 = 平时成绩 * 30 % +卷面成绩 * 70 %
	如果录入课程为专业课，则 综合成绩 = 平时成绩 * 40 % +卷面成绩 * 60 %
*/

int ScoreManage(std::string& classid, int usualscore, int rollscore, int score)
{
	if (classid[0] == 'S')
	{
		score = NumberRounding(usualscore * 0.3 + rollscore * 0.7);
	}
	else if (classid[0] == 'P')
	{
		score = NumberRounding(usualscore * 0.4 + rollscore * 0.6);
	}
	else
	{
		std::cout << "该课程不属于专业课和公共基础课，暂时无法计算总成绩，记作0分" << std::endl;
		std::cout << "------------------------------------------------------------------------" << std::endl;
	}
	return score;
}