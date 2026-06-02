
#include "TcpClient.h"
TcpClient tcpc1("192.168.232.140",1026);
bool CONNECT=false;

void test1()
{
    int type;
    type=1;
    user_info u1={0,"zhangsan","xx131@4","manba"};
    user_info u2={0,"æ¿‚ç•Œæ®?","xx1314","manba"};
    user_info u3={0,"uu47","1#2345678&","manba"};
    user_info u4={0,"44wk","12345678",""};

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u1,sizeof(u1),0);
    
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u2,sizeof(u2),0);

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u3,sizeof(u3),0);

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u4,sizeof(u4),0);
}

void test2()
{
    data_rule dr;
    int type=0;
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    recv(tcpc1.GetSocket_fd(),(char*)&dr,sizeof(dr),0);
    std::cout<<"admin length:"<<dr.admin_len<<std::endl;
    std::cout<<"password length:"<<dr.password_len<<std::endl;
    std::cout<<"name length:"<<dr.name_len<<std::endl;
    std::cout<<"message length:"<<dr.message_len<<std::endl;
}

void test3()
{
    int type=1;
    error_code ec;
    user_info u1={0,"smallCat","520****","é–?å—??åº?"};

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u1,sizeof(u1),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    
    if(ec.value==0)
        std::cout<<"yyy!"<<std::endl;
    else
        std::cout<<"nnn!"<<std::endl;
}

void test4()
{
    int type=2;
    error_code ec;
    user_info u1={0,"lyy3","12306"};
    user_info u2={0,"éŽ´æˆ æ®?","666666"};
    user_info u4={0,"smallCat","520****"};
    user_info u3={0,"lyy3","666666"};
    
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u1,sizeof(u1),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u2,sizeof(u2),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u4,sizeof(u4),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u3,sizeof(u3),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;
}

void test5()
{
    int type=3;
    error_code ec;
    user_info u1={0,"xiaomao"};
    user_info u2={0,"é–?å—›ç»—?"};
    user_info u3={0,"lyy3"};

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u1,sizeof(u1),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u2,sizeof(u2),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u3,sizeof(u3),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;
}

void test6()
{
    int type=2;
    error_code ec;
    user_info u1={0,"lyy3","12306"};
    user_info u2={0,"éŽ´æ","666666"};
    user_info u4={0,"smallCat","520****"};
    user_info u3={0,"zhangsan"};

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u1,sizeof(u1),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u2,sizeof(u2),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;

    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u4,sizeof(u4),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;

    type=3;
    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
    send(tcpc1.GetSocket_fd(),(char*)&u3,sizeof(u3),0);
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    std::cout<<ec.value<<std::endl;
}

void Init()
{
    SetConsoleOutputCP(CP_UTF8);
}

int main()
{
    Init();
    if(tcpc1.Connect())
        std::cout<<"connect success!"<<std::endl;
    else
    {
        std::cout<<"connect error!"<<std::endl;
        return -1;
    }
    //test1();
    //test2();
    //test3();//regist
    //test4();//login
    //test5();//apply
    test6();//login + apply;

    Sleep(6000);
    std::cout<<"program exit..."<<std::endl;
    return 0;
}