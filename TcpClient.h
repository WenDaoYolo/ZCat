#pragma comment("lib","ws2_32.lib")
#include<winsock2.h>
#include<iostream>
#pragma once

struct user_info              //客户端从服务器上动态加载获取相关的配置信息，如数据长度限制等
{                             //与服务器对接的关键点，数据包的大小和格式必须与服务器保持一致
    int id;                   //check length and repassword
    char admin[16];           //apply need target id -> load from server's friend buffer
    char passwd[16];          //合并查询  -> friend buffer + user_info admin (sou_id==id)
    char name[46];            //server send msg to client and insert into db(apply,msg)
};                            //是否需要实时更新消息?    ->   need msg type 1.apply 2.msg 

struct unicast_msg
{
    int sou_id;
    int des_id;
    char times[33];
    char msg[1025];
};

struct group_info
{
	int id;
	char name[46];
	char admin[16];
	int manager_id;
};

struct ug_info{
    user_info ui;
    group_info gi;
};

struct multicast_msg
{
	int member_id;
	char member_name[46];
	int group_id;
	char times[33];
	char msg[1025];
};

struct data_rule
{
    int admin_len;
    int password_len;
    int name_len;
    int message_len;
    int group_admin_len;
    int group_name_len;
    int group_nums;
};

struct error_code
{
    int value;
};

class TcpClient{
    private:
        int socket_fd;
        struct sockaddr_in target_sa;
    public:
        int GetSocket_fd();
        TcpClient(const char* ip,short port);
        bool Connect();
        ~TcpClient();
};