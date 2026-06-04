#include <stdexcept>
#include <iostream>

#include "base85ed.h"

const char ALPHABET[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "!#$%&()*+-;<=>?@^_`{|}~";


// finding index of symbol in an alphabet otherwise return -1
int find_in_alphabet(uint8_t c)
{
    for (int i = 0; i < 85; i++)
    {
        if ((uint8_t)ALPHABET[i] == c)
        {
            return i;
        }
    }
    return -1;
}

// TODO: implement this in C++
std::vector<uint8_t> base85::encode(std::vector<uint8_t> const &bytes)
{
    std::vector<uint8_t> result;

    // blocks are size of 4 bytes
    int n = (int)bytes.size();
    int i = 0;
    while (i < n)
    {
        // how many bites before end
        int left = n - i;
        int block_size = 4;
        if (left < 4)
        {
            block_size = left;
        }

        // make one big number with 4 blocks
        unsigned long long number = 0;
        for (int j = 0; j < 4; j++)
        {
            number = number * 256;
            if (j < block_size)
            {
                number = number + bytes[i + j];
            }
        }

        char digits[5];
        for (int j = 4; j >= 0; j--)
        {
            digits[j] = ALPHABET[number % 85];
            number /= 85;
        }

        int out_count = block_size + 1;
        for (int j = 0; j < out_count; j++)
        {
            result.push_back(digits[j]);
        }

        i = i + 4;
    }

    return result;
}


// TODO: implement this in C++
std::vector<uint8_t> base85::decode(std::vector<uint8_t> const &b85str)
{
    std::vector<uint8_t> result;

    int n = (int)b85str.size();
    int i = 0;
    while (i < n)
    {
        int left = n - i;
        int block_size = 5;
        if (left < 5)
        {
            block_size = left;
        }

        if (block_size < 2)
        {
            throw std::runtime_error("base85: некорректная длина входа");
        }

        unsigned long long number = 0;
        for (int j = 0; j < 5; j++)
        {
            int d;
            if (j < block_size)
            {
                int idx = find_in_alphabet(b85str[i + j]);
                if (idx < 0)
                {
                    throw std::runtime_error("base85: недопустимый символ");
                }
                d = idx;
            }
            else
            {
                d = 84;
            }
            number = number * 85 + d;
        }
        if (number > 0xFFFFFFFFULL)
        {
            throw std::runtime_error("base85: переполнение блока");
        }

        uint8_t out_bytes[4];
        for (int j = 3; j >= 0; j--)
        {
            out_bytes[j] = (uint8_t)(number % 256);
            number = number / 256;
        }

        for (int j = 0; j < block_size - 1; j++)
        {
            result.push_back(out_bytes[j]);
        }

        i = i + 5;
    }

    return result;
}