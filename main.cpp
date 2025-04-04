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

int main(int argc, char** argv) {
    std::cout << "\r\n\r\n\r\n";

    int port = 6203;
    if(argc > 1) port = atoi(argv[1]);

    sf::TcpListener listener;
    if(listener.listen(port) != sf::Socket::Status::Done) {
        std::cout << "Error binding the lintener to the port\r\n";
        return 1;
    }
    std::cout << "Successfully binded the listener to the port " << port << "\r\n\r\n\r\n";

    ThreadPool tasks(20);

    while(true) {
        sf::TcpSocket* client_ptr = new sf::TcpSocket;
        if (listener.accept(*client_ptr) == sf::Socket::Status::Done) {
            tasks.enqueue([client_ptr] {
                char buffer[1024];
                std::size_t received;

                if(client_ptr->receive(buffer, sizeof(buffer), received) == sf::Socket::Status::Done) {
                    // Get the path and the method of the request
                    char request_path[512];
                    rqst::getPath(buffer, request_path, sizeof(request_path));

                    rqst::RequestMethod request_method = rqst::getMethod(buffer);

                    // Print the request
                    auto vars = rqst::printRequest(buffer, client_ptr);

                    // Get the path and the information of the response
                    bool state = true;
                    char response_path[512] = "frontend/pages/home.html";
                    char response_content_type[512] = "text/html";
                    char status[10] = "OK";
                    bool binary_file = false;

                    if(request_method == rqst::RequestMethods::Get)
                        state = rspn::getResponsePath(
                            request_path, vars,
                            response_path, sizeof(response_path),
                            response_content_type, sizeof(response_content_type),
                            &binary_file
                        );
                    else if(request_method == rqst::RequestMethods::Post) {}
                    else return;
                    if(!state) return;


                    // Get the requested file
                    int file_length = fileSize(response_path);
                    if(file_length == -1) return;
                    char content[file_length+1];
                    readFile(response_path, content, sizeof(content));

                    // Build the response
                    int response_length = rspn::length(200, status, response_content_type, content, file_length);
                    char response[response_length];
                    rspn::build(response, response_length, 200, status, response_content_type, content, file_length);
                    std::cout << "--------------- ALMOST --------------- " << response_path << " --- " << request_path << "\r\n\r\n";

                    // Log the response
                    if(client_ptr->send(response, response_length) == sf::Socket::Status::Done)
                    {
                        std::cout << "--------------- RESPONSE ---------------\r\n\r\n";
                        
                        std::cout.write(response, response_length - file_length - 1);
                        if(binary_file) std::cout << "[[[ BINARY FILE OF " << file_length << " BYTES ]]]";
                        else if(file_length > MAX_FILE_DISPLAYED) std::cout.write(content, MAX_FILE_DISPLAYED);
                        else std::cout.write(content, file_length);

                        std::cout << "\r\n\r\n------------------------------------\r\n\r\n\r\n\r\n";
                    }
                    else std::cout << "Error sending the response\r\n\r\n\r\n";
                }
            });
        }
    }
}