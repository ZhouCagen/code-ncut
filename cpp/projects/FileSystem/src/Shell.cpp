#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <limits> 

#include "../include/common/GlobalDefine.hpp"
#include "../include/core/FileSystem.hpp"

uint32_t current_user_inode_id = 0; 
std::string current_user_name = "ROOT";

void show_help_menu() 
{
    std::cout << "\n=== 可用命令 ===" << std::endl;
    std::cout << "  format            : 格式化磁盘 (需退出程序运行 format.exe，此处仅提示)" << std::endl;
    std::cout << "  useradd <name>    : 创建新用户 (新建二级目录)" << std::endl;
    std::cout << "  login <name>      : 登录用户 (进入用户目录)" << std::endl;
    std::cout << "  logout            : 登出 (返回 Root)" << std::endl;
    std::cout << "  ls                : 列出当前目录内容" << std::endl;
    std::cout << "  touch <filename>  : 创建新文件" << std::endl;
    std::cout << "  rm <filename>     : 删除文件或目录" << std::endl;
    std::cout << "  write <file> <txt>: 向文件写入内容 (不支持空格)" << std::endl;
    std::cout << "  cat <filename>    : 显示文件内容" << std::endl;
    std::cout << "  exit              : 退出系统" << std::endl;
    std::cout << "==========================\n" << std::endl;
}

int main() 
{

    FileSystem file_system("Virtual_Disk.vfs");

    if (!file_system.mount_file_system()) 
    {
        std::cerr << "错误：挂载文件系统失败！请确保已运行 'format.exe' 进行格式化。" << std::endl;
        return 1;
    }

    std::cout << "VFS 二级文件系统模拟器启动成功！" << std::endl;
    std::cout << "输入 'help' 查看可用命令。" << std::endl;

    std::string input_line;

    while (true) 
    {
        if (current_user_inode_id == 0) 
            std::cout << "[Root]# ";
        else 
            std::cout << "[" << current_user_name << "]# ";

        if (!std::getline(std::cin, input_line)) break;
        if (input_line.empty()) continue;

        std::stringstream command_stream(input_line);
        std::string command_string;
        command_stream >> command_string;

        if (command_string == "exit") 
        {
            break;
        }
        else if (command_string == "help") 
        {
            show_help_menu();
        }
        else if (command_string == "ls") 
        {
            file_system.list_directory(current_user_inode_id);
        }
        else if (command_string == "useradd") 
        {
            std::string username;
            command_stream >> username;
            
            if (username.empty()) 
            { 
                std::cout << "用法: useradd <用户名>" << std::endl; 
                continue; 
            }
            
            if (current_user_inode_id != 0) 
            {
                std::cout << "错误：必须在 Root 模式下才能创建新用户。" << std::endl;
                continue;
            }

            bool is_created = file_system.create_object(0, username, FILE_TYPE_DIRECTORY);
            
            if (is_created) 
            {
                std::cout << "用户 '" << username << "' 创建成功。" << std::endl;
            } 
            else 
            {
                std::cout << "创建用户失败 (用户可能已存在或磁盘已满)。" << std::endl;
            }
        }
        else if (command_string == "login") 
        {
            std::string username;
            command_stream >> username;
            
            if (username.empty()) 
            { 
                std::cout << "用法: login <用户名>" << std::endl; 
                continue; 
            }

            int32_t found_inode_id = file_system.find_entry(0, username);
            
            if (found_inode_id == -1) 
            {
                std::cout << "错误：用户不存在。" << std::endl;
            } 
            else 
            {
                current_user_inode_id = found_inode_id;
                current_user_name = username;
                std::cout << "已登录为 " << username << "。" << std::endl;
            }
        }
        else if (command_string == "logout") 
        {
            current_user_inode_id = 0;
            current_user_name = "ROOT";
            std::cout << "已登出，返回 Root。" << std::endl;
        }
        else if (command_string == "touch") 
        {
            std::string filename;
            command_stream >> filename;
            
            if (filename.empty()) 
            { 
                std::cout << "用法: touch <文件名>" << std::endl; 
                continue; 
            }

            if (current_user_inode_id == 0) 
            {
                std::cout << "错误：请先登录一个用户 (Root 下不能直接创建文件)。" << std::endl;
                continue;
            }

            bool is_created = file_system.create_object(current_user_inode_id, filename, FILE_TYPE_FILE);
            
            if (is_created) 
            {
                std::cout << "文件创建成功。" << std::endl;
            } 
            else 
            {
                std::cout << "文件创建失败 (文件可能已存在)。" << std::endl;
            }
        }
        else if (command_string == "write") 
        {
            std::string filename;
            std::string content;
            
            command_stream >> filename >> content; 
            
            if (filename.empty() || content.empty()) 
            {
                 std::cout << "用法: write <文件名> <内容(无空格)>" << std::endl; 
                 continue;
            }
            
            int32_t file_inode_id = file_system.find_entry(current_user_inode_id, filename);
            
            if (file_inode_id == -1) 
            {
                std::cout << "错误：文件未找到。" << std::endl;
            } 
            else 
            {
                file_system.write_file_content(file_inode_id, content);
                std::cout << "内容写入完成。" << std::endl;
            }
        }
        else if (command_string == "cat") 
        {
            std::string filename;
            command_stream >> filename;
            
            int32_t file_inode_id = file_system.find_entry(current_user_inode_id, filename);
            
            if (file_inode_id == -1) 
            {
                std::cout << "错误：文件未找到。" << std::endl;
            } 
            else 
            {
                std::cout << "--- 文件内容开始 ---" << std::endl;
                file_system.cat_file(file_inode_id);
                std::cout << "--- 文件内容结束 ---" << std::endl;
            }
        }
        else if (command_string == "rm") 
        {
            std::string target_name;
            command_stream >> target_name;
            
            if (target_name.empty()) 
            {
                std::cout << "用法: rm <文件名>" << std::endl;
                continue;
            }

            uint32_t parent_dir_id = current_user_inode_id;
            
            if (file_system.delete_object(parent_dir_id, target_name)) 
            {
                std::cout << "删除成功。" << std::endl;
            } 
            else 
            {
                std::cout << "删除失败 (文件未找到)。" << std::endl;
            }
        }
        else 
        {
            std::cout << "未知命令: " << command_string << std::endl;
        }
    }

    return 0;
}