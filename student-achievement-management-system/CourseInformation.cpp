#include "CourseInformation.h"

bool CourseInformation::isCourseExist(const std::string& coursenumber, const std::string& coursename, const std::string& courseteacher, std::vector<CourseInformation>& courses)
{
	std::vector<CourseInformation>::const_iterator it;
	for (it = courses.begin(); it != courses.end(); it++)
	{
		if (it->CourseNumber == coursenumber && it->CourseName == coursename && it->CourseTeacher == courseteacher)
		{
			return true;
		}
	}
	return false;
}

void CourseInformation::LoadCoursesFromFile(std::vector<CourseInformation>& courses)
{
	std::ifstream CourseInformationFile;
	CourseInformationFile.open("Course.txt");
	if (CourseInformationFile.is_open())
	{
		std::string line;
		while (std::getline(CourseInformationFile, line))
		{
			std::string source = line;
			std::stringstream ss(source);
			std::string str;
			getline(ss, str, ' ');
			if (!str.empty())
			{
				std::string coursenumber = str;
				getline(ss, str, ' ');
				std::string coursename = str;
				getline(ss, str);
				std::string courseteacher = str;
				courses.emplace_back(coursenumber, coursename, courseteacher);
			}
		}
		CourseInformationFile.close();
	}
	else
	{
		std::cout << "读取课程信息数据失败" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}
}

