
/*
	AdminLoginSystem.cpp中包含：
	AdminLoginProcedure() 为管理员登录后的主功能菜单页
	AdminLoginSystem::SaveUsersToFile() 为管理员添加管理时数据库中的账号密码
	AdminLoginSystem::LoadUsersFromFile() 为登录读取数据库中的账号密码
	AdminLoginSystem::ManageUser(const std::string& username, const std::string& password) 为管理员添加管理时账号密码
	AdminLoginSystem::isLogin(const std::string& username, const std::string& password) 为登录判断操作
*/

#include "AdminLoginSystem.h"

void RevisePassword(const std::string AdminUserName, const std::string AdminPassword);

void AdminLoginProcedure(std::string& AdminUserName, std::string& AdminPassword, int& judgeadmin)
{
	while (1)
	{
		std::cout << "菜单（输入对应序号即可选择）：" << std::endl;
		std::cout << "1--基本信息管理" << std::endl;
		std::cout << "2--成员身份管理" << std::endl;
		std::cout << "3--学生成绩管理" << std::endl;
		std::cout << "4--查询信息管理" << std::endl;
		std::cout << "5--成绩统计管理" << std::endl;
		std::cout << "6--登录密码修改" << std::endl;
		std::cout << "7--退出登录" << std::endl;

		std::string resolution;
		std::getline(std::cin, resolution);

		RefreshScreenBuffer();

		//管理员对基本信息的管理
		if (resolution == "1")
		{
			BasicInformationStorage();

			RefreshScreenBuffer();

		}
		//管理员对成员身份的管理
		else if (resolution == "2")
		{
			AdminManage();

			RefreshScreenBuffer();

		}
		//管理员对学生成绩的管理
		else if (resolution == "3")
		{
			StudentScoreManage(AdminUserName, AdminPassword);
		}
		//管理员对查询信息的管理
		else if (resolution == "4")
		{
			QueryMessage();
		}
		//管理员对成绩统计的管理
		else if (resolution == "5")
		{
			ScoreStatistics();
		}
		//管理员修改登录密码
		else if (resolution == "6")
		{
			//修改管理员密码 AdminPassword就是正常密码

			RevisePassword(AdminUserName, AdminPassword);
			std::cout << "------------------------------------" << std::endl;
		}
		else if (resolution == "7")
		{
			judgeadmin = 0;

			std::cout << "退出成功" << std::endl;
			std::cout << "------------------------------------" << std::endl;

			break;
		}
		else
		{
			std::cout << "选择错误，请重新选择" << std::endl;
		}
	}
}

AdminLoginSystem::AdminLoginSystem(const char* filepath) : AdminFilePath(filepath) 
{
	LoadUsersFromFile();
};

//管理员添加管理时数据库中的账号密码
void AdminLoginSystem::SaveUsersToFile()
{

	std::ofstream adminfile;

	adminfile.open(AdminFilePath);

	if (adminfile.is_open())
	{
		for (auto user = Users.begin(); user != Users.end(); user++)
		{
			adminfile << user->AdminName << " " << user->AdminPassword << std::endl;
		}
	}
	else
	{
		std::cout << "存储用户登录数据失败" << std::endl;
		std::cout << "------------------------------------" << std::endl;
	}
}

void RevisePassword(const std::string AdminUserName, const std::string AdminPassword)
{
	while (1)
	{
		std::cout << "请输入修改后的管理员密码（）" << std::endl;

		std::string adminpassword;

		getline(std::cin, adminpassword);

		RefreshScreenBuffer();

		if (FindSpace(adminpassword) == 1)
		{
			std::cout << "输入课号格式错误 不支持输入空格" << std::endl;
			continue;
		}

		AdminLoginSystem adminLoginsystem("AdminData.txt");

		for (int i = 0; i < adminLoginsystem.Users.size(); i++)
		{
			if (adminLoginsystem.Users[i].AdminName == AdminUserName && adminLoginsystem.Users[i].AdminPassword == md5(AdminPassword))
			{
				adminLoginsystem.Users[i].AdminPassword = md5(adminpassword);

				std::cout << "修改成功" << std::endl;
				std::cout << "------------------------------------" << std::endl;

				break;
			}
		}

		adminLoginsystem.SaveUsersToFile();

		break;
	}
}

//登录读取数据库中的账号密码
void AdminLoginSystem::LoadUsersFromFile()
{

	Users.clear();


	std::ifstream AdminFile;

	AdminFile.open(AdminFilePath);

	if (AdminFile.is_open())
	{
		std::string Line;

		while (std::getline(AdminFile, Line))
		{
			/* 
				std::string::npos是一个静态成员变量 表示字符串中不存在的位置
				substr函数用于从字符串中提取子串
				用户名是从字符串的开始到空格之前（不包括空格）的子串。
				密码是从空格之后（包括空格后的第一个字符）到行尾的子串
				size_t 是一个无符号整数类型，它是标准库中定义的一个别名，用于表示大小或索；
			*/
			size_t pos = Line.find(' ');

			if (pos != std::string::npos) 
			{
				std::string username = Line.substr(0, pos);
				std::string password = Line.substr(pos + 1);
				Users.emplace_back(username, password);
			}
			else
			{
				std::cout << "未成功读取数据库中登录数据的账号密码" << std::endl;
			}
		}
		AdminFile.close();

	}
	else
	{
		std::cout << "读取用户登录数据失败" << std::endl;
	}
}

//管理员添加管理时账号密码
bool AdminLoginSystem::ManageUser(const std::string& username, const std::string& password)
{
	//用于遍历容器中的每个元素 const后不能通过user修改容器中的元素。
	for (const auto &user : Users)
	{
		if (user.AdminName == username)
		{
			return false;//已经是管理员
		}
	}

	Users.emplace_back(username, md5(password));

	SaveUsersToFile();

	return true;//添加成功

	//利用迭代器遍历方法：
	
	//for (std::vector<User>::const_iterator it = users.begin(); it != users.end(); ++it)
	//{
	//	const User& user = *it;
	//	if (user.UserName == username && user)
	//	{
	//		return true;
	//	}
	//}
	//return false;

}

//登录判断
int AdminLoginSystem::isLogin(const std::string& username, const std::string& password)
{
	for (const auto& user : Users)
	{
		if (user.AdminName == username && user.AdminPassword == md5(password))
		{
			return 1;//登录成功
		}
		else if (user.AdminName == username && user.AdminPassword != md5(password))
		{
			return 2;//密码错误
		}
	}
	return 3;//用户名不存在
}