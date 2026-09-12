#include "../include/DataBase.h"

DataBase::DataBase(const std::string& connect_str):c1(connect_str)
{
    if(!c1.is_open())
        std::cout<<"database connect error"<<std::endl;
}

bool DataBase::FindUser(const std::string& admin,user_info* u_info)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select * from user_info where admin=$1",admin);
    if(!r1.empty())
    {
        if(u_info!=NULL)
        {
            u_info->id=r1[0]["id"].as<int>();
            strcpy(u_info->admin,r1[0]["admin"].as<const char*>());
            strcpy(u_info->password,r1[0]["password"].as<const char*>());
            strcpy(u_info->name,r1[0]["name"].as<const char*>());
        }
        return true;
    }
    
    return false;
}

void DataBase::InsertUser(const char* admin,const char* passwd,const char* name)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec("select max(id) from user_info");

    int id=1;
    if(!(r1[0][0].is_null()))
        id=r1[0][0].as<int>()+1;     
    
    w1.exec_params
    (
        "insert into user_info (id,admin,password,name) values($1,$2,$3,$4)",
        id,admin,passwd,name
    );

    w1.commit();
}

void DataBase::InsertLog(const char* ip,const char* admin,const char* active)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec("select max(id) from active_log");

    int id=1;
    if(!(r1[0][0].is_null()))
        id=r1[0][0].as<int>()+1; 
    
    w1.exec_params
    (
        "insert into active_log (id,ip,times,admin,active) values($1,$2,current_timestamp,$3,$4)",
        id,ip,admin,active
    );
    w1.commit();
}

bool DataBase::InsertApply(int sou_id,int des_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from apply_buffer where sou_id=$1 and des_id=$2",
        sou_id,des_id
    );
    
    if(r1.size()!=0)
        return false;

    w1.exec_params("insert into apply_buffer (sou_id,des_id) values($1,$2)",sou_id,des_id);
    w1.commit();
    return true;
}

bool DataBase::InsertGroupApply(int sou_id,int group_id,int manager_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from group_apply_buffer where sou_id=$1 and group_id=$2",
        sou_id,group_id
    );

    if(r1.size()!=0)
        return false;
    
    w1.exec_params
    ("insert into group_apply_buffer (sou_id,group_id,manager_id) values($1,$2,$3)",
        sou_id,group_id,manager_id
    );
    w1.commit();
    return true;    
}

void DataBase::InsertGroupMember(int sou_id,int group_id,int manager_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "insert into group_buffer (member_id,group_id,manager_id) values($1,$2,$3)",
        sou_id,group_id,manager_id
    );
    w1.commit();
}

void DataBase::RemoveApply(int sou_id,int des_id)
{
    pqxx::work w1(c1);
    w1.exec_params("delete from apply_buffer where sou_id=$1 and des_id=$2",sou_id,des_id);
    w1.commit();
}   

void DataBase::RemoveGroupApply(int sou_id,int group_id,int manager_id)
{
    pqxx::work w1(c1);
    w1.exec_params
    (
        "delete from group_apply_buffer where sou_id=$1 and group_id=$2 and manager_id=$3",
        sou_id,group_id,manager_id
    );
    w1.commit();
}

int DataBase::SearchApplyBuffer(int id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select sou_id,admin,name from user_info,apply_buffer where sou_id=id and des_id=$1",
        id
    );
    return r1.size();
}

void DataBase::DownLoadApplyBuffer(user_info** ptr,int id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select sou_id,admin,name from user_info,apply_buffer where sou_id=id and des_id=$1",
        id
    );

    for(int i=0;i<r1.size();i++)
    {
        (*ptr)->id=r1[i]["sou_id"].as<int>();
        strcpy((*ptr)->admin,r1[i]["admin"].as<const char*>());
        strcpy((*ptr)->name,r1[i]["name"].as<const char*>());
        ptr++;
    }
}

int DataBase::SearchFriendBuffer(int id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from friend_buffer where sou_id=$1 or des_id=$1",
        id
    );
    return r1.size();
}

