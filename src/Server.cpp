#include "Server.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <cerrno>
#include <cstring>

Server::Server(uint16_t port, bool debug) : port{port} , debug_mode(debug){}

Server::~Server() {
    if (server_fd >= 0) {
        close(server_fd);                       // Системный вызов Linux, закрывающий файловый дескриптор
    }
}

bool Server::start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cout << "Ошибка создания сокета\n";
        return false;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cout << "Предупреждение: не удалось выставить SO_REUSEADDR\n";
    }

    sockaddr_in server_addr {};                 // Обнуление структуры
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bind_res = bind(server_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
    if (bind_res < 0) {
        std::cout << "Ошибка привязки сокета к порту: " << strerror(errno) << "\n";
        return false;
    }

    int listen_res = listen(server_fd, 8);
    if (listen_res < 0) {
        std::cout << "Ошибка перевода сокета в режим прослушивания\n";
        return false;
    }

    std::cout << "Сервер слушает на " << port << " порту\n";

    while (true) {
        sockaddr_in client_addr {};
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
        if (client_fd < 0) {
            std::cout << "Ошибка accept: " << strerror(errno) << "\n";
            continue;
        }
        client_handler(client_fd);
        close(client_fd);
    };
    return true;
}
