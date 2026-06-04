#include "FindSpace.h"

int FindSpace(std::string& str)
{
	size_t found = str.find(' ');
	return found != std::string::npos;
}