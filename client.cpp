#include "TcpClient.h"
#include <windows.h>
#include<iomanip>
#include<cstring>
#include<thread>


enum status                             
{
    EXIT,
    LEVEL_0,
    LEVEL_1,
    LEVEL_2,
};//有限状态机 

TcpClient tcpc1("192.168.232.140",1026);
status CUR_STATUS=LEVEL_0;
data_rule dr;
char name_buffer[1024]={0};
char admin_buffer[1024]={0};
char password_buffer[1024]={0};
char repassword_buffer[1024]={0};
char group_name_buffer[1024]={0};
char group_admin_buffer[1024]={0};

user_info cur_user;

void PrintMenu()
{
    std::cout<<"--------------------------------------"<<std::endl;
    std::cout<<">               ZCat                  "<<std::endl;
    std::cout<<">                                     "<<std::endl;
    std::cout<<">              1.注册                 "<<std::endl;
    std::cout<<">              2.登录                 "<<std::endl;
    std::cout<<">              3.退出                 "<<std::endl;
    std::cout<<"--------------------------------------"<<std::endl;
}

void PrintIndex()
{
    std::cout<<"--------------------------------------"<<std::endl;
    std::cout<<">              Index                  "<<std::endl;
    std::cout<<">                                     "<<std::endl;
    std::cout<<">            0.好友列表                "<<std::endl;
    std::cout<<">            1.添加好友                "<<std::endl;
    std::cout<<">            2.申请列表                "<<std::endl;
    std::cout<<">            3.退出登录                "<<std::endl;
    std::cout<<">            4.退出程序                "<<std::endl;
    std::cout<<">            5.聊天记录                "<<std::endl;
    std::cout<<">            6.创建群聊                "<<std::endl;
    std::cout<<">            7.加入群聊                "<<std::endl;
    std::cout<<">            8.群聊列表                "<<std::endl;
    std::cout<<">            9.群聊通知                "<<std::endl;
    std::cout<<">           10.查看群成员              "<<std::endl;
    std::cout<<"--------------------------------------"<<std::endl;
}

bool RevalueCheck(error_code ec)
{
    switch(ec.value)
    {
        case 0:
            return true;
        case 1:
            std::cout<<"账号错误!允许的字符：a-z、A-Z、0-9、_、@、#、*、&"<<std::endl;
            break;
        case 2:
            std::cout<<"账号已存在!"<<std::endl;
            break;
        case 3:
            std::cout<<"密码错误!允许的字符：a-z、A-Z、0-9、_、@、#、*、&"<<std::endl;
            break;
        case 4:
            std::cout<<"账号不存在!"<<std::endl;
            break;
        case 5:
            std::cout<<"密码错误!"<<std::endl;
            break;
        case 6:
            std::cout<<"已发送过申请!"<<std::endl;
            break;
        case 7:
            //缓存为空
            break;
        case 8:
            std::cout<<"账号正在使用中!"<<std::endl;
            break;
        case 9:
            std::cout<<"对方已经是好友!"<<std::endl;
            break;
        case 10:
            std::cout<<"创建的群聊太多!"<<std::endl;
            break;
        case 11:
            std::cout<<"已在群聊中!"<<std::endl;
            break;
        default:
            //
            break;
    }
    return false;
}

void regist_test()
{
    memset(admin_buffer,0,sizeof(name_buffer));
    memset(admin_buffer,0,sizeof(admin_buffer));
    memset(admin_buffer,0,sizeof(password_buffer));
    memset(admin_buffer,0,sizeof(repassword_buffer));
    
    std::cout<<"请输入账号>";
    std::cin>>admin_buffer;
    if(strlen(admin_buffer)>dr.admin_len||strlen(admin_buffer)<1)
    {
        std::cout<<"长度错误，为"<<"1~"<<dr.admin_len<<"之间"<<std::endl;
        return;
    }

    std::cout<<"请输入昵称>";
    std::cin>>name_buffer;
    if(strlen(admin_buffer)>dr.name_len||strlen(admin_buffer)<1)
    {
        std::cout<<"长度错误，为"<<"1~"<<dr.name_len<<"之间"<<std::endl;
        return;
    }

    std::cout<<"请输入密码>";
    std::cin>>password_buffer;
    if(strlen(admin_buffer)>dr.password_len||strlen(admin_buffer)<1)
    {
        std::cout<<"长度错误，为"<<"1~"<<dr.password_len<<"之间"<<std::endl;
        return;
    }

    std::cout<<"再次确认密码>";
    std::cin>>repassword_buffer;
    if(strcmp(password_buffer,repassword_buffer))
    {
        std::cout<<"密码不一致"<<std::endl;
        return;
    }
    
    user_info u1;
    error_code ec;
    u1.id=-1;
    strcpy(u1.name,name_buffer);
    strcpy(u1.admin,admin_buffer);
    strcpy(u1.passwd,password_buffer);

    int type=1;
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u1,sizeof(u1),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(u1),0);
    if(RevalueCheck(ec))
    {
        std::cout<<"注册成功!"<<std::endl;
    }
}

