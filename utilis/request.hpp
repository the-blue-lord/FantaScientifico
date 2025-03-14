#ifndef REQUEST_H
    #define REQUEST_H
#endif



#ifndef UTILIS_H
    #include "utilis.hpp"
#endif

#include <string>
#include <cstring>
#include <map>


namespace rqst
{
    typedef int RequestType;

    const char* RequestTypeName[4] = {"Error", "Unknown", "GET", "POST"};
    namespace RequestTypes
    {
        RequestType Error = -1;
        RequestType Unknown = 0;
        RequestType Get = 1;
        RequestType Post = 2;
    }

    const  char* getTypeName(const RequestType type)
    {
        return RequestTypeName[type+1];
    }

    RequestType getType(const char* request)
    {
        if(!request)
        {
            return -1;
        }
        
        if(strncmp(request, "GET ", 4) == 0)
        {
            return 1;
        }
        else if(strncmp(request, "POST ", 5) == 0)
        {
            return 2;
        }
        
        return 0;
    }

    char* getPath(char* request, char* path, const int path_size)
    {
        if(!request || !path || path_size <= 0)
        {
            return nullptr;
        }

        char* start = strchr(request, ' ');
        if(!start)
        {
            return nullptr;
        }
        start++;

        char* end = strchr(start, ' ');
        char* end2 = strchr(start, '?');
        if(!end && !end2)
        {
            end = start + strlen(start);
        }
        else if(!end || (end2 && end2 < end))
        {
            end = end2;
        }

        int length = end - start;
        if(length >= path_size)
        {
            length = path_size - 1;
        }
        
        strncpy(path, start, length);
        path[length] = '\0';

        return path;
    }

    char* getRawArgs(char* request, char* args, const int args_size)
    {
        char* min = strchr(request, ' ');
        if(!min) return nullptr;

        char* end = strchr(min+1, ' ');
        if(!end)
        {
            end = min + strlen(min);
        }

        char* start = strchr(min,'?');
        if(!start || start > end)
        {
            args[0] = '\0';
            return nullptr;
        }
        start++;

        int length = end - start;
        if(length >= args_size) length = args_size - 1;

        strncpy(args, start, length);
        args[length] = '\0';

        return args;
    }

    int countArgs(const char* args, const int args_length)
    {
        if(!args || args_length <= 0 || args[0] == '\0') return 0;

        int cnt = 1;

        for(int i = 0; i < args_length && args[i] != '\0'; i++) if(args[i] == '&') cnt++;

        return cnt;
    }

    std::map<std::string, std::string> getArgs(char* request)
    {
        std::map<std::string, std::string> result;

        char rawArgs[1024];
        getRawArgs(request, rawArgs, sizeof(rawArgs));

        char* args[15];
        int arguments_number = countArgs(rawArgs, sizeof(rawArgs));
        utls::splitArray(rawArgs, '&', args, sizeof(args), sizeof(args[0]));
        
        for(int i = 0; i < arguments_number; i++)
        {
            char* var[2];
            utls::splitArray(args[i], '=', var, sizeof(var), sizeof(var[0]));
            result[var[0]] = var[1];
        }

        return result;
    }

    char* getParameterValue(char* request, char* key, int key_length, char* value, int value_length)
    {
        char *start = strstr(request, key);
        if(!start) return nullptr;
        start += key_length;

        char* end = strstr(start, "\r\n");

        int length = end - start;
        if(length >= value_length) length = value_length - 1;

        strncpy(value, start, length);
        value[length] = '\0';

        return value;
    }
}