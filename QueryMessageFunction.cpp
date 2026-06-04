#include "QueryMessageFunction.h"

void QueryStudentFunction()
{
	while (1)
	{
		std::cout << "菜单（输入序号即可选择）：" << std::endl;
		std::cout << "1--按学号查询学生信息" << std::endl;
		std::cout << "2--按姓名查询学生信息" << std::endl;
		std::cout << "3--按性别查询学生信息" << std::endl;
		std::cout << "4--按年龄查询学生信息" << std::endl;
		std::cout << "5--按所在系查询学校信息" << std::endl;
		std::cout << "6--退出菜单" << std::endl;

		std::string decision;
		std::getline(std::cin, decision);

		RefreshScreenBuffer();

		if (decision == "1")
		{
			std::string id;
			std::cout << "请输入学号" << std::endl;
			std::getline(std::cin, id);

			RefreshScreenBuffer();

			std::string studentid;
			std::string studentname;
			std::string studentgender;
			std::string studentage;
			std::string studentsystem;
			std::vector<StudentInformation>Students;

			StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

			Studentinformation.LoadStudentsFromFile(Students);

			std::vector<StudentInformation>::iterator it;

			bool isQuery = false;

			for (it = Students.begin(); it != Students.end(); it++)
			{
				if (it->StudentID == id)
				{
					std::cout << "查询成功" << std::endl;
					std::cout << "------------------------------------" << std::endl;
					std::cout << "学号：" << it->StudentID << std::endl;
					std::cout << "姓名：" << it->StudentName << std::endl;
					std::cout << "性别：" << it->StudentGender << std::endl;
					std::cout << "年龄：" << it->StudentAge << std::endl;
					std::cout << "所在系：" << it->StudentSystem << std::endl;
					std::cout << "------------------------------------" << std::endl;

					isQuery = true;

					break;
				}
			}

			if (isQuery == 0)
			{
				std::cout << "暂无此学号的学生信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}

		}
		else if (decision == "2")
		{
			std::string name;
			std::cout << "请输入姓名" << std::endl;
			std::getline(std::cin, name);

			RefreshScreenBuffer();

			std::string studentid;
			std::string studentname;
			std::string studentgender;
			std::string studentage;
			std::string studentsystem;
			std::vector<StudentInformation>Students;

			StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

			Studentinformation.LoadStudentsFromFile(Students);

			std::vector<StudentInformation>::iterator it;

			bool isQuery = false;

			for (it = Students.begin(); it != Students.end(); it++)
			{
				if (it->StudentName == name)
				{
					std::cout << "查询成功" << std::endl;
					std::cout << "------------------------------------" << std::endl;
					std::cout << "学号：" << it->StudentID << std::endl;
					std::cout << "姓名：" << it->StudentName << std::endl;
					std::cout << "性别：" << it->StudentGender << std::endl;
					std::cout << "年龄：" << it->StudentAge << std::endl;
					std::cout << "所在系：" << it->StudentSystem << std::endl;
					std::cout << "------------------------------------" << std::endl;
					isQuery = true;

					break;
				}
			}

			if (isQuery == 0)
			{
				std::cout << "暂无此姓名的学生信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}
		}



		else if (decision == "3")
		{
			std::string gender;
			std::cout << "请输入性别" << std::endl;
			std::getline(std::cin, gender);

			RefreshScreenBuffer();

			while (1)
			{
				std::string isconfirm;
				std::cout << "是否有其他查询要求（Yes/No）" << std::endl;
				std::getline(std::cin, isconfirm);

				RefreshScreenBuffer();

				if (isconfirm == "Yes" || isconfirm == "yes")
				{
					while (1)
					{
						std::cout << "菜单（输入序号即可选择）：" << std::endl;
						std::cout << "1--按学号查询学生信息" << std::endl;
						std::cout << "2--按姓名查询学生信息" << std::endl;
						std::cout << "3--按年龄查询学生信息" << std::endl;
						std::cout << "4--按所在系查询学生信息" << std::endl;
						std::string Decision;

						std::getline(std::cin, Decision);

						RefreshScreenBuffer();

						if (Decision == "1")
						{
							std::string id;
							std::cout << "请输入学号" << std::endl;
							std::getline(std::cin, id);

							RefreshScreenBuffer();

							std::string studentid;
							std::string studentname;
							std::string studentgender;
							std::string studentage;
							std::string studentsystem;
							std::vector<StudentInformation>Students;

							StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

							Studentinformation.LoadStudentsFromFile(Students);

							std::vector<StudentInformation>::iterator it;

							bool isQuery = false;
							for (it = Students.begin(); it != Students.end(); it++)
							{
								if (it->StudentGender == gender && it->StudentID == id)
								{
									if (isQuery == 0)
									{
										std::cout << "查询成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									std::cout << "学号：" << it->StudentID << std::endl;
									std::cout << "姓名：" << it->StudentName << std::endl;
									std::cout << "性别：" << it->StudentGender << std::endl;
									std::cout << "年龄：" << it->StudentAge << std::endl;
									std::cout << "所在系：" << it->StudentSystem << std::endl;
									std::cout << "------------------------------------" << std::endl;

									isQuery = true;
								}
							}

							if (isQuery == 0)
							{
								std::cout << "暂无该性别此学号的学生信息" << std::endl;
								std::cout << "------------------------------------" << std::endl;
							}

							break;

						}
						else if (Decision == "2")
						{
							std::string name;
							std::cout << "请输入姓名" << std::endl;
							std::getline(std::cin, name);

							RefreshScreenBuffer();

							std::string studentid;
							std::string studentname;
							std::string studentgender;
							std::string studentage;
							std::string studentsystem;
							std::vector<StudentInformation>Students;

							StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

							Studentinformation.LoadStudentsFromFile(Students);

							std::vector<StudentInformation>::iterator it;

							bool isQuery = false;

							for (it = Students.begin(); it != Students.end(); it++)
							{
								if (it->StudentGender == gender && it->StudentName == name)
								{
									if (isQuery == 0)
									{
										std::cout << "查询成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									std::cout << "学号：" << it->StudentID << std::endl;
									std::cout << "姓名：" << it->StudentName << std::endl;
									std::cout << "性别：" << it->StudentGender << std::endl;
									std::cout << "年龄：" << it->StudentAge << std::endl;
									std::cout << "所在系：" << it->StudentSystem << std::endl;
									std::cout << "------------------------------------" << std::endl;

									isQuery = true;
								}
							}

							if (isQuery == 0)
							{
								std::cout << "暂无该性别此姓名的学生信息" << std::endl;
								std::cout << "------------------------------------" << std::endl;
							}

							break;

						}
						else if (Decision == "3")
						{
							std::string age;
							std::cout << "请输入年龄" << std::endl;
							std::getline(std::cin, age);

							RefreshScreenBuffer();

							while (1)
							{
								std::string Isconfirm;
								std::cout << "是否还有其他查询要求（Yes/No）" << std::endl;
								std::getline(std::cin, Isconfirm);

								RefreshScreenBuffer();

								if (Isconfirm == "Yes" || Isconfirm == "yes")
								{
									while (1)
									{
										std::cout << "菜单（输入序号即可选择）：" << std::endl;
										std::cout << "1--按学号查询学生信息" << std::endl;
										std::cout << "2--按姓名查询学生信息" << std::endl;
										std::cout << "3--按所在系查询学生信息" << std::endl;

										std::string Resolution;

										std::getline(std::cin, Resolution);

										RefreshScreenBuffer();

										if (Resolution == "1")
										{
											std::string id;
											std::cout << "请输入学号" << std::endl;
											std::getline(std::cin, id);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentAge == age && it->StudentGender == gender && it->StudentID == id)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该学号年龄此性别的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "2")
										{
											std::string name;
											std::cout << "请输入姓名" << std::endl;
											std::getline(std::cin, name);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentAge == age && it->StudentGender == gender && it->StudentName == name)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该姓名年龄此性别的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "3")
										{
											std::string system;
											std::cout << "请输入所在系" << std::endl;
											std::getline(std::cin, system);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentAge == age && it->StudentGender == gender && it->StudentSystem == system)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该年龄性别此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else
										{
											std::cout << "选择错误，请重新选择" << std::endl;
											std::cout << "------------------------------------" << std::endl;
										}
									}

									break;
								}
								else if (Isconfirm == "No" || Isconfirm == "no")
								{
									std::string studentid;
									std::string studentname;
									std::string studentgender;
									std::string studentage;
									std::string studentsystem;
									std::vector<StudentInformation>Students;

									StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

									Studentinformation.LoadStudentsFromFile(Students);

									std::vector<StudentInformation>::iterator it;

									bool isQuery = false;
									for (it = Students.begin(); it != Students.end(); it++)
									{
										if (it->StudentAge == age && it->StudentGender == gender)
										{
											if (isQuery == 0)
											{
												std::cout << "查询成功" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											std::cout << "学号：" << it->StudentID << std::endl;
											std::cout << "姓名：" << it->StudentName << std::endl;
											std::cout << "性别：" << it->StudentGender << std::endl;
											std::cout << "年龄：" << it->StudentAge << std::endl;
											std::cout << "所在系：" << it->StudentSystem << std::endl;
											std::cout << "------------------------------------" << std::endl;

											isQuery = true;
										}
									}

									if (isQuery == 0)
									{
										std::cout << "暂无该年龄此性别的学生信息" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									break;

								}
								else
								{
									std::cout << "选择错误，请重新选择" << std::endl;
									std::cout << "------------------------------------" << std::endl;
								}
							}
							break;
						}
						else if (Decision == "4")
						{
							std::string system;
							std::cout << "请输入所在系" << std::endl;
							std::getline(std::cin, system);

							RefreshScreenBuffer();

							while (1)
							{
								std::string Isconfirm;
								std::cout << "是否还有其他查询要求（Yes/No）" << std::endl;
								std::getline(std::cin, Isconfirm);

								RefreshScreenBuffer();

								if (Isconfirm == "Yes" || Isconfirm == "yes")
								{
									while (1)
									{
										std::cout << "菜单（输入序号即可选择）：" << std::endl;
										std::cout << "1--按学号查询学生信息" << std::endl;
										std::cout << "2--按姓名查询学生信息" << std::endl;
										std::cout << "3--按年龄查询学生信息" << std::endl;
										std::string Resolution;

										std::getline(std::cin, Resolution);

										RefreshScreenBuffer();

										if (Resolution == "1")
										{
											std::string id;
											std::cout << "请输入学号" << std::endl;
											std::getline(std::cin, id);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentGender == gender && it->StudentID == id)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该学号性别此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "2")
										{
											std::string name;
											std::cout << "请输入姓名" << std::endl;
											std::getline(std::cin, name);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentGender == gender && it->StudentName == name)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该姓名性别此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "3")
										{
											std::string age;
											std::cout << "请输入年龄" << std::endl;
											std::getline(std::cin, age);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentGender == gender && it->StudentAge == age)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该年龄性别此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else
										{
											std::cout << "选择错误，请重新选择" << std::endl;
											std::cout << "------------------------------------" << std::endl;
										}
									}

									break;
								}
								else if (Isconfirm == "No" || Isconfirm == "no")
								{
									std::string studentid;
									std::string studentname;
									std::string studentgender;
									std::string studentage;
									std::string studentsystem;
									std::vector<StudentInformation>Students;

									StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

									Studentinformation.LoadStudentsFromFile(Students);

									std::vector<StudentInformation>::iterator it;

									bool isQuery = false;
									for (it = Students.begin(); it != Students.end(); it++)
									{
										if (it->StudentSystem == system && it->StudentGender == gender)
										{
											if (isQuery == 0)
											{
												std::cout << "查询成功" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											std::cout << "学号：" << it->StudentID << std::endl;
											std::cout << "姓名：" << it->StudentName << std::endl;
											std::cout << "性别：" << it->StudentGender << std::endl;
											std::cout << "年龄：" << it->StudentAge << std::endl;
											std::cout << "所在系：" << it->StudentSystem << std::endl;
											std::cout << "------------------------------------" << std::endl;

											isQuery = true;
										}
									}

									if (isQuery == 0)
									{
										std::cout << "暂无该性别此所在系的学生信息" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									break;

								}
								else
								{
									std::cout << "选择错误，请重新选择" << std::endl;
									std::cout << "------------------------------------" << std::endl;
								}
							}
							break;
						}
						else
						{
							std::cout << "选择错误，请重新选择" << std::endl;
							std::cout << "------------------------------------" << std::endl;
						}
					}
					break;
				}
				else if (isconfirm == "No" || isconfirm == "no")
				{
					std::string studentid;
					std::string studentname;
					std::string studentgender;
					std::string studentage;
					std::string studentsystem;
					std::vector<StudentInformation>Students;

					StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

					Studentinformation.LoadStudentsFromFile(Students);

					std::vector<StudentInformation>::iterator it;

					bool isQuery = false;
					for (it = Students.begin(); it != Students.end(); it++)
					{
						if (it->StudentGender == gender)
						{
							if (isQuery == 0)
							{
								std::cout << "查询成功" << std::endl;
								std::cout << "------------------------------------" << std::endl;
							}

							std::cout << "学号：" << it->StudentID << std::endl;
							std::cout << "姓名：" << it->StudentName << std::endl;
							std::cout << "性别：" << it->StudentGender << std::endl;
							std::cout << "年龄：" << it->StudentAge << std::endl;
							std::cout << "所在系：" << it->StudentSystem << std::endl;
							std::cout << "------------------------------------" << std::endl;

							isQuery = true;
						}
					}

					if (isQuery == 0)
					{
						std::cout << "暂无该性别的学生信息" << std::endl;
						std::cout << "------------------------------------" << std::endl;
					}

					break;
				}
				else
				{
					std::cout << "选择错误，请重新选择" << std::endl;
					std::cout << "------------------------------------" << std::endl;
				}
			}
		}

		else if (decision == "4")
		{
			std::string age;
			std::cout << "请输入年龄" << std::endl;
			std::getline(std::cin, age);

			RefreshScreenBuffer();

			while (1)
			{
				std::string isconfirm;
				std::cout << "是否有其他查询要求（Yes/No）" << std::endl;
				std::getline(std::cin, isconfirm);

				RefreshScreenBuffer();

				if (isconfirm == "Yes" || isconfirm == "yes")
				{
					while (1)
					{
						std::cout << "菜单（输入序号即可选择）：" << std::endl;
						std::cout << "1--按学号查询学生信息" << std::endl;
						std::cout << "2--按姓名查询学生信息" << std::endl;
						std::cout << "3--按性别查询学生信息" << std::endl;
						std::cout << "4--按所在系查询学生信息" << std::endl;

						std::string Decision;

						std::getline(std::cin, Decision);

						RefreshScreenBuffer();

						if (Decision == "1")
						{
							std::string id;
							std::cout << "请输入学号" << std::endl;
							std::getline(std::cin, id);

							RefreshScreenBuffer();

							std::string studentid;
							std::string studentname;
							std::string studentgender;
							std::string studentage;
							std::string studentsystem;
							std::vector<StudentInformation>Students;

							StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

							Studentinformation.LoadStudentsFromFile(Students);

							std::vector<StudentInformation>::iterator it;

							bool isQuery = false;
							for (it = Students.begin(); it != Students.end(); it++)
							{
								if (it->StudentAge == age && it->StudentID == id)
								{
									if (isQuery == 0)
									{
										std::cout << "查询成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									std::cout << "学号：" << it->StudentID << std::endl;
									std::cout << "姓名：" << it->StudentName << std::endl;
									std::cout << "性别：" << it->StudentGender << std::endl;
									std::cout << "年龄：" << it->StudentAge << std::endl;
									std::cout << "所在系：" << it->StudentSystem << std::endl;
									std::cout << "------------------------------------" << std::endl;

									isQuery = true;
								}
							}

							if (isQuery == 0)
							{
								std::cout << "暂无该年龄此学号的学生信息" << std::endl;
								std::cout << "------------------------------------" << std::endl;
							}

							break;

						}
						else if (Decision == "2")
						{
							std::string name;
							std::cout << "请输入姓名" << std::endl;
							std::getline(std::cin, name);

							RefreshScreenBuffer();

							std::string studentid;
							std::string studentname;
							std::string studentgender;
							std::string studentage;
							std::string studentsystem;
							std::vector<StudentInformation>Students;

							StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

							Studentinformation.LoadStudentsFromFile(Students);

							std::vector<StudentInformation>::iterator it;

							bool isQuery = false;

							for (it = Students.begin(); it != Students.end(); it++)
							{
								if (it->StudentAge == age && it->StudentName == name)
								{
									if (isQuery == 0)
									{
										std::cout << "查询成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									std::cout << "学号：" << it->StudentID << std::endl;
									std::cout << "姓名：" << it->StudentName << std::endl;
									std::cout << "性别：" << it->StudentGender << std::endl;
									std::cout << "年龄：" << it->StudentAge << std::endl;
									std::cout << "所在系：" << it->StudentSystem << std::endl;
									std::cout << "------------------------------------" << std::endl;

									isQuery = true;
								}
							}

							if (isQuery == 0)
							{
								std::cout << "暂无该年龄此姓名的学生信息" << std::endl;
								std::cout << "------------------------------------" << std::endl;
							}

							break;

						}
						else if (Decision == "3")
						{
							std::string gender;
							std::cout << "请输入性别" << std::endl;
							std::getline(std::cin, gender);

							RefreshScreenBuffer();

							while (1)
							{
								std::string Isconfirm;
								std::cout << "是否还有其他查询要求（Yes/No）" << std::endl;
								std::getline(std::cin, Isconfirm);

								RefreshScreenBuffer();

								if (Isconfirm == "Yes" || Isconfirm == "yes")
								{
									while (1)
									{
										std::cout << "菜单（输入序号即可选择）：" << std::endl;
										std::cout << "1--按学号查询学生信息" << std::endl;
										std::cout << "2--按姓名查询学生信息" << std::endl;
										std::cout << "3--按所在系查询学生信息" << std::endl;

										std::string Resolution;

										std::getline(std::cin, Resolution);

										RefreshScreenBuffer();

										if (Resolution == "1")
										{
											std::string id;
											std::cout << "请输入学号" << std::endl;
											std::getline(std::cin, id);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentAge == age && it->StudentGender == gender && it->StudentID == id)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该学号年龄此性别的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "2")
										{
											std::string name;
											std::cout << "请输入姓名" << std::endl;
											std::getline(std::cin, name);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentAge == age && it->StudentGender == gender && it->StudentName == name)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该姓名年龄此性别的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "3")
										{
											std::string system;
											std::cout << "请输入所在系" << std::endl;
											std::getline(std::cin, system);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentAge == age && it->StudentGender == gender && it->StudentSystem == system)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该年龄性别此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else
										{
											std::cout << "选择错误，请重新选择" << std::endl;
											std::cout << "------------------------------------" << std::endl;
										}
									}

									break;
								}
								else if (Isconfirm == "No" || Isconfirm == "no")
								{
									std::string studentid;
									std::string studentname;
									std::string studentgender;
									std::string studentage;
									std::string studentsystem;
									std::vector<StudentInformation>Students;

									StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

									Studentinformation.LoadStudentsFromFile(Students);

									std::vector<StudentInformation>::iterator it;

									bool isQuery = false;
									for (it = Students.begin(); it != Students.end(); it++)
									{
										if (it->StudentAge == age && it->StudentGender == gender)
										{
											if (isQuery == 0)
											{
												std::cout << "查询成功" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											std::cout << "学号：" << it->StudentID << std::endl;
											std::cout << "姓名：" << it->StudentName << std::endl;
											std::cout << "性别：" << it->StudentGender << std::endl;
											std::cout << "年龄：" << it->StudentAge << std::endl;
											std::cout << "所在系：" << it->StudentSystem << std::endl;
											std::cout << "------------------------------------" << std::endl;

											isQuery = true;
										}
									}

									if (isQuery == 0)
									{
										std::cout << "暂无该年龄此性别的学生信息" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									break;

								}
								else
								{
									std::cout << "选择错误，请重新选择" << std::endl;
									std::cout << "------------------------------------" << std::endl;
								}
							}
							break;
						}
						else if (Decision == "4")
						{
							std::string system;
							std::cout << "请输入所在系" << std::endl;
							std::getline(std::cin, system);

							RefreshScreenBuffer();

							while (1)
							{
								std::string Isconfirm;
								std::cout << "是否还有其他查询要求（Yes/No）" << std::endl;
								std::getline(std::cin, Isconfirm);

								RefreshScreenBuffer();

								if (Isconfirm == "Yes" || Isconfirm == "yes")
								{
									while (1)
									{
										std::cout << "菜单（输入序号即可选择）：" << std::endl;
										std::cout << "1--按学号查询学生信息" << std::endl;
										std::cout << "2--按姓名查询学生信息" << std::endl;
										std::cout << "3--按性别查询学生信息" << std::endl;
										std::string Resolution;

										std::getline(std::cin, Resolution);

										RefreshScreenBuffer();

										if (Resolution == "1")
										{
											std::string id;
											std::cout << "请输入学号" << std::endl;
											std::getline(std::cin, id);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentAge == age && it->StudentID == id)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该学号年龄此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "2")
										{
											std::string name;
											std::cout << "请输入姓名" << std::endl;
											std::getline(std::cin, name);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentAge == age && it->StudentName == name)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该姓名年龄此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "3")
										{
											std::string gender;
											std::cout << "请输入性别" << std::endl;
											std::getline(std::cin, gender);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentAge == age && it->StudentGender == gender)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该年龄性别此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else
										{
											std::cout << "选择错误，请重新选择" << std::endl;
											std::cout << "------------------------------------" << std::endl;
										}
									}

									break;
								}
								else if (Isconfirm == "No" || Isconfirm == "no")
								{
									std::string studentid;
									std::string studentname;
									std::string studentgender;
									std::string studentage;
									std::string studentsystem;
									std::vector<StudentInformation>Students;

									StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

									Studentinformation.LoadStudentsFromFile(Students);

									std::vector<StudentInformation>::iterator it;

									bool isQuery = false;
									for (it = Students.begin(); it != Students.end(); it++)
									{
										if (it->StudentSystem == system && it->StudentAge == age)
										{
											if (isQuery == 0)
											{
												std::cout << "查询成功" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											std::cout << "学号：" << it->StudentID << std::endl;
											std::cout << "姓名：" << it->StudentName << std::endl;
											std::cout << "性别：" << it->StudentGender << std::endl;
											std::cout << "年龄：" << it->StudentAge << std::endl;
											std::cout << "所在系：" << it->StudentSystem << std::endl;
											std::cout << "------------------------------------" << std::endl;

											isQuery = true;
										}
									}

									if (isQuery == 0)
									{
										std::cout << "暂无该年龄此所在系的学生信息" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									break;

								}
								else
								{
									std::cout << "选择错误，请重新选择" << std::endl;
									std::cout << "------------------------------------" << std::endl;
								}
							}
							break;
						}
						else
						{
							std::cout << "选择错误，请重新选择" << std::endl;
							std::cout << "------------------------------------" << std::endl;
						}
					}
					break;
				}
				else if (isconfirm == "No" || isconfirm == "no")
				{
					std::string studentid;
					std::string studentname;
					std::string studentgender;
					std::string studentage;
					std::string studentsystem;
					std::vector<StudentInformation>Students;

					StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

					Studentinformation.LoadStudentsFromFile(Students);

					std::vector<StudentInformation>::iterator it;

					bool isQuery = false;
					for (it = Students.begin(); it != Students.end(); it++)
					{
						if (it->StudentAge == age)
						{
							if (isQuery == 0)
							{
								std::cout << "查询成功" << std::endl;
								std::cout << "------------------------------------" << std::endl;
							}

							std::cout << "学号：" << it->StudentID << std::endl;
							std::cout << "姓名：" << it->StudentName << std::endl;
							std::cout << "性别：" << it->StudentGender << std::endl;
							std::cout << "年龄：" << it->StudentAge << std::endl;
							std::cout << "所在系：" << it->StudentSystem << std::endl;
							std::cout << "------------------------------------" << std::endl;

							isQuery = true;
						}
					}

					if (isQuery == 0)
					{
						std::cout << "暂无该年龄的学生信息" << std::endl;
						std::cout << "------------------------------------" << std::endl;
					}

					break;
				}
				else
				{
					std::cout << "选择错误，请重新选择" << std::endl;
					std::cout << "------------------------------------" << std::endl;
				}
			}
		}

		else if (decision == "5")
		{
			std::string system;
			std::cout << "请输入所在系" << std::endl;
			std::getline(std::cin, system);

			RefreshScreenBuffer();
			
			while (1)
			{
				std::string isconfirm;
				std::cout << "是否有其他查询要求（Yes/No）" << std::endl;
				std::getline(std::cin, isconfirm);

				RefreshScreenBuffer();

				if (isconfirm == "Yes" || isconfirm == "yes")
				{
					while (1)
					{
						std::cout << "菜单（输入序号即可选择）：" << std::endl;
						std::cout << "1--按学号查询学生信息" << std::endl;
						std::cout << "2--按姓名查询学生信息" << std::endl;
						std::cout << "3--按性别查询学生信息" << std::endl;
						std::cout << "4--按年龄查询学生信息" << std::endl;

						std::string Decision;

						std::getline(std::cin, Decision);

						RefreshScreenBuffer();

						if (Decision == "1")
						{
							std::string id;
							std::cout << "请输入学号" << std::endl;
							std::getline(std::cin, id);

							RefreshScreenBuffer();

							std::string studentid;
							std::string studentname;
							std::string studentgender;
							std::string studentage;
							std::string studentsystem;
							std::vector<StudentInformation>Students;

							StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

							Studentinformation.LoadStudentsFromFile(Students);

							std::vector<StudentInformation>::iterator it;

							bool isQuery = false;
							for (it = Students.begin(); it != Students.end(); it++)
							{
								if (it->StudentSystem == system && it->StudentID == id)
								{
									if (isQuery == 0)
									{
										std::cout << "查询成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									std::cout << "学号：" << it->StudentID << std::endl;
									std::cout << "姓名：" << it->StudentName << std::endl;
									std::cout << "性别：" << it->StudentGender << std::endl;
									std::cout << "年龄：" << it->StudentAge << std::endl;
									std::cout << "所在系：" << it->StudentSystem << std::endl;
									std::cout << "------------------------------------" << std::endl;

									isQuery = true;
								}
							}

							if (isQuery == 0)
							{
								std::cout << "暂无该学号此所在系的学生信息" << std::endl;
								std::cout << "------------------------------------" << std::endl;
							}

							break;

						}
						else if (Decision == "2")
						{
							std::string name;
							std::cout << "请输入姓名" << std::endl;
							std::getline(std::cin, name);

							RefreshScreenBuffer();

							std::string studentid;
							std::string studentname;
							std::string studentgender;
							std::string studentage;
							std::string studentsystem;
							std::vector<StudentInformation>Students;

							StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

							Studentinformation.LoadStudentsFromFile(Students);

							std::vector<StudentInformation>::iterator it;

							bool isQuery = false;

							for (it = Students.begin(); it != Students.end(); it++)
							{
								if (it->StudentSystem == system && it->StudentName == name)
								{
									if (isQuery == 0)
									{
										std::cout << "查询成功" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									std::cout << "学号：" << it->StudentID << std::endl;
									std::cout << "姓名：" << it->StudentName << std::endl;
									std::cout << "性别：" << it->StudentGender << std::endl;
									std::cout << "年龄：" << it->StudentAge << std::endl;
									std::cout << "所在系：" << it->StudentSystem << std::endl;
									std::cout << "------------------------------------" << std::endl;

									isQuery = true;
								}
							}

							if (isQuery == 0)
							{
								std::cout << "暂无该姓名此所在系的学生信息" << std::endl;
								std::cout << "------------------------------------" << std::endl;
							}

							break;

						}
						else if (Decision == "3")
						{
							std::string gender;
							std::cout << "请输入性别" << std::endl;
							std::getline(std::cin, gender);

							RefreshScreenBuffer();

							while (1)
							{
								std::string Isconfirm;
								std::cout << "是否还有其他查询要求（Yes/No）" << std::endl;
								std::getline(std::cin, Isconfirm);

								RefreshScreenBuffer();

								if (Isconfirm == "Yes" || Isconfirm == "yes")
								{
									while (1)
									{
										std::cout << "菜单（输入序号即可选择）：" << std::endl;
										std::cout << "1--按学号查询学生信息" << std::endl;
										std::cout << "2--按姓名查询学生信息" << std::endl;
										std::cout << "3--按年龄查询学生信息" << std::endl;
										std::string Resolution;

										std::getline(std::cin, Resolution);

										RefreshScreenBuffer();

										if (Resolution == "1")
										{
											std::string id;
											std::cout << "请输入学号" << std::endl;
											std::getline(std::cin, id);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentGender == gender && it->StudentID == id)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该学号性别此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "2")
										{
											std::string name;
											std::cout << "请输入姓名" << std::endl;
											std::getline(std::cin, name);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentGender == gender && it->StudentName == name)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该姓名性别此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "3")
										{
											std::string age;
											std::cout << "请输入年龄" << std::endl;
											std::getline(std::cin, age);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentGender == gender && it->StudentAge == age)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该性别年龄此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else
										{
											std::cout << "选择错误，请重新选择" << std::endl;
											std::cout << "------------------------------------" << std::endl;
										}
									}

									break;
								}
								else if (Isconfirm == "No" || Isconfirm == "no")
								{
									std::string studentid;
									std::string studentname;
									std::string studentgender;
									std::string studentage;
									std::string studentsystem;
									std::vector<StudentInformation>Students;

									StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

									Studentinformation.LoadStudentsFromFile(Students);

									std::vector<StudentInformation>::iterator it;

									bool isQuery = false;
									for (it = Students.begin(); it != Students.end(); it++)
									{
										if (it->StudentSystem == system && it->StudentGender == gender)
										{
											if (isQuery == 0)
											{
												std::cout << "查询成功" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											std::cout << "学号：" << it->StudentID << std::endl;
											std::cout << "姓名：" << it->StudentName << std::endl;
											std::cout << "性别：" << it->StudentGender << std::endl;
											std::cout << "年龄：" << it->StudentAge << std::endl;
											std::cout << "所在系：" << it->StudentSystem << std::endl;
											std::cout << "------------------------------------" << std::endl;

											isQuery = true;
										}
									}

									if (isQuery == 0)
									{
										std::cout << "暂无该性别此所在系的学生信息" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									break;

								}
								else
								{
									std::cout << "选择错误，请重新选择" << std::endl;
									std::cout << "------------------------------------" << std::endl;
								}
							}
							break;
						}

						else if (Decision == "4")
						{
							std::string age;
							std::cout << "请输入年龄" << std::endl;
							std::getline(std::cin, age);

							RefreshScreenBuffer();

							while (1)
							{
								std::string Isconfirm;
								std::cout << "是否还有其他查询要求（Yes/No）" << std::endl;
								std::getline(std::cin, Isconfirm);

								RefreshScreenBuffer();

								if (Isconfirm == "Yes" || Isconfirm == "yes")
								{
									while (1)
									{
										std::cout << "菜单（输入序号即可选择）：" << std::endl;
										std::cout << "1--按学号查询学生信息" << std::endl;
										std::cout << "2--按姓名查询学生信息" << std::endl;
										std::cout << "3--按性别查询学生信息" << std::endl;

										std::string Resolution;

										std::getline(std::cin, Resolution);

										RefreshScreenBuffer();

										if (Resolution == "1")
										{
											std::string id;
											std::cout << "请输入学号" << std::endl;
											std::getline(std::cin, id);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentAge == age && it->StudentID == id)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该学号年龄此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "2")
										{
											std::string name;
											std::cout << "请输入姓名" << std::endl;
											std::getline(std::cin, name);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentAge == age && it->StudentName == name)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该姓名年龄此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else if (Resolution == "3")
										{
											std::string gender;
											std::cout << "请输入性别" << std::endl;
											std::getline(std::cin, gender);

											RefreshScreenBuffer();

											std::string studentid;
											std::string studentname;
											std::string studentgender;
											std::string studentage;
											std::string studentsystem;
											std::vector<StudentInformation>Students;

											StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

											Studentinformation.LoadStudentsFromFile(Students);

											std::vector<StudentInformation>::iterator it;

											bool isQuery = false;
											for (it = Students.begin(); it != Students.end(); it++)
											{
												if (it->StudentSystem == system && it->StudentAge == age && it->StudentGender == gender)
												{
													if (isQuery == 0)
													{
														std::cout << "查询成功" << std::endl;
														std::cout << "------------------------------------" << std::endl;
													}

													std::cout << "学号：" << it->StudentID << std::endl;
													std::cout << "姓名：" << it->StudentName << std::endl;
													std::cout << "性别：" << it->StudentGender << std::endl;
													std::cout << "年龄：" << it->StudentAge << std::endl;
													std::cout << "所在系：" << it->StudentSystem << std::endl;
													std::cout << "------------------------------------" << std::endl;

													isQuery = true;
												}
											}

											if (isQuery == 0)
											{
												std::cout << "暂无该年龄性别此所在系的学生信息" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											break;
										}
										else
										{
											std::cout << "选择错误，请重新选择" << std::endl;
											std::cout << "------------------------------------" << std::endl;
										}
									}

									break;
								}
								else if (Isconfirm == "No" || Isconfirm == "no")
								{
									std::string studentid;
									std::string studentname;
									std::string studentgender;
									std::string studentage;
									std::string studentsystem;
									std::vector<StudentInformation>Students;

									StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

									Studentinformation.LoadStudentsFromFile(Students);

									std::vector<StudentInformation>::iterator it;

									bool isQuery = false;
									for (it = Students.begin(); it != Students.end(); it++)
									{
										if (it->StudentSystem == system && it->StudentAge == age)
										{
											if (isQuery == 0)
											{
												std::cout << "查询成功" << std::endl;
												std::cout << "------------------------------------" << std::endl;
											}

											std::cout << "学号：" << it->StudentID << std::endl;
											std::cout << "姓名：" << it->StudentName << std::endl;
											std::cout << "性别：" << it->StudentGender << std::endl;
											std::cout << "年龄：" << it->StudentAge << std::endl;
											std::cout << "所在系：" << it->StudentSystem << std::endl;
											std::cout << "------------------------------------" << std::endl;

											isQuery = true;
										}
									}

									if (isQuery == 0)
									{
										std::cout << "暂无该该年龄此所在系的学生信息" << std::endl;
										std::cout << "------------------------------------" << std::endl;
									}

									break;

								}
								else
								{
									std::cout << "选择错误，请重新选择" << std::endl;
									std::cout << "------------------------------------" << std::endl;
								}
							}
							break;
						}
						else
						{
							std::cout << "选择错误，请重新选择" << std::endl;
							std::cout << "------------------------------------" << std::endl;
						}
					}
					break;
				}
				else if (isconfirm == "No" || isconfirm == "no")
				{
					std::string studentid;
					std::string studentname;
					std::string studentgender;
					std::string studentage;
					std::string studentsystem;
					std::vector<StudentInformation>Students;

					StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

					Studentinformation.LoadStudentsFromFile(Students);

					std::vector<StudentInformation>::iterator it;

					bool isQuery = false;
					for (it = Students.begin(); it != Students.end(); it++)
					{
						if (it->StudentSystem == system)
						{
							if (isQuery == 0)
							{
								std::cout << "查询成功" << std::endl;
								std::cout << "------------------------------------" << std::endl;
							}

							std::cout << "学号：" << it->StudentID << std::endl;
							std::cout << "姓名：" << it->StudentName << std::endl;
							std::cout << "性别：" << it->StudentGender << std::endl;
							std::cout << "年龄：" << it->StudentAge << std::endl;
							std::cout << "所在系：" << it->StudentSystem << std::endl;
							std::cout << "------------------------------------" << std::endl;

							isQuery = true;
						}
					}

					if (isQuery == 0)
					{
						std::cout << "暂无此所在系的学生信息" << std::endl;
						std::cout << "------------------------------------" << std::endl;
					}

					break;
				}
				else
				{
					std::cout << "选择错误，请重新选择" << std::endl;
					std::cout << "------------------------------------" << std::endl;
				}
			}
		}

		else if (decision == "6")
		{
			std::cout << "退出菜单成功" << std::endl;
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

void QueryCourseFunction()
{
	while (1)
	{
		std::cout << "菜单（输入序号即可选择）：" << std::endl;
		std::cout << "1--按课号查询课程信息" << std::endl;
		std::cout << "2--按课名查询课程信息" << std::endl;
		std::cout << "3--按任课教师查询课程信息" << std::endl;
		std::cout << "4--退出菜单" << std::endl;

		std::string decision;
		std::getline(std::cin, decision);

		RefreshScreenBuffer();

		if (decision == "1")
		{
			std::string id;
			std::cout << "请输入课号" << std::endl;
			std::getline(std::cin, id);

			RefreshScreenBuffer();

			std::string	coursenumber;
			std::string coursename;
			std::string courseteacher;
			std::vector<CourseInformation>courses;


			CourseInformation courseinformation(coursenumber, coursename, courseteacher);

			courseinformation.LoadCoursesFromFile(courses);

			std::vector<CourseInformation>::iterator it;

			bool isQuery = false;
			for (it = courses.begin(); it != courses.end(); it++)
			{
				if (it->CourseNumber == id)
				{
					if (isQuery == 0)
					{
						std::cout << "查找成功" << std::endl;
						std::cout << "------------------------------------" << std::endl;
					}

					std::cout << "课号：" << it->CourseNumber << std::endl;
					std::cout << "课名：" << it->CourseName << std::endl;
					std::cout << "任课教师：" << it->CourseTeacher << std::endl;
					std::cout << "------------------------------------" << std::endl;

					isQuery = true;

				}

			}

			if (isQuery == 0)
			{
				std::cout << "暂无此课号的课程信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}

		}
		else if (decision == "2")
		{
			std::string name;
			std::cout << "请输入课名" << std::endl;
			std::getline(std::cin, name);

			RefreshScreenBuffer();

			std::string	coursenumber;
			std::string coursename;
			std::string courseteacher;
			std::vector<CourseInformation>courses;


			CourseInformation courseinformation(coursenumber, coursename, courseteacher);

			courseinformation.LoadCoursesFromFile(courses);

			std::vector<CourseInformation>::iterator it;

			bool isQuery = false;
			for (it = courses.begin(); it != courses.end(); it++)
			{
				if (it->CourseName == name)
				{
					if (isQuery == 0)
					{
						std::cout << "查找成功" << std::endl;
						std::cout << "------------------------------------" << std::endl;
					}
					std::cout << "课号：" << it->CourseNumber << std::endl;
					std::cout << "课名：" << it->CourseName << std::endl;
					std::cout << "任课教师：" << it->CourseTeacher << std::endl;
					std::cout << "------------------------------------" << std::endl;

					isQuery = true;
				}

			}

			if (isQuery == 0)
			{
				std::cout << "暂无此课名的课程信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}

		}
		else if (decision == "3")
		{
			std::string teacher;
			std::cout << "请输入任课教师" << std::endl;
			std::getline(std::cin, teacher);

			RefreshScreenBuffer();

			std::string	coursenumber;
			std::string coursename;
			std::string courseteacher;
			std::vector<CourseInformation>courses;


			CourseInformation courseinformation(coursenumber, coursename, courseteacher);

			courseinformation.LoadCoursesFromFile(courses);

			std::vector<CourseInformation>::iterator it;

			bool isQuery = false;
			for (it = courses.begin(); it != courses.end(); it++)
			{

				if (it->CourseTeacher == teacher)
				{
					if (isQuery == 0)
					{
						std::cout << "查找成功" << std::endl;
						std::cout << "------------------------------------" << std::endl;
					}

					std::cout << "课号：" << it->CourseNumber << std::endl;
					std::cout << "课名：" << it->CourseName << std::endl;
					std::cout << "任课教师：" << it->CourseTeacher << std::endl;
					std::cout << "------------------------------------" << std::endl;

					isQuery = true;

				}

			}

			if (isQuery == 0)
			{
				std::cout << "暂无此任课老师的课程信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}

		}
		else if (decision == "4")
		{
			std::cout << "退出菜单成功" << std::endl;
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

void QueryScoreFunction()
{
	while (1)
	{
		std::cout << "菜单（输入序号即可选择）：" << std::endl;
		std::cout << "1--按学号查询学生成绩" << std::endl;
		std::cout << "2--按姓名查询学生成绩" << std::endl;
		std::cout << "3--按课号查询学生成绩" << std::endl;
		std::cout << "4--按课名查询学生成绩" << std::endl;
		std::cout << "5--退出菜单" << std::endl;

		std::string decision;
		std::getline(std::cin, decision);

		RefreshScreenBuffer();

		if (decision == "1")
		{
			std::string id;
			std::cout << "请输入学号" << std::endl;
			std::getline(std::cin, id);

			RefreshScreenBuffer();

			std::string Studentid;
			std::string Classid;
			int usualscore = -1;
			int rollscore = -1;
			int score = -1;
			std::vector<ScoreInformation>Scores;

			ScoreInformation scoreinformation(Studentid, Classid, usualscore, rollscore, score);

			scoreinformation.LoadScoresFromFile(Scores);


			std::vector<ScoreInformation>::iterator it;

			bool isQuery = false;

			for (it = Scores.begin(); it != Scores.end(); it++)
			{
				if (it->StudentID == id)
				{
					if (isQuery == 0)
					{
						std::cout << "查询成功" << std::endl;
						std::cout << "------------------------------------" << std::endl;
					}

					std::cout << "学号：" << it->StudentID << std::endl;

					//利用学号在"Student.txt"中查找姓名并输出
					std::string studentid;
					std::string studentname;
					std::string studentgender;
					std::string studentage;
					std::string studentsystem;
					std::vector<StudentInformation>Students;

					StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

					Studentinformation.LoadStudentsFromFile(Students);

					std::vector<StudentInformation>::iterator It;

					for (It = Students.begin(); It != Students.end(); It++)
					{
						if (It->StudentID == it->StudentID)
						{
							std::cout << "姓名：" << It->StudentName << std::endl;
							break;
						}
					}
					//查找姓名结束

					std::cout << "课号：" << it->ClassID << std::endl;

					//利用课号在"Course.txt"中查找课名并输出

					std::string coursenumber;
					std::string coursename;
					std::string courseteacher;

					std::vector<CourseInformation>Courses;

					CourseInformation courseinformation(coursenumber, coursename, courseteacher);

					courseinformation.LoadCoursesFromFile(Courses);

					std::vector<CourseInformation>::iterator IT;

					for (IT = Courses.begin(); IT != Courses.end(); IT++)
					{
						if (IT->CourseNumber == it->ClassID)
						{
							std::cout << "课名：" << IT->CourseName << std::endl;
							break;
						}
					}

					//查找课号结束

					std::cout << "平时成绩：" << it->UsualScore << std::endl;
					std::cout << "卷面成绩：" << it->RollScore << std::endl;
					std::cout << "总成绩：" << it->Score << std::endl;
					std::cout << "------------------------------------" << std::endl;

					isQuery = true;
				}
			}

			if (isQuery == 0)
			{
				std::cout << "暂无此学号的成绩信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}
		}
		else if (decision == "2")
		{
			std::string name;
			std::cout << "请输入姓名" << std::endl;
			std::getline(std::cin, name);

			RefreshScreenBuffer();

			std::string StudentId;

			//利用姓名在"Student.txt"中查找学号并输出
			std::string studentid;
			std::string studentname;
			std::string studentgender;
			std::string studentage;
			std::string studentsystem;
			std::vector<StudentInformation>Students;

			StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

			Studentinformation.LoadStudentsFromFile(Students);

			std::vector<StudentInformation>::iterator It;

			bool isnameexist = false;
			for (It = Students.begin(); It != Students.end(); It++)
			{
				if (It->StudentName == name)
				{
					StudentId = It->StudentID;
					isnameexist = true;
					break;
				}
			}
			//查找学号结束

			if (isnameexist == 0)
			{
				std::cout << "暂无此姓名的学生信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}
			else
			{
				std::string Studentid;
				std::string Classid;
				int usualscore = -1;
				int rollscore = -1;
				int score = -1;
				std::vector<ScoreInformation>Scores;

				ScoreInformation scoreinformation(Studentid, Classid, usualscore, rollscore, score);

				scoreinformation.LoadScoresFromFile(Scores);

				std::vector<ScoreInformation>::iterator it;

				bool isQuery = false;

				for (it = Scores.begin(); it != Scores.end(); it++)
				{
					if (it->StudentID == StudentId)
					{
						if (isQuery == 0)
						{
							std::cout << "查询成功" << std::endl;
							std::cout << "------------------------------------" << std::endl;
						}

						std::cout << "学号：" << it->StudentID << std::endl;
						std::cout << "姓名：" << name << std::endl;
						std::cout << "课号：" << it->ClassID << std::endl;

						//利用课号在"Course.txt"中查找课名并输出

						std::string coursenumber;
						std::string coursename;
						std::string courseteacher;

						std::vector<CourseInformation>Courses;

						CourseInformation courseinformation(coursenumber, coursename, courseteacher);

						courseinformation.LoadCoursesFromFile(Courses);

						std::vector<CourseInformation>::iterator IT;

						for (IT = Courses.begin(); IT != Courses.end(); IT++)
						{
							if (IT->CourseNumber == it->ClassID)
							{
								std::cout << "课名：" << IT->CourseName << std::endl;
								break;
							}
						}

						//查找课号结束

						std::cout << "平时成绩：" << it->UsualScore << std::endl;
						std::cout << "卷面成绩：" << it->RollScore << std::endl;
						std::cout << "总成绩：" << it->Score << std::endl;
						std::cout << "------------------------------------" << std::endl;

						isQuery = true;

					}

				}

				if (isQuery == 0)
				{
					std::cout << "暂无此学号的成绩信息" << std::endl;
					std::cout << "------------------------------------" << std::endl;
				}
			}
		}
		else if (decision == "3")
		{
			std::string id;
			std::cout << "请输入课号" << std::endl;
			std::getline(std::cin, id);

			RefreshScreenBuffer();

			std::string Studentid;
			std::string Classid;
			int usualscore = -1;
			int rollscore = -1;
			int score = -1;
			std::vector<ScoreInformation>Scores;

			ScoreInformation scoreinformation(Studentid, Classid, usualscore, rollscore, score);

			scoreinformation.LoadScoresFromFile(Scores);


			std::vector<ScoreInformation>::iterator it;

			bool isQuery = false;

			for (it = Scores.begin(); it != Scores.end(); it++)
			{
				if (it->ClassID == id)
				{
					if (isQuery == 0)
					{
						std::cout << "查询成功" << std::endl;
						std::cout << "------------------------------------" << std::endl;
					}

					std::cout << "学号：" << it->StudentID << std::endl;

					//利用学号在"Student.txt"中查找姓名并输出
					std::string studentid;
					std::string studentname;
					std::string studentgender;
					std::string studentage;
					std::string studentsystem;
					std::vector<StudentInformation>Students;

					StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

					Studentinformation.LoadStudentsFromFile(Students);

					std::vector<StudentInformation>::iterator It;

					for (It = Students.begin(); It != Students.end(); It++)
					{
						if (It->StudentID == it->StudentID)
						{
							std::cout << "姓名：" << It->StudentName << std::endl;
							break;
						}
					}
					//查找姓名结束

					std::cout << "课号：" << it->ClassID << std::endl;

					//利用课号在"Course.txt"中查找课名并输出

					std::string coursenumber;
					std::string coursename;
					std::string courseteacher;

					std::vector<CourseInformation>Courses;

					CourseInformation courseinformation(coursenumber, coursename, courseteacher);

					courseinformation.LoadCoursesFromFile(Courses);

					std::vector<CourseInformation>::iterator IT;

					for (IT = Courses.begin(); IT != Courses.end(); IT++)
					{
						if (IT->CourseNumber == it->ClassID)
						{
							std::cout << "课名：" << IT->CourseName << std::endl;
							break;
						}
					}

					//查找课号结束

					std::cout << "平时成绩：" << it->UsualScore << std::endl;
					std::cout << "卷面成绩：" << it->RollScore << std::endl;
					std::cout << "总成绩：" << it->Score << std::endl;
					std::cout << "------------------------------------" << std::endl;

					isQuery = true;
				}
			}

			if (isQuery == 0)
			{
				std::cout << "暂无此学号的成绩信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}
		}
		else if (decision == "4")
		{
			std::string name;
			std::cout << "请输入课名" << std::endl;
			std::getline(std::cin, name);

			RefreshScreenBuffer();

			std::string ClassId;

			//利用课名在"Course.txt"中查找课号并输出
			std::string coursenumber;
			std::string coursename;
			std::string courseteacher;
			std::vector<CourseInformation>Courses;

			CourseInformation courseinformation(coursenumber, coursename, courseteacher);

			courseinformation.LoadCoursesFromFile(Courses);

			std::vector<CourseInformation>::iterator IT;

			bool isnameexist = false;

			for (IT = Courses.begin(); IT != Courses.end(); IT++)
			{
				if (IT->CourseName == name)
				{
					ClassId = IT->CourseNumber;
					isnameexist = true;
					break;
				}
			}
			//查找课号结束

			if (isnameexist == 0)
			{
				std::cout << "暂无此课名的课程信息" << std::endl;
				std::cout << "------------------------------------" << std::endl;
			}
			else
			{
				std::string Studentid;
				std::string Classid;
				int usualscore = -1;
				int rollscore = -1;
				int score = -1;
				std::vector<ScoreInformation>Scores;

				ScoreInformation scoreinformation(Studentid, Classid, usualscore, rollscore, score);

				scoreinformation.LoadScoresFromFile(Scores);

				std::vector<ScoreInformation>::iterator it;

				bool isQuery = false;

				for (it = Scores.begin(); it != Scores.end(); it++)
				{
					if (it->ClassID == ClassId)
					{
						if (isQuery == 0)
						{
							std::cout << "查询成功" << std::endl;
							std::cout << "------------------------------------" << std::endl;
						}

						std::cout << "学号：" << it->StudentID << std::endl;

						//利用学号在"Student.txt"中查找姓名并输出
						std::string studentid;
						std::string studentname;
						std::string studentgender;
						std::string studentage;
						std::string studentsystem;
						std::vector<StudentInformation>Students;

						StudentInformation Studentinformation(studentid, studentname, studentgender, studentage, studentsystem);

						Studentinformation.LoadStudentsFromFile(Students);

						std::vector<StudentInformation>::iterator It;

						for (It = Students.begin(); It != Students.end(); It++)
						{
							if (It->StudentID == it->StudentID)
							{
								std::cout << "姓名：" << It->StudentName << std::endl;
								break;
							}
						}

						//查找姓名结束

						std::cout << "课号：" << it->ClassID << std::endl;
						std::cout << "课名：" << name << std::endl;
						std::cout << "平时成绩：" << it->UsualScore << std::endl;
						std::cout << "卷面成绩：" << it->RollScore << std::endl;
						std::cout << "------------------------------------" << std::endl;

						isQuery = true;

					}

				}
				if (isQuery == 0)
				{
					std::cout << "暂无此课号的成绩信息" << std::endl;
					std::cout << "------------------------------------" << std::endl;
				}
			}
		}
		else if (decision == "5")
		{
			std::cout << "退出菜单成功" << std::endl;
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