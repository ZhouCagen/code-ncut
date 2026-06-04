#include "NumberRounding.h"

//该函数为取整函数 将double类型的数进行四舍五入并转化为int类型

int NumberRounding(double num)
{
    int Num = (int)num;

    if (num - Num >= 0.5)
    {
        Num++;
    }

    return Num;
}
