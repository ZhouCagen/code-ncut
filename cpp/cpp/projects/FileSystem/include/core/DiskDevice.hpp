#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../common/GlobalDefine.hpp" 

class DiskDevice 
{
    private:
        std::fstream disk_stream;
        std::string disk_path;

    public:
        DiskDevice(std::string path) : disk_path(path) {}

        ~DiskDevice() 
        {
            if (disk_stream.is_open()) 
            {
                disk_stream.close();
            }
        }

        // 打开磁盘文件
        bool open_disk() 
        {
            disk_stream.open(disk_path, std::ios::binary | std::ios::in | std::ios::out);
            return disk_stream.is_open();
        }

        // 移动读写指针到指定块
        void seek_to_block(uint32_t blockID) 
        {
            disk_stream.seekg((uint64_t)blockID * BLOCK_SIZE);
            disk_stream.seekp((uint64_t)blockID * BLOCK_SIZE);
        }

        // 读取一个块
        void read_block(uint32_t blockID, char* buffer)
        {
            disk_stream.seekg((uint64_t)blockID * BLOCK_SIZE);
            disk_stream.read(buffer, BLOCK_SIZE);
        }

        // 写入一个块
        void write_block(uint32_t blockID, const char* buffer) 
        {
            disk_stream.seekp((uint64_t)blockID * BLOCK_SIZE);
            disk_stream.write(buffer, BLOCK_SIZE);
            disk_stream.flush();
        }

        // 特殊：读写超级块 (位于偏移 0)
        void read_super_block(SuperBlock& super_block) 
        {
            disk_stream.seekg(0);
            disk_stream.read(reinterpret_cast<char*>(&super_block), sizeof(SuperBlock));
        }

        void write_super_block(const SuperBlock& super_block) 
        {
            disk_stream.seekp(0);
            disk_stream.write(reinterpret_cast<const char*>(&super_block), sizeof(SuperBlock));
            disk_stream.flush();
        }
};