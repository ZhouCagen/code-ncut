#include "ScoreStorage.h"

void ScoreStorage()
{
	std::string studentid;
	std::string classid;

	double UsualScore = 0;
	double RollScore = 0;

	std::string usualscore;
	std::string rollscore;

	int Usualscore = 0;
	int Rollscore = 0;
	int Score = 0;

	std::vector<ScoreInformation> scores;

	ScoreInformation scoreinformation(studentid, classid, Usualscore, Rollscore, Score);

	std::cout << "请输入学生成绩的基本信息" << std::endl;

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
		
		std::string Studentid;
		std::string Studentname;
		std::string studentgender;
		std::string studentage;
		std::string studentsystem;

		std::vector<StudentInformation>Students;

		StudentInformation Studentinformation(Studentid, Studentname, studentgender, studentage, studentsystem);

		Studentinformation.LoadStudentsFromFile(Students);

		std::vector<StudentInformation>::iterator it;

		bool StudentIDExist = false;

		for (it = Students.begin(); it != Students.end(); it++)
		{
			if (it->StudentID == studentid)
			{
				StudentIDExist = true;
				break;
			}
		}

		std::cout << "课号：" << std::endl;
		std::getline(std::cin, classid);

		RefreshScreenBuffer();

		if (FindSpace(classid) == 1)
		{
			std::cout << "输入课号格式错误 不支持输入空格" << std::endl;
			continue;
		}

		std::string Coursenumber;
		std::string Coursename;
		std::string Courseteacher;
		std::vector<CourseInformation>Courses;

		CourseInformation Courseinformation(Coursenumber, Coursename, Courseteacher);

		Courseinformation.LoadCoursesFromFile(Courses);

		std::vector<CourseInformation>::iterator It;

		bool CourseIDExist = false;

		for (It = Courses.begin(); It != Courses.end(); It++)
		{
			if (It->CourseNumber == classid)
			{
				CourseIDExist = true;
				break;
			}
		}

		if (StudentIDExist == 0 && CourseIDExist == 0)
		{
			std::cout << "此学号和课号均不存在" << std::endl;
			std::cout << "------------------------------------" << std::endl;
			break;
		}
		if (StudentIDExist == 1 && CourseIDExist == 0)
		{
			std::cout << "此课号不存在" << std::endl;
			std::cout << "------------------------------------" << std::endl;
			break;
		}

		if (StudentIDExist == 0 && CourseIDExist == 1)
		{
			std::cout << "此学号不存在" << std::endl;
			std::cout << "------------------------------------" << std::endl;
			break;
		}

		scoreinformation.LoadScoresFromFile(scores);

		if (ScoreInformation::isScoreExist(studentid, classid, scores) == true)
		{
			std::cout << "错误：此学生的该课程成绩已存在，不能重复保存信息" << std::endl;
			std::cout << "请重新输入学号和课号" << std::endl;
			std::cout << "-----------------------------------------------------" << std::endl;
			continue;
		}

		while (1)
		{

			std::cout << "平时成绩：" << std::endl;
			std::getline(std::cin, usualscore);

			RefreshScreenBuffer();

			if (FindSpace(usualscore) == 1)
			{
				std::cout << "输入平时成绩格式错误 不支持输入空格" << std::endl;
				continue;
			}

			if (NumberJudge(usualscore) == -1)
			{
				std::cout << "未成功输入，请重新输入" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}
			else if (NumberJudge(usualscore) == -2)
			{
				std::cout << "输入成绩格式错误，请重新输入" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}
			else if (NumberJudge(usualscore) == -3)
			{
				std::cout << "输入的成绩范围在0--100之间，且不能超过小数点后两位，请重新输入" << std::endl;
				std::cout << "------------------------------------------------------------------------" << std::endl;
			}
			else
			{
				UsualScore = NumberJudge(usualscore);
				break;
			}
		}

		while (1)
		{

			std::cout << "卷面成绩：" << std::endl;
			std::getline(std::cin, rollscore);

			RefreshScreenBuffer();

			if (FindSpace(rollscore) == 1)
			{
				std::cout << "输入卷面成绩格式错误 不支持输入空格" << std::endl;
				continue;
			}

			if (NumberJudge(rollscore) == -1)
			{
				std::cout << "未成功输入，请重新输入" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}
			else if (NumberJudge(rollscore) == -2)
			{
				std::cout << "输入成绩格式错误，请重新输入" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}
			else if (NumberJudge(rollscore) == -3)
			{
				std::cout << "输入的成绩范围在0--100之间，且不能超过小数点后两位，请重新输入" << std::endl;
				std::cout << "------------------------------------------------------------------------" << std::endl;
			}
			else
			{
				RollScore = NumberJudge(rollscore);
				break;
			}
		}

		/*
			要求：学生成绩录入：成绩信息保存在score文件。
				  学生成绩应该是整数，如果录入成绩不是整数应该允许录入，但系统自动向上取整
				  录入学生成绩信息完毕，应该设置保存功能，保存功能运行前成绩不保存，保存功能运行后，综合成绩根据设定条件自动运算。综合成绩运算条件：
		*/

		/*
			NumberRounding为取整并将double类型转换为int类型 要求虽然说系统自动向上取整 但函数仍严格约等于（四舍五入原则）
			以下过程即为：将平时成绩和卷面成绩处理并调用ScoreManage函数进行计算总成绩 写入vector<ScoreInformation>后
						  利用ScoreInformation::SaveToFile函数保存至"score.txt"文件 

		*/

		Usualscore = NumberRounding(UsualScore);
		Rollscore = NumberRounding(RollScore);
		scores.emplace_back(studentid, classid, Usualscore, Rollscore, ScoreManage(classid, Usualscore, Rollscore, Score));

		std::cout << "此学生的该课程成绩已添加完毕" << std::endl;
		std::cout << "------------------------------------" << std::endl;

		ScoreInformation::SaveToFile(scores);
	}
}

void ScoreStorageSearch()
{

	std::cout << "确认成功，正在读取学生成绩基本信息" << std::endl;
	std::cout << "------------------------------------" << std::endl;

	ScoreInformation::SearchForFile();

	std::cout << "------------------------------------" << std::endl;

}