void DataBase::DownLoadFriendBuffer(user_info** ptr,int id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select sou_id,admin,name from user_info,friend_buffer where"
        " sou_id=id and (sou_id!=$1 and des_id=$1) union all"
        " select des_id,admin,name from user_info,friend_buffer"
        " where des_id=id and (sou_id=$1 and des_id!=$1)",
        id
    );

    for(int i=0;i<r1.size();i++)
    {
        (*ptr)->id=r1[i]["sou_id"].as<int>();
        strcpy((*ptr)->admin,r1[i]["admin"].as<const char*>());
        strcpy((*ptr)->name,r1[i]["name"].as<const char*>());
        ptr++;
    }
}

int DataBase::SearchUnicastMsgBuffer(int id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from unicast_msg_buffer where sou_id=$1 or des_id=$1",
        id
    );
    return r1.size();
}

void DataBase::DownLoadUnicastMsgBuffer(unicast_msg** ptr,int id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from unicast_msg_buffer where sou_id=$1 or des_id=$1",
        id
    );

    for(int i=0;i<r1.size();i++)
    {
        (*ptr)->sou_id=r1[i]["sou_id"].as<int>();
        (*ptr)->des_id=r1[i]["des_id"].as<int>();
        strcpy((*ptr)->times,r1[i]["times"].as<const char*>());
        strcpy((*ptr)->msg,r1[i]["msg"].as<const char*>());

        ptr++;
    }
}

int DataBase::SearchMulticastMsgBuffer(int group_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from multicast_msg_buffer where group_id=$1",
        group_id
    );
    return r1.size();
}

void DataBase::DownLoadMulticastMsgBuffer(multicast_msg** ptr,int group_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from multicast_msg_buffer where group_id=$1",
        group_id
    );

    for(int i=0;i<r1.size();i++)
    {
        (*ptr)->member_id=r1[i]["member_id"].as<int>();
        (*ptr)->group_id=r1[i]["group_id"].as<int>();
        strcpy((*ptr)->member_name,r1[i]["member_name"].as<const char*>());
        strcpy((*ptr)->times,r1[i]["times"].as<const char*>());
        strcpy((*ptr)->msg,r1[i]["msg"].as<const char*>());

        ptr++;
    }
}

int DataBase::SearchGroupBuffer(int id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from group_buffer where member_id=$1",
        id
    );
    return r1.size();
}

void DataBase::DownLoadGroupBuffer(group_info** ptr,int id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select name,admin,group_info.manager_id,id from group_info,group_buffer "
        "where member_id=$1 and group_id=id",
        id
    );

    for(int i=0;i<r1.size();i++)
    {
        (*ptr)->id=r1[i]["id"].as<int>();
        (*ptr)->manager_id=r1[i]["manager_id"].as<int>();
        strcpy((*ptr)->name,r1[i]["name"].as<const char*>());
        strcpy((*ptr)->admin,r1[i]["admin"].as<const char*>());
        ptr++;
    }
}

int DataBase::SearchGroupApplyBuffer(int m_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select * from group_apply_buffer where manager_id=$1",m_id);
    return r1.size();
}

void DataBase::DownLoadGroupApplyBuffer(ug_info** ptr,int m_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select ui.id as uid,ui.admin as uad,ui.name as uname,"
        "gi.id as gid,gi.admin as gad,gi.name as gname,gi.manager_id " 
        "from user_info as ui,group_info as gi,group_apply_buffer as gab "
        "where ui.id=gab.sou_id and gi.id=gab.group_id and gab.manager_id=$1",
        m_id
    );

    for(int i=0;i<r1.size();i++)
    {
        (*ptr)->ui.id=r1[i]["uid"].as<int>();
        (*ptr)->gi.id=r1[i]["gid"].as<int>();
        (*ptr)->gi.manager_id=r1[i]["manager_id"].as<int>();

        strcpy((*ptr)->ui.admin,r1[i]["uad"].as<const char*>());
        strcpy((*ptr)->ui.name,r1[i]["uname"].as<const char*>());
        strcpy((*ptr)->gi.admin,r1[i]["gad"].as<const char*>());
        strcpy((*ptr)->gi.name,r1[i]["gname"].as<const char*>());
        ptr++;
    }
}

