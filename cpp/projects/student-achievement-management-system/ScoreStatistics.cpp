
/*
	ScoreStatistics函数为管理员对成绩统计管理
	其中包含课程成绩分析 课程成绩统计 学生成绩统计的实现
	其中课程成绩统计和学生成绩统计部分以链表方式完成
*/

/*
	任务（8）成绩统计
	显示每门课程的卷面成绩和综合成绩的最高分与平均分；
	显示每门课程卷面分数中，各个分数段的人数统计，包括6个分数段100分、90 - 99分、80 - 89分、70 - 79分、60 - 69分、60分以下等；
	显示每门课程综合分数中，各个分数段的人数统计，包括6个分数段100分、90 - 99分、80 - 89分、70 - 79分、60 - 69分、60分以下等。

	任务（9）汇总报表
	显示每门课程的课号、课名、总人数、不及格人数、不及格百分比；
	显示每个学生的学号、姓名、总课数、不及格课程数。
*/

#include "ScoreStatistics.h"

void ScoreStatistics()
{
	while (1)
	{
		std::cout << "菜单（输入对应序号即可选择）：" << std::endl;
		std::cout << "1--课程成绩分析" << std::endl;
		std::cout << "2--课程成绩统计" << std::endl;
		std::cout << "3--学生成绩统计" << std::endl;
		std::cout << "4--退出登录" << std::endl;

		std::string decision;
		std::getline(std::cin, decision);

		RefreshScreenBuffer();

		//管理员对课程成绩的分析
		if (decision == "1")
		{
			std::string classId;
			std::string className;
			std::string classTeacher;

			std::vector<CourseInformation>Courses;

			CourseInformation Courseinformation(classId, className, classTeacher);

			Courseinformation.LoadCoursesFromFile(Courses);

			std::vector<CourseInformation>::iterator It;

			std::vector<CourseInformation>courses;

			std::vector<CourseInformation>::iterator IT;

			for (It = Courses.begin(); It != Courses.end(); It++)
			{
				bool isExist = false;

				for (IT = courses.begin(); IT != courses.end(); IT++)
				{
					if (It->CourseNumber == IT->CourseNumber)
					{
						isExist = true;
						break;
					}
				}

				if (isExist == 0)
				{
					courses.emplace_back(It->CourseNumber, It->CourseName, It->CourseTeacher);
				}

			}

			for (It = courses.begin(); It != courses.end(); It++)
			{

				std::string Studentid;
				std::string Classid;
				int usualscore = 0;
				int rollscore = 0;
				int score = 0;

				std::vector<ScoreInformation>Scores;

				ScoreInformation Scoreinformation(Studentid, Classid, usualscore, rollscore, score);

				Scoreinformation.LoadScoresFromFile(Scores);

				std::vector<ScoreInformation>::iterator it;

				bool isQuery = false;

				int maxusualscore = 0;
				int minusualscore = 100;
				double averageusualscore = 0;

				int maxrollscore = 0;
				int minrollscore = 100;
				double averagerollscore = 0;

				int maxscore = 0;
				int minscore = 100;
				double averagescore = 0;

				int count = 0;

				int UsualPoint[6] = { 0 };
				int RollPoint[6] = { 0 };
				int ScorePoint[6] = { 0 };

				for (it = Scores.begin(); it != Scores.end(); it++)
				{

					if (It->CourseNumber == it->ClassID)
					{
						count++;

						maxusualscore = std::max(maxusualscore, it->UsualScore);
						minusualscore = std::min(minusualscore, it->UsualScore);
						averageusualscore += it->UsualScore;

						ScoreRank(it->UsualScore, UsualPoint);

						maxrollscore = std::max(maxrollscore, it->RollScore);
						minrollscore = std::min(minrollscore, it->RollScore);
						averagerollscore += it->RollScore;

						ScoreRank(it->RollScore, RollPoint);

						maxscore = std::max(maxscore, it->Score);
						minscore = std::min(minscore, it->Score);
						averagescore += it->Score;

						ScoreRank(it->Score, ScorePoint);

					}
				}

				std::cout << "课名：" << It->CourseName << "    " << "课号：" << It->CourseNumber << std::endl;
				std::cout << "平时成绩：" << std::endl;
				std::cout << "最高分：" << maxusualscore << "    " << "最低分：" << minusualscore << "    " << "平均分：" << std::setprecision(2) << std::fixed << averageusualscore / count << std::endl;
				std::cout << "100分人数：" << UsualPoint[0] << "    90-99分人数：" << UsualPoint[1] << "    80-89分人数：" << UsualPoint[2] << "    70-79分人数：" << UsualPoint[3] << "    60-69分人数：" << UsualPoint[4] << "    60分以下人数：" << UsualPoint[5] << std::endl << std::endl;

				std::cout << "卷面成绩" << std::endl;
				std::cout << "最高分：" << maxrollscore << "    " << "最低分：" << minrollscore << "    " << "平均分：" << std::setprecision(2) << std::fixed << averagerollscore / count << std::endl;
				std::cout << "100分人数：" << RollPoint[0] << "    90-99分人数：" << RollPoint[1] << "    80-89分人数：" << RollPoint[2] << "    70-79分人数：" << RollPoint[3] << "    60-69分人数：" << RollPoint[4] << "    60分以下人数：" << RollPoint[5] << std::endl << std::endl;

				std::cout << "综合成绩" << std::endl;
				std::cout << "最高分：" << maxscore << "    " << "最低分：" << minscore << "    " << "平均分：" << std::setprecision(2) << std::fixed << averagescore / count << std::endl;
				std::cout << "100分人数：" << ScorePoint[0] << "    90-99分人数：" << ScorePoint[1] << "    80-89分人数：" << ScorePoint[2] << "    70-79分人数：" << ScorePoint[3] << "    60-69分人数：" << ScorePoint[4] << "    60分以下人数：" << ScorePoint[5] << std::endl << std::endl;

				std::cout << "------------------------------------------------------------------------" << std::endl;
			}
		}
		//管理员对课程成绩的统计
		else if (decision == "2")
		{
			std::unordered_set<std::string> nameSet;

			std::ifstream File;
			File.open("Course.txt");

			if (File.is_open())
			{
				std::vector<int> heights;

				std::vector<int> totalperson;

				std::vector<std::string>coursenumber;

				List<CourseInformation>* head1 = new List <CourseInformation>;
				List<CourseInformation>* cur1 = head1;

				std::string Line;

				while (std::getline(File, Line))
				{
					cur1->next = new List<CourseInformation>;
					cur1 = cur1->next;

					std::string Source = Line;
					std::stringstream ss1(Source);

					getline(ss1, cur1->val.CourseNumber, ' ');
					getline(ss1, cur1->val.CourseName, ' ');
					getline(ss1, cur1->val.CourseTeacher);

				}

				cur1 = head1->next;

				List<CourseInformation>* head2 = new List <CourseInformation>;

				while (cur1 != nullptr)
				{

					bool flg = false;

					List<CourseInformation>* cur2 = head2;

					if (!nameSet.count(cur1->val.CourseName))
					{
						cur2->next = new List<CourseInformation>;
						cur2 = cur2->next;

						cur2->val.CourseNumber = cur1->val.CourseNumber;
						cur2->val.CourseName = cur1->val.CourseName;
						cur2->val.CourseTeacher = cur1->val.CourseTeacher;

						flg = true;

						nameSet.insert(cur1->val.CourseName);
					}

					std::ifstream file;
					file.open("Score.txt");
					if (file.is_open())
					{
						List<ScoreInformation>* head3 = new List<ScoreInformation>;
						List<ScoreInformation>* cur3 = head3;

						std::string line;

						while (std::getline(file, line))
						{
							cur3->next = new List<ScoreInformation>;
							cur3 = cur3->next;

							std::string source = line;
							std::stringstream ss2(source);

							getline(ss2, cur3->val.StudentID, ' ');
							getline(ss2, cur3->val.ClassID, ' ');

							std::string str;

							getline(ss2, str, ' ');
							cur3->val.UsualScore = stoi(str);
							getline(ss2, str, ' ');
							cur3->val.RollScore = stoi(str);
							getline(ss2, str);
							cur3->val.Score = stoi(str);
						}

						cur3 = head3->next;

						double totalcount = 0;
						double failcount = 0;
						double percentage = 0;

						while (cur3 != nullptr)
						{
							if (cur2->val.CourseNumber == cur3->val.ClassID)
							{
								totalcount++;
								if (cur3->val.Score < 60)
								{
									failcount++;
								}
							}

							cur3 = cur3->next;
						}

						percentage = failcount / totalcount * 100;

						if (flg)
						{
							std::cout << "课名：" << cur2->val.CourseName << "    课号：" << cur2->val.CourseNumber << std::endl;
							std::cout << "总人数：" << (int)totalcount << "    不及格人数：" << (int)failcount << "    不及格百分比" << std::fixed << std::setprecision(1) << percentage << "%" << std::endl;
							std::cout << "------------------------------------------------------" << std::endl;

							heights.emplace_back((int)totalcount - (int)failcount);

							totalperson.emplace_back((int)totalcount);

							coursenumber.emplace_back(cur2->val.CourseNumber);

						}

						deleteList(head3);

						file.close();
					}
					else
					{
						std::cout << "“Score.txt”文件未成功打开" << std::endl;
						std::cout << "------------------------------------------------------" << std::endl;
					}

					cur1 = cur1->next;
				}

				deleteList(head1);

				File.close();

				/*
					绘制课号对应及格人数的直方图
				*/

				std::cout << std::endl << "竖状直方图（横坐标为课号，纵坐标为及格的人数）" << std::endl << std::endl;

				int max_value = *max_element(totalperson.begin(), totalperson.end());

				for (int i = max_value; i >= 0; i--)
				{
					std::cout << std::setw(2) << i << "|";

					for (int j = 0; j < heights.size(); j++)
					{
						if (heights[j] >= i)
						{
							std::cout << "   *  ";
						}
						else
						{
							std::cout << "      ";
						}
					}

					std::cout << std::endl;
				}

				std::cout << "   --------------------------------------------------------" << std::endl;
				std::cout << "     ";

				for (int i = 0; i < heights.size(); i++)
				{
					std::cout << coursenumber[i] << "   ";
				}

				std::cout << std::endl << std::endl;
				std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;


			}
			else
			{
				std::cout << "“Course.txt”文件未成功打开" << std::endl;
				std::cout << "------------------------------------------------------" << std::endl;
			}

		}
		//管理员对学生成绩的统计
		else if (decision == "3")
		{
			std::ifstream File;
			File.open("Student.txt");
			if (File.is_open())
			{
				std::vector<int> heights;

				std::vector<int> totalcourse;

				std::vector<std::string>studentid;

				List<StudentInformation>* head1 = new List <StudentInformation>;
				List<StudentInformation>* cur1 = head1;

				std::string Line;

				while (std::getline(File, Line))
				{

					cur1->next = new List<StudentInformation>;
					cur1 = cur1->next;

					std::string Source = Line;
					std::stringstream ss1(Source);

					getline(ss1, cur1->val.StudentID, ' ');
					getline(ss1, cur1->val.StudentName, ' ');
					getline(ss1, cur1->val.StudentGender, ' ');
					getline(ss1, cur1->val.StudentAge, ' ');
					getline(ss1, cur1->val.StudentSystem);
				}

				cur1 = head1->next;

				while (cur1 != nullptr)
				{
					std::ifstream file;
					file.open("Score.txt");
					if (file.is_open())
					{
						List<ScoreInformation>* head2 = new List<ScoreInformation>;
						List<ScoreInformation>* cur2 = head2;

						std::string line;

						while (std::getline(file, line))
						{
							cur2->next = new List<ScoreInformation>;
							cur2 = cur2->next;

							std::string source = line;
							std::stringstream ss2(source);

							getline(ss2, cur2->val.StudentID, ' ');
							getline(ss2, cur2->val.ClassID, ' ');

							std::string str;

							getline(ss2, str, ' ');
							cur2->val.UsualScore = stoi(str);
							getline(ss2, str, ' ');
							cur2->val.RollScore = stoi(str);
							getline(ss2, str);
							cur2->val.Score = stoi(str);
						}

						cur2 = head2->next;

						int totalcount = 0;
						int failcount = 0;

						while (cur2 != nullptr)
						{
							if (cur2->val.StudentID == cur1->val.StudentID)
							{

								totalcount++;
								if (cur2->val.Score < 60)
								{
									failcount++;
								}
							}

							cur2 = cur2->next;
						}

						std::cout << "姓名：" << cur1->val.StudentName << "    学号：" << cur1->val.StudentID << std::endl;
						std::cout << "总课数：" << totalcount << "    不及格课数：" << failcount << std::endl;
						std::cout << "------------------------------------------------------" << std::endl;

						heights.emplace_back(totalcount - failcount);

						totalcourse.emplace_back(totalcount);

						studentid.emplace_back(cur1->val.StudentID);


						deleteList(head2);

						file.close();
					}
					else
					{
						std::cout << "“Score.txt”文件未成功打开" << std::endl;
						std::cout << "------------------------------------------------------" << std::endl;
					}

					cur1 = cur1->next;
				}

				deleteList(head1);

				File.close();

				/*
					绘制学号对应及格课程数的直方图
				*/

				std::cout << std::endl << "竖状直方图（横坐标为学号，纵坐标为及格的课程数）" << std::endl << std::endl;

				/*
					max_element 它用于在指定范围内查找具有最大值的元素 
					这个函数模板定义在 <algorithm> 头文件中，并返回一个迭代器，指向范围内的最大值元素 如果范围内的所有元素都相等，则返回指向该范围内第一个元素的迭代器
					迭代器本身并不存储元素的值，它只是指向容器中的一个元素 用'*'解引用后存储
				*/

				int max_value = *max_element(totalcourse.begin(), totalcourse.end());

				for (int i = max_value; i >= 0; i--)
				{
					std::cout << std::setw(2) << i << "|";

					for (int j = 0; j < heights.size(); j++)
					{
						if (heights[j] >= i)
						{
							std::cout << "  *  ";
						}
						else
						{
							std::cout << "     ";
						}
					}

					std::cout << std::endl;
				}

				std::cout << "   --------------------------------------------------------" << std::endl;
				std::cout << "     ";

				for (int i = 0; i < heights.size(); i++)
				{
					std::cout << studentid[i] << "   ";
				}

				std::cout << std::endl << std::endl;
				std::cout << "------------------------------------------------------------------------------------------------------------" << std::endl;


			}
			else
			{
				std::cout << "“Student.txt”文件未成功打开" << std::endl;
				std::cout << "------------------------------------------------------" << std::endl;
			}
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