#pragma once

#include <queue>
#include <vector>
#include "../Core/IProcessScheduler.hpp"

class MLFQScheduler : public IProcessScheduler
{
    private:
        std::vector<std::queue<Process*>> readyProcess;
        const std::vector<int> standardTimeSlice = {1, 4, 16};
        int currentTimeSlice;
        int currentLevel;
        Process* lastRunningProcess;

    public:
        MLFQScheduler() : currentTimeSlice(0), currentLevel(0), lastRunningProcess(nullptr) 
        { 
            readyProcess.resize(3);
        }
        std::string GetAlgorithmName() override
        {
            return "多级反馈队列调度——MLFQ（standardTimeSlice0 = 1, standardTimeSlice1 = 4, standardTimeSlice2 =16）";
        }

        void AddProcess(Process* p) override
        {
            readyProcess[0].push(p);
        }

        Process* SelectProcess(int currentTime) override
        {
            for(int i = 0; i < 3; i++)
            {
                if(!readyProcess[i].empty())
                {
                    Process* p = readyProcess[i].front();
                    if(p->status == Finished)
                    {
                        readyProcess[i].pop();
                        currentTimeSlice = 0;
                        return SelectProcess(currentTime);
                    }

                    currentLevel = i;

                    if(p != lastRunningProcess)
                    {
                        currentTimeSlice = 0;
                        lastRunningProcess = p;
                    }

                    return p;
                }
            }

            return nullptr;
        }

        void Tick() override
        {
            if (std::ranges::all_of(readyProcess, [](const auto& q){ return q.empty(); })) { return; }

            Process* p = readyProcess[currentLevel].front();
            if(p->status == Running)
            {
                currentTimeSlice++;
                if(currentTimeSlice >= standardTimeSlice[currentLevel])
                {
                    readyProcess[currentLevel].pop();
                    if(currentLevel < 2)
                    {
                        int nextLevel = currentLevel + 1;
                        readyProcess[nextLevel].push(p);
                    }
                    else
                    {
                        readyProcess[currentLevel].push(p);
                    }

                    p->status = Ready;
                    currentTimeSlice = 0;
                    lastRunningProcess = nullptr;
                }
            }
        }
};
