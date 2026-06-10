#include <iostream>
#include <vector>
#include <windows.h>

#include "Core/Process.hpp"
#include "Core/Simulator.hpp"

#include "Util/RandomDataGenerate.hpp"

#include "Algorithms/FCFS.hpp"
#include "Algorithms/SJF.hpp"
#include "Algorithms/PS.hpp"
#include "Algorithms/RR.hpp"
#include "Algorithms/MLFQ.hpp"


int main()
{
    SetConsoleOutputCP(65001);
    
    int processCount = 10;
    std::cout << "正在生成" << processCount << "个随机进程" << std::endl;
    std::vector<Process> data = RandomDataGenerate::GenerateProcess(processCount);
    
    Simulator simulator(data);
    std::cout << "开始进程调度算法仿真" << std::endl;

    FCFSScheduler FCFS;
    simulator.RunProcessScheduler(&FCFS);

    SJFScheduler SJF;
    simulator.RunProcessScheduler(&SJF);

    PSScheduler PS;
    simulator.RunProcessScheduler(&PS);

    RRScheduler RR(2); 
    simulator.RunProcessScheduler(&RR);

    MLFQScheduler MLFQ;
    simulator.RunProcessScheduler(&MLFQ);

    std::cout << "仿真结束" << std::endl; 

    return 0;
}