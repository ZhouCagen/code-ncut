#pragma once

#include <queue>
#include "../Core/IProcessScheduler.hpp"

class FCFSScheduler: public IProcessScheduler
{
    private:
        std::queue<Process*> readyProcess;
    public:
        std::string GetAlgorithmName() override
        {
            return "先来先服务调度——FCFS";
        }

        void AddProcess(Process* p) override
        {
            readyProcess.push(p);
        }

        Process* SelectProcess(int currentTime) override
        {
            if(readyProcess.empty()) return nullptr;

            Process* p = readyProcess.front();
            if(p->status == Finished)
            {
                readyProcess.pop();
                return SelectProcess(currentTime);
            }
            
            return p;
        }
};