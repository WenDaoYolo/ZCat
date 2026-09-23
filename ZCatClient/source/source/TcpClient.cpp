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

    this->gm_list.len=0;
    this->gm_list.head.last=NULL;
    this->gm_list.head.next=NULL;

    this->u_list.len=0;
    this->u_list.head.last=NULL;
    this->u_list.head.next=NULL;
    this->u_list.tail=&(this->u_list.head);

    this->m_list.len=0;
    this->m_list.head.last=NULL;
    this->m_list.head.next=NULL;
    this->m_list.tail=&(this->m_list.head);

    //设置阻塞时间，防止子线程一直阻塞导致主线程卡死
    //注意该值要尽量设置大一点，要么接受数据时持续等待直到达到预期数据量，否则会因为网络延迟导致
    //数据在设置的超时时长之后才到达，此时socket读取时若在设置的时间内未读取到数据就会立刻返回-1
    int timeout = 400;    
    setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
}

void TcpClient::SetIp(const char* ip)
{
    target_sa.sin_addr.S_un.S_addr=inet_addr(ip);
}

void TcpClient::SetPORT(short port)
{
    target_sa.sin_port=htons(port);
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
    int type=24;
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

void TcpClient::DeleteFriendList(user_info* e)
{
    friend_node* find=this->f_list.head.next;
    while(find!=NULL)
    {
        if(find->data.id==e->id)
        {
            if(find->next==NULL)
            {
                find->last->next=NULL;
            }
            else
            {
                find->last->next=find->next;
                find->next->last=find->last;
            }
            delete find;
            this->f_list.len--;
            
            return;
        }
        find=find->next;
    }
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

void TcpClient::InsertGroupMemberList(user_info* e)
{
    group_member_node* node=new group_member_node;
    node->data.id=e->id;
    strcpy(node->data.admin,e->admin);
    strcpy(node->data.name,e->name);

    node->next=gm_list.head.next;
    node->last=&(gm_list.head);
    
    gm_list.head.next=node;
    if(node->next!=NULL)
    {
        node->next->last=node;
    }

    gm_list.len++;
}

void TcpClient::DeleteGroupMemberList(user_info* e)
{
    group_member_node* find=this->gm_list.head.next;
    while(find!=NULL)
    {
        if(find->data.id==e->id)
        {
            if(find->next==NULL)
            {
                find->last->next=NULL;
            }
            else
            {
                find->last->next=find->next;
                find->next->last=find->last;
            }
            delete find;
            this->gm_list.len--;
            
            return;
        }
        find=find->next;
    }
}

void TcpClient::DestroyGroupMemberList()
{
    group_member_node* find=NULL;
    while(gm_list.head.next!=NULL)
    {
        find=gm_list.head.next;
        gm_list.head.next=find->next;
        delete find;
        this->gm_list.len--;
    }
    gm_list.head.next=NULL;
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

void TcpClient::DeleteGroupList(group_info* e)
{
    group_node* find=this->g_list.head.next;
    while(find!=NULL)
    {
        if(find->data.id==e->id)
        {
            if(find->next==NULL)
            {
                find->last->next=NULL;
            }
            else
            {
                find->last->next=find->next;
                find->next->last=find->last;
            }
            delete find;
            this->g_list.len--;
            
            return;
        }
        find=find->next;
    }
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

void TcpClient::DeleteFriendApplyList(friend_apply_node* e)
{
    friend_apply_node* find=this->fa_list.head.next;
    while(find!=NULL)
    {
        if(find->data.id==e->data.id)
        {
            if(find->next==NULL)
            {
                find->last->next=NULL;
            }
            else
            {
                find->last->next=find->next;
                find->next->last=find->last;
            }
            delete find;
            this->fa_list.len--;
            
            return;
        }
        find=find->next;
    }
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
    while(ga_list.head.next!=NULL)
    {
        find=ga_list.head.next;
        ga_list.head.next=find->next;
        delete find;
        this->ga_list.len--;
    }
    ga_list.head.next=NULL;
}

void TcpClient::DeleteGroupApplyList(group_apply_node* e)
{
    group_apply_node* find=this->ga_list.head.next;
    while(find!=NULL)
    {
        if(find->data.ui.id==e->data.ui.id)
        {
            if(find->next==NULL)
            {
                find->last->next=NULL;
            }
            else
            {
                find->last->next=find->next;
                find->next->last=find->last;
            }
            delete find;
            this->ga_list.len--;
            
            return;
        }
        find=find->next;
    }
}

void TcpClient::InsertUnicastList(unicast_msg* e)
{
    unicast_node* node=new unicast_node;
    
    node->data.sou_id=e->sou_id;
    node->data.des_id=e->des_id;
    strcpy(node->data.times,e->times);
    strcpy(node->data.msg,e->msg);
    
    if(this->u_list.tail!=&(this->u_list.head))
    {
        node->last=this->u_list.tail;
        node->next=(this->u_list.tail)->next;
        this->u_list.tail->next=node;

        this->u_list.tail=node;
    }
    else
    {
        node->next=this->u_list.head.next;
        node->last=&(this->u_list.head);
        this->u_list.head.next=node;
        
        this->u_list.tail=node;
    }
    this->u_list.len++;
}

void TcpClient::DestroyUnicastList()
{
    while(this->u_list.tail!=&(this->u_list.head))
    {
        this->u_list.tail=this->u_list.tail->last;
        delete this->u_list.tail->next;
    }
    this->u_list.head.next=NULL;
}

void TcpClient::DeleteUnicastList(unicast_node* e)
{
    unicast_node* find=this->u_list.head.next;
    while(find!=NULL)
    {
        if(find->data.des_id==e->data.des_id&&find->data.sou_id==e->data.sou_id)
        {
            if(find->next==NULL)
            {
                find->last->next=NULL;
            }
            else
            {
                find->last->next=find->next;
                find->next->last=find->last;
            }
            delete find;
            this->u_list.len--;
            
            return;
        }
        find=find->next;
    }
}

void TcpClient::InsertMulticastList(multicast_msg* e)
{
    multicast_node* node=new multicast_node;
    
    node->data.group_id=e->group_id;
    node->data.member_id=e->member_id;
    strcpy(node->data.msg,e->msg);
    strcpy(node->data.times,e->times);
    strcpy(node->data.member_name,e->member_name);
    
    if(this->m_list.tail!=&(this->m_list.head))
    {
        node->last=this->m_list.tail;
        node->next=(this->m_list.tail)->next;
        this->m_list.tail->next=node;

        this->m_list.tail=node;
    }
    else
    {
        node->next=this->m_list.head.next;
        node->last=&(this->m_list.head);
        this->m_list.head.next=node;
        
        this->m_list.tail=node;
    }
    this->m_list.len++;
}

void TcpClient::DestroyMulticastList()
{
     while(this->m_list.tail!=&(this->m_list.head))
    {
        this->m_list.tail=this->m_list.tail->last;
        delete this->m_list.tail->next;
    }
    this->m_list.head.next=NULL;
}

void TcpClient::DeleteMulticastList(multicast_node* e)
{
    multicast_node* find=this->m_list.head.next;
    while(find!=NULL)
    {
        if(find->data.group_id==e->data.group_id&&find->data.member_id==e->data.member_id)
        {
            if(find->next==NULL)
            {
                find->last->next=NULL;
            }
            else
            {
                find->last->next=find->next;
                find->next->last=find->last;
            }
            delete find;
            this->m_list.len--;
            
            return;
        }
        find=find->next;
    }
}

TcpClient::~TcpClient()
{
    this->DestroyGroupList();
    this->DestroyFriendList();
    this->DestroyFriendApplyList();
    this->DestroyGroupApplyList();
    this->DestroyGroupMemberList();
    this->DestroyUnicastList();
    this->DestroyMulticastList();

    closesocket(socket_fd);
    WSACleanup();
}