void login_test()
{
    memset(admin_buffer,0,sizeof(admin_buffer));
    memset(admin_buffer,0,sizeof(password_buffer));

    std::cout<<"请输入账号>";
    std::cin>>admin_buffer;
    if(strlen(admin_buffer)>dr.admin_len||strlen(admin_buffer)<1)
    {
        std::cout<<"长度错误，为"<<"1~"<<dr.admin_len<<"之间"<<std::endl;
        return;
    }

    std::cout<<"请输入密码>";
    std::cin>>password_buffer;
    if(strlen(admin_buffer)>dr.password_len||strlen(admin_buffer)<1)
    {
        std::cout<<"长度错误，为"<<"1~"<<dr.password_len<<"之间"<<std::endl;
        return;
    }

    user_info u1;
    error_code ec;
    u1.id=-1;
    strcpy(u1.name,"NULL");
    strcpy(u1.admin,admin_buffer);
    strcpy(u1.passwd,password_buffer);

    int type=2;
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u1,sizeof(u1),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(u1),0);

    if(RevalueCheck(ec))
    {
        CUR_STATUS=LEVEL_1;
        std::cout<<"登录成功!"<<std::endl;
        recv(tcpc1.GetSocket_fd(),(char*)&cur_user,sizeof(cur_user),0);
    }
}

void apply_test()
{
    memset(admin_buffer,0,sizeof(name_buffer));

    std::cout<<"请输入账号>";
    std::cin>>admin_buffer;
    if(strlen(admin_buffer)>dr.admin_len||strlen(admin_buffer)<1)
    {
        std::cout<<"长度错误，为"<<"1~"<<dr.admin_len<<"之间"<<std::endl;
        return;
    }

    if(!strcmp(admin_buffer,cur_user.admin))
    {
        std::cout<<"目标为当前账号!"<<std::endl;
        return;
    }

    user_info u1;
    error_code ec;
    u1.id=-1;
    strcpy(u1.admin,admin_buffer);
    strcpy(u1.name,"NULL");
    strcpy(u1.passwd,"NULL");

    int type=3;
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u1,sizeof(u1),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(u1),0);
    if(RevalueCheck(ec))
    {
        std::cout<<"已发送申请!"<<std::endl;
    }
}

void download_apply_list()
{
    int type=10;
    error_code ec;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(RevalueCheck(ec))
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);

        user_info* ptr=new user_info[size];
        char* pos_ptr=(char*)ptr;
        total_target=size*sizeof(user_info);
        while(recv_total<total_target)
        {
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,size*sizeof(user_info),0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }
        
        user_info* look=ptr;
        std::cout<<"--------------------------------------"<<std::endl;
        std::cout<<"             好友申请列表              "<<std::endl<<std::endl;
        for(int i=0;i<size;i++)
        {
            std::cout
            <<std::left<<std::setw(dr.admin_len+2)<<look->admin
            <<std::left<<std::setw(dr.name_len+2)<<look->name
            <<std::endl;
            look++;
        }
        std::cout<<"--------------------------------------"<<std::endl;
        delete[] ptr;
    }
}

void download_friend_list()
{
    int type=11;
    error_code ec;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(RevalueCheck(ec))
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);

        user_info* ptr=new user_info[size];
        char* pos_ptr=(char*)ptr;
        total_target=size*sizeof(user_info);
        while(recv_total<total_target)
        {
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,size*sizeof(user_info),0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }
        
        user_info* look=ptr;
        std::cout<<"--------------------------------------"<<std::endl;
        std::cout<<"               好友列表                "<<std::endl<<std::endl;
        for(int i=0;i<size;i++)
        {
            std::cout
            <<std::left<<std::setw(dr.admin_len+2)<<look->admin
            <<std::left<<std::setw(dr.name_len+2)<<look->name
            <<std::endl;
            look++;
        }
        std::cout<<"--------------------------------------"<<std::endl;
        delete[] ptr;
    }
}

