#include "StudentInformation.h"
#include <regex>

bool StudentInformation::isIDExist(const std::string& studentID, std::vector<StudentInformation>& students)
{
	std::vector<StudentInformation>::const_iterator it;
	for (it = students.begin(); it != students.end(); it++)
	{
		if (it->StudentID == studentID)
		{
			return true;
		}
	}
	return false;
}

void StudentInformation::isIDNameExist(const std::string& StudentId, const std::string& Studentname, std::vector<StudentInformation>& students)
{
	std::vector<StudentInformation>::iterator it;
	for (it = students.begin(); it != students.end(); it++)
	{
		if (it->StudentID == StudentId && it->StudentName == Studentname)
		{
			while (1)
			{
				std::cout << "输入对应序号即可选择:" << std::endl;
				std::cout << "1--修改该学生基本信息" << std::endl;
				std::cout << "2--删除该学生基本信息" << std::endl;
				std::cout << "3--返回上一级菜单" << std::endl;

				std::string Solution;
				std::getline(std::cin, Solution);

				RefreshScreenBuffer();

				if (Solution == "1")
				{
					while (1)
					{
						std::cout << "请选择想修改此学生的信息类型（修改内容不支持输入空格）" << std::endl;
						std::cout << "1--学号" << std::endl;
						std::cout << "2--姓名" << std::endl;
						std::cout << "3--性别" << std::endl;
						std::cout << "4--年龄" << std::endl;
						std::cout << "5--所在系" << std::endl;
						std::cout << "6--结束修改" << std::endl;

						std::string Studentid;
						std::string Studentname;
						std::string Studentgender;
						std::string Studentage;
						std::string Studentsystem;

						std::string solution;
						std::getline(std::cin, solution);

						RefreshScreenBuffer();

						if (solution == "1")
						{
							std::string studentid;
							std::string classid;
							int usualscore = -1;
							int rollscore = -1;
							int score = -1;

							std::vector<ScoreInformation>Scores;

							ScoreInformation scoreinformation(studentid, classid, usualscore, rollscore, score);

							scoreinformation.LoadScoresFromFile(Scores);

							std::vector<ScoreInformation>::iterator It;

							bool ScoreExist = false;

							bool isRevise = true;

							for (It = Scores.begin(); It != Scores.end(); It++)
							{
								if (It->StudentID == StudentId)
								{
									std::cout << "该学生已经录入成绩 不能修改" << std::endl;
									std::cout << "------------------------------------" << std::endl;

									isRevise = false;
									break;
								}
							}

							if (isRevise == 0)
							{
								break;
							}

							while (1)
							{
								std::cout << "请输入修改后的学号(输入End返回上一级)" << std::endl;

								std::getline(std::cin, Studentid);

								RefreshScreenBuffer();

								if (Studentid == "End" || Studentid == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;

									break;
								}

								if (FindSpace(Studentid) == 1)
								{
									std::cout << "输入学号格式错误 不支持输入空格" << std::endl;
									continue;
								}

								RefreshScreenBuffer();

								while (1)
								{
									std::cout << "请再次确认是否修改（Yes/No）" << std::endl;
									std::string decision;
									std::getline(std::cin, decision);

									RefreshScreenBuffer();

									if (decision == "Yes" || decision == "yes")
									{
										it->StudentID = Studentid;

										std::cout << "修改成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else if (decision == "No" || decision == "no")
									{
										std::cout << "已经取消修改" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else
									{
										std::cout << "确认错误，请重新确认" << std::endl;
										std::cout << "------------------------------------" << std::endl;

									}
								}
							}


						}
						else if (solution == "2")
						{
							while (1)
							{
								std::cout << "请输入修改后的姓名(输入End返回上一级)" << std::endl;

								std::getline(std::cin, Studentname);

								RefreshScreenBuffer();

								if (Studentname == "End" || Studentname == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;

									break;
								}

								if (FindSpace(Studentname) == 1)
								{
									std::cout << "输入姓名格式错误 不支持输入空格" << std::endl;
									continue;
								}

								RefreshScreenBuffer();

								while (1)
								{
									std::cout << "请再次确认是否修改（Yes/No）" << std::endl;
									std::string decision;
									std::getline(std::cin, decision);

									RefreshScreenBuffer();

									if (decision == "Yes" || decision == "yes")
									{
										it->StudentName = Studentname;

										std::cout << "修改成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else if (decision == "No" || decision == "no")
									{
										std::cout << "已经取消修改" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else
									{
										std::cout << "确认错误，请重新确认" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}
								}
								break;
							}
						}
						else if (solution == "3")
						{
							while (1)
							{
								std::cout << "请输入修改后的性别(输入End返回上一级)" << std::endl;

								std::getline(std::cin, Studentgender);

								RefreshScreenBuffer();

								if (Studentgender == "End" || Studentgender == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;

									break;
								}

								if (FindSpace(Studentgender) == 1)
								{
									std::cout << "输入性别格式错误 不支持输入空格" << std::endl;
									continue;
								}


								while (1)
								{
									std::cout << "请再次确认是否修改（Yes/No）" << std::endl;

									std::string decision;
									std::getline(std::cin, decision);

									RefreshScreenBuffer();

									if (decision == "Yes" || decision == "yes")
									{
										it->StudentGender = Studentgender;

										std::cout << "修改成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else if (decision == "No" || decision == "no")
									{
										std::cout << "已经取消修改" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else
									{
										std::cout << "确认错误，请重新确认" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}
								}
								break;
							}
						}
						else if (solution == "4")
						{
							while (1)
							{
								std::cout << "请输入修改后的年龄(输入End返回上一级)" << std::endl;

								std::getline(std::cin, Studentage);

								RefreshScreenBuffer();

								if (Studentage == "End" || Studentage == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;

									break;
								}

								if (FindSpace(Studentage) == 1)
								{
									std::cout << "输入年龄格式错误 不支持输入空格" << std::endl;
									continue;
								}


								while (1)
								{
									std::cout << "请再次确认是否修改（Yes/No）" << std::endl;
									std::cout << "------------------------------------" << std::endl;

									std::string decision;
									std::getline(std::cin, decision);

									RefreshScreenBuffer();

									if (decision == "Yes" || decision == "yes")
									{
										it->StudentAge = Studentage;

										std::cout << "修改成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else if (decision == "No" || decision == "no")
									{
										std::cout << "已经取消修改" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else
									{
										std::cout << "确认错误，请重新确认" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}
								}
								break;
							}
						}
						else if (solution == "5")
						{
							while (1)
							{
								std::cout << "请输入修改后的所在系(输入End返回上一级)" << std::endl;

								std::getline(std::cin, Studentsystem);

								RefreshScreenBuffer();

								if (FindSpace(Studentsystem) == 1)
								{
									std::cout << "输入年龄所在系错误 不支持输入空格" << std::endl;
									continue;
								}

								if (Studentsystem == "End" || Studentsystem == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;

									break;
								}

								while (1)
								{
									std::cout << "请再次确认是否修改（Yes/No）" << std::endl;

									std::string decision;
									std::getline(std::cin, decision);

									RefreshScreenBuffer();

									if (decision == "Yes" || decision == "yes")
									{
										it->StudentSystem = Studentsystem;

										std::cout << "修改成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else if (decision == "No" || decision == "no")
									{
										std::cout << "已经取消修改" << std::endl;
										std::cout << "------------------------------------" << std::endl;

										break;
									}
									else
									{
										std::cout << "确认错误，请重新确认" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}
								}
								break;
							}
						}
						else if (solution == "6")
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

				else if (Solution == "2")
				{

					std::string studentid;
					std::string classid;
					int usualscore = -1;
					int rollscore = -1;
					int score = -1;

					std::vector<ScoreInformation>Scores;

					ScoreInformation scoreinformation(studentid, classid, usualscore, rollscore, score);

					scoreinformation.LoadScoresFromFile(Scores);

					std::vector<ScoreInformation>::iterator It;

					bool ScoreExist = false;

					bool isDelete = true;

					for (It = Scores.begin(); It != Scores.end(); It++)
					{
						if (It->StudentID == StudentId)
						{
							std::cout << "该学生已经录入成绩 不能删除" << std::endl;
							std::cout << "------------------------------------" << std::endl;
							isDelete = false;
							break;
						}
					}

					if (isDelete == 0)
					{
						break;
					}


					std::cout << "*此数据非常重要 请谨慎选择是否删除" << std::endl;

					while (1)
					{

						std::cout << "请确认是否删除该学生基本信息（Yes/No）" << std::endl;
						std::string decision1;
						std::getline(std::cin, decision1);

						RefreshScreenBuffer();

						if (decision1 == "Yes" || decision1 == "yes")
						{
							while (1)
							{

								std::cout << "请再次确认是否删除该学生基本信息（Yes/No）" << std::endl;
								std::string decision2;
								std::getline(std::cin, decision2);

								RefreshScreenBuffer();

								if (decision2 == "Yes" || decision2 == "yes")
								{
									students.erase(it);
									std::cout << "删除成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;
									break;
								}
								else if (decision2 == "No" || decision2 == "no")
								{
									break;
								}
								else
								{
									std::cout << "确认错误，请重新确认" << std::endl;
									std::cout << "------------------------------------" << std::endl;
								}
							}
						}
						else if (decision1 == "No" || decision1 == "no")
						{
							std::cout << "已经取消删除" << std::endl;
							std::cout << "------------------------------------" << std::endl;
							break;
						}
						else
						{
							std::cout << "确认错误，请重新确认" << std::endl;
							std::cout << "------------------------------------" << std::endl;
						}
						break;
					}

				}
				else if (Solution == "3")
				{
					break;
				}
				else
				{
					std::cout << "选择错误，请重新选择" << std::endl;
					std::cout << "------------------------------------" << std::endl;
				}
			}
			return;
		}
	}
	std::cout << "无此学生信息" << std::endl;
	std::cout << "------------------------------------" << std::endl;
	return;
}

void StudentInformation::LoadStudentsFromFile(std::vector<StudentInformation> &students)
{
	std::ifstream StudentInformationFile;

	StudentInformationFile.open("Student.txt");

	if (StudentInformationFile.is_open())
	{

		std::string line;

		while (std::getline(StudentInformationFile, line))
		{
			std::string source = line;
			std::stringstream ss(source);
			std::string str;

			getline(ss, str, ' ');

			if (!str.empty())
			{

				std::string studentid = str;
				getline(ss, str, ' ');
				std::string studentname = str;
				getline(ss, str, ' ');
				std::string studentgender = str;
				getline(ss, str, ' ');
				std::string studentage = str;
				getline(ss, str);
				std::string studentsystem = str;

				students.emplace_back(studentid, studentname, studentgender, studentage, studentsystem);
			}

		}
		StudentInformationFile.close();
	}
	else
	{
		std::cout << "读取学生信息数据失败" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}
}

void StudentInformation::SaveToFile(const std::vector<StudentInformation>& students)
{
	std::ofstream studentsFile;

	studentsFile.open("Student.txt");

	if (studentsFile.is_open())
	{
		for (auto student = students.begin(); student != students.end(); student++)
		{
			studentsFile << student->StudentID << " " << student->StudentName << " " << student->StudentGender << " " << student->StudentAge << " " << student->StudentSystem << std::endl;
		}
	}
	else
	{
		std::cout << "文档未成功打开" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}
	studentsFile.close();
}

void StudentInformation::SearchForFile()
{
	std::ifstream File;
	File.open("Student.txt");
	if (File.is_open())
	{
		//此过程要实现格式化后的输出 先将读取的每一个数据分别存储在对应容器中
		std::string Line;

		std::vector<std::string>Studentid;
		std::vector<std::string>Studentname;
		std::vector<std::string>Studentgender;
		std::vector<std::string>Studentage;
		std::vector<std::string>Studentsystem;

		Studentid.emplace_back("学号");
		Studentname.emplace_back("姓名");
		Studentgender.emplace_back("性别");
		Studentage.emplace_back("年龄");
		Studentsystem.emplace_back("所在系");

		while (std::getline(File, Line))
		{
			std::string Source = Line;
			std::stringstream ss(Source);
			std::string str;
			getline(ss, str, ' ');
			Studentid.emplace_back(str);
			getline(ss, str, ' ');
			Studentname.emplace_back(str);
			getline(ss, str, ' ');
			Studentgender.emplace_back(str);
			getline(ss, str, ' ');
			Studentage.emplace_back(str);
			getline(ss, str, ' ');
			Studentsystem.emplace_back(str);
		}

		std::vector<int>maxlength(5, 0);

		//通过遍历各容器判断每列最长的字符长度
		for (int i = 0; i < Studentid.size(); i++)
		{
			maxlength[0] = std::max(maxlength[0], (int)Studentid[i].size());
			maxlength[1] = std::max(maxlength[1], (int)Studentname[i].size());
			maxlength[2] = std::max(maxlength[2], (int)Studentgender[i].size());
			maxlength[3] = std::max(maxlength[3], (int)Studentage[i].size());
			maxlength[4] = std::max(maxlength[4], (int)Studentsystem[i].size());
		}

		const int rowsPerpage = 10;

		//static_cast<int>是 C++ 中的一个类型转换操作符

		std::cout << std::endl;

		for (int i = 0; i < std::min(static_cast<int>(Studentid.size()), rowsPerpage); ++i)
		{
			FormattingPrint(maxlength[0], Studentid[i]);
			FormattingPrint(maxlength[1], Studentname[i]);
			FormattingPrint(maxlength[2], Studentgender[i]);
			FormattingPrint(maxlength[3], Studentage[i]);
			FormattingPrint(maxlength[4], Studentsystem[i]);
			std::cout << std::endl;
		}

		std::cout << "-------------------第 1 页-------------------" << std::endl;

		int pageNumber;
		std::cout << std::endl << "请输入页码查看更多（输入0退出）: " << std::endl;

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		while (std::cin >> pageNumber && pageNumber != 0)
		{

			RefreshScreenBuffer();
			if (pageNumber < 1)
			{
				std::cout << "页码必须大于0" << std::endl;
				std::cout << "------------------------------------" << std::endl;
				std::cout << "请重新输入页码: ";
				continue;
			}

			int startIndex = (pageNumber - 1) * rowsPerpage;
			int endIndex = std::min(static_cast<int>(Studentid.size()), startIndex + rowsPerpage);

			if (startIndex >= endIndex)
			{
				std::cout << "页码超出范围" << std::endl;
				std::cout << "------------------------------------" << std::endl;
				std::cout << "请输入页码查看更多（输入0退出）: ";
				continue;
			}

			for (int i = startIndex; i < endIndex; ++i) 
			{
				FormattingPrint(maxlength[0], Studentid[i]);
				FormattingPrint(maxlength[1], Studentname[i]);
				FormattingPrint(maxlength[2], Studentgender[i]);
				FormattingPrint(maxlength[3], Studentage[i]);
				FormattingPrint(maxlength[4], Studentsystem[i]);
				std::cout << std::endl;
			}

			std::cout << "-------------------第 " << pageNumber << " 页-------------------" << std::endl;

			std::cout << "请输入页码查看更多（输入0退出）: ";
		}

		File.close();

		RefreshScreenBuffer();

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	}
	else
	{
		std::cout << "文件未成功打开" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}
}