#include <iostream>
#include "classes/ThreadPool.hpp"
#include "utilis/request.hpp"
#include <SFML/Network.hpp>
#include <fstream>
#include <sstream>

int main(void) {
    sf::TcpListener listener;
    if(listener.listen(6203) != sf::Socket::Status::Done) {
        std::cout << "Error binding the lintener to the port\n";
        return 1;
    }
    std::cout << "Successfully binded the listener to the port\n\n\n";

    while(true) {
        sf::TcpSocket client;
        if(listener.accept(client) == sf::Socket::Status::Done) {
            char buffer[1024];
            std::size_t received;

            if(client.receive(buffer, sizeof(buffer), received) == sf::Socket::Status::Done) {
                //std::cout << "--------------- REQUEST ---------------\n\n" << buffer << "\n\n\n";


                
                std::cout << "--------------- INFO ---------------\n\n";

                char path[512];
                std::cout << "Request type: " << rqst::getTypeName(rqst::getType(buffer)) << "\n";
                std::cout << "Request path: " << rqst::getPath(buffer, path, sizeof(path)) << "\n";
                std::cout << "Request variables:\n";
                    auto vars = rqst::getArgs(buffer);
                    if(vars.empty()) std::cout << "No vars\n";
                    else for (const auto& var : vars) std::cout << " - " << var.first << " = " << var.second << "\n";

                std::cout << "\n------------------------------------\n\n\n";

                std::ifstream file("templates/index.html");

                std::ostringstream contentStream;
                contentStream << file.rdbuf();
                std::string content = contentStream.str();

                std::ostringstream responseStream;
                responseStream << "HTTP/1.1 200 OK\r\n"
                            << "Content-Type: text/html\r\n"
                            << "Content-Length: " << content.size() << "\r\n"
                            << "\r\n"
                            << content;

                sf::Socket::Status status = client.send(responseStream.str().c_str(), responseStream.str().size());
                //std::cout << "--------------- RESPONSE ---------------\n\n" << responseStream.str() << "\n\n\n";
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