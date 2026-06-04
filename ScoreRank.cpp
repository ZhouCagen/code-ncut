#include "ScoreRank.h"

void ScoreRank(int& a, int point[])
{
	switch (a / 10)
	{
	case 10:
		point[0]++;
		break;
	case 9:
		point[1]++;
		break;
	case 8:
		point[2]++;
		break;
	case 7:
		point[3]++;
		break;
	case 6:
		point[4]++;
		break;
	default:
		point[5]++;
	}

	//数学方法：
	//point[std::max((a / 10) - 5, 0)]++;
}