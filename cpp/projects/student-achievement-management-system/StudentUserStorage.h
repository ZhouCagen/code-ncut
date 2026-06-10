#ifndef StudentUserStorage_H
#define StudentUserStorage_H

#include <iostream>

class StudentUserStorage
{
public:
	std::string UserName;
	std::string Password;

	StudentUserStorage(std::string username, std::string password) :UserName(username), Password(password) {}
};

#endif