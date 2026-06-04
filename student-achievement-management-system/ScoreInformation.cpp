#include"ScoreInformation.h"

bool ScoreInformation::isScoreExist(const std::string& studentID, const std::string& classID,  std::vector<ScoreInformation>& scores)
{
	std::vector<ScoreInformation>::const_iterator it; //迭代器遍历

	for (it = scores.begin(); it != scores.end(); it++)
	{
		if (it->StudentID == studentID && it->ClassID == classID)
		{
			return true;
		}
	}

	return false;
}

void ScoreInformation::LoadScoresFromFile(std::vector<ScoreInformation>& scores)
{
	scores.clear();

	std::ifstream ScoreInformationFile;

	ScoreInformationFile.open("Score.txt");

	if (ScoreInformationFile.is_open())
	{

		std::string line;

		while (std::getline(ScoreInformationFile, line))
		{
			std::string source = line;
			std::stringstream ss(source);
			std::string str;

			getline(ss, str, ' ');

			if (!str.empty())
			{
				std::string studentid = str;
				getline(ss, str, ' ');
				std::string classid = str;
				getline(ss, str, ' ');
				std::string usualscore = str;
				getline(ss, str, ' ');
				std::string rollscore = str;
				getline(ss, str);
				std::string score = str;
				scores.emplace_back(studentid, classid, stoi(usualscore), stoi(rollscore), stoi(score));
			}
		
		}

		ScoreInformationFile.close();
	}
	else
	{
		std::cout << "读取课程信息数据失败" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}
}

void ScoreInformation::isIDNameExist(const std::string& Studentid, const std::string& ClassID, std::vector<ScoreInformation>& scores)
{
	std::vector<ScoreInformation>::iterator it;

	for (it = scores.begin(); it != scores.end(); it++)
	{
		if (it->StudentID == Studentid && it->ClassID == ClassID)
		{
			while (1)
			{
				std::cout << "输入对应序号即可选择:" << std::endl;
				std::cout << "1--修改该成绩基本信息" << std::endl;
				std::cout << "2--删除该成绩基本信息" << std::endl;
				std::cout << "3--返回上一级菜单" << std::endl;

				std::string Solution;
				std::getline(std::cin, Solution);

				RefreshScreenBuffer();

				if (Solution == "1")
				{
					while (1)
					{
						std::cout << "请选择想修改此学生的信息类型" << std::endl;
						std::cout << "1--平时成绩" << std::endl;
						std::cout << "2--卷面成绩" << std::endl;
						std::cout << "3--结束修改" << std::endl;

						std::string Usualscore;
						std::string Rollscore;

						std::string solution;
						std::getline(std::cin, solution);

						RefreshScreenBuffer();

						if (solution == "1")
						{
							while (1)
							{
								std::cout << "请输入修改后的平时成绩(输入End返回上一级)" << std::endl;
								std::getline(std::cin, Usualscore);


								RefreshScreenBuffer();

								if (Usualscore == "End" || Usualscore == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;
									break;
								}

								if (FindSpace(Usualscore) == 1)
								{
									std::cout << "输入平时成绩格式错误 不支持输入空格" << std::endl;
									continue;
								}

								if (NumberJudge(Usualscore) == -1)
								{
									std::cout << "未成功输入，请重新输入" << std::endl;
									std::cout << "---------------------------------" << std::endl;
								}
								else if (NumberJudge(Usualscore) == -2)
								{
									std::cout << "输入成绩格式错误，请重新输入" << std::endl;
									std::cout << "----------------------------------" << std::endl;
								}
								else if (NumberJudge(Usualscore) == -3)
								{
									std::cout << "输入的成绩范围在0--100之间，且不能超过小数点后两位，请重新输入" << std::endl;
									std::cout << "--------------------------------------------------------------------------" << std::endl;
								}
								else
								{
									while (1)
									{
										std::cout << "请再次确认是否修改（Yes/No）" << std::endl;
										std::string decision;
										std::getline(std::cin, decision);

										RefreshScreenBuffer();

										if (decision == "Yes" || decision == "yes")
										{
											it->UsualScore = NumberRounding(NumberJudge(Usualscore));
											std::cout << "修改成功" << std::endl;
											std::cout << "----------------------------------" << std::endl;
											break;
										}
										else if (decision == "No" || decision == "no")
										{
											std::cout << "已经取消修改" << std::endl;
											std::cout << "----------------------------------" << std::endl;
											break;
										}
										else
										{
											std::cout << "确认错误，请重新确认" << std::endl;
											std::cout << "----------------------------------" << std::endl;
										}
									}
									break;
								}
							}
						}
						else if (solution == "2")
						{
							while (1)
							{
								std::cout << "请输入修改后的平时成绩(输入End返回上一级)" << std::endl;
								std::getline(std::cin, Rollscore);

								RefreshScreenBuffer();

								if (Rollscore == "End" || Rollscore == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;
									break;
								}

								if (FindSpace(Rollscore) == 1)
								{
									std::cout << "输入卷面成绩格式错误 不支持输入空格" << std::endl;
									continue;
								}

								if (NumberJudge(Rollscore) == -1)
								{
									std::cout << "未成功输入，请重新输入" << std::endl;
									std::cout << "----------------------------------" << std::endl;
								}
								else if (NumberJudge(Rollscore) == -2)
								{
									std::cout << "输入成绩格式错误，请重新输入" << std::endl;
									std::cout << "----------------------------------" << std::endl;
								}
								else if (NumberJudge(Rollscore) == -3)
								{
									std::cout << "输入的成绩范围在0--100之间，且不能超过小数点后两位，请重新输入" << std::endl;
									std::cout << "--------------------------------------------------------------------------" << std::endl;
								}
								else
								{
									while (1)
									{
										std::cout << "请再次确认是否修改（Yes/No）" << std::endl;
										std::string decision;
										std::getline(std::cin, decision);

										RefreshScreenBuffer();

										if (decision == "Yes" || decision == "yes")
										{
											it->RollScore = NumberRounding(NumberJudge(Rollscore));
											std::cout << "修改成功" << std::endl;
											std::cout << "----------------------------------" << std::endl;
											break;
										}
										else if (decision == "No" || decision == "no")
										{
											std::cout << "已经取消修改" << std::endl;
											std::cout << "----------------------------------" << std::endl;
											break;
										}
										else
										{
											std::cout << "确认错误，请重新确认" << std::endl;
											std::cout << "----------------------------------" << std::endl;
										}
									}
									break;
								}
							}
						}
						else if (solution == "3")
						{
							std::cout << "结束成功" << std::endl;
							std::cout << "----------------------------------" << std::endl;
							break;
						}
						else
						{
							std::cout << "选择错误，请重新选择" << std::endl;
							std::cout << "----------------------------------" << std::endl;
						}
					}
				}
				else if (Solution == "2")
				{
					std::cout << "*此数据非常重要 请谨慎选择是否删除" << std::endl;
					std::cout << "------------------------------------------" << std::endl;

					while (1)
					{

						std::cout << "请确认是否删除该成绩基本信息（Yes/No）" << std::endl;
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
									scores.erase(it);
									std::cout << "删除成功" << std::endl;
									std::cout << "----------------------------------" << std::endl;
									break;
								}
								else if (decision2 == "No" || decision2 == "no")
								{
									break;
								}
								else
								{
									std::cout << "确认错误，请重新确认" << std::endl;
									std::cout << "----------------------------------" << std::endl;
								}
							}
							break;
						}
						else if (decision1 == "No" || decision1 == "no")
						{
							std::cout << "已经取消删除" << std::endl;
							std::cout << "----------------------------------" << std::endl;
							break;
						}
						else
						{
							std::cout << "确认错误，请重新确认" << std::endl;
							std::cout << "----------------------------------" << std::endl;
						}
					}

				}
				else if (Solution == "3")
				{
					std::cout << "返回成功" << std::endl;
					std::cout << "----------------------------------" << std::endl;
					break;
				}
				else
				{
					std::cout << "选择错误，请重新选择" << std::endl;
					std::cout << "----------------------------------" << std::endl;
				}
			}
			return;
		}
	}
	std::cout << "此学生该课程暂无成绩" << std::endl;
	std::cout << "----------------------------------" << std::endl;
	return;
}

