#include <iostream>
#include <SFML/Network.hpp>

#ifndef THREADPOOL_H
    #include "classes/ThreadPool.hpp"
#endif

#ifndef UTILIS_H
    #include "utilis/utilis.hpp"
#endif

#ifndef REQUEST_H
    #include "utilis/request.hpp"
#endif

#ifndef RESPONSE_H
    #include "utilis/response.hpp"
#endif

#define MAX_FILE_DISPLAYED 2048

int main(void) {
    sf::TcpListener listener;
    if(listener.listen(6203) != sf::Socket::Status::Done) {
        std::cout << "Error binding the lintener to the port\r\n";
        return 1;
    }
    std::cout << "Successfully binded the listener to the port\r\n\r\n\r\n";

    ThreadPool tasks(15);

    while(true) {
        sf::TcpSocket* client_ptr = new sf::TcpSocket;
        if (listener.accept(*client_ptr) == sf::Socket::Status::Done) {
            tasks.enqueue([client_ptr]() {
                char buffer[1024];
                std::size_t received;

                if(client_ptr->receive(buffer, sizeof(buffer), received) == sf::Socket::Status::Done) {
                    std::cout << "gggg\n";
                    std::cout << "--------------- REQUEST ---------------\r\n\r\n";
                    
                    rqst::RequestType request_type = rqst::getType(buffer);
                    const char* request_type_name = rqst::getTypeName(request_type);

                    char request_path[512];
                    rqst::getPath(buffer, request_path, sizeof(request_path));


                    std::cout << "Request type: " << request_type_name << "\r\n"
                        << "Request path: " << request_path << "\r\n"
                        << "Request variables:";

                    auto vars = rqst::getArgs(buffer);

                    if(vars.empty()) std::cout << " No vars\r\n";
                    else
                    {
                        std::cout << "\r\n";
                        for (const auto& var : vars) std::cout << " - " << var.first << " = " << var.second << "\r\n";
                    }

                    std::cout << "\r\n------------------------------------\r\n\r\n\r\n\r\n";

                    char response_path[512] = "frontend/pages/home.html";
                    char response_content_type[512] = "text/html";
                    char status[10] = "OK";
                    bool binary_file = false;

                    if(request_type == rqst::RequestTypes::Get)
                    {
                        if(!strcmp(request_path, "/favicon.ico"))
                        {
                            strncpy(response_path, "frontend/images/favicon.ico", sizeof(response_path));
                            strncpy(response_content_type, "image/png", sizeof(response_content_type));
                            binary_file = true;
                        }
                        else if(!strcmp(request_path, "/stylesheet"))
                        {
                            auto it = vars.find("filename");
                            if(it == vars.end())
                            {
                                delete client_ptr;
                                return;
                            }
                            const char* filename = (it->second).c_str();

                            char* index = strstr(request_path, "..");
                            if(index)
                            {
                                delete client_ptr;
                                return;
                            }

                            char style_path[20+strlen(filename)];
                            snprintf(style_path, sizeof(style_path), "frontend/styles/%s", filename);

                            strncpy(response_path, style_path, sizeof(response_path));
                            strncpy(response_content_type, "text/css", sizeof(response_content_type));
                        }
                        else if(!strcmp(request_path, "/script"))
                        {
                            auto it = vars.find("filename");
                            if(it == vars.end())
                            {
                                delete client_ptr;
                                return;
                            }
                            const char* filename = (it->second).c_str();

                            char* index = strstr(request_path, "..");
                            if(index)
                            {
                                delete client_ptr;
                                return;
                            }

                            char style_path[20+strlen(filename)];
                            snprintf(style_path, sizeof(style_path), "frontend/scripts/%s", filename);

                            strncpy(response_path, style_path, sizeof(response_path));
                            strncpy(response_content_type, "text/javascript", sizeof(response_content_type));
                        }
                        else if(!strcmp(request_path, "/login"))
                        {
                            strncpy(response_path, "frontend/pages/login.html", sizeof(response_path));
                            strncpy(response_content_type, "text/html", sizeof(response_content_type));
                            binary_file = true;
                        }
                        else if(!strcmp(request_path, "/punteggi"))
                        {
                            strncpy(response_path, "frontend/pages/punteggi.html", sizeof(response_path));
                            strncpy(response_content_type, "text/html", sizeof(response_content_type));
                            binary_file = true;
                        }
                        else if(!strcmp(request_path, "/regole"))
                        {
                            strncpy(response_path, "frontend/pages/regole.html", sizeof(response_path));
                            strncpy(response_content_type, "text/html", sizeof(response_content_type));
                            binary_file = true;
                        }
                        else if(!strcmp(request_path, "/info"))
                        {
                            strncpy(response_path, "frontend/pages/info.html", sizeof(response_path));
                            strncpy(response_content_type, "text/html", sizeof(response_content_type));
                            binary_file = true;
                        }
                        
                    }
                    else if(request_type == rqst::RequestTypes::Post)
                    {
                        // do smth
                    }

                    
                    int file_length = fileSize(response_path);
                    if(file_length == -1)
                    {
                        delete client_ptr;
                        return;
                    }
                    char content[file_length+1];
                    readFile(response_path, content, sizeof(content));

                    int response_length = rspn::length(200, status, response_content_type, content, file_length);
                    char response[response_length];
                    rspn::build(response, response_length, 200, status, response_content_type, content, file_length);

                    if(client_ptr->send(response, response_length) == sf::Socket::Status::Done)
                    {
                        std::cout << "--------------- RESPONSE ---------------\r\n\r\n";
                        
                        std::cout.write(response, response_length - file_length - 1);
                        if(binary_file) std::cout << "[[[ BINARY FILE OF " << file_length << " BYTES ]]]";
                        else if(file_length > MAX_FILE_DISPLAYED) std::cout.write(content, MAX_FILE_DISPLAYED);
                        else std::cout.write(content, file_length);

                        std::cout << "\r\n\r\n------------------------------------\r\n\r\n\r\n\r\n";
                    }
                }

                delete client_ptr;
                return;
            });
        }
        else delete client_ptr;
    }

    /*

    wait for a request
    run the reuqest
    delay 10ms

    */
}

/*

manage request (request)


*/