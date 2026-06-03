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
    std::cout<<">            1.添加好友                "<<std::endl;
    std::cout<<">            2.申请列表                "<<std::endl;
    std::cout<<">            3.退出登录                "<<std::endl;
    std::cout<<">            4.退出程序                "<<std::endl;
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
            //已发送过申请
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
        strcpy(cur_user.admin,admin_buffer);
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
    //send(tcpc1.GetSocket_fd(),(char*)&cur_user,sizeof(cur_user),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(RevalueCheck(ec))
    {
        int size;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);
        user_info* ptr=new user_info[size];
        recv(tcpc1.GetSocket_fd(),(char*)ptr,size*sizeof(user_info),0);
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
                break;
            case -4:
                break;
            case -5:
                break;
            case -6:
                break;
            case -7:
                break;
        }
    }
}

void SendExitMessage()
{
    // if(strcmp(cur_user.admin,"ccc1"))
    // {
    //     int type=7;
    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     user_info zhangsan={178,"ccc1","NULL","小希大王"};
    //     send(tcpc1.GetSocket_fd(),(char*)&zhangsan,sizeof(zhangsan),0);
    // }

    // if(strcmp(cur_user.admin,"ccc1"))
    // {
    //     int type=8;
    //     send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    //     user_info zhangsan={178,"ccc1","NULL","小希大王"};
    //     send(tcpc1.GetSocket_fd(),(char*)&zhangsan,sizeof(zhangsan),0);
    // }

    int type=6;
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    memset(&cur_user,0,sizeof(cur_user));
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