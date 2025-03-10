#include <iostream>
#include "classes/ThreadPool.hpp"
#include <SFML/Network.hpp>
#include <fstream>

int main(void) {
    std::cout << "hello\n";
    sf::TcpListener listener;
    std::cout << "world\n";
    if(listener.listen(6203) != sf::Socket::Status::Done) {
        std::cout << "Error binding the lintener to the port\n";
        return 1;
    }
    std::cout << "Successfully binded the listener to the port\n";

    while(true) {
        sf::TcpSocket client;
        if(listener.accept(client) == sf::Socket::Status::Done) {
            char buffer[1024];
            std::size_t received;

            if(client.receive(buffer, sizeof(buffer), received) == sf::Socket::Status::Done) {
                std::string request(buffer, received);
                std::cout << "New request:\n\n" << request << "\n\n\n\n\n";

                /*
                std::ifstream file();
                if (!file) {
                    return "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/plain\r\n"
                        "Content-Length: 13\r\n"
                        "\r\n"
                        "404 Not Found";
                }

                std::ostringstream contentStream;
                contentStream << file.rdbuf();
                std::string content = contentStream.str();

                std::ostringstream responseStream;
                responseStream << "HTTP/1.1 200 OK\r\n"
                            << "Content-Type: text/html\r\n"
                            << "Content-Length: " << content.size() << "\r\n"
                            << "\r\n"
                            << content;

                return responseStream.str();*/
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