#include<sys/socket.h>
#include<iostream>
#include<unistd.h>
#pragma once

#define ADMIN_MAX_LEN 15
#define PASSWD_MAX_LEN 15
#define NAME_MAX_LEN 30
#define MESSAGE_MAX_LEN 1024
#define TIMES_MAX_LEN 32
#define IPV4_LEN 16

#define GROUP_ADMIN_MAX_LEN 15
#define GROUP_NAME_MAX_LEN 30
#define MAX_GROUPNUMS 10

#define REGIST_LOG "regist"
#define LOGIN_LOG "login"
#define EXIT_LOG "exit"
#define UPDATE_INFO "update info"
#pragma once

struct user_info
{
    int id;  //if none staff 0
    char admin[ADMIN_MAX_LEN+1];
    char password[PASSWD_MAX_LEN+1];
    char name[NAME_MAX_LEN+1];
    char padding[5];
};

struct unicast_msg
{
    int sou_id;
    int des_id;
    char times[TIMES_MAX_LEN+1];
    char msg[MESSAGE_MAX_LEN+1];
    char padding[6];
};

struct online_client_info         //just for connect client's status,not admin
{
    int id;                       //1 admin is login    0 amdin is exit  
    int status;                   //1 client is oline   0 client unonline
    char ip[IPV4_LEN];
    char admin[ADMIN_MAX_LEN+1];  //16
    char name[NAME_MAX_LEN+1];    //31
};

struct data_rule
{
    int admin;
    int password;
    int name;
    int message;
    int group_admin;
    int group_name;
    int group_nums;
};

struct group_info
{
	int id;
	char name[GROUP_NAME_MAX_LEN+1];
	char admin[GROUP_ADMIN_MAX_LEN+1];
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
	char member_name[NAME_MAX_LEN+1];
	char times[TIMES_MAX_LEN+1];
	char msg[MESSAGE_MAX_LEN+1];
    char padding[7];
};

struct error_code
{
    int value;
    /*
        0.no error(success)
        1.admin format is error
        2.admin is alreadly exists
        3.passowrd format is error
        4.admin is not exists
        5.password is error
        6.is already excute
        7.buffer is empty
        8.admin is already login
        9.is already friend
        10.create group nums is too many
    */
};

void GetDataRule(data_rule& dr);
bool RegistMsg(int fd,user_info& u1);
bool LoginMsg(int fd,user_info& u1);
bool ApplyMsg(int fd,user_info& u1);

bool PermitApplyMsg(int fd,user_info& u1);
bool RefuseApplyMsg(int fd,user_info& u1);
bool PermitGroupApplyMsg(int fd,ug_info& ug);
bool RefuseGroupApplyMsg(int fd,ug_info& ug);

bool DeleteFriendMsg(int fd,user_info& u1);
bool UnicastMsg(int fd,unicast_msg& um1);
bool MulticastMsg(int fd,multicast_msg& mm1);
bool MulticastBufferMsg(int fd,group_info& gi1);

bool CreateGroupMsg(int fd,group_info& gi1);
bool GroupApplyMsg(int fd,group_info& gi1);
bool ViewGroupMembersMsg(int fd,group_info& gi1);

bool ExitGroupMsg(int fd,ug_info& ug);
bool RemoveGroupMemberMsg(int fd,ug_info& ug);

bool UpdatePasswdMsg(int fd,user_info& u1);