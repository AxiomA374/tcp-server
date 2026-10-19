#pragma once
#include <atomic>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

class Server {
private:
    uint16_t port{8080};            // Стандартный порт, если не задан явно
    int server_fd{-1};              // Сокет еще не создан системой
    void client_handler(int client_fd);

    bool debug_mode{false};

    // Массив потоков
    std::vector<std::thread> threads {};

    const size_t max_clients{64};
    std::atomic<uint16_t> active_clients{0};                                   // Не более 65535 подключений одновременно
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
