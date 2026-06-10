#pragma once

#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <concepts>
#include <format>

// CRTP（奇异递归模板模式）

template<typename T>
concept PageReplacementPolicy = requires(T t, int pageID)
{
    { t.access(pageID)} -> std::same_as<bool>;
};

template<typename Child>
class BaseAlgorithm
{
    protected:
        std::string algorithmName;
        std::vector<int> pageReferenceString; 
        std::deque<int> pagePhysicalFrame; 
        int pageFaults;

    public:
        explicit BaseAlgorithm(std::string name) : algorithmName(std::move(name)), pageFaults(0) {}
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

            std::cout << std::format(
                "\n==========================================\n"
                "算法名称: {}\n"
                "------------------------------------------\n"
                "访问总次数: {}\n"
                "缺页次数  : {}\n"
                "缺页率    : {:.2f}%\n"
                "命中率    : {:.2f}%\n"
                "==========================================\n",
                algorithmName, 
                pageReferenceString.size(), 
                pageFaults, 
                faultRate, 
                hitRate
            );
        }
       void run()
       {    
            static_assert(PageReplacementPolicy<Child>, "Fatal Error: Subclass MUST implement 'bool access(int)'!");
            initialization();
            auto& child = static_cast<Child&>(*this);  
            int step = 0;
            std::cout << std::endl << "开始演示 [" << algorithmName << "]" << std::endl;
            for(int pageID : pageReferenceString)
            {   
                bool isHit = true;
                if(!child.access(pageID)) [[unlikely]]
                {
                    pageFaults++;
                    isHit = false;
                }

                std::string statusStr = isHit ? "\033[32m命中\033[0m" : "\033[31m缺页\033[0m";

                std::string frameStr = "[ ";
                for(int frame : pagePhysicalFrame) {
                    frameStr += std::to_string(frame) + " ";
                }
                frameStr += "]";

                std::cout << std::format("Step {:2} | 访问: {} -> {} | 内存: {}\n", 
                                         step++, pageID, statusStr, frameStr);

                /*
                std::string statusStr = isHit ? "\033[32m命中\033[0m" : "\033[31m缺页\033[0m";

                // 注意：默认格式通常是 [a, b, c] (带逗号分隔)，这比你之前的 [ a b c ] 更标准
                std::cout << std::format("Step {:2} | 访问: {} -> {} | 内存: {}\n", 
                                         step++, pageID, statusStr, pagePhysicalFrame);
                */
            }
            
            printReport();
       }

};
