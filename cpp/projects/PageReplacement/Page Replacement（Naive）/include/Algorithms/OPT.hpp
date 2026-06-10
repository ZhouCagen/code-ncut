#pragma once

#include "../Core/BaseAlgorithm.hpp"
#include "../Common/GlobalDefine.hpp"

#include <climits>

class OPT : public BaseAlgorithm
{
    public:
        OPT() : BaseAlgorithm("OPT——理论算法") {}
        void run()
        {        
            std::cout << "正在运行 OPT——理论算法" << std::endl;

            for(size_t i = 0; i < pageReferenceString.size(); i++)
            {
                int pageID = pageReferenceString[i];
                bool isFault = false;

                auto it = std::find(pagePhysicalFrame.begin(), pagePhysicalFrame.end(), pageID);

                if(it == pagePhysicalFrame.end())
                {
                    pageFaults++;
                    isFault = true;
                    if(pagePhysicalFrame.size() >= Config::FRAME_COUNT)
                    {
                        int maxDistance = -1;
                        int VictimIndex = -1;
                        for(size_t m = 0; m < pagePhysicalFrame.size(); m++)
                        {
                            int distance = INT_MAX;
                            for(size_t n = i + 1; n < pageReferenceString.size(); n++)
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
                                VictimIndex = m;
                            }
                        }

                        pagePhysicalFrame[VictimIndex] = pageID;
                    }
                    else
                    {
                        pagePhysicalFrame.push_back(pageID);
                    }
                }

                printStep(i + 1, pageID, isFault);
            }
        }
};
