#include "TcpClient.h"

TcpClient::TcpClient(const char* ip,short port)
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2),&wsadata);
    socket_fd=socket(PF_INET,SOCK_STREAM,0);

    if(socket_fd==INVALID_SOCKET) perror("create socket");
    target_sa.sin_addr.S_un.S_addr=inet_addr(ip);
    target_sa.sin_family=AF_INET;
    target_sa.sin_port=htons(port);

    this->f_list.len=0;
    this->f_list.head.last=NULL;
    this->f_list.head.next=NULL;

    this->g_list.len=0;
    this->g_list.head.last=NULL;
    this->g_list.head.next=NULL;

    this->fa_list.len=0;
    this->fa_list.head.last=NULL;
    this->fa_list.head.next=NULL;

    this->ga_list.len=0;
    this->ga_list.head.last=NULL;
    this->ga_list.head.next=NULL;

    //设置阻塞时间，防止子线程一直阻塞导致主线程卡死
    //注意该值要尽量设置大一点，要么接受数据时持续等待直到达到预期数据量，否则会因为网络延迟导致
    //数据在设置的超时时长之后才到达，此时socket读取时若在设置的时间内未读取到数据就会立刻返回-1
    int timeout = 400;    
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
}

bool TcpClient::Connect()
{
    int adjust=connect(socket_fd,(sockaddr*)&(target_sa),sizeof(target_sa));
    if(adjust==SOCKET_ERROR)
    {
        std::cout<<"error code:"<<WSAGetLastError()<<std::endl;
        return false;
    }
    return true;
}   

int TcpClient::GetSocket_fd()
{
    return this->socket_fd;
}

void TcpClient::get_data_rule()
{
    int type=0;
    send(this->socket_fd,(char*)&type,sizeof(type),0);
}

void TcpClient::InsertFriendList(user_info* e)
{
    friend_node* node=new friend_node;
    node->data.id=e->id;
    strcpy(node->data.admin,e->admin);
    strcpy(node->data.name,e->name);

    node->next=f_list.head.next;
    node->last=&(f_list.head);
    
    f_list.head.next=node;
    if(node->next!=NULL)
    {
        node->next->last=node;
    }

    f_list.len++;
}

void TcpClient::DestroyFriendList()
{
    friend_node* find=NULL;
    while(f_list.head.next!=NULL)
    {
        find=f_list.head.next;
        f_list.head.next=find->next;
        delete find;
        this->f_list.len--;
    }
    f_list.head.next=NULL;
}

void TcpClient::InsertGroupList(group_info* e)
{
    group_node* node=new group_node;
    node->data.id=e->id;
    node->data.manager_id=e->manager_id;
    strcpy(node->data.admin,e->admin);
    strcpy(node->data.name,e->name);

    node->next=g_list.head.next;
    node->last=&(g_list.head);
    
    g_list.head.next=node;
    if(node->next!=NULL)
    {
        node->next->last=node;
    }

    g_list.len++;
}

void TcpClient::DestroyGroupList()
{
    group_node* find=NULL;
    while(g_list.head.next!=NULL)
    {
        find=g_list.head.next;
        g_list.head.next=find->next;
        delete find;
        this->g_list.len--;
    }
    g_list.head.next=NULL;
}

void TcpClient::InsertFriendApplyList(user_info* e)
{
    friend_apply_node* node=new friend_apply_node;
    node->data.id=e->id;
    strcpy(node->data.admin,e->admin);
    strcpy(node->data.name,e->name);

    node->next=fa_list.head.next;
    node->last=&(fa_list.head);
    
    fa_list.head.next=node;
    if(node->next!=NULL)
    {
        node->next->last=node;
    }

    fa_list.len++;
}

void TcpClient::DestroyFriendApplyList()
{
    friend_apply_node* find=NULL;
    while(fa_list.head.next!=NULL)
    {
        find=fa_list.head.next;
        fa_list.head.next=find->next;
        delete find;
        this->fa_list.len--;
    }
    fa_list.head.next=NULL;
}

void TcpClient::InsertGroupApplyList(ug_info* e)
{
    group_apply_node* node=new group_apply_node;
    
    node->data.gi.id=e->gi.id;
    node->data.gi.manager_id=e->gi.manager_id;
    strcpy(node->data.gi.admin,e->gi.admin);
    strcpy(node->data.gi.name,e->gi.name);
    node->data.ui.id=e->ui.id;
    strcpy(node->data.ui.name,e->ui.name);
    strcpy(node->data.ui.admin,e->ui.admin);

    node->next=ga_list.head.next;
    node->last=&(ga_list.head);
    
    ga_list.head.next=node;
    if(node->next!=NULL)
    {
        node->next->last=node;
    }

    ga_list.len++;
}

void TcpClient::DestroyGroupApplyList()
{
    group_apply_node* find=NULL;
    while(g_list.head.next!=NULL)
    {
        find=ga_list.head.next;
        ga_list.head.next=find->next;
        delete find;
        this->ga_list.len--;
    }
    ga_list.head.next=NULL;
}

TcpClient::~TcpClient()
{
    this->DestroyGroupList();
    this->DestroyFriendList();
    this->DestroyFriendApplyList();
    this->DestroyGroupApplyList();

    closesocket(socket_fd);
    WSACleanup();
}
