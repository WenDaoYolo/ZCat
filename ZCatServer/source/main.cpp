#include "../include/TcpServer.h"


int LoadPort()
{   
    std::fstream f1;
    char port_str[128]={0};
    f1.open("./CONFIG.txt",std::ios::in);
    
    if(f1.is_open())
    {
        f1.getline(port_str,128,'\n');
        memset(port_str,0,sizeof(port_str));

        f1.getline(port_str,128,'\n');
        f1.close();
    }
    return std::stoi(port_str);
}

int main()
{
    TcpServer tcps1(LoadPort(),6);

    std::thread AdminPanel([&](void){tcps1.AdminPanel();});
    tcps1.StartUp();
    AdminPanel.join();

    return 0;
}
