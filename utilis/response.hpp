#ifndef RESPONSE_H
    #define RESPONSE_H
#endif



#ifndef UTILIS_H
    #include "utilis.hpp"
#endif

#include <cstring>
#include <map>

namespace rspn {
    bool getResponsePath(char* request_path, std::map<std::string, std::string> vars, int* statusCode, char* response_path, const int response_path_size, char* response_content_type, const int response_content_type_size, bool* binary_file)
    {
        if(!strcmp(request_path, "/"))
        {
            strncpy(response_path, "frontend/pages/home.html", response_path_size);
            strncpy(response_content_type, "text/html", response_content_type_size);
            *binary_file = true;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/favicon.ico"))
        {
            strncpy(response_path, "frontend/images/favicon.ico", response_path_size);
            strncpy(response_content_type, "image/x-icon", response_content_type_size);
            *binary_file = true;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/stylesheet"))
        {
            auto it = vars.find("filename");
            if(it == vars.end()) return false;
            const char* const_filename = (it->second).c_str();
            char filename[strlen(const_filename)+8];
            snprintf(filename, sizeof(filename), "%s", const_filename);

            char* index = strstr(filename, "..");
            if(index) {
                strncpy(response_path, "frontend/errors/403.html", response_path_size);
                strncpy(response_content_type, "text/html", response_content_type_size);
                *binary_file = false;
                *statusCode = 403;

                return false;
            }

            char style_path[20+strlen(filename)];
            snprintf(style_path, sizeof(style_path), "frontend/styles/%s", filename);

            strncpy(response_path, style_path, response_path_size);
            strncpy(response_content_type, "text/css", response_content_type_size);

            *binary_file = false;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/script"))
        {
            auto it = vars.find("filename");
            if(it == vars.end()) return false;
            const char* const_filename = (it->second).c_str();
            char filename[strlen(const_filename)+8];
            snprintf(filename, sizeof(filename), "%s", const_filename);

            char* index = strstr(filename, "..");
            if(index) {
                strncpy(response_path, "frontend/errors/403.html", response_path_size);
                strncpy(response_content_type, "text/html", response_content_type_size);
                *binary_file = false;
                *statusCode = 403;

                return false;
            }

            char style_path[20+strlen(filename)];
            snprintf(style_path, sizeof(style_path), "frontend/scripts/%s", filename);

            strncpy(response_path, style_path, response_path_size);
            strncpy(response_content_type, "text/javascript", response_content_type_size);

            *binary_file = false;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/image"))
        {
            auto it = vars.find("filename");
            if(it == vars.end()) return false;
            const char* const_filename = (it->second).c_str();
            char filename[strlen(const_filename)+8];
            snprintf(filename, sizeof(filename), "%s", const_filename);

            char* index = strstr(filename, "..");
            if(index) {
                strncpy(response_path, "frontend/errors/403.html", response_path_size);
                strncpy(response_content_type, "text/html", response_content_type_size);
                *binary_file = false;
                *statusCode = 403;

                return false;
            }

            char style_path[20+strlen(filename)];
            snprintf(style_path, sizeof(style_path), "frontend/images/%s", filename);

            strncpy(response_path, style_path, response_path_size);
            strncpy(response_content_type, "image/*", response_content_type_size);

            *binary_file = false;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/component"))
        {
            auto it = vars.find("filename");
            if(it == vars.end()) return false;
            const char* const_filename = (it->second).c_str();
            char filename[strlen(const_filename)+8];
            snprintf(filename, sizeof(filename), "%s", const_filename);

            char* index = strstr(filename, "..");
            if(index) {
                strncpy(response_path, "frontend/errors/403.html", response_path_size);
                strncpy(response_content_type, "text/html", response_content_type_size);
                *binary_file = false;
                *statusCode = 403;

                return false;
            }

            char style_path[20+strlen(filename)];
            snprintf(style_path, sizeof(style_path), "frontend/components/menu-bar.html");

            strncpy(response_path, style_path, response_path_size);
            strncpy(response_content_type, "text/html", response_content_type_size);

            *binary_file = false;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/login"))
        {
            strncpy(response_path, "frontend/pages/login.html", response_path_size);
            strncpy(response_content_type, "text/html", response_content_type_size);
            *binary_file = false;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/punteggi"))
        {
            strncpy(response_path, "frontend/pages/punteggi.html", response_path_size);
            strncpy(response_content_type, "text/html", response_content_type_size);
            *binary_file = false;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/regole"))
        {
            strncpy(response_path, "frontend/pages/regole.html", response_path_size);
            strncpy(response_content_type, "text/html", response_content_type_size);
            *binary_file = false;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/info"))
        {
            strncpy(response_path, "frontend/pages/info.html", response_path_size);
            strncpy(response_content_type, "text/html", response_content_type_size);
            *binary_file = false;
            *statusCode = 200;
        }
        else if(!strcmp(request_path, "/loaderio-b8dc8aec6e95bc53b4b87ad0820b4901.txt"))
        {
            strncpy(response_path, "loaderio-b8dc8aec6e95bc53b4b87ad0820b4901.txt", response_path_size);
            strncpy(response_content_type, "text/plain", response_content_type_size);
            *binary_file = false;
            *statusCode = 200;
        }

        return true;
    }

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
        memcpy(response + strLen, content, content_length);
        response[responseLen] = '\0';

        return response;
    }
}