void CourseInformation::isIDNameExist(const std::string& courseNumber, const std::string& courseName, const std::string& courseTeacher, std::vector<CourseInformation>& courses)
{
	std::vector<CourseInformation>::iterator it;
	for (it = courses.begin(); it != courses.end(); it++)
	{
		if (it->CourseNumber == courseNumber && it->CourseName == courseName && it->CourseTeacher == courseTeacher)
		{
			while (1)
			{
				std::cout << "输入对应序号即可选择:" << std::endl;
				std::cout << "1--修改该课程基本信息" << std::endl;
				std::cout << "2--删除该课程基本信息" << std::endl;
				std::cout << "3--返回上一级菜单" << std::endl;

				std::string Solution;
				std::getline(std::cin, Solution);

				RefreshScreenBuffer();

				if (Solution == "1")
				{
					while (1)
					{
						std::cout << "请选择想修改此课程的信息类型（修改内容不支持输入空格）" << std::endl;
						std::cout << "1--课号" << std::endl;
						std::cout << "2--课名" << std::endl;
						std::cout << "3--任课教师" << std::endl;
						std::cout << "4--结束修改" << std::endl;

						std::string Coursenumber;
						std::string Coursename;
						std::string Courseteacher;

						std::string solution;
						std::getline(std::cin, solution);

						RefreshScreenBuffer();

						if (solution == "1")
						{
							//判断该课程是否已经录入成绩 如果录入则课号不能被修改
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
								if (It->ClassID == courseNumber)
								{
									std::cout << "该课程已经录入成绩 不能修改课号" << std::endl;
									std::cout << "------------------------------------" << std::endl;
									isRevise = false;
									break;
								}
							}

							if (isRevise == 0)
							{
								break;
							}

							//还没有录入成绩 可以修改
							while (1)
							{
								std::cout << "请输入修改后的课号(输入End返回上一级)" << std::endl;
								std::getline(std::cin, Coursenumber);

								RefreshScreenBuffer();

								if (Coursenumber == "End" || Coursenumber == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;
									break;
								}

								if (Coursenumber[0] != 'P' && Coursenumber[0] != 'S')
								{
									std::cout << "输入课号格式错误（课号的第一位号码需为S/P 分别代表专业课/公共基础课）" << std::endl;
									std::cout << "------------------------------------------------------------------------" << std::endl;
									continue;
								}

								if (FindSpace(Coursenumber) == 1)
								{
									std::cout << "输入课号格式错误 不支持输入空格" << std::endl;
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
										it->CourseNumber = Coursenumber;
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
						else if (solution == "2")
						{
							while (1)
							{
								std::cout << "请输入修改后的课名(输入End返回上一级)" << std::endl;
								std::getline(std::cin, Coursename);

								RefreshScreenBuffer();

								if (Coursename == "End" || Coursename == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;
									break;
								}

								if (FindSpace(Coursenumber) == 1)
								{
									std::cout << "输入课名格式错误 不支持输入空格" << std::endl;
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
										it->CourseName = Coursename;
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
								std::cout << "请输入修改后的任课教师(输入End返回上一级)" << std::endl;
								std::getline(std::cin, Courseteacher);

								RefreshScreenBuffer();

								if (Courseteacher == "End" || Courseteacher == "end")
								{
									std::cout << "返回成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;
									break;
								}

								if (FindSpace(Courseteacher) == 1)
								{
									std::cout << "输入任课教师格式错误 不支持输入空格" << std::endl;
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
										it->CourseTeacher = Courseteacher;
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
					//判断该课程是否已经录入成绩 如果录入不能被删除 
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
						if (It->ClassID == courseNumber)
						{
							std::cout << "该课程已经录入成绩 不能删除" << std::endl;
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

						std::cout << "请确认是否删除该课程基本信息（Yes/No）" << std::endl;
						std::string decision1;
						std::getline(std::cin, decision1);

						RefreshScreenBuffer();

						if (decision1 == "Yes" || decision1 == "yes")
						{
							while (1)
							{

								std::cout << "请再次确认是否删除该课程基本信息（Yes/No）" << std::endl;
								std::string decision2;
								std::getline(std::cin, decision2);

								RefreshScreenBuffer();

								if (decision2 == "Yes" || decision2 == "yes")
								{
									courses.erase(it);
									std::cout << "删除成功" << std::endl;
									std::cout << "------------------------------------" << std::endl;
									break;
								}
								else if (decision2 == "No" || decision2 == "no")
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

				break;
			}
			return;
		}
	}
	std::cout << "无此课程信息" << std::endl;
	std::cout << "------------------------------------" << std::endl;
	return;
}

void CourseInformation::SaveToFile(const std::vector<CourseInformation>& courses)
{
	std::ofstream coursesFile;
	coursesFile.open("Course.txt");
	if (coursesFile.is_open())
	{
		for (auto course = courses.begin(); course != courses.end(); course++)
		{
			coursesFile << course->CourseNumber << " " << course->CourseName << " " << course->CourseTeacher << std::endl;
		}
	}
	else
	{
		std::cout << "文档未成功打开" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}
	coursesFile.close();
}

void CourseInformation::SearchForFile()
{
	std::ifstream File;
	File.open("Course.txt");
	if (File.is_open())
	{
		//此过程要实现格式化后的输出 先将读取的每一个数据分别存储在对应容器中
		std::string Line;

		std::vector<std::string>Coursenumber;
		std::vector<std::string>Coursename;
		std::vector<std::string>Courseteacher;

		Coursenumber.emplace_back("课号");
		Coursename.emplace_back("课名");
		Courseteacher.emplace_back("任课教师");

		while (std::getline(File, Line))
		{
			std::string Source = Line;
			std::stringstream ss(Source);
			std::string str;
			getline(ss, str, ' ');
			Coursenumber.emplace_back(str);
			getline(ss, str, ' ');
			Coursename.emplace_back(str);
			getline(ss, str);
			Courseteacher.emplace_back(str);
		}

		std::vector<int>maxlength(3, 0);

		//通过遍历各容器判断每列最长的字符长度
		for (int i = 0; i < Coursenumber.size(); i++)
		{
			maxlength[0] = std::max(maxlength[0], (int)Coursenumber[i].size());
			maxlength[1] = std::max(maxlength[1], (int)Coursename[i].size());
			maxlength[2] = std::max(maxlength[2], (int)Courseteacher[i].size());
		}

		const int rowsPerpage = 10;

		std::cout << std::endl;

		for (int i = 0; i < std::min(static_cast<int>(Coursenumber.size()), rowsPerpage); i++)
		{
			FormattingPrint(maxlength[0], Coursenumber[i]);
			FormattingPrint(maxlength[1], Coursename[i]);
			FormattingPrint(maxlength[2], Courseteacher[i]);

			std::cout << std::endl;
		}

		std::cout << "--------------第 1 页--------------" << std::endl;

		int pageNumber;

		std::cout << std::endl << "请输入页码查看更多（输入0退出）: " << std::endl;

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		while (std::cin >> pageNumber && pageNumber != 0)
		{

			if (pageNumber == 0)
			{
				std::cout << "退出成功" << std::endl;
				std::cout << "------------------------------------" << std::endl;
				break;
			}

			RefreshScreenBuffer();

			if (pageNumber < 1)
			{
				std::cout << "页码必须大于0" << std::endl;
				std::cout << "------------------------------------" << std::endl;
				std::cout << "请重新输入页码: ";
				continue;
			}

			int startIndex = (pageNumber- 1) * rowsPerpage;

			int endIndex = std::min(static_cast<int>(Coursenumber.size()), startIndex + rowsPerpage);

			if (startIndex >= endIndex)
			{
				std::cout << "页码超出范围" << std::endl;
				std::cout << "------------------------------------" << std::endl;
				std::cout << "请输入页码查看更多（输入0退出）: ";
				continue;
			}

			for (int i = startIndex; i < endIndex; i++)
			{
				FormattingPrint(maxlength[0], Coursenumber[i]);
				FormattingPrint(maxlength[1], Coursename[i]);
				FormattingPrint(maxlength[2], Courseteacher[i]);

				std::cout << std::endl;
			}

			std::cout << "--------------第 " << pageNumber << " 页--------------" << std::endl;

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