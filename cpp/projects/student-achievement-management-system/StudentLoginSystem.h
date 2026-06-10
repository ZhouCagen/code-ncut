#ifndef StudentLoginSystem_H
#define StudentLoginSystem_H

#include "StudentUserStorage.h"
#include "StudentLoginSystem.h"
#include "StudentInformation.h"
#include "CourseInformation.h"
#include "ScoreInformation.h"
#include "CourseStorage.h"
#include "MD5.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>




class StudentLoginSystem
{
public:
	std::vector<StudentUserStorage> users;

	const char* StudentFilePath;

	void LoadUsersFromFile();

	void SaveUsersToFile();

	StudentLoginSystem(const char* file);

	bool RegisterUser(const std::string& username, const std::string& password);

	int isLogin(const std::string& username, const std::string& password);
};

void StudentLoginProcedure(std::string& StudentUserName, int& judgestudent);

#endif
