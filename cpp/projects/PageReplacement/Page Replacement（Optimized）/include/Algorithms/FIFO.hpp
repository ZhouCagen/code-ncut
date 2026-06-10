#pragma once

#include "../Core/BaseAlgorithm.hpp"
#include "../Common/GlobalDefine.hpp"

#include <unordered_set>

class FIFO : public BaseAlgorithm<FIFO>
{
    private:
        static constexpr int HASH_THRESHOLD = 16;

        std::unordered_set<int> activePages;
        bool isUseHashMode = false;
        int frameCapacity;

    public:
        explicit FIFO (int framecapacity) : BaseAlgorithm("FIFO ——先进先出"), frameCapacity(framecapacity) 
        {
            if(frameCapacity > HASH_THRESHOLD)
            {
                isUseHashMode = true;
                activePages.reserve(frameCapacity);
                this->algorithmName += " [Hash Optimized]";
            }
            else
            {
                isUseHashMode = false;
                this->algorithmName += " [Linear Scan]";
            }
        }

        bool access(int pageID)
        {
            if (pagePhysicalFrame.empty() && !activePages.empty())
            {
                activePages.clear();
            }

            if(isUseHashMode)
            {
                if(activePages.count(pageID))
                {
                    return true;
                }
                else
                {
                    if(pagePhysicalFrame.size() >= frameCapacity)
                    {
                        int victim = pagePhysicalFrame.front();
                        pagePhysicalFrame.pop_front();
                        activePages.erase(victim);
                    }
                    
                    pagePhysicalFrame.push_back(pageID);
                    activePages.insert(pageID);
                    return false;
                }
            }
            else
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
                    return true;
                }

            }
        }

};