void download_unicast_list()
{
    int type=12;
    error_code ec;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(RevalueCheck(ec))
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);

        unicast_msg* ptr=new unicast_msg[size];
        char* pos_ptr=(char*)ptr;
        total_target=size*sizeof(unicast_msg);
        while(recv_total<total_target)
        {
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,size*sizeof(unicast_msg),0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }

        unicast_msg* look=ptr;
        std::cout<<"--------------------------------------"<<std::endl;
        std::cout<<"               聊天记录                "<<std::endl<<std::endl;
        for(int i=0;i<size;i++)
        {
            if(look->sou_id==cur_user.id)
                std::cout<<"我:";
            else
                std::cout<<look->sou_id<<":";

            std::cout
            <<std::left<<std::setw(40)<<look->msg
            <<std::left<<std::setw(40)<<look->times
            <<std::endl;
            look++;
        }
        std::cout<<"--------------------------------------"<<std::endl;
        delete[] ptr;
    }
}

void download_group_list()
{
    int type=16;
    error_code ec;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(RevalueCheck(ec))
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);

        group_info* ptr=new group_info[size];
        char* pos_ptr=(char*)ptr;
        total_target=size*sizeof(group_info);
        while(recv_total<total_target)
        {
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,size*sizeof(group_info),0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }
        
        group_info* look=ptr;
        std::cout<<"--------------------------------------"<<std::endl;
        std::cout<<"               群聊列表                "<<std::endl<<std::endl;
        for(int i=0;i<size;i++)
        {
            std::cout
            <<std::left<<std::setw(dr.group_admin_len+2)<<look->admin
            <<std::left<<std::setw(dr.group_name_len+2)<<look->name
            <<std::endl;
            look++;
        }
        std::cout<<"--------------------------------------"<<std::endl;
        delete[] ptr;
    }
}

void download_group_apply_list()
{
    int type=17;
    error_code ec;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(RevalueCheck(ec))
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);

        ug_info* ptr=new ug_info[size];
        char* pos_ptr=(char*)ptr;
        total_target=size*sizeof(group_info);
        while(recv_total<total_target)
        {
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,size*sizeof(ug_info),0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }
        
        ug_info* look=ptr;
        std::cout<<"--------------------------------------"<<std::endl;
        std::cout<<"               群聊通知                "<<std::endl<<std::endl;
        for(int i=0;i<size;i++)
        {
            std::cout
            <<look->ui.name<<"("<<look->ui.admin<<") 请求加入群聊"
            <<look->gi.name<<"("<<look->gi.admin<<")"<<std::endl;
            look++;
        }
        std::cout<<"--------------------------------------"<<std::endl;
        delete[] ptr;
    }
}

void join_group()
{
    memset(group_admin_buffer,0,sizeof(group_admin_buffer));

    std::cout<<"请输入群号>";
    std::cin>>group_admin_buffer;
    if(strlen(group_admin_buffer)<1||strlen(group_admin_buffer)>dr.group_admin_len)
    {
        std::cout<<"长度错误，为1~"<<dr.group_admin_len<<std::endl;
        return;
    }

    int type=15;
    error_code ec;
    group_info gi1;
    strcpy(gi1.admin,group_admin_buffer);

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&gi1,sizeof(gi1),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);

    if(RevalueCheck(ec))
    {
        std::cout<<"已发送申请!"<<std::endl;
    }
}

void view_group_members()
{
    group_info gi;
    std::cout<<"请输入要查看群聊的id(简单测试，图形化中只需点击无需输入)>";
    std::cin>>gi.id;

    int type=18;
    error_code ec;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&gi,sizeof(gi),0);
    
    int size,recv_once,recv_total=0,total_target;
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);

    user_info* ptr=new user_info[size];
    char* pos_ptr=(char*)ptr;
    total_target=size*sizeof(user_info);
    while(recv_total<total_target)
    {
        recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,size*sizeof(ug_info),0);
        recv_total+=recv_once;
        pos_ptr+=recv_once;
    }
        
    user_info* look=ptr;
    std::cout<<"--------------------------------------"<<std::endl;
    std::cout<<"             群聊成员("<<gi.id<<")"<<std::endl<<std::endl;
    for(int i=0;i<size;i++)
    {
        std::cout<<std::left<<std::setw(dr.admin_len)<<look->admin
        <<std::left<<std::setw(dr.name_len/2)<<look->name<<std::endl;
        look++;
    }
    std::cout<<"--------------------------------------"<<std::endl;
    delete[] ptr;
}

