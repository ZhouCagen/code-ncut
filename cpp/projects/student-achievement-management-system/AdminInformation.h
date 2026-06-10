#ifndef AdminInformation_H
#define AdminInformation_H

#include <iostream>
#include <string>

class AdminInformation
{
public:
	std::string AdminName;
	std::string AdminPassword;

	AdminInformation(std::string adminname, std::string adminpassword) : AdminName(adminname), AdminPassword(adminpassword) {}
};

#endif