int DataBase::SearchGroupMembers(int group_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select * from group_buffer where group_id=$1",group_id);
    return r1.size();
}

void DataBase::DownLoadGroupMembers(user_info** ptr,int group_id)
{   
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select ui.id,ui.admin,ui.name from user_info as ui,group_buffer as gb "
        "where gb.group_id=$1 and gb.member_id=ui.id",
        group_id
    );
    
    for(int i=0;i<r1.size();i++)
    {
        (*ptr)->id=r1[i]["id"].as<int>();
        strcpy((*ptr)->admin,r1[i]["admin"].as<const char*>());
        strcpy((*ptr)->name,r1[i]["name"].as<const char*>());
        ptr++;
    }
}

bool DataBase::FindFriend(int sou_id,int des_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from friend_buffer where (sou_id=$1 and des_id=$2) or (sou_id=$3 and des_id=$4)",
        sou_id,
        des_id,
        des_id,
        sou_id
    );

    if(r1.size()>0)
        return true;
    return false;
}

void DataBase::InsertFriend(int sou_id,int des_id)
{
    pqxx::work w1(c1);
    w1.exec_params("insert into friend_buffer (sou_id,des_id) values($1,$2)",sou_id,des_id);
    w1.commit();
}

void DataBase::DeleteFriend(int sou_id,int des_id)
{
    pqxx::work w1(c1);
    w1.exec_params
    (
        "delete from friend_buffer where (sou_id=$1 and des_id=$2) or (sou_id=$3 and des_id=$4)",
        sou_id,
        des_id,
        des_id,
        sou_id
    );
    w1.commit();
}

void DataBase::InsertUnicastMsg(unicast_msg* um1)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec("select localtimestamp(0)");
    strcpy(um1->times,r1[0][0].as<const char*>());

    w1.exec_params
    (
        "insert into unicast_msg_buffer (sou_id,des_id,times,msg) values($1,$2,$3,$4)",
        um1->sou_id,
        um1->des_id,
        um1->times,
        um1->msg
    );

    w1.commit();
}

void DataBase::InsertMulticastMsg(multicast_msg* mm1)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec("select localtimestamp(0)");
    strcpy(mm1->times,r1[0][0].as<const char*>());

    w1.exec_params
    (
        "insert into multicast_msg_buffer (member_id,member_name,group_id,msg,times) values($1,$2,$3,$4,$5)",
        mm1->member_id,mm1->member_name,mm1->group_id,mm1->msg,mm1->times
    );

    w1.commit();
}

bool DataBase::FindGroup(const char* admin,group_info* gi1)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select * from group_info where admin=$1",admin);
    if(!r1.empty())
    {
        if(gi1!=NULL)
        {
            strcpy(gi1->admin,r1[0]["admin"].as<const char*>());
            strcpy(gi1->name,r1[0]["name"].as<const char*>());
            gi1->id=r1[0]["id"].as<int>();
            gi1->manager_id=r1[0]["manager_id"].as<int>();
        }
        return true;
    }
    return false;
}

void DataBase::InsertGroup(group_info& gi1)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select max(id) from group_info");
    
    int id=1;
    if(!r1[0][0].is_null())
        id=r1[0][0].as<int>()+1;
    
    r1=w1.exec_params
    (
        "insert into group_info (id,name,admin,manager_id) values($1,$2,$3,$4)",
        id,
        gi1.name,
        gi1.admin,
        gi1.manager_id
    );
    w1.commit();
}

int DataBase::CountGroupNums(int id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select count(id) from group_info where manager_id=$1",id);
    return r1[0][0].as<int>();
}

bool DataBase::FindGroupMember(int sou_id,int group_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from group_buffer where member_id=$1 and group_id=$2",
        sou_id,group_id
    );

    if(r1.size()!=0)
        return true;
    return false;
}

