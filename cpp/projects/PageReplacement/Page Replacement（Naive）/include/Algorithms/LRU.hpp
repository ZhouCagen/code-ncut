#pragma once

#include "../Core/BaseAlgorithm.hpp"
#include "../Common/GlobalDefine.hpp"

class LRU : public BaseAlgorithm
{
    public:
        LRU() : BaseAlgorithm("LRU——近期最少使用算法") {}
        void run() 
        {
            std::cout << "正在运行 LRU——近期最少使用算法" << std::endl;
            for(size_t i = 0; i < pageReferenceString.size(); i++)
            {
                int pageID = pageReferenceString[i];
                bool isFault = false;

                auto it = std::find(pagePhysicalFrame.begin(), pagePhysicalFrame.end(), pageID);

                if(it == pagePhysicalFrame.end())
                {
                    isFault = true;
                    pageFaults++;
                    if(pagePhysicalFrame.size() >= Config::FRAME_COUNT)
                    {
                        pagePhysicalFrame.pop_front();
                    }
                    pagePhysicalFrame.push_back(pageID);
                }
                else
                {
                    pagePhysicalFrame.erase(it);
                    pagePhysicalFrame.push_back(pageID);
                }

                printStep(i + 1, pageID, isFault);
            }
        }
};
