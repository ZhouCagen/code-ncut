#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <memory>

#include "../include/core/FileSystem.hpp" 

const std::string VIRTUAL_DISK_PATH = "Virtual_Disk.vfs";

int main() 
{

    std::cout << "\n===========================================================" << std::endl;
    std::cout << "              VFS 文件系统基础功能测试                     " << std::endl;
    std::cout << "===========================================================" << std::endl;

    FileSystem fs(VIRTUAL_DISK_PATH);

    std::cout << "[步骤 1] 正在挂载文件系统..." << std::endl;
    if (!fs.mount_file_system()) 
    {
        std::cerr << "[严重错误] 挂载失败！请先运行 'format.exe' 进行格式化。" << std::endl;
        return 1;
    }
    std::cout << "-> 文件系统挂载成功。" << std::endl;

    std::string test_dir_name = "测试目录";
    std::cout << "\n[步骤 2] 正在创建目录 '" << test_dir_name << "'..." << std::endl;
    
    if (fs.find_entry(0, test_dir_name) != -1) 
    {
        std::cout << "-> 目录已存在，跳过创建。" << std::endl;
    } 
    else 
    {
        if (fs.create_object(0, test_dir_name, FILE_TYPE_DIRECTORY)) 
        {
            std::cout << "-> 目录创建成功。" << std::endl;
        }
        else 
        {
            std::cerr << "-> 目录创建失败！" << std::endl;
        }
    }

    std::string test_file_name = "zkj.txt";
    std::string file_content = "一个基础测试字符串。";
    
    std::cout << "\n[步骤 3] 正在目录 '" << test_dir_name << "' 下创建文件 '" << test_file_name << "'..." << std::endl;
    
    int32_t dir_id = fs.find_entry(0, test_dir_name);
    if (dir_id == -1) 
    {
        std::cerr << "-> 错误：找不到测试目录的 Inode ID。" << std::endl;
        return 1;
    }

    if (fs.find_entry(dir_id, test_file_name) != -1)
    {
        std::cout << "-> 文件已存在，将覆盖内容。" << std::endl;
    }
    else 
    {
        if (fs.create_object(dir_id, test_file_name, FILE_TYPE_FILE)) 
        {
            std::cout << "-> 文件创建成功。" << std::endl;
        } else 
        {
            std::cerr << "-> 文件创建失败！" << std::endl;
            return 1;
        }
    }

    std::cout << "-> 正在写入内容: \"" << file_content << "\"" << std::endl;
    int32_t file_id = fs.find_entry(dir_id, test_file_name);
    fs.write_file_content(file_id, file_content);
    std::cout << "-> 写入操作完成。" << std::endl;

    std::cout << "\n[步骤 4] 正在读取并验证内容..." << std::endl;
    
    std::cout << "-> 文件内容如下:" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    fs.cat_file(file_id);
    std::cout << "------------------------------------------------" << std::endl;
    
    std::cout << "-> 请确认上方内容是否匹配: \"" << file_content << "\"" << std::endl;

    std::cout << "\n[步骤 5] 列出目录内容..." << std::endl;
    std::cout << "-> 根目录 (Root, ID 0):" << std::endl;
    fs.list_directory(0);
    
    std::cout << "\n-> 测试目录 (ID " << dir_id << "):" << std::endl;
    fs.list_directory(dir_id);

    std::cout << "\n===========================================================" << std::endl;
    std::cout << "                     基础测试已完成                        " << std::endl;
    std::cout << "===========================================================" << std::endl;

    return 0;
}