#include <iostream>
#include "include/Server.hpp"
using namespace std;

int main() {
    Server server(8080);
    cout << "Запуск TCP сервера\n";
    if (!server.start()) {
        cout << "Не удалось запустить сервер\n";
        return 1;
    }
    cout << "Сервер запущен\n";
    return 0;
}
