#include "TcpClient.h"

TcpClient::TcpClient(const char* ip,short port)
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2),&wsadata);
    socket_fd=socket(PF_INET,SOCK_STREAM,0);
    if(socket_fd==INVALID_SOCKET) perror("create socket");
    target_sa.sin_addr.S_un.S_addr=inet_addr(ip);
    target_sa.sin_family=AF_INET;
    target_sa.sin_port=htons(port);
}

bool TcpClient::Connect()
{
    int adjust=connect(socket_fd,(sockaddr*)&(target_sa),sizeof(target_sa));
    if(adjust==SOCKET_ERROR)
    {
        std::cout<<"error code:"<<WSAGetLastError()<<std::endl;
        return false;
    }
    return true;
}   

TcpClient::~TcpClient()
{
    closesocket(socket_fd);
    WSACleanup();
}

