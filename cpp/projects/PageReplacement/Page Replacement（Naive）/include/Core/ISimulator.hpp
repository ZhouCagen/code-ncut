#pragma once

class ISimulator
{
    public:
        virtual ~ISimulator() = default;

        virtual void initialization() = 0;

        virtual void run() = 0;

        virtual void printReport() = 0;
};