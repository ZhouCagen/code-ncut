#pragma once

#include "ISimulator.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <deque>

class BaseAlgorithm : public ISimulator
{
    protected:
        std::string algorithmName;
        std::vector<int> pageReferenceString; 
        std::deque<int> pagePhysicalFrame;
    
        int pageFaults;

        void printStep(int step, int pageID, bool isFault)
        {
            std::cout << "Step " << std::setw(2) << step << ": 访问 [" << pageID << "] -> [ ";
            for (int pagephysicalframe : pagePhysicalFrame) std::cout << pagephysicalframe << " ";
            std::cout << "]";
            if (isFault) std::cout << "  <-- 缺页";
            else         std::cout << "  <-- 命中";
            std::cout << std::endl;
        }

    public:
        BaseAlgorithm(std::string algorithmname) : algorithmName(algorithmname), pageFaults(0) {}

        void loadData(const std::vector<int>& data)
        {
            pageReferenceString = data;
        }
        void initialization()
        {
            pageFaults = 0;
            pagePhysicalFrame.clear();
        }
        void printReport()
        {
            double faultRate = (double)pageFaults / pageReferenceString.size() * 100.0;
            double hitRate = 100.0 - faultRate;

            std::cout << std::endl;
            std::cout << "==========================================" << std::endl;
            std::cout << "算法名称: " << algorithmName << std::endl;
            std::cout << "------------------------------------------" << std::endl;
            std::cout << "访问总次数: " << pageReferenceString.size() << std::endl;
            std::cout << "缺页次数  : " << pageFaults << std::endl;
            std::cout << "缺页率    : " << std::fixed << std::setprecision(2) << faultRate << "%" << std::endl;
            std::cout << "命中率    : " << std::fixed << std::setprecision(2) << hitRate << "%" << std::endl;
            std::cout << "==========================================" << std::endl;
        }
        virtual void run() override = 0;
};


