#ifndef UserLogin_H
#define UserLogin_H

#include "RefreshScreenBuffer.h"
#include "StudentLoginSystem.h"
#include "StudentInformation.h"
#include "CourseInformation.h"
#include "AdminLoginSystem.h"
#include "ProgressBar.h"

#include <iostream>
#include <iomanip>

class UserLogin
{
public:
	std::string UserName;
	std::string Password;

	void inputUserName();
};

#endif