#pragma once

#include <vector>
#include <iomanip>
#include <iostream>
#include "IProcessScheduler.hpp"

class Simulator
{
    private:
        std::vector<Process> process;

    public:
        Simulator(const std::vector<Process>& data) : process(data) {}
        void RunProcessScheduler(IProcessScheduler* scheduler)
        {
            for(auto& p : process)
            {
                p.remainingTime = p.burstTime;
                p.startTime = -1;
                p.finishTime = 0;
                p.status = Ready;
            }

            int currentTime = 0;
            int totalProcess = process.size();
            int completedProcess = 0;

            std::string timeLine = "";
            Process* lastRun = nullptr;

            std::cout << scheduler->GetAlgorithmName()<< "：" << std::endl;

            while(completedProcess < totalProcess)
            {
                for(auto& p : process)
                {
                    if(p.arrivalTime == currentTime)
                    {
                        scheduler->AddProcess(&p);
                    }
                }

                Process* currentProcess = scheduler->SelectProcess(currentTime);

                if(currentProcess != nullptr)
                {
                    if(currentProcess->startTime == -1)
                    {
                        currentProcess->startTime = currentTime;
                    }

                    currentProcess->status = Running;

                    if (currentProcess != lastRun) 
                    {
                        timeLine += currentProcess->name;
                        lastRun = currentProcess;
                    } 
                    else 
                    {
                        timeLine += ".";
                    }

                    currentProcess->remainingTime--;

                    if(currentProcess->remainingTime == 0)
                    {
                        currentProcess->status = Finished;
                        currentProcess->finishTime = currentTime + 1;
                        completedProcess++;
                    }
                }
                else
                {
                    timeLine += "_";
                    lastRun = nullptr;
                }

                scheduler->Tick();
                currentTime++;
            }

            std::cout << std::endl << "进程执行时间轴: " << timeLine << std::endl;
            PrintReport();
        }
    
    private:
        void PrintReport()
        {
            std::cout << std::endl;
            std::cout << std::left 
                      << std::setw(6) << "Name" 
                      << std::setw(8) << "Arrive"
                      << std::setw(12) << "Priority"  
                      << std::setw(12) << "BurstTime" 
                      << std::setw(12) << "StartTime"
                      << std::setw(12) << "FinishTime" 
                      << std::setw(12) << "TurnTime" 
                      << std::setw(12) << "WeightedTurnTime";

            std::cout << std::endl << std::string(90, '-') << std::endl;

            double totalTurnAround = 0;
            double totalWeightedTurnAround = 0;

            for (const auto& p : process)
            {
                int turnTime = p.finishTime - p.arrivalTime; 
                
                double weightedTurnTime = (double)turnTime / p.burstTime;

                totalTurnAround += turnTime;
                totalWeightedTurnAround += weightedTurnTime;

                std::cout << std::left 
                          << std::setw(6) << p.name
                          << std::setw(8) << p.arrivalTime
                          << std::setw(12) << p.priority
                          << std::setw(12) << p.burstTime
                          << std::setw(12) << p.startTime
                          << std::setw(12) << p.finishTime
                          << std::setw(12) << turnTime
                          << std::fixed << std::setprecision(2) << std::setw(12) << weightedTurnTime
                          << std::endl;
            }

            std::cout << std::string(90, '-') << std::endl;

            if (!process.empty()) 
            {
                std::cout << "平均周转时间: " << totalTurnAround / process.size() << std::endl;
                std::cout << "平均带权周转: " << totalWeightedTurnAround / process.size() << std::endl;
            }
            std::cout << std::endl;
        }
};