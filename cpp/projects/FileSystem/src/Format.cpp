#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

#include "../include/common/GlobalDefine.hpp"

int main()
{
    std::string disk_name = "Virtual_Disk.vfs";
    
    std::ofstream disk(disk_name, std::ios::binary | std::ios::out);
    if(!disk)
    {
        std::cerr << "创建磁盘失败！" << std::endl;
        return 1;
    }

    disk.seekp(DISK_SIZE - 1);
    char null_char = 0;
    disk.write(&null_char, 1);

    SuperBlock superblock;
    std::memset(&superblock, 0, sizeof(superblock));
    superblock.s_blocks_count = DISK_SIZE / BLOCK_SIZE;

    superblock.s_magic = MAGIC_NUMBER;

    superblock.s_inode_bitmap_ptr = 1;
    superblock.s_data_bitmap_ptr = superblock.s_inode_bitmap_ptr + INODE_BITMAP_BLOCKS;
    
    superblock.s_data_bitmap_level1_ptr = superblock.s_data_bitmap_ptr;
    superblock.s_data_bitmap_level2_ptr = superblock.s_data_bitmap_ptr + 1;
    superblock.s_data_bitmap_level3_ptr = superblock.s_data_bitmap_ptr + 2;

    superblock.s_inode_table_ptr = superblock.s_data_bitmap_ptr + DATA_BITMAP_BLOCKS;
    superblock.s_journal_ptr = superblock.s_inode_table_ptr + INODE_TABLE_BLOCKS;
    superblock.s_data_block_ptr = superblock.s_journal_ptr + JOURNAL_BLOCKS;

    superblock.s_free_inodes = INODE_TABLE_BLOCKS * (BLOCK_SIZE / sizeof(Inode));
    superblock.s_free_blocks = superblock.s_blocks_count - superblock.s_data_block_ptr;

    disk.seekp(0);
    disk.write(reinterpret_cast<char*>(&superblock), sizeof(superblock));

    char empty_block[BLOCK_SIZE];
    std::memset(empty_block, 0, BLOCK_SIZE);

    for(uint32_t i = 1; i < superblock.s_blocks_count; i++)
    {
        disk.seekp((uint64_t)i * BLOCK_SIZE);
        disk.write(empty_block, BLOCK_SIZE);
    }

    Inode root_inode;
    std::memset(&root_inode, 0, sizeof(root_inode));
    root_inode.i_mode = FILE_TYPE_DIRECTORY;
    root_inode.i_link_count = 1;
    
    uint32_t root_inode_offset = superblock.s_inode_table_ptr * BLOCK_SIZE;
    disk.seekp(root_inode_offset);
    disk.write(reinterpret_cast<char*>(&root_inode), sizeof(root_inode));

    disk.seekp((uint64_t)superblock.s_inode_bitmap_ptr * BLOCK_SIZE);
    char bitmap_byte = 0x01;
    disk.write(&bitmap_byte, 1);

    superblock.s_free_inodes--;
    disk.seekp(0);
    disk.write(reinterpret_cast<char*>(&superblock), sizeof(superblock));

    std::cout << "========================================" << std::endl;
    std::cout << "VFS 格式化成功：" << std::endl;
    std::cout << "魔数: 0x" << std::hex << superblock.s_magic << std::dec << std::endl;
    std::cout << "磁盘布局分布: " << std::endl;
    std::cout << "      [0] SuperBlock" << std::endl;
    std::cout << "   [" << superblock.s_inode_bitmap_ptr << "-" << superblock.s_data_bitmap_ptr-1 << "] Inode Bitmap (" << INODE_BITMAP_BLOCKS << " Blocks)" << std::endl;
    std::cout << "  [" << superblock.s_data_bitmap_ptr << "-" << superblock.s_inode_table_ptr-1 << "] Data Bitmap  (" << DATA_BITMAP_BLOCKS << " Blocks)" << std::endl;
    std::cout << " [" << superblock.s_inode_table_ptr << "-" << superblock.s_journal_ptr-1 << "] Inode Table  (" << INODE_TABLE_BLOCKS << " Blocks)" << std::endl;
    std::cout << "[" << superblock.s_journal_ptr << "-" << superblock.s_data_block_ptr-1 << "] Journal      (" << JOURNAL_BLOCKS << " Blocks)" << std::endl;
    std::cout << "[" << superblock.s_data_block_ptr << "-END] Data Area    (" << superblock.s_free_blocks << " Blocks)" << std::endl;
    std::cout << "========================================" << std::endl;

    disk.close();

    return 0;
}