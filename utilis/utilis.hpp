#include <cstring>

namespace utls
{
    char** splitArray(char* array, char splitter, char** result, int result_length, int result_element_length)
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
}