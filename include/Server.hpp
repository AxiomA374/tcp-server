#pragma once
#include <cstdint>
#include <iostream>

class Server {
private:
    uint16_t port{8080};            // Стандартный порт, если не задан явно
    int server_fd{-1};              // Сокет еще не создан системой
    bool debug_mode{false};
    void client_handler(int client_fd);
public:
    explicit Server(
        uint16_t port = 8080,
        bool debug_mode = false
    );
    ~Server();                      // Деструктор
    bool start();                   // Запуск

    // Функция логирования
    template <typename... Args>
    void log(Args&&... args) const {
        if (debug_mode) {
            std::cout <<"[DEBUG]: ";
            (std::cout << ... << args) << "\n";
        }
    }

    // Защита от копирования класса
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
};
