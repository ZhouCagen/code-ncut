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

template <typename AlgorithmType>
void RunTest(const std::vector<int>& data, int capacity)
{
    AlgorithmType algorithm(capacity);
    
    algorithm.loadData(data);
    
    algorithm.run();
}

int main()
{
    const int frameCapacity = Config::DEFAULT_FRAME_COUNT; 
    const int pageCount = Config::DEFAULT_REFERENCE_STRING_LENGTH; 

    std::cout << "页面置换算法模拟器启动" << std::endl;
    std::cout << "物理帧容量: " << frameCapacity << std::endl;
    std::cout << "访问序列长度: " << pageCount << std::endl;

    std::vector<int> testData = RandomDataGenerator::GeneratorPageGenerator(pageCount);

    RunTest<OPT>(testData, frameCapacity);
    RunTest<FIFO>(testData, frameCapacity);
    RunTest<LRU>(testData, frameCapacity);
    RunTest<LFU>(testData, frameCapacity);
    RunTest<CLOCK>(testData, frameCapacity);

    std::cout << std::endl << "所有模拟运行结束" << std::endl;

    return 0;
} 