#include "TcpServer.h"

void TcpServer::SetNonSignal()
{   
    signal(SIGPIPE,SIG_IGN);
}

void TcpServer::SetNonBlocking(int fd)
{
    int flags;
    if(fcntl(fd,F_GETFL)==-1)
        flags=0;
     fcntl(fd,F_SETFL,flags|O_NONBLOCK);
}

void TcpServer::DisableNagle(int fd)
{
    int flag = 1;
    setsockopt(fd, IPPROTO_TCP,TCP_NODELAY, &flag, sizeof(flag));
}

TcpServer::TcpServer(int port,int listen_len):
    stop(false),user_db(DEF_STR),
    user_info_pool(sizeof(user_info)),
    group_info_pool(sizeof(group_info)),
    ug_info_pool(sizeof(ug_info)),
    unicast_msg_pool(sizeof(unicast_msg)),
    multicast_msg_pool(sizeof(multicast_msg))
{
    listen_fd=socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd==-1) perror("create socket");
    my_sa.sin_family=PF_INET;
    my_sa.sin_port=htons(port);
    my_sa.sin_addr.s_addr=htonl(INADDR_ANY);
    memset(oci,0,sizeof(oci));    

    SetNonSignal();
    SetNonBlocking(listen_fd);
    //DisableNagle(listen_fd);

    epolls=epoll_create(1);
    epoll_event listen_event;
    listen_event.data.fd=listen_fd;
    listen_event.events=EPOLLIN|EPOLLET;
    epoll_ctl(epolls,EPOLL_CTL_ADD,listen_fd,&listen_event);
    
    bind(listen_fd,(sockaddr*)&my_sa,sizeof(my_sa));
    listen(listen_fd,listen_len);
}

TcpServer::~TcpServer()
{
    epoll_ctl(epolls,EPOLL_CTL_DEL,listen_fd,NULL);
    close(listen_fd);
}

void TcpServer::Rule(int fd)
{
    data_rule dr;
    GetDataRule(dr);

    int type=24;
    send(fd,&type,sizeof(type),MSG_DONTWAIT);
    send(fd,&dr,sizeof(dr),MSG_DONTWAIT);
}

int TcpServer::FindTarget(int des_id)
{
    for(int i=1;i<ONLINE_USERS;i++)
    {
        if(oci[i].status==0)
            continue;

        if(oci[i].id==des_id)
            return i;
    }
    return 0;
}

