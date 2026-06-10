#pragma once

#include "../Core/BaseAlgorithm.hpp"
#include "../Common/GlobalDefine.hpp"

class CLOCK : public BaseAlgorithm
{
    private:
        std::vector<bool> referenceBits;
        size_t handPointer;       // 时钟指针

    public:
        CLOCK() : BaseAlgorithm("CLOCK——时钟置换算法"), handPointer(0) {}
        void run()
        {
            std::cout << "正在运行 CLOCK ——时钟页面置换算法" << std::endl;
            for(size_t i = 0; i < pageReferenceString.size(); i++)
            {
                int pageID = pageReferenceString[i];
                bool isFault = false;
                
                auto it = std::find(pagePhysicalFrame.begin(), pagePhysicalFrame.end(), pageID);
                
                if(it != pagePhysicalFrame.end())
                {
                    int index = std::distance(pagePhysicalFrame.begin(), it);
                    referenceBits[index] = true;
                }
                else
                {   
                    pageFaults++;
                    isFault = true;
                    if(pagePhysicalFrame.size() >= Config::FRAME_COUNT)
                    {
                        while(true)
                        {
                            if(referenceBits[handPointer] == true)
                            {
                                referenceBits[handPointer] = false;
                                handPointer = (handPointer + 1) % Config::FRAME_COUNT;
                            }
                            else
                            {
                                pagePhysicalFrame[handPointer] = pageID;
                                referenceBits[handPointer] = true;
                                handPointer = (handPointer + 1) % Config::FRAME_COUNT;
                                break;
                            }
                        }
                    }
                    else
                    {
                        pagePhysicalFrame.push_back(pageID);
                        referenceBits.push_back(true);
                    }
                }

                printStep(i + 1, pageID, isFault);
            }

        }
};