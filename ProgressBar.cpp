
/*
    printProgressBar函数为"进度条"显示函数
    barwith表示进度条宽度
    int pos = barwidth * progress / total 根据当前进度和总进度的比例，计算出在进度条上应该显示“已完成”部分的位置（pos）
    使用for循环进行遍历进度条位置

    如果当前位置i<pos（即“已完成”部分），则打印等号（=）来表示已完成
    如果当前位置i=pos（即“正在进行”的位置），则打印大于号（>）来表示当前进度
    如果当前位置i>pos（即“未完成”部分），则打印空格来表示尚未完成

     std::cout.flush() 刷新输出缓冲区，确保立即显示
     std::this_thread 是一个命名空间 它提供了一种方便的方式来查询或操作当前正在执行的线程 sleep_for 是 std::this_thread 命名空间中的一个函数
     用于使当前线程暂停执行指定的时间段。这个函数接受一个表示时间段的参数，并且在该时间段结束之前，当前线程不会继续执行。

     std::chrono::milliseconds 是 std::chrono 命名空间中的一个持续时间类型，专门用于表示以毫秒为单位的时间段。

     std::this_thread::sleep_for(std::chrono::milliseconds(100))这行代码被执行时，它会暂停当前线程的执行，直到指定的时间段（100毫秒）过去。
     这通常用于模拟耗时操作、控制执行速率或简单地延迟执行。

*/

#include "ProgressBar.h"

void printProgressBar(int progress, int total)
{
    int barwidth = 70;
    int pos = barwidth * progress / total;

    std::cout << "[";

    for (int i = 0; i < barwidth; ++i)
    {
        if (i < pos)
        {
            std::cout << "=";
        }
        else if (i == pos)
        {
            std::cout << ">";
        }
        else
        {
            std::cout << " ";
        }
    }

    std::cout << "] " << int((progress / (float)total) * 100.0) << " %\r";

    std::cout.flush();  

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
