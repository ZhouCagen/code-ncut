#pragma once
#include <random>
#include <vector>

class RandomDataGenerator 
{
    private:
        std::mt19937 random_generator;
        std::uniform_int_distribution<unsigned short> byte_distribution; 

    public:
        RandomDataGenerator(unsigned int seed_value) : random_generator(seed_value), byte_distribution(0, 255) {}

        // 填充缓冲区
        void fill_buffer(char* buffer, int buffer_size) 
        {
            for (int i = 0; i < buffer_size; ++i) 
            {
                buffer[i] = (char)byte_distribution(random_generator);
            }
        }
};