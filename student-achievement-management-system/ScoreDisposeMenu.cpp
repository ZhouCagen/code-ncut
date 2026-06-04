#include "ScoreDisposeMenu.h"

void ScoreDisposeMenu(const std::string adminusername, const std::string adminpassword)
{
	while (1)
	{
		std::cout << "确认是否选择修改/删除成绩基本信息内容（Yes/No）" << std::endl;
		std::string Scoredetermine;
		std::getline(std::cin, Scoredetermine);

		RefreshScreenBuffer();

		if (Scoredetermine == "yes" || Scoredetermine == "Yes")
		{
			while (1)
			{
				std::cout << "需要再次输入登录时的账号密码 确保操作无误" << std::endl;
				std::cout << "------------------------------------------------" << std::endl;
				std::cout << "请输入管理员账号（输入End返回上一级）" << std::endl;

				std::string username;
				std::getline(std::cin, username);

				RefreshScreenBuffer();

				if (username == "End" || username == "end")
				{
					std::cout << "返回成功" << std::endl;
					std::cout << "------------------------------------" << std::endl;
					break;
				}

				RefreshScreenBuffer();

				std::cout << "请输入管理员密码" << std::endl;

				std::string password;
				std::getline(std::cin, password);

				RefreshScreenBuffer();

				if (username == adminusername && password == adminpassword)
				{
					while (1)
					{
						std::cout << "验证成功" << std::endl;

						std::cout << "将查找需修改/删除的学生基本信息" << std::endl;
						std::cout << "------------------------------------" << std::endl;

						std::cout << "请输入学生编号" << std::endl;

						std::string Studentid;
						std::getline(std::cin, Studentid);

						RefreshScreenBuffer();

						if (FindSpace(Studentid) == 1)
						{
							std::cout << "输入学号格式错误 不支持输入空格" << std::endl;
							continue;
						}

						std::cout << "请输入课程编号" << std::endl;

						std::string Classid;
						std::getline(std::cin, Classid);

						RefreshScreenBuffer();

						if (FindSpace(Classid) == 1)
						{
							std::cout << "输入课号格式错误 不支持输入空格" << std::endl;
							continue;
						}

						std::vector<ScoreInformation>Scores;
						int usualscore = 0;
						int rollscore = 0;
						int score = 0;


						ScoreInformation Scoreinformation(Studentid, Classid, usualscore, rollscore, score);

						Scoreinformation.LoadScoresFromFile(Scores);
						Scoreinformation.isIDNameExist(Studentid, Classid, Scores);
						ScoreInformation::SaveToFile(Scores);

						break;
					}
				}

				else
				{
					std::cout << "登录错误 请重新登录" << std::endl;
					std::cout << "------------------------------------" << std::endl;

					continue;
				}

				break;
			}
		}
		else if (Scoredetermine == "no" || Scoredetermine == "No")
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
