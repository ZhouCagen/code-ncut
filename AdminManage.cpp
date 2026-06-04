
/*
	AdminManage() 为管理员对成员身份的管理的实现函数
	此函数可以将特定账号密码设置为管理员登录的账号密码
*/
#include "AdminManage.h"

void AdminManage()
{
	while (1)
	{
		int judge1 = 1;

		AdminLoginSystem  adminloginsystem("AdminData.txt");

		std::cout << "请输入设为管理的账号（输入End返回上一级）" << std::endl;
		std::string username;
		std::getline(std::cin, username);

		RefreshScreenBuffer();


		if (username == "End" || username == "end")
		{
			break;
		}
		else
		{
			while (1)
			{
				std::cout << "请输入设为管理的密码" << std::endl;
				std::string password1;
				std::getline(std::cin, password1);

				std::cout << "请再次输入设为管理的密码" << std::endl;
				std::string password2;
				std::getline(std::cin, password2);

				RefreshScreenBuffer();

				
				if (password1 == password2)
				{

					if (adminloginsystem.ManageUser(username, password1) == 1)
					{
						std::cout << "设置成功" << std::endl;
						std::cout << "------------------------------------" << std::endl;
						break;
					}
					else
					{
						std::cout << "用户已存在，不能重复设置" << std::endl;
						std::cout << "------------------------------------" << std::endl;
						break;
					}
				}
				else
				{
					std::cout << "两次密码不相同，请重新输入您的密码" << std::endl;
					std::cout << "------------------------------------" << std::endl;
				}
			}
		}
	}
}
