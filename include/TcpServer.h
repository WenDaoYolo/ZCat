#include"BusinessModule.h"
#include"DataAnalysis.h"
#include<netinet/tcp.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include"MemoryPool.h"
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<signal.h>
#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<thread>
#pragma once

#define CS_EVENTS_LEN 8192
#define ONLINE_USERS 8192

class TcpServer
{
    public:
        bool stop;
        int epolls;
        int listen_fd;
        DataBase user_db;
        sockaddr_in my_sa;
        online_client_info oci[ONLINE_USERS];     //fd with  array index,just index source id?
        epoll_event c_events[CS_EVENTS_LEN];
        FixedSizePool user_info_pool;
        FixedSizePool group_info_pool;
        FixedSizePool ug_info_pool;
        FixedSizePool unicast_msg_pool;
        FixedSizePool multicast_msg_pool;


        TcpServer(int port,int listen_len);
        ~TcpServer();

        void SetNonBlocking(int fd);
        void DisableNagle(int fd);              //just for query,must not really disable
        void SetNonSignal();

        void StartUp();
        void AdminPanel();

        void Rule(int fd);
        void Regist(int fd);
        void Login(int fd);
        void Apply(int fd);
        void Exit(int fd);
    
        int FindTarget(int des_id);       //0 is not online
        void DeleteFriend(int fd);
        void UniCast(int fd);
        void MultiCast(int fd);

        void SendApplyBuffer(int fd);
        void SendFriendBuffer(int fd);
        void SendGroupBuffer(int fd);
        void SendUnicastMsgBuffer(int fd);
        void SendMulticastMsgBuffer(int fd);
        void SendGroupApplyBuffer(int fd);

        void PermitApply(int fd);
        void RefuseApply(int fd);
        void PermitGroupApply(int fd);
        void RefuseGroupApply(int fd);

        void CreateGroup(int fd);
        void GroupApply(int fd);
        void ViewGroupMembers(int fd);

        void ExitGroup(int fd);
        void RemoveGroupMember(int fd);

        void UpdatePasswd(int fd);
};