void ScoreInformation::SaveToFile(const std::vector<ScoreInformation>& scores)
{
	std::ofstream scoresFile;
	scoresFile.open("Score.txt");
	if (scoresFile.is_open())
	{
		for (auto score = scores.begin(); score != scores.end(); score++)
		{
			scoresFile << score->StudentID << " " << score->ClassID << " " << score->UsualScore << " " << score->RollScore << " " << score->Score << std::endl;
		}
	}
	else
	{
		std::cout << "文档未成功打开" << std::endl;
	}
	scoresFile.close();
}

void ScoreInformation::SearchForFile()
{
	std::ifstream File;
	File.open("Score.txt");
	if (File.is_open())
	{
		//此过程要实现格式化后的输出 先将读取的每一个数据分别存储在对应容器中
		std::string Line;

		std::vector<std::string>Studentid;
		std::vector<std::string>Classid;
		std::vector<std::string>Usualscore;
		std::vector<std::string>Rollscore;
		std::vector<std::string>Score;

		Studentid.emplace_back("学号");
		Classid.emplace_back("课号");
		Usualscore.emplace_back("平时成绩");
		Rollscore.emplace_back("卷面成绩");
		Score.emplace_back("总成绩");

		while (std::getline(File, Line))
		{
			std::string Source = Line;
			std::stringstream ss(Source);
			std::string str;
			getline(ss, str, ' ');
			Studentid.emplace_back(str);
			getline(ss, str, ' ');
			Classid.emplace_back(str);
			getline(ss, str, ' ');
			Usualscore.emplace_back(str);
			getline(ss, str, ' ');
			Rollscore.emplace_back(str);
			getline(ss, str, ' ');
			Score.emplace_back(str);
		}

		std::vector<int>maxlength(5, 0);

		//通过遍历各容器判断每列最长的字符长度
		for (int i = 0; i < Studentid.size(); i++)
		{
			maxlength[0] = std::max(maxlength[0], (int)Studentid[i].size());
			maxlength[1] = std::max(maxlength[1], (int)Classid[i].size());
			maxlength[2] = std::max(maxlength[2], (int)Usualscore[i].size());
			maxlength[3] = std::max(maxlength[3], (int)Rollscore[i].size());
			maxlength[4] = std::max(maxlength[4], (int)Score[i].size());
		}
		//遍历输出
		for (int i = 0; i < Studentid.size(); i++)
		{
			//利用FormattingPrint函数进行居中对齐输出

			FormattingPrint(maxlength[0], Studentid[i]);
			FormattingPrint(maxlength[1], Classid[i]);
			FormattingPrint(maxlength[2], Usualscore[i]);
			FormattingPrint(maxlength[3], Rollscore[i]);
			FormattingPrint(maxlength[4], Score[i]);

			std::cout << std::endl;
		}

		File.close();
	}
	else
	{
		std::cout << "文件未成功打开" << std::endl;
	}
}