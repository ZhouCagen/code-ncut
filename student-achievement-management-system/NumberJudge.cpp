#include "NumberJudge.h"

double NumberJudge(const std::string &str)
{
	if (str.empty())
	{
		return -1;	// 输入为空
	}

	double score = 0.0;
	bool hasDecimalPoint = false;
	int PointNumber = 0;

	for (char c : str)
	{
		if (c == '.')
		{
			if (hasDecimalPoint == 1)
			{
				return -2;	// 输入多个小数点 输入错误
			}

			hasDecimalPoint = true;
			continue;
		}

		if ((c < '0') || (c > '9'))
		{
			return -2;	// 非字符数字 输入错误
		}

		if (hasDecimalPoint == 1)
		{
			PointNumber++;
			if (PointNumber > 2)
			{
				return -3;	// 输入成绩不符合要求
			}
		}

		if (hasDecimalPoint == 1 && PointNumber == 0)
		{
			return -2;	// 输入小数点后 没有输入数字
		}
	}

	score = std::stod(str);

	if (score < 0 || score>100)
	{
		return -3;
	}

	return std::stod(str);
}
