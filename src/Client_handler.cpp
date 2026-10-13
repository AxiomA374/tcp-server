#include "Server.hpp"
#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <cstring>
using namespace std;

void Server::client_handler(int client_fd) {
    char buffer[1024] {};
    while (true) {
        ssize_t read_client = read(client_fd, buffer, sizeof(buffer) - 1);
        if (read_client > 0) {
            buffer[read_client] = '\0';
            cout << "Получено сообщение: " << buffer << "\n";
        } else if (read_client == 0) {
            cout << "Клиент закрыл соединение\n";
            break;
        } else {
            cout << "Ошибка чтения: " << strerror(errno) << "\n";
            break;
        }
    }
}
