#pragma once

#include "../Core/BaseAlgorithm.hpp"
#include "../Common/GlobalDefine.hpp"
#include <vector>
#include <algorithm>

class CLOCK : public BaseAlgorithm<CLOCK>
{
    private:
        std::vector<bool> referenceBits; // 引用位 (访问位)
        int handPointer;                 // 时钟指针 (Hand)
        int frameCapacity;               // 物理帧容量

    public:
        explicit CLOCK(int capacity) : BaseAlgorithm("CLOCK——时钟置换算法"), handPointer(0), frameCapacity(capacity) {}

        bool access(int pageID)
        {
            auto it = std::find(pagePhysicalFrame.begin(), pagePhysicalFrame.end(), pageID);

            if(it != pagePhysicalFrame.end())
            {
                int index = std::distance(pagePhysicalFrame.begin(), it);
                
                referenceBits[index] = true;
                
                return true; 
            }
            else
            {
                if(pagePhysicalFrame.size() < frameCapacity)
                {
                    pagePhysicalFrame.push_back(pageID);
                    referenceBits.push_back(true);
                }
                else
                {
                    while(true)
                    {
                        if(referenceBits[handPointer] == true)
                        {
                            referenceBits[handPointer] = false;
                            
                            handPointer = (handPointer + 1) % frameCapacity;
                        }
                        else
                        {
                            pagePhysicalFrame[handPointer] = pageID;
                            referenceBits[handPointer] = true;
                            handPointer = (handPointer + 1) % frameCapacity;
                            
                            break;
                        }
                    }
                }
                return false;
            }
        }
};