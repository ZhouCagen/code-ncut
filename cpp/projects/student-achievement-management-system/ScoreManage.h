#ifndef ScoreManage_H
#define ScoreManage_H

#include "ScoreDisposeMenu.h"
#include "NumberRounding.h"
#include "ScoreStorage.h"

#include <iostream>
#include <string>


int ScoreManage(std::string& classid, int usualscore, int rollscore, int score);

void StudentScoreManage(std::string& AdminUserName, std::string& AdminPassword);

#endif