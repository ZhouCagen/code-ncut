#pragma once

#include "../Core/BaseAlgorithm.hpp"
#include "../Common/GlobalDefine.hpp"

struct PageNode
{
    int pageID;
    int frequency;
    int lastTime;
};


class LFU : public BaseAlgorithm
{
    private:
        std::vector<PageNode> memory;
        void updateBaseState()
        {
            pagePhysicalFrame.clear();
            for(const auto& pagenode : memory)
            {
                pagePhysicalFrame.push_back(pagenode.pageID);
            }
        }
    public:
        LFU() : BaseAlgorithm("LFU——最少使用算法") {}
        void run()
        {
            std::cout << "正在运行 LFU——最少使用算法" << std::endl;
            for(size_t i = 0; i < pageReferenceString.size(); i++)
            {
                int targetPageID = pageReferenceString[i];
                bool isFault = false;
                int currentTime = i;
            
                auto it = std::find_if(memory.begin(), memory.end(), [targetPageID](const PageNode& pagenode)
                {
                    return pagenode.pageID == targetPageID;
                });

                if(it != memory.end())
                {
                    it->frequency++;
                    it->lastTime = currentTime;
                }
                else
                {
                    pageFaults++;
                    isFault = true;
                    
                    if(memory.size() >= Config::FRAME_COUNT)
                    {
                        auto victimIt = std::min_element(memory.begin(), memory.end(), [](const PageNode& a, const PageNode& b){
                            if(a.frequency != b.frequency)
                            {
                                return a.frequency < b.frequency;
                            }
                            else
                            {
                                return a.lastTime < b.lastTime;
                            }
                        });

                        victimIt->pageID = targetPageID;
                        victimIt->frequency = 1;
                        victimIt->lastTime = currentTime;
                    }
                    else
                    {
                        memory.push_back({targetPageID, 1, currentTime});
                    }
                }

                updateBaseState();
                printStep(i + 1, targetPageID, isFault);
            }
        }
};