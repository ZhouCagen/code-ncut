#ifndef StudentInformation_H
#define StudentInformation_H

#include "RefreshScreenBuffer.h"
#include "StudentDisposeMenu.h"
#include "ScoreInformation.h"
#include "FormattingPrint.h"
#include "FindSpace.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

class StudentInformation
{
public:
	std::string StudentID;
	std::string StudentName;
	std::string StudentGender;
	std::string StudentAge;
	std::string StudentSystem;

	StudentInformation() {};

	StudentInformation(std::string studentid, std::string studentname, std::string studentgender, std::string studentage, std::string studentsystem)
		:StudentID(studentid), StudentName(studentname), StudentGender(studentgender), StudentAge(studentage), StudentSystem(studentsystem) {}

	static bool isIDExist(const std::string& ID, std::vector<StudentInformation>& students);

	static void isIDNameExist(const std::string& ID, const std::string& Name, std::vector<StudentInformation>& students);

	void LoadStudentsFromFile(std::vector<StudentInformation>& students);

	static void SaveToFile(const std::vector<StudentInformation>& students);

	static void SearchForFile();
};

#endif 