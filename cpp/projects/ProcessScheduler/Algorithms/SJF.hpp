#pragma once

#include <vector>
#include <algorithm>
#include "../Core/IProcessScheduler.hpp"

class SJFScheduler:public IProcessScheduler
{
    private:
        std::vector<Process*> readyProcess;
    public:
        std::string GetAlgorithmName() override
        {
            return "短作业优先调度——SJF";
        }

        void AddProcess(Process* p) override
        {
            readyProcess.push_back(p);
        }

        Process* SelectProcess(int currentTime) override
        {
            for(auto it = readyProcess.begin(); it != readyProcess.end();)
            {
                if((*it)->status == Finished)
                {
                    it = readyProcess.erase(it);
                }
                else
                {
                    it++;
                }
            }

            if(readyProcess.empty()) return nullptr;

            for(auto* readyprocess : readyProcess)
            {
                if(readyprocess->status == Running)
                {
                    return readyprocess;
                }
            }

            Process* minProcess = readyProcess[0];
            for(auto* readyprocess: readyProcess)
            {
                if(readyprocess->burstTime < minProcess->burstTime)
                {
                    minProcess = readyprocess;
                }
            }
            
            // std::sort(readyProcess.begin(), readyProcess.end(), [](Process *a, Process *b)
            // {
            //     return a->burstTime < b->burstTime;
            // });

            // std::ranges::sort(readyProcess, {}, &Process::burstTime);
            // Process* minProcess = readyProcess[0];

            return minProcess;
        }
};