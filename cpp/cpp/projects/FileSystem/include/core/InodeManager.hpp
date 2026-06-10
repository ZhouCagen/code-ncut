#pragma once
#include <cstring>

#include "DiskDevice.hpp"

class InodeManager 
{
    private:
        DiskDevice* disk_device;
        SuperBlock* super_block;

        int find_free_bit(char byte_value) 
        {
            for (int i = 0; i < 8; i++) 
            {
                if (!((byte_value >> i) & 1)) return i;
            }
            return -1;
        }

    public:
        InodeManager(DiskDevice* device, SuperBlock* sb) : disk_device(device), super_block(sb) {}

        void read_inode(uint32_t inodeID, Inode& inode_node) 
        {
            uint32_t blockID = super_block->s_inode_table_ptr + (inodeID * sizeof(Inode)) / BLOCK_SIZE;
            uint32_t offset = (inodeID * sizeof(Inode)) % BLOCK_SIZE;
            
            char buffer[BLOCK_SIZE];
            disk_device->read_block(blockID, buffer);
            std::memcpy(&inode_node, buffer + offset, sizeof(Inode));
        }

        void write_inode(uint32_t inodeID, const Inode& inode_node) 
        {
            uint32_t blockID = super_block->s_inode_table_ptr + (inodeID * sizeof(Inode)) / BLOCK_SIZE;
            uint32_t offset = (inodeID * sizeof(Inode)) % BLOCK_SIZE;

            char buffer[BLOCK_SIZE];
            disk_device->read_block(blockID, buffer);
            std::memcpy(buffer + offset, &inode_node, sizeof(Inode));
            disk_device->write_block(blockID, buffer);
        }

        int32_t allocate_inode() 
        {
            char buffer[BLOCK_SIZE];
            disk_device->read_block(super_block->s_inode_bitmap_ptr, buffer);

            for (int i = 0; i < BLOCK_SIZE; i++) 
            {
                if ((unsigned char)buffer[i] != 0xFF) 
                {
                    int bit_index = find_free_bit(buffer[i]);
                    
                    buffer[i] |= (1 << bit_index);
                    disk_device->write_block(super_block->s_inode_bitmap_ptr, buffer);

                    super_block->s_free_inodes--;
                    disk_device->write_super_block(*super_block);

                    return i * 8 + bit_index;
                }
            }
            return -1; 
        }
};