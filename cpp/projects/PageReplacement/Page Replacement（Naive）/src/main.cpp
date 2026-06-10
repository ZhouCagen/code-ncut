#include <iostream>
#include <vector>
#include <memory>

#include "../include/Common/GlobalDefine.hpp"
#include "../include/Common/RandomDataGenerator.hpp"

#include "../include/Core/BaseAlgorithm.hpp"

#include "../include/Algorithms/FIFO.hpp"
#include "../include/Algorithms/OPT.hpp"
#include "../include/Algorithms/LRU.hpp"
#include "../include/Algorithms/LFU.hpp"
#include "../include/Algorithms/CLOCK.hpp"

int main()
{
    std::cout << "=== 页面置换算法模拟器启动 ===" << std::endl;
    std::vector<int> testData = RandomDataGenerator::GeneratorPageGenerator(Config::REFERENCE_STRING_LENGTH);
    
    std::vector<BaseAlgorithm*> algorithms;

    algorithms.push_back(new FIFO());
    algorithms.push_back(new OPT());
    algorithms.push_back(new LRU());
    algorithms.push_back(new LFU());
    algorithms.push_back(new CLOCK());

    for(auto algorithm : algorithms)
    {
        algorithm->loadData(testData);
        
        algorithm->initialization();

        algorithm->run();

        algorithm->printReport();

        std::cout << "\n\nPress Enter to continue next algorithm...";
        std::cin.get(); 
    }

    for(auto algorithm : algorithms)
    {
        delete algorithm;
    }

    std::cout << "模拟结束。" << std::endl;

    return 0;
}