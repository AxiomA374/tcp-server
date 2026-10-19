#include <iostream>
#include <string_view>
#include "include/Server.hpp"

int main(int argc, char* argv[]) {
    bool enable_debug = false;

    for (int i = 1; i < argc; i++) {
        if (std::string_view (argv[i]) == "--debug") {
            enable_debug = true;
            break;
        }
    }

    Server server(8080, enable_debug);
    std::cout << "Запуск TCP сервера\n";
    if (!server.start()) {
        std::cout << "Не удалось запустить сервер\n";
        return 1;
    }
    return 0;
}
