#pragma once
#include <string>

enum ProcessStatus {Ready, Running, Finished};

struct Process
{
    std::string name;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int startTime;
    int finishTime;
    ProcessStatus status = Ready;

    Process(std::string n, int arrivetime, int bursttime, int prioity) : name(n), arrivalTime(arrivetime), burstTime(bursttime), priority(prioity) // 显式指认！
    {
        remainingTime = bursttime;
        startTime = -1;
        finishTime = 0;
        status = Ready;
    }
};