void Apply_msg()
{
    user_info temp;
    recv(tcpc1.GetSocket_fd(),(char*)&temp,sizeof(temp),0);   
    //将信息加载到客户端列表容器
    std::cout<<std::endl<<temp.name<<"\a请求添加为好友("<<temp.admin<<")"<<std::endl;
}

void Permit_Apply_msg()
{
    user_info temp;
    recv(tcpc1.GetSocket_fd(),(char*)&temp,sizeof(temp),0);   
    std::cout<<std::endl<<temp.name<<"("<<temp.admin<<")\a同意了你的好友申请"<<std::endl;
}

void Unicast_msg()
{
    unicast_msg um1;
    recv(tcpc1.GetSocket_fd(),(char*)&um1,sizeof(um1),0);
    std::cout<<um1.sou_id<<":"<<um1.msg<<"("<<um1.times<<")"<<std::endl;
}

void Group_Apply_msg()
{
    ug_info ug;
    recv(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0); 

    std::cout<<std::endl<<
    ug.ui.name<<"("<<ug.ui.admin<<")申请加入群聊("
    <<ug.gi.admin<<" "<<ug.gi.name<<")"<<std::endl;
}

void Permit_Group_Apply_msg()
{
    group_info gi;
    recv(tcpc1.GetSocket_fd(),(char*)&gi,sizeof(gi),0); 

    std::cout<<std::endl<<"群聊申请"<<gi.name
    <<"("<<gi.admin<<")已通过!"<<std::endl;
}

void Exit_Group_msg()
{
    ug_info ug1;
    recv(tcpc1.GetSocket_fd(),(char*)&ug1,sizeof(ug1),0);

    std::cout<<ug1.ui.name<<"("<<ug1.ui.admin<<")退出了群聊 "
    <<ug1.gi.name<<"("<<ug1.gi.admin<<")"<<std::endl;
}

void Remove_Group_Member_msg()
{
    ug_info ug1;
    recv(tcpc1.GetSocket_fd(),(char*)&ug1,sizeof(ug1),0);

    std::cout<<"你被移出了群聊 "<<ug1.gi.name<<"("<<ug1.gi.admin<<")"<<std::endl;
}

void RecvMessage()
{
    int type;
    while(CUR_STATUS==LEVEL_1)
    {
        type=0;
        recv(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),MSG_PEEK);
        if(type>=0)
            continue;
        recv(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
        switch(type)
        {
            case -1:
                Apply_msg();
                break;
            case -2:
                Permit_Apply_msg();
                break;
            case -3:
                Unicast_msg();
                break;
            case -4:
                break;
            case -5:
                Group_Apply_msg();
                break;
            case -6:
                Permit_Group_Apply_msg();
                break;
            case -7:
                Exit_Group_msg();
                break;
            case -8:
                Remove_Group_Member_msg();
                break;
        }
    }
}

void SendExitMessage()
{
    // if(strcmp(cur_user.admin,"zhangsan"))
    // {
    //     int type=20;
    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     ug_info ug={{101,"","",""},{2,"张三别进","t122",193}};
    //     send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);
    // }

    // if(strcmp(cur_user.admin,"zhangsan"))
    // {
    //     int type=19;
    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     ug_info ug={{101,"","",""},{2,"张三别进","t122",193}};
    //     send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);
    // }

    // if(strcmp(cur_user.admin,"ccc2"))
    // {
    //     int type=9;
    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     user_info zhangsan={179,"ccc2","NULL","xxx"};
    //     send(tcpc1.GetSocket_fd(),(char*)&zhangsan,sizeof(zhangsan),0);
    // }

    // if(strcmp(cur_user.admin,"ccc1"))
    // {
    //     int type=8;
    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     user_info zhangsan={178,"ccc1","NULL","小希大王"};
    //     send(tcpc1.GetSocket_fd(),(char*)&zhangsan,sizeof(zhangsan),0);
    // }

    // if(strcmp(cur_user.admin,"ccc1"))
    // {
    //     int type=7;
    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     user_info zhangsan={178,"ccc1","NULL","小希大王"};
    //     send(tcpc1.GetSocket_fd(),(char*)&zhangsan,sizeof(zhangsan),0);
    // }

    // {
    //     int type=4;
    //     unicast_msg um1={cur_user.id,181,"NULL","你在干什么?小凑猫O.o"};
    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     send(tcpc1.GetSocket_fd(),(char*)&um1,sizeof(um1),0);
    // }

    // if(strcmp(cur_user.admin,"yyy3w"))
    // {
    //     int type=21;
    //     ug_info ug;

    //     ug.ui.id=cur_user.id;
    //     strcpy(ug.ui.admin,cur_user.admin);
    //     strcpy(ug.ui.name,cur_user.name);
    //     ug.gi.id=2;
    //     ug.gi.manager_id=193;
    //     strcpy(ug.gi.admin,"t122");
    //     strcpy(ug.gi.name,"张三就进!");

    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);
    // }

    // if(!strcmp(cur_user.admin,"yyy3w"))
    // {
    //     int type=22;
    //     ug_info ug;

    //     ug.ui.id=101;
    //     ug.gi.id=2;
    //     ug.gi.manager_id=cur_user.id;
    //     strcpy(ug.gi.admin,"t122");
    //     strcpy(ug.gi.name,"张三别进");

    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);
    // }

    int type=6;
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    memset(&cur_user,0,sizeof(cur_user));
}

