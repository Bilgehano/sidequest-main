#include <iostream>

#include <server_application.h>

int main() {
    std::cout << "Sidequest Server " << std::endl;

    Sidequest::Server::ServerApplication application("localhost", 80);
    application.run();

    return 0;
}
