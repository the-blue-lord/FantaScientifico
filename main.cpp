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

    while(true) {
        sf::TcpSocket client;
        if(listener.accept(client) == sf::Socket::Status::Done) {
            char buffer[1024];
            std::size_t received;

            if(client.receive(buffer, sizeof(buffer), received) == sf::Socket::Status::Done) {
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

                std::cout << "\r\n------------------------------------\r\n\r\n\r\n";

                char response_path[512];
                char response_content_type[512];

                if(request_type == rqst::RequestTypes::Get)
                {
                    if(!strcmp(request_path, "/favicon.ico"))
                    {
                        strncpy(response_path, "images/favicon.ico", sizeof(response_path));
                        strncpy(response_content_type, "image/png", sizeof(response_content_type));
                    }
                    else if(!strcmp(request_path, "/"))
                    {
                        strncpy(response_path, "templates/test.html", sizeof(response_path));
                        strncpy(response_content_type, "text/html", sizeof(response_content_type));
                    }
                }
                else if(request_type == rqst::RequestTypes::Post)
                {
                    // do smth
                }

                int file_length = fileSize(response_path);
                if(file_length == -1) continue;
                char content[file_length];
                readFile(response_path, content, sizeof(content));

                int response_length = rspn::length(200, "ERROR", response_content_type, content, sizeof(content));
                char response[response_length];
                rspn::build(response, response_length, 200, "ERROR", response_content_type, content, sizeof(content));

                if(client.send(response, response_length) == sf::Socket::Status::Done)
                {
                    std::cout << "--------------- RESPONSE ---------------\r\n\r\n";
                    std::cout.write(response, sizeof(response) > MAX_FILE_DISPLAYED ? MAX_FILE_DISPLAYED : sizeof(response));
                    if(sizeof(response) > MAX_FILE_DISPLAYED) std::cout << " [[[ MORE CONTENT ]]]";
                    std::cout << "\r\n\r\n------------------------------------\r\n\r\n\r\n";
                }
            }
        }
    }

    ThreadPool tasks(2);

    /*

    wait for a request
    run the reuqest
    delay 10ms

    */
}

/*

manage request (request)


*/