void TcpServer::Regist(int fd)
{
    user_info u1={0};
    if(!RegistMsg(fd,u1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }
    error_code ec1=RegistModule(u1,oci[fd]);
    int type=1;
    
    send(fd,&type,sizeof(type),MSG_DONTWAIT);
    send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
}

void TcpServer::Login(int fd)
{
    user_info u1={0};
    if(!LoginMsg(fd,u1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    user_info temp;
    user_db.FindUser(u1.admin,&temp);
    int isonline=FindTarget(temp.id);

    error_code ec1=LoginModule(u1,oci[fd],isonline);
    int type=2;
    
    send(fd,&type,sizeof(type),MSG_DONTWAIT);
    send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);

    if(ec1.value==0)
    {
        strcpy(temp.password,"NULL");
        send(fd,&temp,sizeof(temp),MSG_DONTWAIT);
    }
}

void TcpServer::Apply(int fd)
{
    user_info u1={0};
    if(!ApplyMsg(fd,u1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }
    error_code ec1=ApplyModule(oci[fd].id,u1);
    int type=3;
    
    send(fd,&type,sizeof(type),MSG_DONTWAIT);
    send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);

    if(ec1.value==0)
    {
        int tg_fd=FindTarget(u1.id);
        if(tg_fd)
        {
            int type=-1;
            user_info temp;
            temp.id=oci[fd].id;
            strcpy(temp.admin,oci[fd].admin);
            strcpy(temp.name,oci[fd].name);
            
            send(tg_fd,(char*)&type,sizeof(type),MSG_DONTWAIT);
            send(tg_fd,(char*)&temp,sizeof(temp),MSG_DONTWAIT);
        }
    }
}

void TcpServer::Exit(int fd)
{
    if(oci[fd].id)//need to check id,not status
    {
        user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
        oci[fd].id=0;
        memset(oci[fd].admin,0,sizeof(oci[fd].admin));
    }
}

void TcpServer::SendApplyBuffer(int fd)
{
    int size=GetApplyBuffer_len(oci[fd].admin);
    error_code ec1={7};
    
    int type=10;
    send(fd,&type,sizeof(type),MSG_DONTWAIT);

    if(size>0)
    {
        user_info** blocks=new user_info*[size];
        for(int i=0;i<size;i++)
        {
            blocks[i]=(user_info*)user_info_pool.allocate_block();
        }

        ec1=GetApplyBufferModule(blocks,oci[fd].admin);

        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
        send(fd,&size,sizeof(size),MSG_DONTWAIT);

        int total=size*sizeof(user_info),send_once=0,send_count=0,full_pos=0;
        //full pos,if is poll will return -1,then save the pos for next send;
        while(send_count<total)
        {
            for(int i=full_pos;i<size;i++)
            {
                send_once=send(fd,blocks[i],sizeof(user_info),MSG_DONTWAIT);
                if(send_once<0)//is full
                {
                    full_pos=i;
                    usleep(100);
                }

                if(send_once>0)
                    send_count+=send_once;
                
                if(send_once==0)//target disconnect
                    break;
            }
        }

        for(int i=0;i<size;i++)
        {
            user_info_pool.deallocate_block(blocks[i]);
        }
        delete[] blocks;
    }
    else
        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
}

void TcpServer::SendFriendBuffer(int fd)
{
    int size=GetFriendBuffer_len(oci[fd].admin);
    error_code ec1={7};
    
    int type=11;
    send(fd,&type,sizeof(type),MSG_DONTWAIT);

    if(size>0)
    {
        user_info** blocks=new user_info*[size];
        for(int i=0;i<size;i++)
        {
            blocks[i]=(user_info*)user_info_pool.allocate_block();
        }

        ec1=GetFriendBufferModule(blocks,oci[fd].admin);

        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
        send(fd,&size,sizeof(size),MSG_DONTWAIT);

        int total=size*sizeof(user_info),send_once=0,send_count=0,full_pos=0;
        //full pos,if is poll will return -1,then save the pos for next send;
        while(send_count<total)
        {
            for(int i=full_pos;i<size;i++)
            {
                send_once=send(fd,blocks[i],sizeof(user_info),MSG_DONTWAIT);
                if(send_once<0)//is full
                {
                    full_pos=i;
                    usleep(100);
                }

                if(send_once>0)
                    send_count+=send_once;
                
                if(send_once==0)//target disconnect
                    break;
            }
        }
        
        for(int i=0;i<size;i++)
        {
            user_info_pool.deallocate_block(blocks[i]);
        }
        delete[] blocks;
    }
    else
        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
}

void TcpServer::SendUnicastMsgBuffer(int fd)
{
    int size=GetUnicastMsgBuffer_len(oci[fd].id);
    error_code ec1={7};

    int type=12;
    send(fd,&type,sizeof(type),MSG_DONTWAIT);

    if(size>0)
    {
        unicast_msg** blocks=new unicast_msg*[size];
        for(int i=0;i<size;i++)
        {
            blocks[i]=(unicast_msg*)unicast_msg_pool.allocate_block();
        }

        ec1=GetUnicastMsgBufferModule(blocks,oci[fd].id);

        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
        send(fd,&size,sizeof(size),MSG_DONTWAIT);
        
        int total=size*sizeof(unicast_msg),send_once=0,send_count=0,full_pos=0;
        //full pos,if is poll will return -1,then save the pos for next send;
        while(send_count<total)
        {
            for(int i=full_pos;i<size;i++)
            {
                send_once=send(fd,blocks[i],sizeof(unicast_msg),MSG_DONTWAIT);
                if(send_once<0)//is full
                {
                    full_pos=i;
                    usleep(100);
                }

                if(send_once>0)
                    send_count+=send_once;
                
                if(send_once==0)//target disconnect
                    break;
            }
        }

        for(int i=0;i<size;i++)
        {   
            unicast_msg_pool.deallocate_block(blocks[i]);
        }
        delete[] blocks;
    }
    else
        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
}

void TcpServer::SendMulticastMsgBuffer(int fd)
{
    group_info gi;
    if(!MulticastBufferMsg(fd,gi))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    int size=GetMulticastMsgBuffer_len(gi.id);
    error_code ec1={7};

    int type=13;
    send(fd,&type,sizeof(type),MSG_DONTWAIT);
    
    if(size>0)
    {
        multicast_msg** blocks=new multicast_msg*[size];
        for(int i=0;i<size;i++)
        {
            blocks[i]=(multicast_msg*)multicast_msg_pool.allocate_block();
        }
        
        ec1=GetMulticastMsgBufferModule(blocks,gi.id);
        
        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
        send(fd,&size,sizeof(size),MSG_DONTWAIT);

        int total=size*sizeof(multicast_msg),send_once=0,send_count=0,full_pos=0;
        //full pos,if is poll will return -1,then save the pos for next send;
        while(send_count<total)
        {
            for(int i=full_pos;i<size;i++)
            {
                send_once=send(fd,blocks[i],sizeof(multicast_msg),MSG_DONTWAIT);
                if(send_once<0)//is full
                {
                    full_pos=i;
                    usleep(100);
                }

                if(send_once>0)
                    send_count+=send_once;
                
                if(send_once==0)//target disconnect
                    break;
            }
        }

        for(int i=0;i<size;i++)
        {
            multicast_msg_pool.deallocate_block(blocks[i]);
        }
        delete[] blocks;
    }
    else
        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
}

void TcpServer::SendGroupBuffer(int fd)
{
    int size=GetGroupBuffer_len(oci[fd].id);
    error_code ec1={7};

    int type=16;
    send(fd,&type,sizeof(type),MSG_DONTWAIT);

    if(size>0)
    {
        group_info** blocks=new group_info*[size];
        for(int i=0;i<size;i++)
        {
            blocks[i]=(group_info*)group_info_pool.allocate_block();
        }

        ec1=GetGroupBufferModule(blocks,oci[fd].id);

        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
        send(fd,&size,sizeof(size),MSG_DONTWAIT);
        
        int total=size*sizeof(group_info),send_once=0,send_count=0,full_pos=0;
        //full pos,if is poll will return -1,then save the pos for next send;
        while(send_count<total)
        {
            for(int i=full_pos;i<size;i++)
            {
                send_once=send(fd,blocks[i],sizeof(group_info),MSG_DONTWAIT);
                if(send_once<0)//is full
                {
                    full_pos=i;
                    usleep(100);
                }

                if(send_once>0)
                    send_count+=send_once;
                
                if(send_once==0)//target disconnect
                    break;
            }
        }

        for(int i=0;i<size;i++)
        {
            group_info_pool.deallocate_block(blocks[i]);
        }
        delete[] blocks;
    }
    else
        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
}

void TcpServer::SendGroupApplyBuffer(int fd)
{
    int size=GetGroupApplyBuffer_len(oci[fd].id);
    error_code ec1={7};

    int type=17;
    send(fd,&type,sizeof(type),MSG_DONTWAIT);

    if(size>0)
    {
        ug_info** blocks=new ug_info*[size];
        for(int i=0;i<size;i++)
        {
            blocks[i]=(ug_info*)ug_info_pool.allocate_block();
        }

        ec1=GetGroupApplyBufferModule(blocks,oci[fd].id);

        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
        send(fd,&size,sizeof(size),MSG_DONTWAIT);

        int total=size*sizeof(ug_info),send_once=0,send_count=0,full_pos=0;
        //full pos,if is poll will return -1,then save the pos for next send;
        while(send_count<total)
        {
            for(int i=full_pos;i<size;i++)
            {
                send_once=send(fd,blocks[i],sizeof(ug_info),MSG_DONTWAIT);
                if(send_once<0)//is full
                {
                    full_pos=i;
                    usleep(100);
                }

                if(send_once>0)
                    send_count+=send_once;
                
                if(send_once==0)//target disconnect
                    break;
            }
        }

        for(int i=0;i<size;i++)
        {
            ug_info_pool.deallocate_block(blocks[i]);
        }
        delete[] blocks;
    }
    else
        send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
}

void TcpServer::PermitApply(int fd)
{
    user_info u1={0};
    if(!PermitApplyMsg(fd,u1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    error_code ec=PermitApplyModule(u1.id,oci[fd].id);
    if(ec.value==0)
    {
        int tg_fd=FindTarget(u1.id);
        if(tg_fd)
        {
            int type=-2;
            user_info temp;
            temp.id=oci[fd].id;
            strcpy(temp.admin,oci[fd].admin);
            strcpy(temp.name,oci[fd].name);

            send(tg_fd,&type,sizeof(type),MSG_DONTWAIT);
            send(tg_fd,&temp,sizeof(temp),MSG_DONTWAIT);
        }
    }
}

void TcpServer::RefuseApply(int fd)
{
    user_info u1={0};
    if(!RefuseApplyMsg(fd,u1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    RefuseApplyModule(u1.id,oci[fd].id);
}

void TcpServer::PermitGroupApply(int fd)
{
    ug_info ug={0};
    if(!PermitGroupApplyMsg(fd,ug))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    error_code ec=PermitGroupApplyModule(ug);
    if(ec.value==0)
    {
        int tg_fd=FindTarget(ug.ui.id);
        if(tg_fd)
        {
            int type=-6;
            group_info temp;
            temp.id=ug.gi.id;
            strcpy(temp.admin,ug.gi.admin);
            strcpy(temp.name,ug.gi.name);
            temp.manager_id=ug.gi.manager_id;

            send(tg_fd,&type,sizeof(type),MSG_DONTWAIT);
            send(tg_fd,&temp,sizeof(temp),MSG_DONTWAIT);
        }
    }
}

void TcpServer::RefuseGroupApply(int fd)
{
    ug_info ug={0};
    if(!RefuseGroupApplyMsg(fd,ug))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    RefuseGroupApplyModule(ug);
}

void TcpServer::DeleteFriend(int fd)
{
    user_info u1={0};
    if(!DeleteFriendMsg(fd,u1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    DeleteFriendModule(u1.id,oci[fd].id);

    int tg_fd=FindTarget(u1.id);
    if(tg_fd)
    {
        int type=-9;
        user_info temp;
        temp.id=oci[fd].id;

        send(tg_fd,&type,sizeof(type),MSG_DONTWAIT);
        send(tg_fd,&temp,sizeof(temp),MSG_DONTWAIT);
    }
}

void TcpServer::UniCast(int fd)
{
    unicast_msg um1;
    if(!UnicastMsg(fd,um1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    UnicastMsgModule(um1);
    int tg_fd=FindTarget(um1.des_id);

    if(tg_fd)
    {
        int type=-3;
        send(tg_fd,&type,sizeof(type),MSG_DONTWAIT);
        send(tg_fd,&um1,sizeof(um1),MSG_DONTWAIT);
    }
}

void TcpServer::MultiCast(int fd)
{
    multicast_msg mm1;
    if(!MulticastMsg(fd,mm1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    MulticastMsgModule(mm1);
    int size=GetGroupMemberNumModule(mm1.group_id);
    int* g_ms_ptr=new int[size];
    GetGroupMemberIDModule(g_ms_ptr,mm1.group_id);

    int* find=g_ms_ptr;
    for(int i=0;i<size;i++)
    {
        int tg_fd=FindTarget(*find);
        if(tg_fd&&tg_fd!=fd)
        {
            int type=-4;
            send(tg_fd,&type,sizeof(type),MSG_DONTWAIT);
            send(tg_fd,&mm1,sizeof(mm1),MSG_DONTWAIT);
        }
        find++;
    }
    delete[] g_ms_ptr;
}

void TcpServer::CreateGroup(int fd)
{
    group_info gi1;
    if(!CreateGroupMsg(fd,gi1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    int type=14;
    error_code ec1=CreateGroupModule(gi1);

    send(fd,&type,sizeof(type),MSG_DONTWAIT);
    send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);

    if(ec1.value==0)
        send(fd,&gi1,sizeof(gi1),MSG_DONTWAIT);
}

void TcpServer::GroupApply(int fd)
{
    group_info gi1;
    if(!GroupApplyMsg(fd,gi1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    int type=15;
    error_code ec=GroupApplyModule(oci[fd].id,gi1);

    send(fd,&type,sizeof(type),MSG_DONTWAIT);
    send(fd,&ec,sizeof(ec),MSG_DONTWAIT);

    if(ec.value==0)
    {
        int tg_fd=FindTarget(gi1.manager_id);
        if(tg_fd)
        {
            int type=-5;
            user_info temp;
            temp.id=oci[fd].id;
            strcpy(temp.admin,oci[fd].admin);
            strcpy(temp.name,oci[fd].name);
            ug_info ug1={temp,gi1};

            send(tg_fd,(char*)&type,sizeof(type),MSG_DONTWAIT);
            send(tg_fd,(char*)&ug1,sizeof(ug1),MSG_DONTWAIT);
        }
    }
}

void TcpServer::ViewGroupMembers(int fd)
{   
    group_info gi1;
    if(!ViewGroupMembersMsg(fd,gi1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    error_code ec1;
    int size=GetGroupMembers_len(gi1.id);//size at least be 2(manager and query people)

    user_info** blocks=new user_info*[size];
    for(int i=0;i<size;i++)
    {
        blocks[i]=(user_info*)user_info_pool.allocate_block();
    }

    ec1=ViewGroupMembersModule(blocks,gi1.id);

    int type=18;
    send(fd,&type,sizeof(type),MSG_DONTWAIT);

    send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
    send(fd,&size,sizeof(size),MSG_DONTWAIT);
    
    for(int i=0;i<size;i++)
    {
        send(fd,blocks[i],sizeof(user_info),MSG_DONTWAIT);
    }

    for(int i=0;i<size;i++)
    {
        user_info_pool.deallocate_block(blocks[i]);
    }
    delete[] blocks;  
}

void TcpServer::ExitGroup(int fd)
{
    ug_info ug1;
    if(!ExitGroupMsg(fd,ug1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    ExitGroupModule(ug1);

    int tg_fd=FindTarget(ug1.gi.manager_id);
    if(tg_fd)
    {
        int type=-7;
        send(tg_fd,(char*)&type,sizeof(type),MSG_DONTWAIT);
        send(tg_fd,(char*)&ug1,sizeof(ug1),MSG_DONTWAIT);
    }
}

void TcpServer::RemoveGroupMember(int fd)
{
    ug_info ug1;
    if(!RemoveGroupMemberMsg(fd,ug1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }

    RemoveGroupMemberModule(ug1);

    int tg_fd=FindTarget(ug1.ui.id);
    if(tg_fd)
    {
        int type=-8;
        send(tg_fd,(char*)&type,sizeof(type),MSG_DONTWAIT);
        send(tg_fd,(char*)&ug1,sizeof(ug1),MSG_DONTWAIT);
    }
}

void TcpServer::UpdatePasswd(int fd)
{
    user_info u1={0};
    if(!UpdatePasswdMsg(fd,u1))
    {
        if(oci[fd].status)
        {
            if(oci[fd].id)
            {
                oci[fd].id=0;
                user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
            }

            oci[fd].status=0;
            std::cout<<"client "<<fd<<" disconnect"<<std::endl;
            
            epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
            close(fd);
            return;
        }
    }
    error_code ec1=UpdatePasswdModule(u1,oci[fd]);
    int type=23;
    
    send(fd,&type,sizeof(type),MSG_DONTWAIT);
    send(fd,&ec1,sizeof(ec1),MSG_DONTWAIT);
}

/**************************************------main------**************************************/

void TcpServer::StartUp()
{
    while(!stop)
    {
        int adjust=epoll_wait(epolls,c_events,CS_EVENTS_LEN,1000);
        
        if(adjust<0)
        {
            if(errno==EINTR)
            {
                //std::cout<<"epoll_wait pause(not error)"<<std::endl;
            }
            else
            {
                std::cout<<"call function(epoll_wait) error!"<<std::endl;
            }
            continue;
        }

        if(adjust==0)
        {
            //std::cout<<"no event..."<<std::endl;//just for test
            continue;
        }
        
        for(int i=0;i<adjust;i++)
        {
            if(c_events[i].data.fd==listen_fd)
            {
                sockaddr_in client_fd;
                socklen_t c_len=sizeof(client_fd);
                int new_fd=accept(listen_fd,(sockaddr*)&client_fd,&c_len);

                if(new_fd==-1&&errno!=EAGAIN)
                    perror("accept");
                else
                {
                    SetNonBlocking(new_fd);
                    //DisableNagle(new_fd);
                    epoll_event new_event;
                    new_event.data.fd=new_fd;
                    new_event.events=EPOLLIN|EPOLLET;
                    epoll_ctl(epolls,EPOLL_CTL_ADD,new_fd,&new_event);

                    strcpy(oci[new_fd].ip,inet_ntoa(client_fd.sin_addr));
                    oci[new_fd].status=1;
                    std::cout<<"client "<<new_fd<<" is join"<<std::endl;
                    
                }
            }
            else
            {
                if((c_events[i].events&EPOLLIN)==1)
                {
                    int fd=c_events[i].data.fd;
                    while(true)
                    {
                        int msg_type=0;
                        int recv_once=recv(fd,&msg_type,sizeof(msg_type),MSG_DONTWAIT);

                        if(recv_once<=0)
                        {
                            if(errno==EAGAIN&&recv_once<0)
                                ;//finnished recv current data
                            else
                            {
                                if(oci[fd].status)
                                {
                                    if(oci[fd].id)
                                    {
                                        oci[fd].id=0;
                                        user_db.InsertLog(oci[fd].ip,oci[fd].admin,EXIT_LOG);
                                    }

                                    oci[fd].status=0;
                                    std::cout<<"client "<<fd<<" disconnect"<<std::endl;
                                    
                                    epoll_ctl(epolls,EPOLL_CTL_DEL,fd,NULL);
                                    close(fd);
                                }
                            }
                            break;
                        }
                        else
                        {
                            switch(msg_type)
                            {
                                case 1:
                                    Regist(fd);
                                    break;
                                case 2:
                                    Login(fd);
                                    break;
                                case 3:
                                    Apply(fd);
                                    break;
                                case 4:
                                    UniCast(fd);
                                    break;
                                case 5:
                                    MultiCast(fd);
                                    break;
                                case 6:
                                    Exit(fd);
                                    break;
                                case 7:
                                    PermitApply(fd);
                                    break;
                                case 8:
                                    RefuseApply(fd);
                                    break;
                                case 9:
                                    DeleteFriend(fd);
                                    break;
                                case 10:
                                    SendApplyBuffer(fd);
                                    break;
                                case 11:
                                    SendFriendBuffer(fd);
                                    break;
                                case 12:
                                    SendUnicastMsgBuffer(fd);
                                    break;
                                case 13:
                                    SendMulticastMsgBuffer(fd);
                                    break;
                                case 14:
                                    CreateGroup(fd);
                                    break;
                                case 15:
                                    GroupApply(fd);
                                    break;
                                case 16:
                                    SendGroupBuffer(fd);
                                    break;
                                case 17:
                                    SendGroupApplyBuffer(fd);
                                    break;
                                case 18:
                                    ViewGroupMembers(fd);
                                    break;
                                case 19:
                                    PermitGroupApply(fd);
                                    break;
                                case 20:
                                    RefuseGroupApply(fd);
                                    break;
                                case 21:
                                    ExitGroup(fd);
                                    break;
                                case 22:
                                    RemoveGroupMember(fd);
                                    break;
                                case 23:
                                    UpdatePasswd(fd);
                                    break;
                                case 24:
                                    Rule(fd);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}

/**************************************------ADPL------**************************************/

void TcpServer::AdminPanel()
{
    char buffer[1024];

    std::cout<<"loading..."<<std::endl;
    sleep(2);//wait for main thread function
    std::cout<<"server is running..."<<std::endl;
    sleep(1);
    std::cout<<"welcome to ZCat server admin panel, input 'help' to display command list"<<std::endl;

    while(!stop)
    {
        memset(buffer,0,sizeof(buffer));
	
        std::cin>>buffer;

        if(!strcmp("help",buffer))
        {
            std::cout<<"=************************************************************************="<<std::endl;
            std::cout<<"                            Command List                                  "<<std::endl;
            std::cout<<"                                                                          "<<std::endl;
            std::cout<<"'ban x(admin)'                          ban user to black user list"<<std::endl;
            std::cout<<"'clear'                                 clear control panel screen"<<std::endl;
            std::cout<<"'clsatlog'                              clear all active log information"<<std::endl;
            std::cout<<"'disip'                                 display server's ip address"<<std::endl;
            std::cout<<"'disbul'                                display black user list"<<std::endl;
            std::cout<<"'dismps'                                display memory pool status"<<std::endl;
	        std::cout<<"'discfg'                                display server config information"<<std::endl;
            std::cout<<"'disuser'                               display all user infomation"<<std::endl;
            std::cout<<"'disouser'                              display online user information"<<std::endl;
            std::cout<<"'dissinfo'                              display server information"<<std::endl;
            std::cout<<"'disatlog'                              display active log information"<<std::endl;
            std::cout<<"'delatlog x(id)'                        delete one active log information"<<std::endl;
            std::cout<<"'help'                                  display system command"<<std::endl;
            std::cout<<"'stop'                                  stop server"<<std::endl;
            std::cout<<"'unban x(admin)'                        unban user to black user list"<<std::endl;
            std::cout<<"                                                                          "<<std::endl;
            std::cout<<"=************************************************************************="<<std::endl;
        }

        if(!strcmp("dismps",buffer))
        {
            user_info_pool.PoolInformaton("User_Info_Pool");
            unicast_msg_pool.PoolInformaton("Unicast_Msg_Pool");
            multicast_msg_pool.PoolInformaton("Multicast_Msg_Pool");
            group_info_pool.PoolInformaton("Group_Info_Pool");
            ug_info_pool.PoolInformaton("Ug_Info_Pool");
        }

        if(!strcmp("stop",buffer))
        {
            stop=true;
        }

        if(!strcmp("clear",buffer))
        {
            system("clear");
        }

        if(!strcmp("disuser",buffer))
        {
            user_db.View_All_Users();
        }

        if(!strcmp("disouser",buffer))
        {
            int count=0;

            std::cout
            <<std::left<<std::setw(10)<<"ID"
            <<std::left<<std::setw(15)<<"ADMIN"
            <<std::left<<std::setw(35)<<"IP"
            <<std::left<<std::setw(30)<<"NAME"//mabye have chinese,so should in the back
            <<std::endl;

            for(int i=0;i<ONLINE_USERS;i++)
            {
                if(this->oci[i].id!=0)
                {
                    std::cout
                    <<std::left<<std::setw(10)<<this->oci[i].id
                    <<std::left<<std::setw(15)<<this->oci[i].admin
                    <<std::left<<std::setw(35)<<this->oci[i].ip
                    <<std::left<<std::setw(30)<<this->oci[i].name
                    <<std::endl;
                    count++;
                }
            }
            std::cout<<"online user nums:"<<count<<std::endl;
        }

        if(!strcmp("ban",buffer))
        {
            char admin[1024];
            std::cin>>admin;
            user_info temp;

            if(this->user_db.FindUser(admin,&temp))
            {
                if(this->user_db.IsBlackUser(admin))
                {
                    std::cout<<admin<<" is already in the black list"<<std::endl;
                }
                else
                {   
                    if(this->user_db.InsertBlackList(temp.id))
                    {
                        std::cout<<"ban "<<admin<<" success"<<std::endl;
                    }
                }
            }
            else
            {
                std::cout<<"admin is not exist"<<std::endl;
            }
        }

        if(!strcmp("unban",buffer))
        {
            char admin[1024];
            std::cin>>admin;
            user_info temp;

            if(this->user_db.FindUser(admin,&temp))
            {
                if(this->user_db.IsBlackUser(admin))
                {
                    if(this->user_db.RemoveBlackList(temp.id))
                    {
                        std::cout<<"unban "<<admin<<" success"<<std::endl;
                    }
                }   
                else
                {   
                    std::cout<<admin<<" is not in the black list"<<std::endl;
                }
            }
            else
            {
                std::cout<<"admin is not exist"<<std::endl;
            }
        }

        if(!strcmp(buffer,"disbul"))
        {
            this->user_db.ViewBlackList();
        }

        if(!strcmp(buffer,"disatlog"))
        {
            this->user_db.ViewActiveLog();
        }

        if(!strcmp(buffer,"clsatlog"))
        {
            this->user_db.ClearActiveLog();
            std::cout<<"clear success"<<std::endl;
        }

        if(!strcmp(buffer,"delatlog"))
        {
            int id;
            std::cin>>id;
            if(this->user_db.DeleteOneActiveLog(id))
            {
                std::cout<<"delete success"<<std::endl;
            }
            else
            {
                std::cout<<"active is not exist"<<std::endl;
            }
        }

        if(!strcmp(buffer,"disip"))
        {
            system("ip addr");
        }

	    if(!strcmp(buffer,"discfg"))
	    {
		    std::cout<<"[Server]"<<std::endl;
            std::cout<<"ip="<<ntohl(INADDR_ANY)
            <<" port="<<ntohs(this->my_sa.sin_port)<<std::endl<<std::endl;

            std::cout<<"[Data Base]"<<std::endl;
            std::cout<<DEF_STR<<std::endl<<std::endl;
	    }

        if(!strcmp(buffer,"dissinfo"))
        {
            std::cout<<"[version]"<<std::endl;
            std::cout<<"ZCat Server 1.0(2026-7-4)"<<std::endl;
            std::cout<<"ZCat Client 1.0(2026-7-4)"<<std::endl<<std::endl;
            std::cout<<"[author]"<<std::endl;
            std::cout<<"WenDao"<<std::endl<<std::endl;
        }
    }
}
