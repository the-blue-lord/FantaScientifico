#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <SFML/Network.hpp>

class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;

public:
    ThreadPool(size_t numThreads) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread & worker : workers) worker.join();
    }
};

void handleRequest(sf::TcpSocket* client) {
    char data[1024];
    std::size_t received;
    if (client->receive(data, sizeof(data), received) == sf::Socket::Done) {
        std::cout << "Received: " << std::string(data, received) << "\n";
    }

    std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
    client->send(response.c_str(), response.size());

    client->disconnect();
    delete client;
}

int main() {
    sf::TcpListener listener;
    if (listener.listen(8080) != sf::Socket::Done) {
        std::cerr << "Failed to bind port!\n";
        return -1;
    }

    ThreadPool pool(4);  // 4 worker threads

    while (true) {
        sf::TcpSocket* client = new sf::TcpSocket;
        if (listener.accept(*client) == sf::Socket::Done) {
            pool.enqueue([client] { handleRequest(client); });
        } else {
            delete client;
        }
    }

    return 0;
}