void create_group()
{
    memset(&group_name_buffer,0,sizeof(group_name_buffer));
    memset(&group_admin_buffer,0,sizeof(group_admin_buffer));
    
    std::cout<<"请输入群号>";
    std::cin>>group_admin_buffer;
    if(strlen(group_admin_buffer)<1||strlen(group_admin_buffer)>dr.group_admin_len)
    {
        std::cout<<"长度错误,为1~"<<dr.group_admin_len<<std::endl;
        return;
    }

    std::cout<<"请输入群名>";
    std::cin>>group_name_buffer;
    if(strlen(group_name_buffer)<1||strlen(group_name_buffer)>dr.group_name_len)
    {
        std::cout<<"长度错误,为1~"<<dr.group_name_len<<std::endl;
        return;
    }

    int type=14;
    error_code ec;
    group_info gi1;
    strcpy(gi1.name,group_name_buffer);
    strcpy(gi1.admin,group_admin_buffer);
    gi1.manager_id=cur_user.id;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&gi1,sizeof(gi1),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);

    if(RevalueCheck(ec))
    {
        std::cout<<"创建成功!"<<std::endl;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
void LEVEL0()
{
    int choose;
    while(CUR_STATUS==LEVEL_0)
    {
        PrintMenu();
        std::cout<<"请选择>";
        std::cin>>choose;

        system("cls");
        switch(choose)
        {
            case 1:
                regist_test();
                break;
            case 2:
                login_test();
                break;
            case 3:
                CUR_STATUS=EXIT;
                break;
            default:
                std::cout<<"选择无效!"<<std::endl;
                break;
        }
    }
}

//登录成功后才算正式进入平台，此时退出才需要发送消息
void LEVEL1()
{
    int choose;
    if(CUR_STATUS==LEVEL_1)
    {
        std::thread recv_msg(RecvMessage);
        while(CUR_STATUS==LEVEL_1)
        {
            PrintIndex();
            std::cout<<"请选择>";
            std::cin>>choose;

            system("cls");
            switch(choose)
            {
                case 0:
                    download_friend_list();
                    break;
                case 1:
                    apply_test();
                    break;
                case 2:
                    download_apply_list();
                    break;
                case 3:
                    CUR_STATUS=LEVEL_0;
                    SendExitMessage();
                    break;
                case 4:
                    CUR_STATUS=EXIT;
                    break;
                case 5:
                    download_unicast_list();
                    break;
                case 6:
                    create_group();
                    break;
                case 7:
                    join_group();
                    break;
                case 8:
                    download_group_list();
                    break;
                case 9:
                    download_group_apply_list();
                    break;
                case 10:
                    view_group_members();
                    break;
                default:
                    std::cout<<"选择无效!"<<std::endl;
                    break;
            }
        }
        recv_msg.join();
    }
}

void LEVEL2()
{
    while(CUR_STATUS==LEVEL_2)
    {
        ;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void Init()
{   
    SetConsoleOutputCP(CP_UTF8);        
    SetConsoleCP(CP_UTF8);
    CUR_STATUS=LEVEL_0;

    if(!tcpc1.Connect())
    {
        std::cout<<"网络不佳，请稍后重试(5秒后自动退出)..."<<std::endl;
        CUR_STATUS=EXIT;
        Sleep(5000);
        return;
    }

    //later time can dynamic load charactor code
    //目前统一使用utf8(客户端、服务器、数据库、终端)
    int type=0;
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    recv(tcpc1.GetSocket_fd(),(char*)&dr,sizeof(dr),0);
}

void loop()
{
    while(CUR_STATUS)
    {
        LEVEL0();      //首页
        LEVEL1();      //主页
        LEVEL2();      //....
    }
}

void Destory()
{
    ;
}
    
int main()
{
    Init();
    loop();
    Destory();

    return 0;
}