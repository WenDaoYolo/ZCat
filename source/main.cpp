#include "../include/TcpServer.h"

int main()
{
    TcpServer tcps1(1026,6);

    std::thread AdminPanel([&](void){tcps1.AdminPanel();});
    tcps1.StartUp();
    AdminPanel.join();

    return 0;
}