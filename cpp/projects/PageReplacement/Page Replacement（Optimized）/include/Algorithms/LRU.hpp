#pragma once

#include "../Core/BaseAlgorithm.hpp"
#include "../Common/GlobalDefine.hpp"

#include <list>
#include <unordered_map>

class LRU : public BaseAlgorithm<LRU>
{
    private:
        static constexpr int HASH_THRESHOLD = 64;
        bool isUseHashMode = false;
        int frameCapacity;

        std::list<int> pageTimeline;
        std::unordered_map<int, std::list<int>::iterator> pageMap;
    public:
        explicit LRU(int capacity) : BaseAlgorithm("LRU——近期最少使用算法"), frameCapacity(capacity)
        {
            if(frameCapacity >= HASH_THRESHOLD)
            {
                isUseHashMode = true;
                this->algorithmName += " [O(1) Map+List]";
            }
            else
            {
                isUseHashMode = false;
                this->algorithmName += " [Linear Vector]";
            }
        }

        bool access(int pageID)
        {
            if(isUseHashMode)
            {
                return accessHashMode(pageID);
            }
            else
            {
                return accessLinearMode(pageID);
            }
        }

        private: 
            bool accessLinearMode(int pageID)
            {
                auto it = std::find(pagePhysicalFrame.begin(), pagePhysicalFrame.end(), pageID);
                if(it == pagePhysicalFrame.end())
                {
                    if(pagePhysicalFrame.size() >= frameCapacity)
                    {
                        pagePhysicalFrame.pop_front();
                    }
                    pagePhysicalFrame.push_back(pageID);

                    return false;
                }
                else
                {
                    pagePhysicalFrame.erase(it);
                    pagePhysicalFrame.push_back(pageID);
                    return true;
                }
            }
            bool accessHashMode(int pageID)
            {
                if(pageMap.count(pageID))
                {
                    auto it = pageMap[pageID];
                    pageTimeline.splice(pageTimeline.begin(), pageTimeline, it);
                    return true;
                }
                else
                {
                    if(pageMap.size() >= frameCapacity)
                    {
                        int victimID = pageTimeline.back();
                        pageMap.erase(victimID);
                        pageTimeline.pop_back();

                        if(!pagePhysicalFrame.empty())
                        {
                            auto it = std::find(pagePhysicalFrame.begin(), pagePhysicalFrame.end(), victimID);
                            if(it != pagePhysicalFrame.end())
                            {
                                pagePhysicalFrame.erase(it);
                            }
                        }
                    }
                    pageTimeline.push_front(pageID);
                    pageMap[pageID] = pageTimeline.begin();

                    pagePhysicalFrame.push_back(pageID);
                    return false;
                }
            }
};