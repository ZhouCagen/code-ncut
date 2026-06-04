#ifndef ScoreInformation_H
#define ScoreInformation_H

#include "RefreshScreenBuffer.h"
#include "FormattingPrint.h"
#include "NumberRounding.h"
#include "NumberJudge.h"
#include "FindSpace.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

class ScoreInformation
{
public:
	std::string StudentID;
	std::string ClassID;
	int UsualScore = 0;
	int RollScore = 0;
	int Score = 0;

	ScoreInformation() {}

	ScoreInformation(std::string studentid, std::string classid, int usualscore, int rollscore, int score)
		: StudentID(studentid), ClassID(classid), UsualScore(usualscore), RollScore(rollscore), Score(score) {}

	static bool isScoreExist(const std::string& studentID, const std::string &classID, std::vector<ScoreInformation>& scores);

	void LoadScoresFromFile(std::vector<ScoreInformation>& students);

	static void isIDNameExist(const std::string& StudentID, const std::string& ClassID, std::vector<ScoreInformation>& scores);

	static void SaveToFile(const std::vector<ScoreInformation>& scores);

	static void SearchForFile();

};

#endif