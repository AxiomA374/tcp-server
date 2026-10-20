#pragma once
#include <cstdint>
#include <iostream>
#include <sys/select.h>

class Server {
private:
    uint16_t port {8080};            // Стандартный порт, если не задан явно
    int server_fd {-1};              // Сокет еще не создан системой
    bool client_handler(int client_fd);

    bool debug_mode {false};

    size_t current_clients {0};
    const size_t max_clients {64};

    fd_set master_set {};            // Битовая маска для пользовательских дескрипторов
    int max_fd {-1};                 // Верхняя граница дескрипторов по умолчанию (if (!user) = -1)
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
