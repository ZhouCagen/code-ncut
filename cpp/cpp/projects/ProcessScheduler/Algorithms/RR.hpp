#pragma once

#include <queue>
#include "../Core/IProcessScheduler.hpp"

class RRScheduler : public IProcessScheduler
{
    private:
        std::queue<Process*> readyProcess;
        int standardTimeSlice;
        int currentTimeSlice;
    public:
        RRScheduler(int standardtimeslice) : standardTimeSlice(standardtimeslice), currentTimeSlice(0) {}
        std::string GetAlgorithmName() override
        {
            return "时间片轮转调度——RR(TimeSlice=" + std::to_string(standardTimeSlice) + ")";
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
                currentTimeSlice = 0;
                return SelectProcess(currentTime);
            }

            return p;
        }

        void Tick() override
        {
            if(readyProcess.empty()) return;

            Process* p = readyProcess.front();
            if(p->status == Running)
            {
                currentTimeSlice++;
                if(currentTimeSlice >= standardTimeSlice)
                {
                    readyProcess.pop();
                    p->status = Ready;
                    readyProcess.push(p);
                    currentTimeSlice = 0;
                }
            }
        }
};