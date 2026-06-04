#define _CRT_SECURE_NO_WARNINGS

#include "RefreshScreenBuffer.h"

void printtime()
{

    /* 获取当前时间点
       std::chrono::system_clock::now()获取当前时间点
       std::chrono::system_clock是<chrono>头文件中定义的一个时钟类型
       now()是一个静态函数，返回表示当前时间点的std::chrono::system_clock::time_point类型的值。time_point是一个时间点
    */
    auto now = std::chrono::system_clock::now();
    /* 转换为time_t类型
       to_time_t 是一个静态成员函数，它将 std::chrono::system_clock::time_point 转换为 std::time_t 类型的值
       std::time_t 是一个在 <ctime> 头文件中定义的算术类型
    */
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    /* 转换为本地时间
       std::localtime 是一个在 <ctime> 头文件中定义的函数，它接受一个指向 std::time_t对象的指针，并返回一个指向 std::tm 结构的指针
       std::tm 是一个结构体，包含了分解的时间信息（如年、月、日、小时、分钟、秒等）

       localtime函数被认为是不安全的，因为它可能引发缓冲区溢出等安全问题 
    */
    std::tm* now_c = std::localtime(&in_time_t);

    /* 使用put_time来格式化并显示时间
       std::put_time 是一个在 <iomanip> 头文件中定义的I/O操纵符，它接受一个指向 std::tm 对象的指针和一个格式字符串，并将格式化的时间输出到与之关联的流中。
       格式字符串 "%Y-%m-%d %H:%M:%S" 指定了输出的格式，其中 %Y、%m、%d 分别代表四位数的年份、月份、日期，%H、%M、%S 分别代表小时、分钟、秒。
    */

    std::cout << "+------------------------------------------+" << std::endl;
    std::cout << "|" << "      当前时间: " << std::put_time(now_c, "%Y-%m-%d %H:%M:%S") << "       " << "|" << std::endl;
    std::cout << "+------------------------------------------+" << std::endl << std::endl;
}


/*
    #ifdef _WIN32   是一个预处理指令，用于检查 _WIN32 宏是否已定义。_WIN32 宏在 Windows 系统上由编译器自动定义。
    如果 _WIN32 已定义，则编译器会编译std::system("cls") 在 Windows 上，这行代码调用 std::system 函数执行 cls 命令，该命令用于清除命令行窗口的屏幕。
     
    #elif __linux__ 如果 _WIN32 未定义，则预处理器会检查 __linux__ 宏是否已定义。__linux__ 宏在 Linux 系统上由编译器自动定义。
    如果 _WIN32 未定义且 __linux__ 已定义，则编译器会std::system("clear") 在 Linux 上，这行代码调用 std::system 函数执行 clear 命令，该命令用于清除终端的屏幕。
*/
void cls()
{

#ifdef _WIN32
    std::system("cls");
#elif  __linux__ 
    std::system("clear");
#endif

}

void RefreshScreenBuffer()
{
	cls();
    printtime();
}