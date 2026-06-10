#pragma once

#include "GlobalDefine.hpp"

#include <random>
#include <vector>
#include <iostream>
#include <iomanip>

namespace RandomDataGenerator
{
    inline std::vector<int> GeneratorPageGenerator(int count)
    {
        std::vector<int> data;

        std::random_device seed;
        std::mt19937 generator(seed());

        std::uniform_int_distribution<int> pageDistribution(0, Config::DEFAULT_MAX_PAGE_ID);

        for(int i = 0; i < count; i++)
        {
            data.emplace_back(pageDistribution(generator));
        }

        std::cout << std::endl << "随机页面序列生成完毕 (Count: " << count << "):" << std::endl;
        
        int totalWidth = 9 + data.size() * 4;
        std::string separatorLine(totalWidth, '-');

        std::cout << separatorLine << std::endl;

        std::cout << "Index  : ";
        for (int i = 0; i < data.size(); ++i) 
        {
            std::cout << std::setw(3) << (i + 1) << " "; 
        }
        std::cout << std::endl;

        std::cout << "Page ID: ";
        for (int i = 0; i < data.size(); ++i) 
        {
            std::cout << std::setw(3) << data[i] << " ";
        }
        std::cout << std::endl;
        
        std::cout << separatorLine << std::endl;
        std::cout << std::endl;

        return data;
    } 
}