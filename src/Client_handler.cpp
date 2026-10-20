#include "Server.hpp"
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>

bool Server::client_handler(int client_fd) {
    char buffer[1024] {};
    ssize_t read_client = read(client_fd, buffer, sizeof(buffer) - 1);

    if (read_client > 0) {
        log("Клиент с дескриптором ", client_fd, " прислал:\n");
        buffer[read_client] = '\0';
        std::cout << "Получено сообщение: " << buffer << "\n";
        return true;
    } else if (read_client == 0) {
        log("Клиент с дескриптором ", client_fd," закрыл соединение\n");
        close(client_fd);
        return false;
    } else {
        if (errno == EINTR) return true;                                   // Обработка системного прерывания
        log("Ошибка чтения: ", strerror(errno), "\n");
        close(client_fd);
        return false;
    }
    return true;
}
