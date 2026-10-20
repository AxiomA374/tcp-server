#include "Server.hpp"
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>

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

    // Перевод сокета из блок. режима в неблок.
    int server_flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, server_flags | O_NONBLOCK);
    FD_ZERO(&master_set);                               // Убедились что все 1024 бита выключены
    FD_SET(server_fd, &master_set);                     // Регистрация сокета
    max_fd = server_fd;                                 // Задали верхнюю границу сокета

    while (true) {
        sockaddr_in client_addr {};
        socklen_t client_len = sizeof(client_addr);

        fd_set working_set = master_set;
        int activity = select(max_fd+1, &working_set, nullptr, nullptr, nullptr);    // (if (activity < 0) то системная ошибка)
        if (activity < 0) {
            if (errno == EINTR) {
                continue;
            }
            std::cout << "Ошибка select: " << strerror(errno) << "\n";
            break;
        }

        for (int fd = 0; fd <= max_fd; fd++) {
            if (FD_ISSET(fd, &working_set)) {
                if (fd == server_fd) {                  // Новый клиент
                    int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);

                    if (client_fd < 0) {
                        std::cout << "Ошибка accept: " << strerror(errno) << "\n";
                        continue;
                    }

                    if (current_clients >= max_clients) {
                        std::cout << "Превышено количество дескрипторов!\n";
                        close(client_fd);
                        continue;
                    }

                    current_clients++;

                    // Клиентский сокет не блокирующий
                    int client_flags = fcntl(client_fd, F_GETFL, 0);
                    fcntl(client_fd, F_SETFL, client_flags | O_NONBLOCK);

                    // Эталонная маска
                    FD_SET(client_fd, &master_set);
                    if (max_fd < client_fd) max_fd = client_fd;
                } else {                                // Старый клиент
                    if (!client_handler(fd)) {
                        FD_CLR(fd, &master_set);               // Заходим в client_handler
                        current_clients--;
                    }
                }
            }
        }
    }
    return true;
}
