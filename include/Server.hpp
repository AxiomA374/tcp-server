#pragma once
#include <cstdint>

class Server {
private:
    uint16_t port{8080};            // Стандартный порт, если не задан явно
    int server_fd{-1};              // Сокет еще не создан системой
    void client_handler(int client_fd);
public:
    explicit Server(uint16_t port = 8080);
    ~Server();                      // Деструктор
    bool start();                   // Запуск
};
