#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include "../Core/Process.hpp"

namespace RandomDataGenerate
{
    inline std::vector<Process> GenerateProcess(int count)
    {
        std::vector<Process> data;

        std::random_device seed;
        std::mt19937 generator(seed());

        std::uniform_int_distribution<int> arrivalDistribution(0, 100);
        std::uniform_int_distribution<int> burstDistribution(1, 50);
        std::uniform_int_distribution<int> priorityDistribution(1, 10);

        for(int i = 0 ; i < count; i++)
        {
            std::string name( 1, 'A' + i);
            data.emplace_back(
                name,
                arrivalDistribution(generator),
                burstDistribution(generator),
                priorityDistribution(generator)
            );
        }

        // std::sort(data.begin(), data.end(), [](Process& a, Process& b){ return a.arrivalTime < b.arrivalTime ;})
        std::ranges::sort(data, {}, &Process::arrivalTime);

        std::cout << std::endl <<"[系统消息] 随机进程生成完毕 (Count: " << count << "):" << std::endl;
        std::cout << "+------+---------+-------+----------+" << std::endl;
        std::cout << "| Name | Arrival | Burst | Priority |" << std::endl;
        std::cout << "+------+---------+-------+----------+" << std::endl;

        for (const auto& p : data)
        {
            std::cout << "| " << std::left 
                      << std::setw(4) << p.name << " | " 
                      << std::setw(7) << p.arrivalTime << " | " 
                      << std::setw(5) << p.burstTime << " | " 
                      << std::setw(8) << p.priority << " |" 
                      << std::endl;
        }
        std::cout << "+------+---------+-------+----------+" << std::endl;

        return data;
    }
}