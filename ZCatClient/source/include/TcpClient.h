#pragma comment("lib","ws2_32.lib")
#include<winsock2.h>
#include<windows.h>
#include<iostream>
#pragma once

struct user_info              
{                             
    int id;                   
    char admin[16];           
    char passwd[16];          
    char name[31];            
    char padding[5];
};
                            
struct unicast_msg
{
    int sou_id;
    int des_id;
    char times[33];
    char msg[1025];
    char padding[6];
};

struct group_info
{
	int id;
	char name[31];
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
    int group_id;
	char member_name[31];
	char times[33];
	char msg[1025];
    char padding[7];
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

struct friend_node
{
    user_info data;
    friend_node* last;
    friend_node* next;
};

struct friend_list
{
    friend_node head;
    int len;
};

struct group_node
{
    group_info data;
    group_node* last;
    group_node* next;
};

struct group_list
{
    group_node head;
    int len;
};

struct friend_apply_node
{
    user_info data;
    friend_apply_node* last;
    friend_apply_node* next;
};

struct friend_apply_list
{
    friend_apply_node head;
    int len;
};

struct group_apply_node
{
    ug_info data;
    group_apply_node* last;
    group_apply_node* next;
};

struct group_apply_list
{
    group_apply_node head;
    int len;
};

struct group_member_node
{
    user_info data;
    group_member_node* last;
    group_member_node* next;
};

struct group_member_list
{
    group_member_node head;
    int len;
};

struct unicast_node
{
    unicast_msg data;
    unicast_node* last;
    unicast_node* next;
};

struct unicast_list
{
    unicast_node head;
    unicast_node* tail;
    int len;
};

struct multicast_node
{
    multicast_msg data;
    multicast_node* last;
    multicast_node* next;
};

struct multicast_list
{
    multicast_node head;
    multicast_node* tail;
    int len;
};

//class type
class TcpClient{
    private:
        int socket_fd;
        sockaddr_in target_sa;
    public:
        friend_list f_list;
        group_list g_list;
        friend_apply_list fa_list;
        group_apply_list ga_list;
        group_member_list gm_list;

        unicast_list u_list;
        multicast_list m_list;

        int GetSocket_fd();
        TcpClient(const char* ip,short port);
        void SetIp(const char* ip);
        void SetPORT(short port);
        bool Connect();
        ~TcpClient();
        
        void InsertFriendList(user_info* e);
        void DeleteFriendList(user_info* e);
        void DestroyFriendList();

        void InsertGroupMemberList(user_info* e);
        void DeleteGroupMemberList(user_info* e);
        void DestroyGroupMemberList();

        void InsertGroupList(group_info* e);
        void DeleteGroupList(group_info* e);
        void DestroyGroupList();

        void InsertFriendApplyList(user_info* e);
        void DestroyFriendApplyList(); 
        void DeleteFriendApplyList(friend_apply_node* e);

        void InsertGroupApplyList(ug_info* e);
        void DestroyGroupApplyList();
        void DeleteGroupApplyList(group_apply_node* e);

        //tail insert promise order
        void InsertUnicastList(unicast_msg* e);
        void DestroyUnicastList();
        void DeleteUnicastList(unicast_node* e);//temp disable

        void InsertMulticastList(multicast_msg* e);
        void DestroyMulticastList();
        void DeleteMulticastList(multicast_node* e);//temp disable

        void get_data_rule();
};