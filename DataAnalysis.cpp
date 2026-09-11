#include "DataAnalysis.h"


void GetDataRule(data_rule& dr)
{
    dr.admin=ADMIN_MAX_LEN;
    dr.password=PASSWD_MAX_LEN;
    dr.name=NAME_MAX_LEN;
    dr.message=MESSAGE_MAX_LEN;
    dr.group_admin=GROUP_ADMIN_MAX_LEN;
    dr.group_name=GROUP_NAME_MAX_LEN;
    dr.group_nums=MAX_GROUPNUMS;
}

bool RegistMsg(int fd,user_info& u1)
{
    int r_total=0,t_total=sizeof(u1);
    char* position_ptr=(char*)&u1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool LoginMsg(int fd,user_info& u1)
{
    int r_total=0,t_total=sizeof(u1);
    char* position_ptr=(char*)&u1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool ApplyMsg(int fd,user_info& u1)
{
    int r_total=0,t_total=sizeof(u1);
    char* position_ptr=(char*)&u1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool PermitApplyMsg(int fd,user_info& u1)
{
    int r_total=0,t_total=sizeof(u1);
    char* position_ptr=(char*)&u1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool RefuseApplyMsg(int fd,user_info& u1)
{
    int r_total=0,t_total=sizeof(u1);
    char* position_ptr=(char*)&u1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool DeleteFriendMsg(int fd,user_info& u1)
{
    int r_total=0,t_total=sizeof(u1);
    char* position_ptr=(char*)&u1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool UnicastMsg(int fd,unicast_msg& um1)
{
    int r_total=0,t_total=sizeof(um1);
    char* position_ptr=(char*)&um1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool MulticastMsg(int fd,multicast_msg& mm1)
{
    int r_total=0,t_total=sizeof(mm1);
    char* position_ptr=(char*)&mm1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool MulticastBufferMsg(int fd,group_info& gi1)
{
    int r_total=0,t_total=sizeof(gi1);
    char* position_ptr=(char*)&gi1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool CreateGroupMsg(int fd,group_info& gi1)
{
    int r_total=0,t_total=sizeof(gi1);
    char* position_ptr=(char*)&gi1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool GroupApplyMsg(int fd,group_info& gi1)
{
    int r_total=0,t_total=sizeof(gi1);
    char* position_ptr=(char*)&gi1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool ViewGroupMembersMsg(int fd,group_info& gi1)
{
    int r_total=0,t_total=sizeof(gi1);
    char* position_ptr=(char*)&gi1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool PermitGroupApplyMsg(int fd,ug_info& ug)
{
    int r_total=0,t_total=sizeof(ug);
    char* position_ptr=(char*)&ug;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool RefuseGroupApplyMsg(int fd,ug_info& ug)
{
    int r_total=0,t_total=sizeof(ug);
    char* position_ptr=(char*)&ug;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool ExitGroupMsg(int fd,ug_info& ug)
{
    int r_total=0,t_total=sizeof(ug);
    char* position_ptr=(char*)&ug;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool RemoveGroupMemberMsg(int fd,ug_info& ug)
{
    int r_total=0,t_total=sizeof(ug);
    char* position_ptr=(char*)&ug;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}

bool UpdatePasswdMsg(int fd,user_info& u1)
{
    int r_total=0,t_total=sizeof(u1);
    char* position_ptr=(char*)&u1;
    while(r_total<t_total)
    {
        int recv_once=recv(fd,position_ptr,t_total-r_total,MSG_DONTWAIT);
        if(recv_once>0)
        {
            r_total+=recv_once;
            position_ptr+=recv_once;
            continue;
        }
        if(recv_once==0||(recv_once<0&&errno!=EAGAIN))  
        {
            if(r_total<t_total)          //data not all
                return false;
        }
    }
    return true;
}