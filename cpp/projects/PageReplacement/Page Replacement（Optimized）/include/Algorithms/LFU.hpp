#pragma once

#include "../Core/BaseAlgorithm.hpp"
#include "../Common/GlobalDefine.hpp"

#include <unordered_map>
#include <list>

class LFU : public BaseAlgorithm<LFU>
{
    private:
        static constexpr int HASH_THRESHOLD = 64;
        bool isUseHashMode = false;
        int frameCapacity;
        int logicalTime = 0;

        struct PageNodeLinear
        {
            int pageID;
            int frequency;
            int lastTime;
        };
        std::vector<PageNodeLinear> memory;

        struct PageNodeHash
        {
            int frequency;
            std::list<int>::iterator it;
        };
        int minFrequency;
        std::unordered_map<int, std::list<int>> frequencyBuckets; 
        std::unordered_map<int, PageNodeHash> pageMap;    

        public:
            explicit LFU(int capacity) : BaseAlgorithm("LFU——最少使用算法"), frameCapacity(capacity)
            {
                if(frameCapacity >= HASH_THRESHOLD)
                {
                    isUseHashMode = true;
                    this->algorithmName += "[O(1) Optimized]";
                }
                else
                {
                    isUseHashMode = false;
                    this->algorithmName += " [Linear Scan]";
                }
            }

            bool access(int pageID)
            {
                logicalTime++;
                if(pagePhysicalFrame.empty())
                {
                }
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
                auto it = std::find_if(memory.begin(), memory.end(), [pageID](const PageNodeLinear& pagenode)
                {
                    return pagenode.pageID == pageID;
                });

                if(it != memory.end())
                {
                    it->frequency++;
                    it->lastTime = logicalTime;
                    return true;
                }
                else
                {
                    if(memory.size() >= frameCapacity)
                    {
                        auto victimIt = std::min_element(memory.begin(), memory.end(), [](const PageNodeLinear& a, const PageNodeLinear& b){
                            if(a.frequency != b.frequency)
                            {
                                return a.frequency < b.frequency;
                            }
                            else
                            {
                                return a.lastTime < b.lastTime;
                            }
                        });

                        int victimID = victimIt->pageID;
                        
                        memory.erase(victimIt);
                        
                        if(!pagePhysicalFrame.empty()) 
                        {
                            auto it = std::find(pagePhysicalFrame.begin(), pagePhysicalFrame.end(), victimID);
                            if(it != pagePhysicalFrame.end())
                            {
                                pagePhysicalFrame.erase(it);
                            }                    
                        }
                    }
                    
                    memory.push_back({pageID, 1, logicalTime});
            
                    pagePhysicalFrame.push_back(pageID);

                    return false;
                }
            }

            bool accessHashMode(int pageID)
            {
                if(pageMap.count(pageID))
                {
                    PageNodeHash& node = pageMap[pageID];
                    int oldFrequency = node.frequency;
                    int newFrequency = oldFrequency + 1;

                    frequencyBuckets[oldFrequency].erase(node.it);
                    if(frequencyBuckets[oldFrequency].empty())
                    {
                        frequencyBuckets.erase(oldFrequency);
                        if(minFrequency == oldFrequency) minFrequency++;
                    }   

                    frequencyBuckets[newFrequency].push_front(pageID);

                    node.frequency = newFrequency;
                    node.it = frequencyBuckets[newFrequency].begin();

                    return true;
                }
                else
                {
                    if(pageMap.size() >= frameCapacity)
                    {
                        int victimID = frequencyBuckets[minFrequency].back();

                        frequencyBuckets[minFrequency].pop_back();
                        if(frequencyBuckets[minFrequency].empty()) frequencyBuckets.erase(minFrequency);

                        pageMap.erase(victimID);

                        if(!pagePhysicalFrame.empty()) 
                        {
                            auto it = std::find(pagePhysicalFrame.begin(), pagePhysicalFrame.end(), victimID);
                            if(it != pagePhysicalFrame.end())
                            {
                                pagePhysicalFrame.erase(it);
                            }
                        }
                    }  

                    minFrequency = 1;

                    frequencyBuckets[1].push_front(pageID);
                    pageMap[pageID] = {1, frequencyBuckets[1].begin()};

                    pagePhysicalFrame.push_back(pageID);
                    
                    return false;
                }
            }
};