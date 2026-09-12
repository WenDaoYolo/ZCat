#include "BusinessModule.h"

bool CheckValidData(const char* str)    //check format and character code
{
    const char* c=str;
    for(int i=0;i<strlen(str);i++)
    {
        if((*c>='0'&&*c<='9')||(*c>='a'&&*c<='z')||(*c>='A'&&*c<='Z')
        ||(*c=='_')||(*c=='@')||(*c=='#')||(*c=='&')||(*c=='*'))
        {
            c++;
            continue;
        }
        return false;
    }
    return true;
}

error_code RegistModule(user_info& u1,online_client_info& o1)
{   
    error_code ec={0};
    DataBase db1(DEF_STR);

    if(!CheckValidData(u1.admin))
    {
        ec.value=1;
        return ec;
    }

    if(db1.FindUser(u1.admin,NULL))
    {
        ec.value=2;
        return ec;
    }

    if(!CheckValidData(u1.password))
    {
        ec.value=3;
        return ec;
    }
    
    if(ec.value==0)
    {
        strcpy(o1.admin,u1.admin);
        db1.InsertUser(u1.admin,u1.password,u1.name);
        db1.InsertLog(o1.ip,o1.admin,REGIST_LOG);
    }
        
    return ec;
}

error_code LoginModule(user_info& u1,online_client_info& o1,int isonline)
{
    error_code ec={0};
    DataBase db1(DEF_STR);
    user_info user1;

    if(!CheckValidData(u1.admin))
    {
        ec.value=1;
        return ec;
    }

    if(!CheckValidData(u1.password))
    {
        ec.value=3;
        return ec;
    }

    if(!db1.FindUser(u1.admin,&user1))
    {
        ec.value=4;
        return ec;
    }
    else
    {
        if(strcmp(u1.password,user1.password))
        {
            ec.value=5;
            return ec;
        }
    }

    if(db1.IsBlackUser(u1.admin))
    {
        ec.value=-1;
        return ec;
    }

    if(isonline>0)
    {
        ec.value=8;
        return ec;
    }

    strcpy(o1.admin,user1.admin);
    strcpy(o1.name,user1.name);
    o1.id=user1.id;
    db1.InsertLog(o1.ip,o1.admin,LOGIN_LOG);

    return ec;
}

error_code ApplyModule(int source_id,user_info& u1)
{
    error_code ec={0};
    DataBase db1(DEF_STR);
    user_info user1;

    if(!CheckValidData(u1.admin))
    {
        ec.value=1;
        return ec;
    }

    if(!db1.FindUser(u1.admin,&user1))
    {
        ec.value=4;
        return ec;
    }
    u1.id=user1.id;

    if(db1.FindFriend(source_id,u1.id))
    {
        ec.value=9;
        return ec;
    }

    if(!db1.InsertApply(source_id,u1.id))
    {
        ec.value=6;
        return ec;
    }

    return ec;
}

int GetApplyBuffer_len(const char* admin)
{
    user_info temp;
    DataBase db1(DEF_STR);
    db1.FindUser(admin,&temp);
    return db1.SearchApplyBuffer(temp.id);
}

error_code GetApplyBufferModule(user_info** ptr,const char* admin)
{
    error_code ec={0};

    user_info temp;
    DataBase db1(DEF_STR);
    db1.FindUser(admin,&temp);
    user_info** find=ptr;
    db1.DownLoadApplyBuffer(find,temp.id);

    return ec;
}

int GetFriendBuffer_len(const char* admin)
{
    user_info temp;
    DataBase db1(DEF_STR);
    db1.FindUser(admin,&temp);
    return db1.SearchFriendBuffer(temp.id);
}

error_code GetFriendBufferModule(user_info** ptr,const char* admin)
{
    error_code ec={0};

    user_info temp;
    DataBase db1(DEF_STR);
    db1.FindUser(admin,&temp);
    user_info** find=ptr;
    db1.DownLoadFriendBuffer(find,temp.id);

    return ec;
}

int GetUnicastMsgBuffer_len(int des_id)
{
    DataBase db1(DEF_STR);
    return db1.SearchUnicastMsgBuffer(des_id);;
}

error_code GetUnicastMsgBufferModule(unicast_msg** ptr,int id)
{
    error_code ec={0};
    DataBase db1(DEF_STR);
    unicast_msg** find=ptr;
    db1.DownLoadUnicastMsgBuffer(find,id);
    return ec;
};

int GetMulticastMsgBuffer_len(int group_id)
{
    DataBase db1(DEF_STR);
    return db1.SearchMulticastMsgBuffer(group_id);;
}

error_code GetMulticastMsgBufferModule(multicast_msg** ptr,int group_id)
{
    error_code ec={0};
    DataBase db1(DEF_STR);
    multicast_msg** find=ptr;
    db1.DownLoadMulticastMsgBuffer(find,group_id);
    return ec;
}

int GetGroupBuffer_len(int id)
{
    DataBase db1(DEF_STR);
    return db1.SearchGroupBuffer(id);
}

error_code GetGroupBufferModule(group_info** ptr,int id)
{
    error_code ec={0};
    DataBase db1(DEF_STR);
    group_info** find=ptr;
    db1.DownLoadGroupBuffer(find,id);
    return ec;
}

