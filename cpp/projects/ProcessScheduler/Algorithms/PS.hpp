#pragma once

#include <vector>
#include "../Core/IProcessScheduler.hpp"

class PSScheduler : public IProcessScheduler
{
    private:
        std::vector<Process*> readyProcess;
    public:
        std::string GetAlgorithmName() override
        {
            return "优先权调度——PS";
        }

        void AddProcess(Process* p) override
        {
            readyProcess.push_back(p);
        }

        Process* SelectProcess(int currentTime) override
        {
            for(auto it = readyProcess.begin(); it != readyProcess.end(); )
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
                if(readyprocess -> status == Running)
                {
                    return readyprocess;
                }
            }

            Process* maxPriorityProcess = readyProcess[0];
            for(auto* readyprocess : readyProcess)
            {
                if(readyprocess->priority > maxPriorityProcess->priority)
                {
                    maxPriorityProcess = readyprocess;
                }
            }

            return maxPriorityProcess;
        }
};