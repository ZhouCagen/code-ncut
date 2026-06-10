#pragma once
#include <memory>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <vector>

#include "DiskDevice.hpp"
#include "BlockManager.hpp"
#include "InodeManager.hpp"

class FileSystem 
{
    private:
        std::unique_ptr<DiskDevice> disk_device;
        std::unique_ptr<BlockManager> block_manager;
        std::unique_ptr<InodeManager> inode_manager;
        SuperBlock super_block;
        std::string device_path;

        uint32_t get_indirect_block(uint32_t& parent_block_ptr, uint32_t logical_index, int depth) 
        {
            const uint32_t pointers_per_block = BLOCK_SIZE / sizeof(uint32_t);
            
            if (parent_block_ptr == 0) 
            {
                uint32_t new_block_id = block_manager->allocate_data_block();
                if (new_block_id == 0) return 0;
                block_manager->clear_block(new_block_id);
                parent_block_ptr = new_block_id;
            }

            uint32_t span = 1;
            for (int k = 0; k < depth; k++) span *= pointers_per_block;

            uint32_t index_in_block = logical_index / span;
            uint32_t remaining_logical = logical_index % span;

            uint32_t index_buffer[pointers_per_block];
            disk_device->read_block(parent_block_ptr, (char*)index_buffer);

            uint32_t result_block_id = 0;

            if (depth == 0) 
            {
                if (index_buffer[index_in_block] == 0) 
                {
                    index_buffer[index_in_block] = block_manager->allocate_data_block();
                    if (index_buffer[index_in_block] != 0) 
                    {
                        disk_device->write_block(parent_block_ptr, (char*)index_buffer);
                    }
                }
                result_block_id = index_buffer[index_in_block];
            } 
            else 
            {
                uint32_t next_level_block = index_buffer[index_in_block];
                result_block_id = get_indirect_block(next_level_block, remaining_logical, depth - 1);
                
                if (index_buffer[index_in_block] != next_level_block) 
                {
                    index_buffer[index_in_block] = next_level_block;
                    disk_device->write_block(parent_block_ptr, (char*)index_buffer);
                }
            }
            return result_block_id;
        }

    public:
        FileSystem(std::string path) : device_path(path) 
        {
            disk_device = std::make_unique<DiskDevice>(path);
        }

        bool mount_file_system() 
        {
            if (!disk_device->open_disk()) return false;
            
            disk_device->read_super_block(super_block);
            
            if (super_block.s_magic != MAGIC_NUMBER) return false;

            block_manager = std::make_unique<BlockManager>(disk_device.get(), &super_block);
            inode_manager = std::make_unique<InodeManager>(disk_device.get(), &super_block);
            
            return true;
        }

        int32_t find_entry(uint32_t parent_id, const std::string& entry_name) {
            Inode parent_inode;
            inode_manager->read_inode(parent_id, parent_inode);
        
            if (parent_inode.i_mode != FILE_TYPE_DIRECTORY) return -1;

            if (parent_inode.i_blocks > 100000) return -1; 

            char block_buffer[BLOCK_SIZE];
            
            for (uint32_t i = 0; i < parent_inode.i_blocks && i < 12; i++) 
            {
                disk_device->read_block(parent_inode.i_address[i], block_buffer);
                DirectoryEntry* entries = (DirectoryEntry*)block_buffer;
                
                for (int k = 0; k < BLOCK_SIZE / sizeof(DirectoryEntry); k++) 
                {
                    if (entries[k].inode_id != 0) 
                    {
                        char safe_name[32] = {0};
                        std::memcpy(safe_name, entries[k].file_name, sizeof(entries[k].file_name));
                        safe_name[sizeof(entries[k].file_name) - 1] = '\0';

                        if (std::string(safe_name) == entry_name) 
                        {
                            return entries[k].inode_id;
                        }
                    }
                }
            }
            return -1;
        }

        bool add_entry(uint32_t parent_id, const std::string& entry_name, uint32_t child_id) 
        {
            Inode parent_inode;
            inode_manager->read_inode(parent_id, parent_inode);
            char block_buffer[BLOCK_SIZE];

            for (uint32_t i = 0; i < parent_inode.i_blocks && i < 12; i++) 
            {
                uint32_t physical_block = parent_inode.i_address[i];
                disk_device->read_block(physical_block, block_buffer);
                
                DirectoryEntry* entries = (DirectoryEntry*)block_buffer;
                for (int k = 0; k < BLOCK_SIZE / sizeof(DirectoryEntry); k++) 
                {
                    if (entries[k].inode_id == 0) { // 找到空槽位
                        entries[k].inode_id = child_id;
                        std::strncpy(entries[k].file_name, entry_name.c_str(), 27);
                        disk_device->write_block(physical_block, block_buffer);
                        return true;
                    }
                }
            }

            if (parent_inode.i_blocks < 12) 
            {
                int32_t new_block_id = block_manager->allocate_data_block();
                if (new_block_id == 0) return false;

                std::memset(block_buffer, 0, BLOCK_SIZE);
                DirectoryEntry* entries = (DirectoryEntry*)block_buffer;
                entries[0].inode_id = child_id;
                std::strncpy(entries[0].file_name, entry_name.c_str(), 27);
                
                disk_device->write_block(new_block_id, block_buffer);

                parent_inode.i_address[parent_inode.i_blocks] = new_block_id;
                parent_inode.i_blocks++;
                parent_inode.i_size += BLOCK_SIZE;
                inode_manager->write_inode(parent_id, parent_inode);
                return true;
            }
            return false;
        }

