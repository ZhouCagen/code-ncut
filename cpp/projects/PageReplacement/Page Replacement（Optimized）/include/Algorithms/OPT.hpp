#pragma once

#include "../Core/BaseAlgorithm.hpp"
#include "../Common/GlobalDefine.hpp"

#include <climits>
#include <cassert>

class OPT : public BaseAlgorithm<OPT>
{
    private:
        int current_idx = 0;
        int frameCapacity;
    public:
        explicit OPT(int framecapacity) : BaseAlgorithm("OPT——理论算法"), frameCapacity(framecapacity) {}
        
        bool access(int pageID)
        {
            if (pagePhysicalFrame.empty() && current_idx != 0) 
            {
                current_idx = 0;
            }

            auto it = std::find(pagePhysicalFrame.begin(), pagePhysicalFrame.end(), pageID);
            if(it == pagePhysicalFrame.end())
            {
                if(pagePhysicalFrame.size() >= frameCapacity)
                {
                    int maxDistance = -1;
                    int victimIndex = -1;
                    for(size_t m = 0; m < pagePhysicalFrame.size(); m++)
                    {
                        int distance = INT_MAX;
                        for(size_t n = current_idx + 1; n < pageReferenceString.size(); n++)
                        {
                            if(pageReferenceString[n] == pagePhysicalFrame[m])
                            {
                                distance = n;
                                break;
                            }
                        }

                        if(distance > maxDistance)
                        {
                            maxDistance = distance;
                            victimIndex = m;
                        }
                    }
                    
                    assert(victimIndex != -1);
                    pagePhysicalFrame[victimIndex] = pageID;
                }
                else
                {   
                    pagePhysicalFrame.push_back(pageID);
                }

                current_idx++;
                return false;
            }
            else
            {
                current_idx++;
                return true;
            }
        }
};