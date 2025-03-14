#ifndef UTILIS_H
    #define UTILIS_H
#endif

#include <cstring>
#include <filesystem>
#include <fstream>

namespace utls
{
    char** splitArray(char* array, const char splitter, char** result, const int result_length, const int result_element_length)
    {
        char* start = array;
        int result_elements_number = result_length/result_element_length;
        for(int i = 0; i < result_elements_number; i++)
        {
            result[i] = start;
            start = strchr(start, splitter);
            if(!start) return result;
            *start = '\0';
            start++;
        }

        return result;
    }

    char** stringSplitArray(char* array, const char* splitter, const int splitter_length, char** result, const int result_length, const int result_element_length)
    {
        char* start = array;
        int result_elements_number = result_length/result_element_length;
        for(int i = 0; i < result_elements_number; i++)
        {
            result[i] = start;
            start = strstr(start, splitter);
            if(!start) return result;
            *start = '\0';
            start += splitter_length;
        }

        return result;
    }
}

int fileSize(const char* filename)
{
    if(!std::filesystem::exists(filename)) return -1;

    int result = std::filesystem::file_size(filename);

    return result;
}

char* readFile(const char* filename, char* buffer, const int bufferSize)
{
    if(!std::filesystem::exists(filename)) return nullptr;

    std::ifstream file(filename, std::ios::binary);

    if (!file) return nullptr;

    int file_length = fileSize(filename);

    if (file_length >= bufferSize) file_length = bufferSize - 1;

    if (!file.read(buffer, file_length)) return nullptr;

    buffer[file_length] = '\0';

    return buffer;
}