        bool create_object(uint32_t parent_dir_id, std::string object_name, uint32_t object_type) 
        {
            if (find_entry(parent_dir_id, object_name) != -1) return false;
            
            int32_t new_inode_id = inode_manager->allocate_inode();
            if (new_inode_id == -1) return false;

            Inode new_node;
            std::memset(&new_node, 0, sizeof(new_node));
            new_node.i_mode = object_type;
            new_node.i_link_count = 1;
            inode_manager->write_inode(new_inode_id, new_node);

            return add_entry(parent_dir_id, object_name, new_inode_id);
        }

        uint32_t get_block_for_write(Inode& inode_node, uint32_t logical_index) 
        {
            if (logical_index < 12) 
            {
                if (inode_node.i_address[logical_index] == 0) 
                {
                    uint32_t new_block_id = block_manager->allocate_data_block();
                    if (new_block_id == 0) return 0;
                    inode_node.i_address[logical_index] = new_block_id;
                    inode_node.i_blocks++;
                }
                return inode_node.i_address[logical_index];
            }

            logical_index -= 12;
            const uint32_t pointers_per_block = BLOCK_SIZE / sizeof(uint32_t);
         
            if (logical_index < pointers_per_block) 
            {
                return get_indirect_block(inode_node.i_address[12], logical_index, 0);
            }
            logical_index -= pointers_per_block;

            if (logical_index < pointers_per_block * pointers_per_block) 
            {
                return get_indirect_block(inode_node.i_address[13], logical_index, 1);
            }
            logical_index -= pointers_per_block * pointers_per_block;

            if (logical_index < pointers_per_block * pointers_per_block * pointers_per_block)
            {
                return get_indirect_block(inode_node.i_address[14], logical_index, 2);
            }

            return 0; 
        }

        void write_file_content(uint32_t inode_id, std::string content) 
        {
            Inode inode_node;
            inode_manager->read_inode(inode_id, inode_node);
            
            uint32_t block_id = get_block_for_write(inode_node, 0);
            
            char buffer[BLOCK_SIZE] = {0};
            std::strncpy(buffer, content.c_str(), BLOCK_SIZE - 1);
            
            disk_device->write_block(block_id, buffer);
            
            inode_node.i_size = content.length();
            inode_manager->write_inode(inode_id, inode_node);
        }

        void list_directory(uint32_t directory_id) 
        {
            Inode directory_inode;
            inode_manager->read_inode(directory_id, directory_inode);
            
            std::cout << "--- 目录列表 ---" << std::endl;
            char block_buffer[BLOCK_SIZE];
            
            for (uint32_t i = 0; i < directory_inode.i_blocks && i < 12; i++) 
            {
                disk_device->read_block(directory_inode.i_address[i], block_buffer);
                DirectoryEntry* entries = (DirectoryEntry*)block_buffer;
                
                for (int k = 0; k < BLOCK_SIZE / sizeof(DirectoryEntry); k++) 
                {
                    if (entries[k].inode_id != 0) 
                    {
                        Inode child_node;
                        inode_manager->read_inode(entries[k].inode_id, child_node);
                        
                        std::string type_string = (child_node.i_mode == FILE_TYPE_DIRECTORY) ? "<目录>" : "<文件>";
                        std::cout << entries[k].file_name << " " << type_string << " " << child_node.i_size << " 字节" << std::endl;
                    }
                }
            }
        }

        // 读取文件 (Cat)
        void cat_file(uint32_t inode_id) 
        {
            Inode inode_node;
            inode_manager->read_inode(inode_id, inode_node);
            
            if (inode_node.i_blocks > 0) 
            {
                char buffer[BLOCK_SIZE + 1];
                disk_device->read_block(inode_node.i_address[0], buffer);
                buffer[BLOCK_SIZE] = 0; 
                std::cout << buffer << std::endl;
            }
        }

        // 删除对象
        bool delete_object(uint32_t parent_dir_id, std::string object_name) 
        {
            int32_t target_id = find_entry(parent_dir_id, object_name);
            if (target_id == -1) return false;

            Inode parent_inode;
            inode_manager->read_inode(parent_dir_id, parent_inode);
            char block_buffer[BLOCK_SIZE];

            for (int i = 0; i < parent_inode.i_blocks; i++) 
            {
                disk_device->read_block(parent_inode.i_address[i], block_buffer);
                DirectoryEntry* entries = (DirectoryEntry*)block_buffer;
                
                for (int k = 0; k < BLOCK_SIZE / sizeof(DirectoryEntry); k++) 
                {
                    if (entries[k].inode_id == target_id) 
                    {
                        entries[k].inode_id = 0; // 标记删除
                        disk_device->write_block(parent_inode.i_address[i], block_buffer);
                        return true;
                    }
                }
            }
            return false;
        }
};