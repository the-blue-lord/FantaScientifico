#ifndef RESPONSE_H
    #define RESPONSE_H
#endif



#ifndef UTILIS_H
    #include "utilis.hpp"
#endif

#include <cstring>
#include <map>

namespace rspn {
    std::map<std::string, std::string, bool> paths;

    void addPath(std::string request_path, std::string file_path, bool needs_replace) {
        //paths[request_path] = (file_path, needs_replace);
    }

    int length(int statusCode, const char* status, const char* contentType, const char* content, const int content_length)
    {
        const char* str1 = "HTTP/1.1 ";
        char str2[10+strlen(status)];
        snprintf(str2, sizeof(str2), "%d %s", statusCode, status);
        const char* str3 = "\r\nContent-Type: ";
        const char* str4 = contentType;
        const char* str5 = "\r\nContent-Length: ";
        char str6[10];
        snprintf(str6, sizeof(str6), "%d", content_length);
        const char* str7 = "\r\nConnection: close\r\n\r\n";

        return strlen(str1) + strlen(str2) + strlen(str3) + strlen(str4) + strlen(str5) + strlen(str6) + strlen(str7) + content_length + 1;
    }

    char* build(char* response, int response_length, int statusCode, const char* status, const char* contentType, const char* content, const int content_length)
    {
        const char* str1 = "HTTP/1.1 ";
        char str2[10+strlen(status)];
        snprintf(str2, sizeof(str2), "%d %s", statusCode, status);
        const char* str3 = "\r\nContent-Type: ";
        const char* str4 = contentType;
        const char* str5 = "\r\nContent-Length: ";
        char str6[10];
        snprintf(str6, sizeof(str6), "%d", content_length);
        const char* str7 = "\r\nConnection: close\r\n\r\n";

        int strLen = strlen(str1) + strlen(str2) + strlen(str3) + strlen(str4) + strlen(str5) + strlen(str6) + strlen(str7);
        int responseLen = strLen + content_length + 1;

        if(response_length < responseLen) responseLen = response_length;

        response[0] = '\0';

        strncat(response, str1, responseLen);
        strncat(response, str2, responseLen);
        strncat(response, str3, responseLen);
        strncat(response, str4, responseLen);
        strncat(response, str5, responseLen);
        strncat(response, str6, responseLen);
        strncat(response, str7, responseLen);
        strncat(response, content, responseLen);

        return response;
    }
}