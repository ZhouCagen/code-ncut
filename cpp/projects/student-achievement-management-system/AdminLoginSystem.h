#ifndef AdminLoginSystem_H
#define AdminLoginSystem_H

#include "BasicInformationStorage.h"
#include "AdminInformation.h"
#include "ScoreStatistics.h"
#include "QueryMessage.h"
#include "AdminManage.h"
#include "ScoreManage.h"
#include "UserLogin.h"
#include "MD5.h"

#include <iostream>
#include <fstream>
#include <vector>

class AdminLoginSystem
{
public:
	std::vector<AdminInformation> Users;

	const char * AdminFilePath;

	void LoadUsersFromFile();
	
	void SaveUsersToFile();


	AdminLoginSystem(const char * filepath);

	int isLogin(const std::string& username, const std::string& password);

	bool ManageUser(const std::string& username, const std::string& password);


};

void AdminLoginProcedure(std::string& AdminUserName, std::string& AdminPassword, int& judgeadmin);


#endif