void DataBase::RemoveGroupMember(int sou_id,int group_id,int manager_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "delete from group_buffer where member_id=$1 and group_id=$2 and manager_id =$3",
        sou_id,group_id,manager_id
    );
    w1.commit();
}

int DataBase::FindGroupMemberNum(int group_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select * from group_buffer where group_id=$1",group_id
    );
    return r1.size();
}

void DataBase::GetGroupMemberID(int* ptr,int group_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params
    (
        "select member_id from group_buffer where group_id=$1",group_id
    );

    for(int i=0;i<r1.size();i++)
    {
        *ptr=r1[i]["member_id"].as<int>();
        ptr++;
    }
}

void DataBase::View_All_Users()
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select * from user_info");

    std::cout
    <<std::left<<std::setw(10)<<"ID"
    <<std::left<<std::setw(15)<<"ADMIN"
    <<std::left<<std::setw(30)<<"NAME"//mabye have chinese,so should in the back
    <<std::endl;

    for(int i=0;i<r1.size();i++)
    {

        std::cout
        <<std::left<<std::setw(10)<<r1[i]["id"].as<int>()
        <<std::left<<std::setw(15)<<r1[i]["admin"].as<const char*>()
        <<std::left<<std::setw(30)<<r1[i]["name"].as<const char*>()
        <<std::endl;
    }
    std::cout<<"user nums:"<<r1.size()<<std::endl;
}   

bool DataBase::IsBlackUser(const char* admin)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select id from user_info where admin=$1",admin);
    pqxx::result r2=w1.exec_params("select * from black_user_info where id=$1",r1[0]["id"].as<int>());

    if(r2.size()>0)
        return true;
    return false;
}

bool DataBase::InsertBlackList(int id)
{
    pqxx::work w1(c1);
    w1.exec_params("insert into black_user_info (id) values($1)",id);
    w1.commit();
    return true;
}

bool DataBase::RemoveBlackList(int id)
{
    pqxx::work w1(c1);
    w1.exec_params("delete from black_user_info where id=$1",id);
    w1.commit();
    return true;
}

void DataBase::ViewBlackList()
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec("select * from black_user_info");

    std::cout<<"ID"<<std::endl;
    for(int i=0;i<r1.size();i++)
    {
        std::cout<<r1[i]["id"].as<int>()<<std::endl;
    }
    std::cout<<"black user nums:"<<r1.size()<<std::endl;
}

void DataBase::ViewActiveLog()
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select * from active_log");

    std::cout
    <<std::left<<std::setw(10)<<"ID"
    <<std::left<<std::setw(30)<<"IP"
    <<std::left<<std::setw(30)<<"TIMES"
    <<std::left<<std::setw(20)<<"ADMIN"
    <<std::left<<std::setw(10)<<"ACTIVE"
    <<std::endl;

    for(int i=0;i<r1.size();i++)
    {

        std::cout
        <<std::left<<std::setw(10)<<r1[i]["id"].as<int>()
        <<std::left<<std::setw(30)<<r1[i]["ip"].as<const char*>()
        <<std::left<<std::setw(30)<<r1[i]["times"].as<const char*>()
        <<std::left<<std::setw(20)<<r1[i]["admin"].as<const char*>()
        <<std::left<<std::setw(10)<<r1[i]["active"].as<const char*>()
        <<std::endl;
    }
    std::cout<<"log nums:"<<r1.size()<<std::endl;
}

void DataBase::ClearActiveLog()
{
    pqxx::work w1(c1);
    w1.exec_params("delete from active_log");
    w1.commit();
}

bool DataBase::DeleteOneActiveLog(int act_id)
{
    pqxx::work w1(c1);
    pqxx::result r1=w1.exec_params("select * from active_log where id=$1",act_id);

    if(r1.size()==0)
    {
        return false;
    }
    
    w1.exec_params("delete from active_log where id=$1",act_id);
    w1.commit();
    return true;
}

void DataBase::UpdatePasswd(int id,const char* passwd)
{
    pqxx::work w1(c1);
    w1.exec_params("update user_info set password=$1 where id=$2",passwd,id);
    w1.commit();
}

DataBase::~DataBase()
{
    ;
}