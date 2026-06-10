#pragma once

#include <string>
#include "Process.hpp"

class IProcessScheduler
{
    public:
        virtual ~IProcessScheduler() = default;

        virtual std::string GetAlgorithmName() = 0;

        virtual void AddProcess(Process* p) =0;
    
        virtual Process* SelectProcess(int currentTime) = 0;

        virtual void Tick(){};
};