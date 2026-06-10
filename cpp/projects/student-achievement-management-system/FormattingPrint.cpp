
/*
	std::setw 它定义在 <iomanip> 头文件中 用于控制紧随其后的输出项的最小宽度
	如果输出项的字符数少于指定的宽度 则输出将用空格（或其他填充字符，如果使用 std::setfill 指定了填充字符）填充到指定的宽度
*/

#include "FormattingPrint.h"

void FormattingPrint(const int& maxlength, const std::string& str)
{
	std::cout << std::setw((maxlength - str.size()) / 2) << "" << std::setw(str.size()) << str << std::setw(maxlength - (maxlength + str.size()) / 2 + 4) << "";
}
