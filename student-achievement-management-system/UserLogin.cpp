#include "UserLogin.h"

void UserLogin::inputUserName()
{

	// 创建学生和管理员登录系统的实例，分别加载学生和管理员数据
	StudentLoginSystem studentloginsystem("StudentData.txt"); 
	AdminLoginSystem  adminloginsystem("AdminData.txt");

	int judge1 = 1, judge2 = 1;
	int judgestudent = 0, judgeadmin = 0;
	int Judge = 1;

	std::string AdminUserName;
	std::string AdminPassword;
	std::string StudentUserName;
	std::string StudentPassword;

	RefreshScreenBuffer();

	//主循环，用于处理用户登录和注册
	while (1)
	{
		// 显示登录方式选择 
		std::cout << "请选择您的登录方式（输入End退出程序）" << std::endl;
		std::cout << "1--学生登录" << std::endl;
		std::cout << "2--管理员登录" << std::endl;

		adminloginsystem.LoadUsersFromFile();

		std::string determinenumber;

		std::getline(std::cin, determinenumber);

		RefreshScreenBuffer();

		if (determinenumber == "End" || determinenumber == "end")
		{
			std::cout << "退出成功" << std::endl;
			std::cout << "------------------------------------" << std::endl;

			break;
		}
		// 处理学生登录
		else if (determinenumber == "1")
		{
			while (1)
			{
				studentloginsystem.LoadUsersFromFile();
				std::cout << "您可以选择注册或登录（输入End返回上一级）" << std::endl;
				std::cout << "1--注册" << std::endl;
				std::cout << "2--登录" << std::endl;

				std::string choicenumber;
				std::getline(std::cin, choicenumber);

				RefreshScreenBuffer();

				if (choicenumber == "end" || choicenumber == "End")
				{
					std::cout << "返回成功" << std::endl;
					std::cout << "------------------------------------" << std::endl;

					break;
				}

				// 用户选择注册
				else if (choicenumber == "1")
				{
					std::string username;

					while (1)
					{
						std::cout << "注册--请输入您的学号（不支持输入空格）" << std::endl;

						std::getline(std::cin, username);

						RefreshScreenBuffer();

						if (username.find(' ') != std::string::npos)
						{
							std::cout << "输入格式错误 不支持输入空格" << std::endl;
							std::cout << "------------------------------------" << std::endl;

							continue;
						}
						break;

					}

					while (1)
					{
						std::cout << "注册--请输入您的密码（不支持输入空格）" << std::endl;
						std::string password1;
						std::getline(std::cin, password1);

						if (password1.find(' ') != std::string::npos)
						{
							std::cout << "输入格式错误 不支持输入空格" << std::endl;
							continue;
						}

						std::cout << "注册--请再次输入以便确认您的密码（不支持输入空格）" << std::endl;
						std::string password2;
						std::getline(std::cin, password2);

						RefreshScreenBuffer();

						if (password1 == password2)
						{
							if (studentloginsystem.RegisterUser(username, password1) == 1)
							{
								std::cout << "注册成功" << std::endl;
								std::cout << "------------------------------------" << std::endl;
								judge1 = 0;
								break;
							}
							else
							{
								std::cout << "用户已存在，不能重复注册" << std::endl;
								std::cout << "------------------------------------" << std::endl;
								Judge = 0;
								break;
							}
						}
						else
						{
							std::cout << "两次密码不相同，请重新输入您的密码" << std::endl;
							std::cout << "------------------------------------" << std::endl;
						}
					}
					if (judge1 == 0)
					{
						break;
					}
				}
				// 用户选择登录
				else if (choicenumber == "2")
				{

					while (1)
					{
						std::cout << "登录--请输入您的学号（输入End返回上一级） " << std::endl;

						std::getline(std::cin, UserName);

						if (UserName == "End" || UserName == "end")
						{
							RefreshScreenBuffer();

							std::cout << "返回成功" << std::endl;
							std::cout << "------------------------------------" << std::endl;
							
							break;
						}

						RefreshScreenBuffer();

						std::cout << "登录--请输入您的密码" << std::endl;
						std::getline(std::cin, Password);

						RefreshScreenBuffer();

						/*
							登录进度条
						*/

						std::cout << "正在登录中 请稍后" << std::endl;

						for (int i = 0; i <= 100; i = i + 4)
						{
							printProgressBar(i, 100);
						}

						RefreshScreenBuffer();

						/*
							进度条加载完成
						*/

						if (studentloginsystem.isLogin(UserName, Password) == 1)
						{
							std::cout << "登陆成功" << std::endl;
							std::cout << "------------------------------------" << std::endl;
							judge1 = 0;
							judgestudent = 1;
							StudentUserName = UserName;
							StudentPassword = Password;
							break;
						}

						if (studentloginsystem.isLogin(UserName, Password) == 2)
						{
							std::cout << "密码错误 请重新登录" << std::endl;
							std::cout << "------------------------------------" << std::endl;
						}

						if (studentloginsystem.isLogin(UserName, Password) == 3)
						{
							std::cout << "该用户名尚未注册 请重新登录" << std::endl;
							std::cout << "------------------------------------" << std::endl;
						}
					}

					if (judge1 == 0)
					{
						break;
					}
				}
				else
				{
					std::cout << "错误操作" << std::endl;
					std::cout << "------------------------------------" << std::endl;
				}
			}
			if (judge2 == 0)
			{
				break;
			}

		}

		// 处理管理员登录 
		else if (determinenumber == "2")
		{
			adminloginsystem.LoadUsersFromFile();

			// 管理员登录循环  
			while (1)
			{
				std::cout << "登录--请输入您的管理员用户名（输入End返回上一级） " << std::endl;

				std::getline(std::cin, UserName);

				RefreshScreenBuffer();

				if (UserName == "end" || UserName == "End")
				{

					std::cout << "返回成功" << std::endl;
					std::cout << "------------------------------------" << std::endl;
					break;
				}

				std::cout << "登录--请输入您的密码" << std::endl;

				std::getline(std::cin, Password);

				RefreshScreenBuffer();

				/*
					登录进度条
				*/

				std::cout << "正在登录中 请稍后" << std::endl;

				for (int i = 0; i <= 100; i = i + 4)
				{
					printProgressBar(i, 100);
				}

				RefreshScreenBuffer();

				/*
					进度条加载完成
				*/

				if (adminloginsystem.isLogin(UserName, Password) == 1)
				{
					std::cout << "登陆成功" << std::endl;
					std::cout << "------------------------------------" << std::endl;
					judge2 = 0;
					judgeadmin = 1;
					AdminUserName = UserName;
					AdminPassword = Password;
					break;
				}

				if (adminloginsystem.isLogin(UserName, Password) == 2)
				{
					std::cout << "密码错误 请重新登录" << std::endl;				
					std::cout << "------------------------------------" << std::endl;
				}

				if (adminloginsystem.isLogin(UserName, Password) == 3)
				{
					std::cout << "该用户名尚未注册 请重新登录" << std::endl;
					std::cout << "------------------------------------" << std::endl;
				}
			}
		}
		else
		{
			std::cout << "选择错误，请重新选择您的登录方式" << std::endl;
			std::cout << "------------------------------------" << std::endl;
		}

	//以下为成功登录后的菜单和程序

	    //学生登录系统：
		if (judgestudent == 1)
		{
			StudentLoginProcedure(StudentUserName, judgestudent);
		}

		//管理员登录系统：
		if (judgeadmin == 1)
		{
			AdminLoginProcedure(AdminUserName, AdminPassword, judgeadmin);
		}

	}
}