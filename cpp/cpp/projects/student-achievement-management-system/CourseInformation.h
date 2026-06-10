#ifndef CourseInformatiomStorage_H
#define CourseInformatiomStorage_H

#include "ScoreInformation.h"
#include "FormattingPrint.h"
#include "FindSpace.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

class CourseInformation
{
public:
	std::string CourseNumber;
	std::string CourseName;
	std::string CourseTeacher;

	CourseInformation() {}

	CourseInformation(std::string coursenumber, std::string coursename, std::string courseteacher)
		:CourseNumber(coursenumber), CourseName(coursename), CourseTeacher(courseteacher) {}

	static bool isCourseExist(const std::string& courseNumber, const std::string& coursename, const std::string& courseteacher, std::vector<CourseInformation>& courses);

	void LoadCoursesFromFile(std::vector<CourseInformation>& students);

	static void isIDNameExist(const std::string& courseNumber, const std::string& courseName, const std::string& courseTeacher, std::vector<CourseInformation>& courses);

	static void SaveToFile(const std::vector<CourseInformation>& courses);

	static void SearchForFile();
};

#endif