#pragma once
#include <cstring>

#include "DiskDevice.hpp"

class BlockManager 
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
        BlockManager(DiskDevice* device, SuperBlock* sb) 
            : disk_device(device), super_block(sb) {}

        uint32_t allocate_data_block() 
        {
            char bitmap_buffer[BLOCK_SIZE];
            
            disk_device->read_block(super_block->s_data_bitmap_level3_ptr, bitmap_buffer);

            for (int i = 0; i < BLOCK_SIZE; i++) 
            {
                if ((unsigned char)bitmap_buffer[i] != 0xFF) 
                {
                    int bit_index = find_free_bit(bitmap_buffer[i]);
                    
                    bitmap_buffer[i] |= (1 << bit_index);
                    disk_device->write_block(super_block->s_data_bitmap_level3_ptr, bitmap_buffer);

                    super_block->s_free_blocks--;
                    disk_device->write_super_block(*super_block);

                    uint32_t offset = i * 8 + bit_index;
                    return super_block->s_data_block_ptr + offset;
                }
            }
            return 0;
        }

        void clear_block(uint32_t blockID) 
        {
            char empty_buffer[BLOCK_SIZE] = {0};
            disk_device->write_block(blockID, empty_buffer);
        }
};