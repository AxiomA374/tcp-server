#include "Server.hpp"
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>

void Server::client_handler(int client_fd) {
    char buffer[1024] {};
    while (true) {
        ssize_t read_client = read(client_fd, buffer, sizeof(buffer) - 1);
        if (read_client > 0) {
            log("Клиент открыл соединение\n");
            buffer[read_client] = '\0';
            std::cout << "Получено сообщение: " << buffer << "\n";
        } else if (read_client == 0) {
            log("Клиент закрыл соединение\n");
            break;
        } else {
            if (errno == EINTR) {                                                    // Обработка системного прерывания
                continue;
            }
            log("Ошибка чтения: ", strerror(errno), "\n");
            break;
        }
    }
}