int GetGroupApplyBuffer_len(int m_id)
{
    DataBase db1(DEF_STR);
    return db1.SearchGroupApplyBuffer(m_id);
}

error_code GetGroupApplyBufferModule(ug_info** ptr,int m_d)
{
    error_code ec={0};
    DataBase db1(DEF_STR);
    ug_info** find=ptr;
    db1.DownLoadGroupApplyBuffer(find,m_d);
    return ec;
}

int GetGroupMembers_len(int group_id)
{
    DataBase db1(DEF_STR);
    return db1.SearchGroupMembers(group_id);
}

error_code ViewGroupMembersModule(user_info** ptr,int group_id)
{
    error_code ec={0};
    DataBase db1(DEF_STR);
    user_info** find=ptr;
    db1.DownLoadGroupMembers(find,group_id);
    return ec;
}

error_code PermitApplyModule(int sou_id,int des_id)
{
    DataBase db1(DEF_STR);
    error_code ec={9};
    
    db1.RemoveApply(sou_id,des_id);
    db1.RemoveApply(des_id,sou_id);

    if(!db1.FindFriend(sou_id,des_id))
    {
        db1.InsertFriend(sou_id,des_id);
        ec.value=0;
        return ec;
    }
    return ec;
}

void RefuseApplyModule(int sou_id,int des_id)
{
    DataBase db1(DEF_STR);
    db1.RemoveApply(sou_id,des_id);
}

void DeleteFriendModule(int sou_id,int des_id)
{
    DataBase db1(DEF_STR);
    db1.DeleteFriend(sou_id,des_id);
}

void UnicastMsgModule(unicast_msg& um1)
{
    DataBase db1(DEF_STR);
    db1.InsertUnicastMsg(&um1);
}

void MulticastMsgModule(multicast_msg& mm1)
{
    DataBase db1(DEF_STR);
    db1.InsertMulticastMsg(&mm1);
}

error_code CreateGroupModule(group_info& gi1)
{
    DataBase db1(DEF_STR);
    error_code ec={0};

    if(!CheckValidData(gi1.admin))
    {
        ec.value=1;
        return ec;
    }

    if(db1.FindGroup(gi1.admin,NULL))
    {
        ec.value=2;
        return ec;
    }

    if(db1.CountGroupNums(gi1.manager_id)>=MAX_GROUPNUMS)
    {
        ec.value=10;
        return ec;
    }

    db1.InsertGroup(gi1);
    db1.FindGroup(gi1.admin,&gi1);
    db1.InsertGroupMember(gi1.manager_id,gi1.id,gi1.manager_id);
    return ec;
}

error_code GroupApplyModule(int source_id,group_info& gi1)
{
    DataBase db1(DEF_STR);
    error_code ec={0};

    if(!CheckValidData(gi1.admin))
    {
        ec.value=1;
        return ec;    
    }

    if(!db1.FindGroup(gi1.admin,&gi1))
    {
        ec.value=4;
        return ec;
    }

    if(db1.FindGroupMember(source_id,gi1.id))
    {
        ec.value=11;
        return ec;
    }

    if(!db1.InsertGroupApply(source_id,gi1.id,gi1.manager_id))
    {
        ec.value=6;
        return ec;
    }
    return ec;
}

error_code PermitGroupApplyModule(ug_info& ug)
{
    DataBase db1(DEF_STR);
    error_code ec={11};
    db1.RemoveGroupApply(ug.ui.id,ug.gi.id,ug.gi.manager_id);

    if(!db1.FindGroupMember(ug.ui.id,ug.gi.id))
    {
        db1.InsertGroupMember(ug.ui.id,ug.gi.id,ug.gi.manager_id);
        ec.value=0;
        return ec;
    }
    return ec;
}

void RefuseGroupApplyModule(ug_info& ug)
{
    DataBase db1(DEF_STR);
    db1.RemoveGroupApply(ug.ui.id,ug.gi.id,ug.gi.manager_id);
}

void ExitGroupModule(ug_info& ug)
{
    DataBase db1(DEF_STR);
    db1.RemoveGroupMember(ug.ui.id,ug.gi.id,ug.gi.manager_id);
}

void RemoveGroupMemberModule(ug_info& ug)
{
    DataBase db1(DEF_STR);
    db1.RemoveGroupMember(ug.ui.id,ug.gi.id,ug.gi.manager_id);
}

int GetGroupMemberNumModule(int group_id)
{
    DataBase db1(DEF_STR);
    return db1.FindGroupMemberNum(group_id);
}   

void GetGroupMemberIDModule(int* ptr,int group_id)
{
    DataBase db1(DEF_STR);
    int* find=ptr;
    db1.GetGroupMemberID(ptr,group_id);
}

error_code UpdatePasswdModule(user_info& u1,online_client_info& o1)
{   
    error_code ec={0};
    DataBase db1(DEF_STR);

    if(!CheckValidData(u1.password))
    {
        ec.value=3;
        return ec;
    }
    
    if(ec.value==0)
    {
        strcpy(o1.admin,u1.admin);
        db1.UpdatePasswd(u1.id,u1.password);
        db1.InsertLog(o1.ip,o1.admin,UPDATE_INFO);
    }
        
    return ec;
}