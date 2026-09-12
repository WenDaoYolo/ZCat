#include "main.h"
#include<iomanip>
#include<thread>

/***************************************************-----RecvMessage-----***************************************************/

void get_data_rule_msg()
{
    recv(tcpc1.GetSocket_fd(),(char*)&dr,sizeof(dr),0);
}

void get_regist_msg()
{
    recv(tcpc1.GetSocket_fd(),(char*)&cur_error,sizeof(cur_error),0);
}

void get_login_msg()
{
    recv(tcpc1.GetSocket_fd(),(char*)&cur_error,sizeof(cur_error),0);
    if(cur_error.value==0)
    {
        recv(tcpc1.GetSocket_fd(),(char*)&cur_user,sizeof(cur_user),0);
        memset(cur_admin_text.text,0,sizeof(cur_admin_text.text));
        strcpy(cur_admin_text.text,"当前账号:");
        strcat(cur_admin_text.text,cur_user.admin);

        //download friend list
        int type=11;
        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);

        //download group list
        type=16;
        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
        
        //download friend apply list
        type=10;
        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);

        //download group apply list
        type=17;
        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);

        //download friend msg list
        type=12;
        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);

        clear_input_buffer();
    }
}

void get_friend_list_msg()
{
    error_code ec;
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(ec.value==0)
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);
        
        user_info* temp=new user_info[size];
        char* pos_ptr=(char*)temp;
        total_target=size*sizeof(user_info);
        while(recv_total<total_target)
        {
            int need = total_target-recv_total;
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,need,0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }

        user_info* find=temp;
        for(int i=0;i<size;i++)
            tcpc1.InsertFriendList(temp+i);
        delete[] temp;

        cur_friend_page_first=tcpc1.f_list.head.next;
        if(tcpc1.f_list.len>0)
        {
            friend_cur_page=1;
        }
        else
            friend_cur_page=0;
    }

    if(ec.value==7)
    {
        cur_friend_page_first=NULL;
    }

    download_friend_list=true;
}

void get_unicast_list_msg()
{
    error_code ec;
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(ec.value==0)
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);
        
        unicast_msg* temp=new unicast_msg[size];
        char* pos_ptr=(char*)temp;
        total_target=size*sizeof(unicast_msg);
        while(recv_total<total_target)
        {
            int need = total_target-recv_total;
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,need,0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }

        unicast_msg* find=temp;
        for(int i=0;i<size;i++)
            tcpc1.InsertUnicastList(temp+i);
        delete[] temp;
    }
    download_unicast_list=true;
}

void get_multicast_list_msg()
{
    error_code ec;
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(ec.value==0)
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);
        
        multicast_msg* temp=new multicast_msg[size];
        char* pos_ptr=(char*)temp;
        total_target=size*sizeof(multicast_msg);
        while(recv_total<total_target)
        {
            int need = total_target-recv_total;
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,need,0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }

        multicast_msg* find=temp;
        for(int i=0;i<size;i++)
            tcpc1.InsertMulticastList(temp+i);
        delete[] temp;

        multicast_node* t=tcpc1.m_list.head.next;
    }
    download_multicast_list=true;
}

void get_group_list_msg()
{
    error_code ec;
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(ec.value==0)
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);
        
        group_info* temp=new group_info[size];
        char* pos_ptr=(char*)temp;
        total_target=size*sizeof(group_info);
        while(recv_total<total_target)
        {
            int need = total_target-recv_total;
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,need,0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }

        group_info* find=temp;
        for(int i=0;i<size;i++)
            tcpc1.InsertGroupList(temp+i);
        delete[] temp;

        cur_group_page_first=tcpc1.g_list.head.next;

        if(tcpc1.g_list.len>0)
            group_cur_page=1;
        else
            group_cur_page=0;
    }

    if(ec.value==7)
    {
        cur_group_page_first=NULL;
    }

    download_group_list=true;
}

void get_friend_apply_list_msg()
{
    error_code ec;
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(ec.value==0)
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);
        
        user_info* temp=new user_info[size];
        char* pos_ptr=(char*)temp;
        total_target=size*sizeof(user_info);
        while(recv_total<total_target)
        {
            int need = total_target-recv_total;
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,need,0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }

        user_info* find=temp;
        for(int i=0;i<size;i++)
            tcpc1.InsertFriendApplyList(temp+i);
        delete[] temp;

        cur_friend_apply_page_first=tcpc1.fa_list.head.next;

        if(tcpc1.fa_list.len>0)
            friend_apply_cur_page=1;
        else
            friend_apply_cur_page=0;
    }

    if(ec.value==7)
    {
        cur_friend_apply_page_first=NULL;
    }

    download_friend_apply_list=true;
}

void get_group_apply_list_msg()
{
    error_code ec;
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(ec.value==0)
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);
        
        ug_info* temp=new ug_info[size];
        char* pos_ptr=(char*)temp;
        total_target=size*sizeof(ug_info);
        while(recv_total<total_target)
        {
            int need = total_target-recv_total;
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,need,0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }

        ug_info* find=temp;
        for(int i=0;i<size;i++)
            tcpc1.InsertGroupApplyList(temp+i);
        delete[] temp;

        cur_group_apply_page_first=tcpc1.ga_list.head.next;

         if(tcpc1.ga_list.len>0)
            group_apply_cur_page=1;
        else
            group_apply_cur_page=0;
    }

    if(ec.value==7)
    {
        cur_group_apply_page_first=NULL;
    }

    download_group_apply_list=true;
}

void get_group_member_list_msg()
{
    error_code ec;
    recv(tcpc1.GetSocket_fd(),(char*)&ec,sizeof(ec),0);
    if(ec.value==0)
    {
        int size,recv_once,recv_total=0,total_target;
        recv(tcpc1.GetSocket_fd(),(char*)&size,sizeof(size),0);
        
        user_info* temp=new user_info[size];
        char* pos_ptr=(char*)temp;
        total_target=size*sizeof(user_info);
        while(recv_total<total_target)
        {
            int need = total_target-recv_total;
            recv_once=recv(tcpc1.GetSocket_fd(),(char*)pos_ptr,need,0);
            recv_total+=recv_once;
            pos_ptr+=recv_once;
        }

        user_info* find=temp;
        for(int i=0;i<size;i++)
            tcpc1.InsertGroupMemberList(temp+i);
        delete[] temp;

        cur_group_member_page_first=tcpc1.gm_list.head.next;

         if(tcpc1.gm_list.len>0)
            group_member_cur_page=1;
        else
            group_member_cur_page=0;

        LEVEL_1_MS=LEVEL_1_GROUP_MEMBER_MSG_CONTENT;
        LEVEL_1_TS=LEVEL_1_NONE;
    }

    if(ec.value==7)
    {
        cur_group_member_page_first=NULL;
    }
}

void get_apply_friend_msg()
{
    recv(tcpc1.GetSocket_fd(),(char*)&cur_error,sizeof(cur_error),0);
}

void get_join_group_msg()
{
    recv(tcpc1.GetSocket_fd(),(char*)&cur_error,sizeof(cur_error),0);
}

void get_create_group_msg()
{
    group_info gi;
    recv(tcpc1.GetSocket_fd(),(char*)&cur_error,sizeof(cur_error),0);
    if(cur_error.value==0)
    {
        recv(tcpc1.GetSocket_fd(),(char*)&gi,sizeof(gi),0);

        //更新群聊列表
        tcpc1.InsertGroupList(&(gi));
        cur_group_page_first=tcpc1.g_list.head.next;
        group_cur_page=1;
    }
}

void get_change_passwd_msg()
{
    recv(tcpc1.GetSocket_fd(),(char*)&cur_error,sizeof(cur_error),0);
}

void new_friend_msg()
{
    unicast_msg temp;
    recv(tcpc1.GetSocket_fd(),(char*)&(temp),sizeof(unicast_msg),0);
    tcpc1.InsertUnicastList(&temp);
}

void new_group_msg()
{
    multicast_msg temp;
    recv(tcpc1.GetSocket_fd(),(char*)&(temp),sizeof(multicast_msg),0);
    tcpc1.InsertMulticastList(&temp);
}

void member_exit_group_msg()
{
    ug_info ug;
    recv(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);
    memset(notice_button.text,0,sizeof(notice_button.text));
    strcpy(notice_button.text,ug.ui.name);
    strcat(notice_button.text,"退出了群聊-");
    strcat(notice_button.text,ug.gi.name);

    notice_button.rect.y=0-notice_button.rect.h;
    close_notice_button.rect.y=0-close_notice_button.rect.h;
}

void remove_group_member_msg()
{
    ug_info ug;
    recv(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);
    memset(notice_button.text,0,sizeof(notice_button.text));
    strcat(notice_button.text,"你被移除了群聊-");
    strcat(notice_button.text,ug.gi.name);

    
    notice_button.rect.y=0-notice_button.rect.h;
    close_notice_button.rect.y=0-close_notice_button.rect.h;

    //当前处于被移除的群聊，则清空会话状态

    if(tcpc1.g_list.len==1)
    {
        cur_group_page_first=NULL;
        group_cur_page=0;
    }
    else
    {
        if(cur_group_page_first->data.id==ug.gi.id)
        {
            if(cur_group_page_first->next!=NULL)
            {
                cur_group_page_first=cur_group_page_first->next;
            }
            else if(cur_group_page_first->last!=&(tcpc1.g_list.head)&&group_cur_page>1)
            {
                for(int i=0;i<10;i++)
                {
                    cur_group_page_first=cur_group_page_first->last;
                }
                group_cur_page--;
            }
            else
                cur_group_page_first=NULL;
        }
    }

    tcpc1.DeleteGroupList(&(ug.gi));

    if(cur_view_group!=NULL)
    {
        if(cur_view_group->data.id==ug.gi.id)
        {
            //子线程不能直接改状态，此时可能处于会话状态，直接修改可能导致主线程访问NULL指针从而崩溃，等待主线程当前循环结束再
            //自己判断并修改状态，后续改进，线程应该使用公共信号量来传递消息，而不是直接修改对方线程的状态，很容易造成线程崩溃
            cur_group_status_die=true;
        }   
    }
}

void apply_friend_permited_msg()
{
    friend_node tmp;
    recv(tcpc1.GetSocket_fd(),(char*)&tmp.data,sizeof(tmp.data),0);

    char str1[]="新的通知: ";
    char str2[]="同意了你的好友申请";
    memset(notice_button.text,0,sizeof(notice_button.text));
    strcpy(notice_button.text,str1);
    strcat(notice_button.text,tmp.data.name);
    strcat(notice_button.text,str2);

    tcpc1.InsertFriendList(&(tmp.data));
    cur_friend_page_first=tcpc1.f_list.head.next;
    friend_cur_page=1;

    notice_button.rect.y=0-notice_button.rect.h;
    close_notice_button.rect.y=0-close_notice_button.rect.h;
}

void apply_group_permited_msg()
{
    group_info tmp;
    recv(tcpc1.GetSocket_fd(),(char*)&tmp,sizeof(tmp),0);

    char str1[]="新的通知: 你的群聊申请已通过-";
    memset(notice_button.text,0,sizeof(notice_button.text));
    strcpy(notice_button.text,str1);
    strcat(notice_button.text,tmp.name);
    
    tcpc1.InsertGroupList(&(tmp));
    cur_group_page_first=tcpc1.g_list.head.next;
    group_cur_page=1;

    notice_button.rect.y=0-notice_button.rect.h;
    close_notice_button.rect.y=0-close_notice_button.rect.h;
}

void new_friend_apply_msg()
{
    friend_apply_node tmp;
    recv(tcpc1.GetSocket_fd(),(char*)&(tmp.data),sizeof(user_info),0);

    char str1[]="新的通知: ";
    char str2[]="请求添加为好友";
    memset(notice_button.text,0,sizeof(notice_button.text));
    strcpy(notice_button.text,str1);
    strcat(notice_button.text,tmp.data.name);
    strcat(notice_button.text,str2);

    tcpc1.InsertFriendApplyList(&(tmp.data));
    cur_friend_apply_page_first=tcpc1.fa_list.head.next;
    friend_apply_cur_page=1;

    notice_button.rect.y=0-notice_button.rect.h;
    close_notice_button.rect.y=0-close_notice_button.rect.h;
}

void new_group_apply_msg()
{
    group_apply_node tmp;
    recv(tcpc1.GetSocket_fd(),(char*)&(tmp.data),sizeof(ug_info),0);

    char str1[]="新的通知: ";
    char str2[]="请求加入群聊-";
    memset(notice_button.text,0,sizeof(notice_button.text));
    strcpy(notice_button.text,str1);
    strcat(notice_button.text,tmp.data.ui.name);
    strcat(notice_button.text,str2);
    strcat(notice_button.text,tmp.data.gi.name);

    tcpc1.InsertGroupApplyList(&(tmp.data));
    cur_group_apply_page_first=tcpc1.ga_list.head.next;
    group_apply_cur_page=1;

    notice_button.rect.y=0-notice_button.rect.h;
    close_notice_button.rect.y=0-close_notice_button.rect.h;
}

void delete_friend_msg()
{
    user_info u1;
    recv(tcpc1.GetSocket_fd(),(char*)&u1,sizeof(u1),0);
    need_update_friend_list=true;
    
    if(cur_view_friend!=NULL)
    {
        if(cur_view_friend->data.id==u1.id)
        {
            cur_friend_status_die=true;
        }
    }
}

void RecvMessage()
{
    int type;
    while(CUR_STATUS)
    {
        type=-999;
        int check=recv(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
        if(check==-1)
            continue;
        switch(type)
        {
            case 1:
                get_regist_msg();
                break;
            case 2:
                get_login_msg();
                break;
            case 3:
                get_apply_friend_msg();
                break;
            case 10:
                get_friend_apply_list_msg();
                break;
            case 11:
                get_friend_list_msg();
                break;
            case 12:
                get_unicast_list_msg();
                break;
            case 13:
                get_multicast_list_msg();
                break;
            case 14:
                get_create_group_msg();
                break;
            case 15:
                get_join_group_msg();
                break;
            case 16:
                get_group_list_msg();
                break;
            case 17:
                get_group_apply_list_msg();
                break;
            case 18:
                get_group_member_list_msg();
                break;
            case 23:
                get_change_passwd_msg();
                break;
            case 24:
                get_data_rule_msg();
                break;
            case -1:
                new_friend_apply_msg();
                break;
            case -2:
                apply_friend_permited_msg();
                break;
            case -3:
                new_friend_msg();
                break;
            case -4:
                new_group_msg();
                break;
            case -5:
                new_group_apply_msg();
                break;
            case -6:
                apply_group_permited_msg();
                break;
            case -7:
                member_exit_group_msg();
                break;
            case -8:
                remove_group_member_msg();
                break;
            case -9:
                delete_friend_msg();
                break;
        }
    }
}

/*****************************************-----level0-----*****************************************/

void draw_level_0_error_ban()
{
    RectButton r1(level_0_error_rect,black,white,14,red,"账号因违规行为已被封禁!");
    r1.Draw(rdr);
}

void draw_level_0_error_0()
{
    RectButton r1(level_0_error_rect,black,white,14,red,"操作成功！");
    r1.Draw(rdr);
}

void draw_level_0_error_1()
{
    RectButton r1(level_0_error_rect,black,white,14,red,"账号格式错误！允许的字符：字母 数字 _ @ # & *");
    r1.Draw(rdr);
}

void draw_level_0_error_2()
{
    RectButton r1(level_0_error_rect,black,white,14,red,"账号已经存在！");
    r1.Draw(rdr);
}

void draw_level_0_error_3()
{
    RectButton r1(level_0_error_rect,black,white,14,red,"密码格式错误！允许的字符：字母 数字 _ @ # & *");
    r1.Draw(rdr);
}

void draw_level_0_error_4()
{
    RectButton r1(level_0_error_rect,black,white,14,red,"账号不存在！");
    r1.Draw(rdr);
}

void draw_level_0_error_5()
{
    RectButton r1(level_0_error_rect,black,white,14,red,"密码错误！");
    r1.Draw(rdr);
}

void draw_level_0_error_8()
{
    RectButton r1(level_0_error_rect,black,white,14,red,"账号正在使用中！");
    r1.Draw(rdr);
}

void draw_level_0_error_12()
{
    RectButton r1(level_0_error_rect,black,white,14,red,"密码不一致！");
    r1.Draw(rdr);
}

void draw_level_0_error()
{
    switch(cur_error.value)
    {
        case -1:
            draw_level_0_error_ban();
            break;
        case 0:
            draw_level_0_error_0();
            break;
        case 1:
            draw_level_0_error_1();
            break;
        case 2:
            draw_level_0_error_2();
            break;
        case 3:
            draw_level_0_error_3();
            break;
        case 4:
            draw_level_0_error_4();
            break;
        case 5:
            draw_level_0_error_5();
            break;
        case 8:
            draw_level_0_error_8();
            break;
        case 12:
            draw_level_0_error_12();
            break;
    }
}

void draw_level_connect_error()
{
    SDL_SetRenderDrawColor(rdr,245,245,235,255);
    SDL_RenderClear(rdr);

    Text t1(TITLE_X-110,TITLE_Y+100,red,30,"连接失败");
    Text t2(TITLE_X-110,TITLE_Y+175,black,16,"1.网络环境不佳，请稍后重试...");
    Text t3(TITLE_X-110,TITLE_Y+225,black,16,"2.服务器关闭，请联系管理员(18271312836@163.com)");
    
    SDL_Rect rect1={TITLE_X-120,TITLE_Y+150,400,125};
    RectButton r1(rect1,black,white,20,black,NULL);

    r1.Draw(rdr);
    t1.Draw(rdr);
    t2.Draw(rdr);
    t3.Draw(rdr);

    SDL_RenderPresent(rdr);
}

void draw_level_0_regist()
{
    SDL_SetRenderDrawColor(rdr,245,245,235,255);
    SDL_RenderClear(rdr);

    r_regist.Draw(rdr);
    r_login.Draw(rdr);
    r_commit.Draw(rdr);
    r_admin.Draw(rdr);
    r_passwd.Draw(rdr);
    r_name.Draw(rdr);
    r_repasswd.Draw(rdr);

    t_title.Draw(rdr);
    t_admin.Draw(rdr);
    t_passwd.Draw(rdr);
    t_name.Draw(rdr);
    t_repasswd.Draw(rdr);

    if(strlen(admin_buffer)>0)
    {
        Text text(ADMIN_INPUT_X+3,ADMIN_INPUT_Y+8,black,14,admin_buffer);
        text.Draw(rdr);
    }

    if(strlen(passwd_buffer)>0)
    {
        for(int i=0;i<strlen(passwd_buffer);i++)
        {
            passwd_temp_buffer[i]='*';
        }

        Text text(PASSWD_INPUT_X+3,PASSWD_INPUT_Y+8,black,14,passwd_temp_buffer);
        text.Draw(rdr);
        memset(passwd_temp_buffer,0,sizeof(passwd_temp_buffer));
    }

    if(strlen(name_buffer)>0)
    {
        Text text(NAME_INPUT_X+3,NAME_INPUT_Y+8,black,14,name_buffer);
        text.Draw(rdr);
    }   

    if(strlen(repasswd_buffer)>0)
    {
        for(int i=0;i<strlen(repasswd_buffer);i++)
        {
            passwd_temp_buffer[i]='*';
        }

        Text text(REPASSWD_INPUT_X+3,REPASSWD_INPUT_Y+8,black,14,passwd_temp_buffer);
        text.Draw(rdr);
        memset(passwd_temp_buffer,0,sizeof(passwd_temp_buffer));
    }

    draw_level_0_error();
    SDL_RenderPresent(rdr);
    SDL_Delay(10);
}

void draw_level_0_login()
{
    SDL_SetRenderDrawColor(rdr,245,245,235,255);
    SDL_RenderClear(rdr);

    r_regist.Draw(rdr);
    r_login.Draw(rdr);
    r_commit.Draw(rdr);
    r_admin.Draw(rdr);
    r_passwd.Draw(rdr);

    t_title.Draw(rdr);
    t_admin.Draw(rdr);
    t_passwd.Draw(rdr);

    if(strlen(admin_buffer)>0)
    {
        Text text(ADMIN_INPUT_X+3,ADMIN_INPUT_Y+8,black,14,admin_buffer);
        text.Draw(rdr);
    }

    if(strlen(passwd_buffer)>0)
    {
        for(int i=0;i<strlen(passwd_buffer);i++)
        {
            passwd_temp_buffer[i]='*';
        }

        Text text(PASSWD_INPUT_X+3,PASSWD_INPUT_Y+8,black,14,passwd_temp_buffer);
        text.Draw(rdr);
        memset(passwd_temp_buffer,0,sizeof(passwd_temp_buffer));
    }

    draw_level_0_error();
    SDL_RenderPresent(rdr);
    SDL_Delay(10);
}

void event_loop_connect_error()
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                CUR_STATUS=LEVEL_EXIT;
                break;
        }
    }
}

//event_loop_level0
//已通过图形化的方式限制了输入长度，无需再检查
void event_loop_regist()
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                CUR_STATUS=LEVEL_EXIT;
                break;
            case SDL_MOUSEMOTION:
                mouse_pt.x=event.motion.x,mouse_pt.y=event.motion.y;
                r_regist.InteractResponse(mouse_pt,regist_rect,white,green);
                r_login.InteractResponse(mouse_pt,login_rect,white,green);
                r_commit.InteractResponse(mouse_pt,commit_rect,white,green);
                break;
            case SDL_WINDOWEVENT:
                if(event.window.event==SDL_WINDOWEVENT_LEAVE)
                    ResetButton();
                if(event.window.event==SDL_WINDOWEVENT_ENTER)
                    ;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_pt.x=event.motion.x,mouse_pt.y=event.motion.y;
                if(SDL_PointInRect(&mouse_pt,&regist_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    clear_input_buffer();
                    CUR_STATUS=LEVEL_0_REGIST;
                    std::cout<<"已切换为注册模块"<<std::endl;
                }

                if(SDL_PointInRect(&mouse_pt,&login_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    clear_input_buffer();
                    CUR_STATUS=LEVEL_0_LOGIN;
                    std::cout<<"已切换为登录模块"<<std::endl;
                }

                if
                (
                    SDL_PointInRect(&mouse_pt,&commit_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&
                    strlen(passwd_buffer)>0&&strlen(admin_buffer)>0&&strlen(name_buffer)>0&&strlen(repasswd_buffer)>0
                )
                {
                    if(strcmp(passwd_buffer,repasswd_buffer))
                    {
                        cur_error.value=12;
                        clear_input_buffer();
                        return;
                    }

                    int type=1;
                    user_info ug;
                    strcpy(ug.admin,admin_buffer);
                    strcpy(ug.passwd,passwd_buffer);
                    strcpy(ug.name,name_buffer);

                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);

                    r_commit.fill_color.r=255;
                    r_commit.fill_color.g=255;
                    r_commit.fill_color.b=255;
                    clear_input_buffer();
                }

                if(SDL_PointInRect(&mouse_pt,&admin_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    SDL_Rect rect=admin_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);

                    r_admin.border_color.g=220;
                    admin_editing=true;
                }
                else
                {
                    admin_editing=false;
                    r_admin.border_color.g=0;
                }

                if(SDL_PointInRect(&mouse_pt,&passwd_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    SDL_Rect rect=passwd_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);

                    r_passwd.border_color.g=220;
                    passwd_editing=true;
                }
                else
                {
                    passwd_editing=false;
                    r_passwd.border_color.g=0;
                }
                    

                if(SDL_PointInRect(&mouse_pt,&name_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    SDL_Rect rect=name_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);

                    r_name.border_color.g=220;
                    name_editing=true;
                }
                else
                {
                    name_editing=false;
                    r_name.border_color.g=0;
                }

                if(SDL_PointInRect(&mouse_pt,&repasswd_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    SDL_Rect rect=repasswd_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);

                    r_repasswd.border_color.g=220;
                    repasswd_editing=true;
                }
                else
                {
                    repasswd_editing=false;
                    r_repasswd.border_color.g=0;
                }
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_BACKSPACE) 
                {
                    int len;
                    if(admin_editing)
                    {
                        len=strlen(admin_buffer);
                        cur_buffer=admin_buffer;
                    } 
                    else if(passwd_editing)
                    {
                        len=strlen(passwd_buffer);
                        cur_buffer=passwd_buffer;
                    }
                    else if(name_editing)
                    {
                        len=strlen(name_buffer);
                        cur_buffer=name_buffer;
                    }
                    else if(repasswd_editing)
                    {
                        len=strlen(repasswd_buffer);
                        cur_buffer=repasswd_buffer;
                    }

                    if(len == 0) return;
    
                    if(len >= 3) 
                    {
                        unsigned char ch = cur_buffer[len-3];
                        //中文UTF-8首字节范围
                        if((ch & 0xE0) == 0xE0 && ch != 0xC0 && ch != 0xC1)
                        {
                            cur_buffer[len-3] = '\0';//删3字节
                            return;
                        }
                    }
                    cur_buffer[len-1] = '\0';//删1字节
                }
                break;
            case SDL_TEXTEDITING:
                //std::cout<<event.edit.text<<std::endl;
                break;
            case SDL_TEXTINPUT:
                if(admin_editing&&strlen(admin_buffer)+strlen(event.text.text)<=15)
                {
                    strcat(admin_buffer,event.text.text);
                }
                if(passwd_editing&&strlen(passwd_buffer)+strlen(event.text.text)<=15)
                {
                    strcat(passwd_buffer,event.text.text);
                }

                int len=0;
                for(int i=0;i<strlen(name_buffer);i++)
                {
                    if((name_buffer[i]&0xE0)==0xE0&&name_buffer[i]!=0xC0&&name_buffer[i]!=0xC1)
                    {
                        len+=3;//UTF-8中文统计为3个字节
                        i+=2;  //跳过一个完整的中文字符
                    }
                    else
                        len+=2;//数字字母其他统计为2个字节
                }

                for(int i=0;i<strlen(event.text.text);i++)
                {
                    if((event.text.text[i]&0xE0)==0xE0&&event.text.text[i]!=0xC0&&event.text.text[i]!=0xC1)
                    {
                        len+=3;//UTF-8中文统计为3个字节
                        i+=2;  //跳过一个完整的中文字符
                    }
                    else
                        len+=2;//数字字母其他统计为2个字节
                }

                if(name_editing&&len<=30)
                {    
                    strcat(name_buffer,event.text.text);
                }

                if(repasswd_editing&&strlen(repasswd_buffer)+strlen(event.text.text)<=15)
                {
                    strcat(repasswd_buffer,event.text.text);
                }
                break;
        }
    }
}

void event_loop_login()
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                CUR_STATUS=LEVEL_EXIT;
                break;
            case SDL_MOUSEMOTION:
                mouse_pt.x=event.motion.x,mouse_pt.y=event.motion.y;
                r_regist.InteractResponse(mouse_pt,regist_rect,white,green);
                r_login.InteractResponse(mouse_pt,login_rect,white,green);
                r_commit.InteractResponse(mouse_pt,commit_rect,white,green);
    
                break;
            case SDL_WINDOWEVENT:
                if(event.window.event==SDL_WINDOWEVENT_LEAVE)
                    ResetButton();
                if(event.window.event==SDL_WINDOWEVENT_ENTER)
                    ;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_pt.x=event.motion.x,mouse_pt.y=event.motion.y;
                if(SDL_PointInRect(&mouse_pt,&regist_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    clear_input_buffer();
                    CUR_STATUS=LEVEL_0_REGIST;
                    std::cout<<"已切换为注册模块"<<std::endl;
                }

                if(SDL_PointInRect(&mouse_pt,&login_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    clear_input_buffer();
                    CUR_STATUS=LEVEL_0_LOGIN;
                    std::cout<<"已切换为登录模块"<<std::endl;
                }

                if
                (
                    SDL_PointInRect(&mouse_pt,&commit_rect)&&(event.button.button==SDL_BUTTON_LEFT)
                    &&strlen(admin_buffer)>0&&strlen(passwd_buffer)>0
                )
                {
                    int type=2;
                    user_info ug;
                    strcpy(ug.admin,admin_buffer);
                    strcpy(ug.passwd,passwd_buffer);

                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);

                    r_commit.fill_color.r=255;
                    r_commit.fill_color.g=255;
                    r_commit.fill_color.b=255;
                    clear_input_buffer();
                }

                if(SDL_PointInRect(&mouse_pt,&admin_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    SDL_Rect rect=admin_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);

                    r_admin.border_color.g=220;
                    admin_editing=true;
                }
                else
                {
                    admin_editing=false;
                    r_admin.border_color.g=0;
                }

                if(SDL_PointInRect(&mouse_pt,&passwd_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    SDL_Rect rect=passwd_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);

                    r_passwd.border_color.g=220;
                    passwd_editing=true;
                }
                else
                {
                    passwd_editing=false;
                    r_passwd.border_color.g=0;
                }
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_BACKSPACE) 
                {
                    int len;
                    if(admin_editing)
                    {
                        len=strlen(admin_buffer);
                        cur_buffer=admin_buffer;
                    } 
                    else if(passwd_editing)
                    {
                        len=strlen(passwd_buffer);
                        cur_buffer=passwd_buffer;
                    }

                    if(len == 0) return;
    
                    if(len >= 3) 
                    {
                        unsigned char ch = cur_buffer[len-3];
                        //中文UTF-8首字节范围
                        if((ch & 0xE0) == 0xE0 && ch != 0xC0 && ch != 0xC1)
                        {
                            cur_buffer[len-3] = '\0';//删3字节
                            return;
                        }
                    }
                    cur_buffer[len-1] = '\0';//删1字节
                }
                break;
            case SDL_TEXTEDITING:
                //std::cout<<event.edit.text<<std::endl;
                break;
            case SDL_TEXTINPUT:
                if(admin_editing&&strlen(admin_buffer)+strlen(event.text.text)<=15)
                {
                    strcat(admin_buffer,event.text.text);
                }
                if(passwd_editing&&strlen(passwd_buffer)+strlen(event.text.text)<=15)
                {
                    strcat(passwd_buffer,event.text.text);
                }
                break;
        }
    }
}

/*****************************************-----level1-----*****************************************/

void draw_level_1_error_0()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"操作成功!");
    t1.Draw(rdr);
}

void draw_level_1_error_1()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"账号格式错误！");
    Text t2(level_1_error_rect.x,level_1_error_rect.y+20,red,16,"允许的字符：字母 数字 _ @ # & *");
    
    t1.Draw(rdr);
    t2.Draw(rdr);
}

void draw_level_1_error_2()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"账号已经存在!");
    t1.Draw(rdr);
}

void draw_level_1_error_4()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"账号不存在!");
    t1.Draw(rdr);
}

void draw_level_1_error_6()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"已发送过申请!");
    t1.Draw(rdr);
}

void draw_level_1_error_9()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"已经是好友!");
    t1.Draw(rdr);
}

void draw_level_1_error_10()
{
    char buffer[256]="创建的群聊过多!最大数:";
    char str1[32];
    snprintf(str1, sizeof(str1),"%d",dr.group_nums);
    strcat(buffer,str1);
    
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,buffer);
    t1.Draw(rdr);
}

void draw_level_1_error_11()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"已在群聊中!");
    t1.Draw(rdr);
}

void draw_level_1_error_13()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"目标为当前账号!");
    t1.Draw(rdr);
}

void GetListPageToButton(int cur,int total,RectButton& des_button,const char* front_str)//the front of '/' 's char
{
    char final_str[32];
    char str1[32];
    char str2[32];
    
    snprintf(str1, sizeof(str1),"%d",cur);
    snprintf(str2, sizeof(str2),"%d",total);

    strcpy(final_str,front_str);
    strcat(final_str,str1);
    strcat(final_str," / ");
    strcat(final_str,str2);

    strcpy(des_button.text,final_str);
}

void draw_level_1_error()
{
    switch(cur_error.value)
    {
        case 0:
            draw_level_1_error_0();
            break;
        case 1:
            draw_level_1_error_1();
            break;
        case 2:
            draw_level_1_error_2();
            break;
        case 3:
            ;
            break;
        case 4:
            draw_level_1_error_4();
            break;
        case 5:
            ;
        case 6:
            draw_level_1_error_6();
            break;
        case 8:
            ;
            break;
        case 9:
            draw_level_1_error_9();
            break;
        case 10:
            draw_level_1_error_10();
            break;
        case 11:
            draw_level_1_error_11();
            break;
        case 13:
            draw_level_1_error_13();
            break;
    }
}

void draw_level_1_add_friend()
{
    //animation
    if(tool_all_button.rect.x>175)
        tool_all_button.rect.x-=25;

    if(add_friend_text.text_rect.x>250)
        add_friend_text.text_rect.x-=25;

    if(add_friend_input_button.rect.x>350)
        add_friend_input_button.rect.x-=25;

    if(close_page_button.rect.x>175)
        close_page_button.rect.x-=25;

    if(commit_page_button.rect.x>525)
        commit_page_button.rect.x-=25;
    
    if(add_friend_title.text_rect.x>325)
        add_friend_title.text_rect.x-=25;

    
    tool_all_button.Draw(rdr);
    add_friend_title.Draw(rdr);

    add_friend_text.Draw(rdr);
    add_friend_input_button.Draw(rdr);

    close_page_button.Draw(rdr);
    commit_page_button.Draw(rdr);
}

void draw_level_1_join_group()
{
    //animation
    if(tool_all_button.rect.x>175)
        tool_all_button.rect.x-=25;

    if(join_group_text.text_rect.x>250)
        join_group_text.text_rect.x-=25;

    if(join_group_input_button.rect.x>350)
        join_group_input_button.rect.x-=25;

    if(close_page_button.rect.x>175)
        close_page_button.rect.x-=25;

    if(commit_page_button.rect.x>525)
        commit_page_button.rect.x-=25;
    
    if(join_group_title.text_rect.x>325)
        join_group_title.text_rect.x-=25;


    tool_all_button.Draw(rdr);
    join_group_title.Draw(rdr);

    join_group_text.Draw(rdr);
    join_group_input_button.Draw(rdr);

    close_page_button.Draw(rdr);
    commit_page_button.Draw(rdr);
}

void draw_level_1_create_group()
{
    if(tool_all_button.rect.x>175)
        tool_all_button.rect.x-=25;

    if(close_page_button.rect.x>175)
        close_page_button.rect.x-=25;

    if(commit_page_button.rect.x>525)
        commit_page_button.rect.x-=25;
    
    if(create_group_title.text_rect.x>325)
        create_group_title.text_rect.x-=25;


    if(create_group_admin_text.text_rect.x>250)
        create_group_admin_text.text_rect.x-=25;

    if(create_group_admin_input_button.rect.x>350)
        create_group_admin_input_button.rect.x-=25;

    if(create_group_name_text.text_rect.x>250)
        create_group_name_text.text_rect.x-=25;

    if(create_group_name_input_button.rect.x>350)
        create_group_name_input_button.rect.x-=25;


    tool_all_button.Draw(rdr);
    create_group_title.Draw(rdr);

    create_group_admin_text.Draw(rdr);
    create_group_admin_input_button.Draw(rdr);

    create_group_name_text.Draw(rdr);
    create_group_name_input_button.Draw(rdr);

    close_page_button.Draw(rdr);
    commit_page_button.Draw(rdr);
}

void draw_level_1_friend_apply_list()
{
    //animation
    if(tool_all_button.rect.x>175)
        tool_all_button.rect.x-=25;

    if(friend_apply_title.text_rect.x>325)
        friend_apply_title.text_rect.x-=25;

    if(apply_control_last_button.rect.x>225)
        apply_control_last_button.rect.x-=25;

    if(apply_control_page_button.rect.x>275)
        apply_control_page_button.rect.x-=25;

    if(apply_control_next_button.rect.x>475)
        apply_control_next_button.rect.x-=25;

    if(close_page_button.rect.x>175)
        close_page_button.rect.x-=25;

    if(clear_page_button.rect.x>525)
        clear_page_button.rect.x-=25;

    for(int i=0;i<10;i++)
    {
        if(friend_apply_button[i].rect.x>friend_apply_rect[i].x)
            friend_apply_button[i].rect.x-=25;

        if(permit_button[i].rect.x>permit_apply_rect[i].x)
            permit_button[i].rect.x-=25;

        if(refuse_button[i].rect.x>refuse_apply_rect[i].x)
            refuse_button[i].rect.x-=25;
    }
    //animation

    tool_all_button.Draw(rdr);
    friend_apply_title.Draw(rdr);
    
    friend_apply_node* ptr3=cur_friend_apply_page_first;
    for(int i=0;i<10;i++)
    {
        if(ptr3!=NULL)
        {
            strcpy(friend_apply_button[i].text,ptr3->data.name);
            ptr3=ptr3->next;
        }

        if(strlen(friend_apply_button[i].text)>0)
        {
            friend_apply_button[i].Draw(rdr);
            permit_button[i].Draw(rdr);
            refuse_button[i].Draw(rdr);
        }

        memset(friend_apply_button[i].text,0,sizeof(friend_apply_button[i].text));
    }

    friend_apply_total_page=tcpc1.fa_list.len/10;
    if(tcpc1.fa_list.len-friend_apply_total_page*10>0)
        friend_apply_total_page++;
    GetListPageToButton(friend_apply_cur_page,friend_apply_total_page,apply_control_page_button,"         ");
    
    apply_control_last_button.Draw(rdr);
    apply_control_page_button.Draw(rdr);
    apply_control_next_button.Draw(rdr);

    close_page_button.Draw(rdr);
    clear_page_button.Draw(rdr);
}

void draw_level_1_group_apply_list()
{
    //animation
    if(tool_all_button.rect.x>175)
        tool_all_button.rect.x-=25;

    if(group_apply_title.text_rect.x>325)
        group_apply_title.text_rect.x-=25;

    if(apply_control_last_button.rect.x>225)
        apply_control_last_button.rect.x-=25;

    if(apply_control_page_button.rect.x>275)
        apply_control_page_button.rect.x-=25;

    if(apply_control_next_button.rect.x>475)
        apply_control_next_button.rect.x-=25;

    if(close_page_button.rect.x>175)
        close_page_button.rect.x-=25;

    if(clear_page_button.rect.x>525)
        clear_page_button.rect.x-=25;

    for(int i=0;i<10;i++)
    {
        if(group_apply_button[i].rect.x>group_apply_rect[i].x)
            group_apply_button[i].rect.x-=25;

        if(permit_button[i].rect.x>permit_apply_rect[i].x)
            permit_button[i].rect.x-=25;

        if(refuse_button[i].rect.x>refuse_apply_rect[i].x)
            refuse_button[i].rect.x-=25;
    }
    //animation

    tool_all_button.Draw(rdr);
    group_apply_title.Draw(rdr);

    group_apply_node* ptr4=cur_group_apply_page_first;
    for(int i=0;i<10;i++)
    {
        if(ptr4!=NULL)
        {
            strcpy(group_apply_button[i].text,ptr4->data.ui.name);
            strcat(group_apply_button[i].text,"请求加入群聊");
            strcat(group_apply_button[i].text,ptr4->data.gi.name);

            ptr4=ptr4->next;
        }

        if(strlen(group_apply_button[i].text)>0)
        {
            group_apply_button[i].Draw(rdr);
            permit_button[i].Draw(rdr);
            refuse_button[i].Draw(rdr);
        }
        memset(group_apply_button[i].text,0,sizeof(group_apply_button[i].text));
    }

    group_apply_total_page=tcpc1.ga_list.len/10;
    if(tcpc1.ga_list.len-group_apply_total_page*10>0)
        group_apply_total_page++;
    GetListPageToButton(group_apply_cur_page,group_apply_total_page,apply_control_page_button,"         ");

    apply_control_last_button.Draw(rdr);
    apply_control_page_button.Draw(rdr);
    apply_control_next_button.Draw(rdr);

    close_page_button.Draw(rdr);
    clear_page_button.Draw(rdr);
}

void draw_level_1_setting()
{
    if(tool_all_button.rect.x>175)
        tool_all_button.rect.x-=25;

    if(setting_title.text_rect.x>325)
        setting_title.text_rect.x-=25;

    if(cur_admin_text.text_rect.x>175)
        cur_admin_text.text_rect.x-=25;

    if(exit_login_button.rect.x>325)
        exit_login_button.rect.x-=25;
    
    if(close_page_button.rect.x>175)
        close_page_button.rect.x-=25;

    if(help_button.rect.x>325)
        help_button.rect.x-=25;
    
    if(about_me_button.rect.x>325)
        about_me_button.rect.x-=25;
    
    if(version_button.rect.x>325)
        version_button.rect.x-=25;
    
    if(change_passwd_button.rect.x>325)
        change_passwd_button.rect.x-=25;
    
    if(change_style_button.rect.x>325)
        change_style_button.rect.x-=25;

    tool_all_button.Draw(rdr);
    setting_title.Draw(rdr);
    close_page_button.Draw(rdr);

    cur_admin_text.Draw(rdr);
    exit_login_button.Draw(rdr);
    help_button.Draw(rdr);
    about_me_button.Draw(rdr);
    version_button.Draw(rdr);
    change_passwd_button.Draw(rdr);
    change_style_button.Draw(rdr);
}

void draw_level_1_friend_msg()
{
    //animation
    if(msg_base_button.rect.x>175)
        msg_base_button.rect.x-=25;
    
    if(msg_title.text_rect.x>175)
        msg_title.text_rect.x-=25;

    if(msg_more_button.rect.x>175)
        msg_more_button.rect.x-=25;

    if(msg_send_button.rect.x>225)
        msg_send_button.rect.x-=25;

    if(msg_close_button.rect.x>475)
        msg_close_button.rect.x-=25;

    if(msg_input_button.rect.x>175)
        msg_input_button.rect.x-=25;
    //animation

    msg_base_button.Draw(rdr);

    //animation is finished，then display msg content
    if(cur_view_friend!=NULL&&msg_base_button.rect.x==175)
    {
        unicast_node* view=tcpc1.u_list.tail;
        msg_pos=msg_start;
        msg_total_len=0;

        while(view!=NULL&&view!=&(tcpc1.u_list.head))
        {
            if(view->data.des_id==cur_view_friend->data.id||view->data.sou_id==cur_view_friend->data.id)
            {
                memset(mine_msg.text,0,sizeof(mine_msg.text));
                memset(target_msg.text,0,sizeof(target_msg.text));

                int total_weight=0;
                SDL_Surface* surf = TTF_RenderUTF8_Blended(g_font, view->data.msg, white);
                if (surf)
                {
                    //文本总宽度
                    total_weight=surf->w;
                    SDL_FreeSurface(surf);
                }

                //获取当前消息的行数
                int lines=total_weight/(225);
                if(lines*225<total_weight)
                    lines++;

                if(cur_user.id==view->data.sou_id)
                {
                    strcpy(mine_msg.text,view->data.msg);
                    mine_msg.rect.h=(lines*15);
                    if(lines!=1)
                        mine_msg.rect.h+=26;
                    else
                        mine_msg.rect.h+=10;

                    //更新总长度以及当前滚动条位置
                    msg_pos-=(mine_msg.rect.h+5);
                    msg_total_len+=(mine_msg.rect.h+5);
                    mine_msg.rect.y=msg_pos;

                    //减少性能消耗,只在范围内的消息才渲染输出
                    if(mine_msg.rect.y>=75&&mine_msg.rect.y<575)
                        mine_msg.Draw(rdr,225);
                }
                else
                {
                    strcpy(target_msg.text,view->data.msg);
                    target_msg.rect.h=(lines*15);
                    
                    if(lines!=1)
                        target_msg.rect.h+=26;
                    else
                        target_msg.rect.h+=10;


                    msg_pos-=(target_msg.rect.h+5);
                    msg_total_len+=(target_msg.rect.h+5);
                    target_msg.rect.y=msg_pos;

                    if(target_msg.rect.y>=75&&target_msg.rect.y<575)
                        target_msg.Draw(rdr,225);
                }
            }

            //头插法，最新消息位于链表的末尾
            view=view->last;
        }

        //重置对话框组件的y坐标
        mine_msg.rect.y=mine_msg_rect.y;
        target_msg.rect.y=target_msg_rect.y;
    }

    msg_title.Draw(rdr);
    msg_more_button.Draw(rdr);
    msg_send_button.Draw(rdr);
    msg_close_button.Draw(rdr);
    msg_input_button.Draw(rdr);
    
    //input_mod
    if(strlen(message_buffer)>0)
    {
        MessageText text(msg_input_rect.x+5,msg_input_rect.y+5,black,18,message_buffer);
        text.Draw(rdr,msg_input_button.rect.w-5);
    }

    if(msg_friend_more_tool)
    {
        msg_more_delete_friend_button.Draw(rdr);
    }
}

void draw_level_1_group_msg()
{
    //animation
    if(msg_base_button.rect.x>175)
        msg_base_button.rect.x-=25;
    
    if(msg_title.text_rect.x>175)
        msg_title.text_rect.x-=25;

    if(msg_more_button.rect.x>175)
        msg_more_button.rect.x-=25;

    if(msg_send_button.rect.x>225)
        msg_send_button.rect.x-=25;

    if(msg_close_button.rect.x>475)
        msg_close_button.rect.x-=25;

    if(msg_input_button.rect.x>175)
        msg_input_button.rect.x-=25;
    //animation

    msg_base_button.Draw(rdr);

    //animation is finished，then display msg content
    if(cur_view_group!=NULL&&msg_base_button.rect.x==175)
    {
        multicast_node* view=tcpc1.m_list.tail;
        msg_pos=msg_start;
        msg_total_len=0;

        while(view!=NULL&&view!=&(tcpc1.m_list.head))
        {
            if(view->data.group_id==cur_view_group->data.id)
            {
                memset(mine_msg.text,0,sizeof(mine_msg.text));
                memset(target_msg.text,0,sizeof(target_msg.text));
                memset(target_msg_title.text,0,sizeof(target_msg_title.text));

                int total_weight=0;
                SDL_Surface* surf = TTF_RenderUTF8_Blended(g_font, view->data.msg, white);
                if (surf)
                {
                    //文本总宽度
                    total_weight=surf->w;
                    SDL_FreeSurface(surf);
                }

                //获取当前消息的行数
                int lines=total_weight/(225);
                if(lines*225<total_weight)
                    lines++;

                strcpy(target_msg_title.text,view->data.member_name);

                if(cur_user.id==view->data.member_id)
                {
                    strcpy(mine_msg.text,view->data.msg);
                    mine_msg.rect.h=(lines*15);

                    if(lines!=1)
                        mine_msg.rect.h+=26;
                    else
                        mine_msg.rect.h+=10;

                    //更新总长度以及当前滚动条位置
                    msg_pos-=(mine_msg.rect.h+5);
                    msg_total_len+=(mine_msg.rect.h+5);
                    mine_msg.rect.y=msg_pos;

                    //减少性能消耗,只在范围内的消息才渲染输出
                    if(mine_msg.rect.y>=75&&mine_msg.rect.y<575)
                        mine_msg.Draw(rdr,225);
                }
                else
                {
                    strcpy(target_msg.text,view->data.msg);
                    target_msg.rect.h=(lines*15);

                    if(lines!=1)
                        target_msg.rect.h+=26;
                    else
                        target_msg.rect.h+=10;

                    msg_pos-=(target_msg.rect.h+5);
                    msg_total_len+=(target_msg.rect.h+5);
                    target_msg.rect.y=msg_pos;

                    if(target_msg.rect.y>=75&&target_msg.rect.y<575)
                        target_msg.Draw(rdr,225);

                    msg_pos-=(target_msg_title.text_rect.h+5);    
                    msg_total_len+=(target_msg_title.text_rect.h+5);
                    target_msg_title.text_rect.y=msg_pos;
                    
                    if(target_msg_title.text_rect.y>=75&&target_msg_title.text_rect.y<575)
                        target_msg_title.Draw(rdr);
                }
            }
            
            //头插法，最新消息位于链表的末尾
            view=view->last;
        }
        

        //重置对话框组件的y坐标
        mine_msg.rect.y=mine_msg_rect.y;
        target_msg.rect.y=target_msg_rect.y;
    }

    msg_title.Draw(rdr);
    msg_more_button.Draw(rdr);
    msg_send_button.Draw(rdr);
    msg_close_button.Draw(rdr);
    msg_input_button.Draw(rdr);

    //input_mod
    if(strlen(message_buffer)>0)
    {
        MessageText text(msg_input_rect.x+5,msg_input_rect.y+5,black,18,message_buffer);
        text.Draw(rdr,msg_input_button.rect.w-5);
    }

    if(msg_group_more_tool)
    {
        msg_more_group_member_button.Draw(rdr);

        if(cur_view_group->data.manager_id!=cur_user.id)//群主不能退出群聊
            msg_more_exit_group_button.Draw(rdr);
    }
}

void draw_level_1_group_member_msg()
{
    group_member_base_button.Draw(rdr);
    msg_title.Draw(rdr);

    group_member_total_page=tcpc1.gm_list.len/25;
    if(tcpc1.gm_list.len-group_member_total_page*25>0)
        group_member_total_page++;
    GetListPageToButton(group_member_cur_page,group_member_total_page,gm_pages_button,"               ");

    gm_last_page_button.Draw(rdr);
    gm_pages_button.Draw(rdr);
    gm_next_page_button.Draw(rdr);

    group_member_node* tmp5=cur_group_member_page_first;
    for(int i=0;i<25;i++)
    {
        memset(group_member_button[i].text,0,sizeof(group_member_button[i].text));

        if(tmp5!=NULL)
        {
            memset(group_member_type_button[i].text,0,sizeof(group_member_type_button[i].text));
            if(tmp5->data.id==cur_view_group->data.manager_id)
                strcpy(group_member_type_button[i].text,"群主");
            else
                strcpy(group_member_type_button[i].text,"成员");

            strcpy(group_member_button[i].text,tmp5->data.name);
            tmp5=tmp5->next;
        }

        if(strlen(group_member_button[i].text)>0)
        {
            group_member_button[i].Draw(rdr);
            group_member_type_button[i].Draw(rdr);
        
            if(cur_user.id==cur_view_group->data.manager_id)
                remove_member_button[i].Draw(rdr);
        }
    }

    
    if(event_remove_manager)
    {
        Text t1(180,52,red,18,"群主不能退出群聊");
        t1.Draw(rdr);
    }
}

void draw_level_1_help()
{
    msg_setting_base_button.Draw(rdr);
    help_title.Draw(rdr);
    help_c1.Draw(rdr);
    help_c2.Draw(rdr);
    help_c3.Draw(rdr);
    help_c4.Draw(rdr);
    help_c5.Draw(rdr);
    help_c6.Draw(rdr);
    help_c7.Draw(rdr);
    help_c8.Draw(rdr);
    help_c9.Draw(rdr);
    help_c10.Draw(rdr);
    help_c11.Draw(rdr);
    help_c12.Draw(rdr);
    help_c13.Draw(rdr);
}

void draw_level_1_version()
{
    msg_setting_base_button.Draw(rdr);
    version_title.Draw(rdr);
    version_c1.Draw(rdr);
    version_c2.Draw(rdr);
    version_c3.Draw(rdr);
}

void draw_level_1_about_me()
{
    msg_setting_base_button.Draw(rdr);
    about_me_title.Draw(rdr);
    about_me_c1.Draw(rdr);
    about_me_c2.Draw(rdr);
    about_me_c3.Draw(rdr);
    about_me_c4.Draw(rdr);
    about_me_c5.Draw(rdr);
    about_me_c6.Draw(rdr);
    about_me_c7.Draw(rdr);
    about_me_c8.Draw(rdr);
    about_me_c9.Draw(rdr);
    about_me_c10.Draw(rdr);
    about_me_c11.Draw(rdr);
}

void draw_level_1_change_passwd()
{
    msg_setting_base_button.Draw(rdr);
    change_passwd_title.Draw(rdr);

    input_new_passwd1_title.Draw(rdr);
    input_new_passwd2_title.Draw(rdr);
    input_new_passwd1_button.Draw(rdr);
    input_new_passwd2_button.Draw(rdr);
    commit_new_passwd_button.Draw(rdr);

    if(strlen(passwd_buffer)>0)
    {
        for(int i=0;i<strlen(passwd_buffer);i++)
        {
            passwd_temp_buffer[i]='*';
        }

        Text text(input_new_passwd1_button.rect.x+3,input_new_passwd1_button.rect.y+8,black,14,passwd_temp_buffer);
        text.Draw(rdr);
        memset(passwd_temp_buffer,0,sizeof(passwd_temp_buffer));
    }

    if(strlen(repasswd_buffer)>0)
    {
        for(int i=0;i<strlen(repasswd_buffer);i++)
        {
            passwd_temp_buffer[i]='*';
        }

        Text text(input_new_passwd2_button.rect.x+3,input_new_passwd2_button.rect.y+8,black,14,passwd_temp_buffer);
        text.Draw(rdr);
        memset(passwd_temp_buffer,0,sizeof(passwd_temp_buffer));
    }

    switch(cur_error.value)
    {
        case 0:
            tt0.Draw(rdr);
            break;
        case 3:
            tt31.Draw(rdr);
            tt32.Draw(rdr);
            break;
        case 12:
            tt12.Draw(rdr);
            break;
    }
}

void draw_level_1_index()
{
    //background
    SDL_SetRenderDrawColor(rdr,245,245,230,255);
    SDL_RenderClear(rdr);

    //friend_mod
    friend_title_text.Draw(rdr);
    

    friend_node* ptr=cur_friend_page_first;

    for(int i=0;i<10;i++)
    {
        if(ptr!=NULL)
        {
            strcpy(friend_content_button[i].text,ptr->data.name);
            ptr=ptr->next;
        }
        friend_content_button[i].Draw(rdr);
        
    }

    friend_total_page=tcpc1.f_list.len/10;
    if(tcpc1.f_list.len-friend_total_page*10>0)
        friend_total_page++;
    GetListPageToButton(friend_cur_page,friend_total_page,friend_control_page_button,"");

    friend_control_last_button.Draw(rdr);
    friend_control_page_button.Draw(rdr);
    friend_control_next_button.Draw(rdr);
    //group_mod
    group_title_text.Draw(rdr);

    group_node* ptr2=cur_group_page_first;
    for(int i=0;i<10;i++)
    {
        if(ptr2!=NULL)
        {
            strcpy(group_content_button[i].text,ptr2->data.name);
            ptr2=ptr2->next;
        }

        group_content_button[i].Draw(rdr);
    }

    group_total_page=tcpc1.g_list.len/10;
    if(tcpc1.g_list.len-group_total_page*10>0)
        group_total_page++;
    GetListPageToButton(group_cur_page,group_total_page,group_control_page_button,"");

    group_control_last_button.Draw(rdr);
    group_control_page_button.Draw(rdr);
    group_control_next_button.Draw(rdr);

    //tool_mod
    tool_title_text.Draw(rdr);
    tool_add_friend_button.Draw(rdr);
    tool_friend_apply_button.Draw(rdr);
    tool_join_group_button.Draw(rdr);
    tool_group_apply_button.Draw(rdr);
    tool_create_group_button.Draw(rdr);
    tool_setting_button.Draw(rdr);
    //content_mod

    //main_content
    switch(LEVEL_1_MS)
    {
        case LEVEL_1_FRIEND_MSG_CONTENT:
            draw_level_1_friend_msg();
            break;
        case LEVEL_1_GROUP_MSG_CONTENT:
            draw_level_1_group_msg();
            break;
        case LEVEL_1_GROUP_MEMBER_MSG_CONTENT:
            draw_level_1_group_member_msg();
            break;
        case LEVEL_1_HELP:
            draw_level_1_help();
            break;
        case LEVEL_1_VERSION:
            draw_level_1_version();
            break;
        case LEVEL_1_ABOUT_ME:
            draw_level_1_about_me();
            break;
        case LEVEL_1_CHANGE_PASSWD:
            draw_level_1_change_passwd();
            break;
        //flag2
    }

    //tool_content
    switch(LEVEL_1_TS)
    {
        case LEVEL_1_ADD_FRIEND:
            draw_level_1_add_friend();
            break;
        case LEVEL_1_JOIN_GROUP:
            draw_level_1_join_group();
            break;
        case LEVEL_1_CREATE_GROUP:
            draw_level_1_create_group();
            break;
        case LEVEL_1_FRIEND_APPLY:
            draw_level_1_friend_apply_list();
            break;
        case LEVEL_1_GROUP_APPLY:
            draw_level_1_group_apply_list();
            break;
        case LEVEL_1_SETTING:
            draw_level_1_setting();
            break;
    }

    //notice ui
    if(strlen(notice_button.text)>0)
    {
        //animation
        if(notice_button.rect.y<0)
        {
            notice_button.rect.y+=1;
            close_notice_button.rect.y+=1;
        }

        //换一个思路，每次新通知则重新下载列表
        //不需要发数据包，就发一个标志位表示需要更新
        notice_button.Draw(rdr);
        close_notice_button.Draw(rdr);
    }

    //input_mod
    if(strlen(admin_buffer)>0)
    {
        Text text(353,308,black,14,admin_buffer);
        text.Draw(rdr);
    }

    if(strlen(group_admin_buffer)>0)
    {
        Text text(353,308,black,14,group_admin_buffer);
        text.Draw(rdr);
    }

    if(strlen(group_name_buffer)>0)
    {
        Text text(353,343,black,14,group_name_buffer);
        text.Draw(rdr);
    }

    if(LEVEL_1_TS!=LEVEL_1_NONE)
        draw_level_1_error();

    SDL_RenderPresent(rdr);
    SDL_Delay(10);
}

//level1_event_loop

void event_permit_friend_apply()
{
    for(int i=0;i<10;i++)
    {
        friend_apply_node* temp=cur_friend_apply_page_first;
        int type=7;
        if((SDL_PointInRect(&mouse_pt,&permit_apply_rect[i])))
        {
            //如果要删除的首页的第一个元素，则page_first需要更新，如果下一个不为空则更新到下一个，否则为上一页的首个
            if(i==0)
            {
                //下一个节点为非空
                if(cur_friend_apply_page_first->next!=NULL)
                    cur_friend_apply_page_first=cur_friend_apply_page_first->next;
                else
                {
                    //当前页已经清理完，并且为最后一页，则自动跳转到上一页并更新页码
                    //检查条件，必须满足总页数>1的情况下才更新
                    if(tcpc1.fa_list.len>10)
                        friend_apply_cur_page--;
                    for(int s=0;s<10;s++)
                    {
                        //上一个节点为非空(非头结点)
                        if(cur_friend_apply_page_first->last!=&(tcpc1.fa_list.head))
                            cur_friend_apply_page_first=cur_friend_apply_page_first->last;
                    }
                }
            }
            
            //找到链表中对应的节点
            for(int j=0;j<i;j++)
                temp=temp->next;

            send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
            user_info u=temp->data;
            send(tcpc1.GetSocket_fd(),(char*)&u,sizeof(u),0);      
            
            //更新好友列表
            tcpc1.InsertFriendList(&u);
            cur_friend_page_first=tcpc1.f_list.head.next;
            friend_cur_page=1;

            //删除最后一个元素后，让遍历指针指向NULL
            tcpc1.DeleteFriendApplyList(temp);
            if(tcpc1.fa_list.len==0)
            {
                cur_friend_apply_page_first=tcpc1.fa_list.head.next;
                friend_apply_cur_page=0;
            }
        }
    }
}

void event_refuse_friend_apply()
{
    for(int i=0;i<10;i++)
    {
        friend_apply_node* temp=cur_friend_apply_page_first;
        int type=8;
        if((SDL_PointInRect(&mouse_pt,&refuse_apply_rect[i])))
        {
            //如果要删除的首页的第一个元素，则page_first需要更新，如果下一个不为空则更新到下一个，否则为上一页的首个
            if(i==0)
            {
                //下一个节点为非空
                if(cur_friend_apply_page_first->next!=NULL)
                    cur_friend_apply_page_first=cur_friend_apply_page_first->next;
                else
                {
                    //当前页已经清理完，并且为最后一页，则自动跳转到上一页并更新页码
                    friend_apply_cur_page--;
                    for(int s=0;s<10;s++)
                    {
                        //上一个节点为非空(非头结点)
                        if(cur_friend_apply_page_first->last!=&(tcpc1.fa_list.head))
                            cur_friend_apply_page_first=cur_friend_apply_page_first->last;
                    }
                }
            }
                    
            //找到链表中对应的节点
            for(int j=0;j<i;j++)
                temp=temp->next;

            send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
            user_info u=temp->data;
            send(tcpc1.GetSocket_fd(),(char*)&u,sizeof(u),0);        
            
            //删除最后一个元素后，让遍历指针指向NULL
            tcpc1.DeleteFriendApplyList(temp);
            if(tcpc1.fa_list.len==0)
            {
                cur_friend_apply_page_first=tcpc1.fa_list.head.next;
                friend_apply_cur_page=0;
            }
        }
    }
}
 
void event_permit_group_apply()
{
    for(int i=0;i<10;i++)
    {
        group_apply_node* temp=cur_group_apply_page_first;
        int type=19;
        if((SDL_PointInRect(&mouse_pt,&permit_apply_rect[i])))
        {
            //如果要删除的首页的第一个元素，则page_first需要更新，如果下一个不为空则更新到下一个，否则为上一页的首个
            if(i==0)
            {
                //下一个节点为非空
                if(cur_group_apply_page_first->next!=NULL)
                    cur_group_apply_page_first=cur_group_apply_page_first->next;
                else
                {
                    //当前页已经清理完，并且为最后一页，则自动跳转到上一页并更新页码，同理检查条件
                    if(tcpc1.ga_list.len>10)
                        group_apply_cur_page--;
                    for(int s=0;s<10;s++)
                    {
                        //上一个节点为非空(非头结点)
                        if(cur_group_apply_page_first->last!=&(tcpc1.ga_list.head))
                            cur_group_apply_page_first=cur_group_apply_page_first->last;
                    }
                }
            }
            
            //找到链表中对应的节点
            for(int j=0;j<i;j++)
                temp=temp->next;

            send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
            ug_info ug=temp->data;
            send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);      

            //删除最后一个元素后，让遍历指针指向NULL
            tcpc1.DeleteGroupApplyList(temp);
            if(tcpc1.ga_list.len==0)
            {
                cur_group_apply_page_first=tcpc1.ga_list.head.next;
                group_apply_cur_page=0;
            }
        }
    }
}

void event_refuse_group_apply()
{
    for(int i=0;i<10;i++)
    {
        group_apply_node* temp=cur_group_apply_page_first;
        int type=20;
        if((SDL_PointInRect(&mouse_pt,&refuse_apply_rect[i])))
        {
            //如果要删除的首页的第一个元素，则page_first需要更新，如果下一个不为空则更新到下一个，否则为上一页的首个
            if(i==0)
            {
                //下一个节点为非空
                if(cur_group_apply_page_first->next!=NULL)
                    cur_group_apply_page_first=cur_group_apply_page_first->next;
                else
                {
                    //当前页已经清理完，并且为最后一页，则自动跳转到上一页并更新页码
                    group_apply_cur_page--;
                    for(int s=0;s<10;s++)
                    {
                        //上一个节点为非空(非头结点)
                        if(cur_group_apply_page_first->last!=&(tcpc1.ga_list.head))
                            cur_group_apply_page_first=cur_group_apply_page_first->last;
                    }
                }
            }
            
            //找到链表中对应的节点
            for(int j=0;j<i;j++)
                temp=temp->next;

            send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
            ug_info ug=temp->data;
            send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);      
            
            //删除最后一个元素后，让遍历指针指向NULL
            tcpc1.DeleteGroupApplyList(temp);
            if(tcpc1.ga_list.len==0)
            {
                cur_group_apply_page_first=tcpc1.ga_list.head.next;
                group_apply_cur_page=0;
            }
        }
    }
}

void event_clear_all_friend_apply()
{
    int type=8;
    friend_apply_node* ptr=cur_friend_apply_page_first;

    if((SDL_PointInRect(&mouse_pt,&clear_page_rect)))
    {
        while(ptr!=NULL)
        {
            send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
            send(tcpc1.GetSocket_fd(),(char*)&(ptr->data),sizeof(user_info),0);
            ptr=ptr->next;
        }

        friend_apply_cur_page=0;
        tcpc1.DestroyFriendApplyList();
        cur_friend_apply_page_first=tcpc1.fa_list.head.next;//the next is NULL
    }
}

void event_clear_all_group_apply()
{
    int type=20;
    group_apply_node* ptr=cur_group_apply_page_first;

    if((SDL_PointInRect(&mouse_pt,&clear_page_rect)))
    {
        while(ptr!=NULL)
        {
            send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
            send(tcpc1.GetSocket_fd(),(char*)&(ptr->data),sizeof(ug_info),0);
            ptr=ptr->next;
        }

        group_apply_cur_page=0;
        tcpc1.DestroyGroupApplyList();
        cur_group_apply_page_first=tcpc1.ga_list.head.next;
    }
}

//当链表为空且发生节点插入行为时，则重新更新遍历指针(因为遍历指针只在初次登录时进行了指向，过程中若表空后则会一直指向NULL)
void event_loop_index()
{
    while(SDL_PollEvent(&event))
    {
        friend_node* temp=cur_friend_page_first;
        group_node* temp2=cur_group_page_first;

        switch(event.type)
        {
            case SDL_QUIT:
                CUR_STATUS=LEVEL_EXIT;
                break;
            case SDL_MOUSEMOTION:
                mouse_pt.x=event.motion.x,mouse_pt.y=event.motion.y;
                
                if(LEVEL_1_TS==LEVEL_1_SETTING)
                {
                    exit_login_button.InteractResponse(mouse_pt,exit_login_rect,white,green);
                    help_button.InteractResponse(mouse_pt,help_rect,white,green);
                    about_me_button.InteractResponse(mouse_pt,about_me_rect,white,green);
                    version_button.InteractResponse(mouse_pt,version_rect,white,green);
                    change_passwd_button.InteractResponse(mouse_pt,change_passwd_rect,white,green);
                    change_style_button.InteractResponse(mouse_pt,change_style_rect,white,green);
                }
                
                for(int i=0;i<10;i++)
                    friend_content_button[i].InteractResponse(mouse_pt,friend_content_rect[i],white,green);

                friend_control_last_button.InteractResponse(mouse_pt,friend_control_last_rect,white,green);
                friend_control_next_button.InteractResponse(mouse_pt,friend_control_next_rect,white,green);

                for(int i=0;i<10;i++)
                    group_content_button[i].InteractResponse(mouse_pt,group_content_rect[i],white,green);

                if(LEVEL_1_TS==LEVEL_1_FRIEND_APPLY||LEVEL_1_TS==LEVEL_1_GROUP_APPLY)
                {
                    for(int i=0;i<10;i++)
                        permit_button[i].InteractResponse(mouse_pt,permit_apply_rect[i],white,green);

                    for(int i=0;i<10;i++)
                        refuse_button[i].InteractResponse(mouse_pt,refuse_apply_rect[i],white,red);
                }

                if(LEVEL_1_MS!=LEVEL_1_NONE2)
                {
                    msg_more_button.InteractResponse(mouse_pt,msg_more_rect,white,green);
                    msg_close_button.InteractResponse(mouse_pt,msg_close_rect,white,green);
                    msg_send_button.InteractResponse(mouse_pt,msg_send_rect,white,green);
                }

                group_control_last_button.InteractResponse(mouse_pt,group_control_last_rect,white,green);
                group_control_next_button.InteractResponse(mouse_pt,group_control_next_rect,white,green);

                tool_add_friend_button.InteractResponse(mouse_pt,tool_add_friend_rect,white,green);
                tool_friend_apply_button.InteractResponse(mouse_pt,tool_friend_apply_rect,white,green);
                tool_join_group_button.InteractResponse(mouse_pt,tool_join_group_rect,white,green);
                tool_group_apply_button.InteractResponse(mouse_pt,tool_group_apply_rect,white,green);
                tool_create_group_button.InteractResponse(mouse_pt,tool_create_group_rect,white,green);
                tool_setting_button.InteractResponse(mouse_pt,tool_setting_rect,white,green);

                if(strlen(notice_button.text)>0)
                    close_notice_button.InteractResponse(mouse_pt,close_notice_rect,white,green);

                if(LEVEL_1_TS!=LEVEL_1_NONE)
                {
                    close_page_button.InteractResponse(mouse_pt,close_page_rect,white,green);
                    if(LEVEL_1_TS!=LEVEL_1_GROUP_APPLY&&LEVEL_1_TS!=LEVEL_1_FRIEND_APPLY)
                        commit_page_button.InteractResponse(mouse_pt,commit_page_rect,white,green);
                }

                if(LEVEL_1_TS==LEVEL_1_FRIEND_APPLY||LEVEL_1_TS==LEVEL_1_GROUP_APPLY)
                {
                    apply_control_last_button.InteractResponse(mouse_pt,apply_control_last_rect,white,green);
                    apply_control_next_button.InteractResponse(mouse_pt,apply_control_next_rect,white,green);
                    clear_page_button.InteractResponse(mouse_pt,clear_page_rect,white,red);
                }

                if(LEVEL_1_MS==LEVEL_1_GROUP_MSG_CONTENT&&msg_group_more_tool)
                {
                    msg_more_group_member_button.InteractResponse(mouse_pt,msg_more_group_member_rect,white,green);

                    if(cur_view_group->data.manager_id!=cur_user.id)//群主不能退出群聊
                        msg_more_exit_group_button.InteractResponse(mouse_pt,msg_more_exit_group_rect,white,green);
                }

                if(LEVEL_1_MS==LEVEL_1_FRIEND_MSG_CONTENT&&msg_friend_more_tool)
                {
                    msg_more_delete_friend_button.InteractResponse(mouse_pt,msg_more_delete_friend_rect,white,green);
                }

                if(LEVEL_1_MS==LEVEL_1_GROUP_MEMBER_MSG_CONTENT)
                {
                    gm_last_page_button.InteractResponse(mouse_pt,gm_last_page_rect,white,green);
                    gm_next_page_button.InteractResponse(mouse_pt,gm_next_page_rect,white,green);

                    if(cur_view_group->data.manager_id==cur_user.id)
                    {
                        for(int i=0;i<25;i++)
                        {
                            remove_member_button[i].InteractResponse(mouse_pt,remove_member_rect[i],white,red);
                        }
                    }
                }

                if(LEVEL_1_MS==LEVEL_1_CHANGE_PASSWD)
                {
                    commit_new_passwd_button.InteractResponse(mouse_pt,commit_new_passwd_rect,white,green);
                }
                
                break;
            case SDL_WINDOWEVENT:
                if(event.window.event==SDL_WINDOWEVENT_LEAVE)
                    ResetButton();
                if(event.window.event==SDL_WINDOWEVENT_ENTER)
                    ;
                break;
            case SDL_MOUSEWHEEL:
                if(LEVEL_1_MS==LEVEL_1_GROUP_MSG_CONTENT||LEVEL_1_MS==LEVEL_1_FRIEND_MSG_CONTENT)
                {
                    if(event.wheel.y>0&&msg_start<msg_total_len)
                    {
                        msg_start+=25;
                    }

                    if(event.wheel.y<0&&msg_start>500)
                    {
                        msg_start-=25;
                    }
                }
                
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_pt.x=event.motion.x,mouse_pt.y=event.motion.y;

                if(SDL_PointInRect(&mouse_pt,&close_notice_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&strlen(notice_button.text)>0)
                {
                    memset(notice_button.text,0,sizeof(notice_button.text));
                }

                if(LEVEL_1_TS==LEVEL_1_SETTING&&SDL_PointInRect(&mouse_pt,&exit_login_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    int type=6;
                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    
                    CUR_STATUS=LEVEL_0_LOGIN;
                    std::cout<<"已退出登录"<<std::endl;
                    clear_last_admin_data();
                    data_init();
                    return;     //关键点，退出登录窗口变小，后面首页的事件不应该再检测和处理，因为原来的一些旧区域的内存已经回收
                                //改变窗口大小时，变大不会导致卡死，但变小屏幕某些区域失效，此时对旧区域访问就会内存越界导致崩溃
                }

                if(LEVEL_1_TS==LEVEL_1_SETTING&&SDL_PointInRect(&mouse_pt,&help_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_MS=LEVEL_1_HELP;
                    LEVEL_1_TS=LEVEL_1_NONE;
                    help_button.fill_color.r=255;
                    help_button.fill_color.g=255;
                    help_button.fill_color.b=255;
                }

                if(LEVEL_1_TS==LEVEL_1_SETTING&&SDL_PointInRect(&mouse_pt,&version_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_MS=LEVEL_1_VERSION;
                    LEVEL_1_TS=LEVEL_1_NONE;
                    version_button.fill_color.r=255;
                    version_button.fill_color.g=255;
                    version_button.fill_color.b=255;
                }

                if(LEVEL_1_TS==LEVEL_1_SETTING&&SDL_PointInRect(&mouse_pt,&about_me_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_MS=LEVEL_1_ABOUT_ME;
                    LEVEL_1_TS=LEVEL_1_NONE;
                    about_me_button.fill_color.r=255;
                    about_me_button.fill_color.g=255;
                    about_me_button.fill_color.b=255;
                }

                if(LEVEL_1_TS==LEVEL_1_SETTING&&SDL_PointInRect(&mouse_pt,&change_passwd_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_MS=LEVEL_1_CHANGE_PASSWD;
                    LEVEL_1_TS=LEVEL_1_NONE;
                    change_passwd_button.fill_color.r=255;
                    change_passwd_button.fill_color.g=255;
                    change_passwd_button.fill_color.b=255;
                }
                //flag1

                for(int i=0;i<10;i++)
                {
                    if
                    (
                        SDL_PointInRect(&mouse_pt,&(friend_content_rect[i]))&&
                        (event.button.button==SDL_BUTTON_LEFT)&&strlen(friend_content_button[i].text)>0
                    )
                    {
                        if(temp!=NULL)
                        {
                            memset(msg_title.text,0,sizeof(msg_title.text));
                            
                            strcpy(msg_title.text,"会话:");
                            strcat(msg_title.text,temp->data.name);
                            strcat(msg_title.text,"(");
                            strcat(msg_title.text,temp->data.admin);
                            strcat(msg_title.text,")");

                            cur_view_friend=temp;
                            LEVEL_1_MS=LEVEL_1_FRIEND_MSG_CONTENT;
                            LEVEL_1_TS=LEVEL_1_NONE;
                            clear_input_buffer();
                            event_remove_manager=false;
                        }
                            //clear last status
                            msg_close_button.fill_color.r=255;
                            msg_close_button.fill_color.g=255;
                            msg_close_button.fill_color.b=255;

                            //start animation once
                            msg_base_button.rect.x=WINDOW_WIDTH;
                            msg_title.text_rect.x=175+msg_base_button.rect.w;
                            msg_more_button.rect.x=175+msg_base_button.rect.w;
                            msg_send_button.rect.x=225+msg_base_button.rect.w;
                            msg_close_button.rect.x=475+msg_base_button.rect.w;
                            msg_input_button.rect.x=175+msg_base_button.rect.w;

                            msg_start=500;
                            cur_error.value=-999;
                    }

                    if
                    (
                        SDL_PointInRect(&mouse_pt,&(group_content_rect[i]))&&
                        (event.button.button==SDL_BUTTON_LEFT)&&strlen(group_content_button[i].text)>0
                    )
                    {
                        if(temp2!=NULL)
                        {
                            memset(msg_title.text,0,sizeof(msg_title.text));

                            strcpy(msg_title.text,"会话:");
                            strcat(msg_title.text,temp2->data.name);
                            strcat(msg_title.text,"(");
                            strcat(msg_title.text,temp2->data.admin);
                            strcat(msg_title.text,")");

                            cur_view_group=temp2;
                            LEVEL_1_MS=LEVEL_1_GROUP_MSG_CONTENT;
                            LEVEL_1_TS=LEVEL_1_NONE;
                            clear_input_buffer();
                            event_remove_manager=false;
                        }
                            //clear last status
                            msg_close_button.fill_color.r=255;
                            msg_close_button.fill_color.g=255;
                            msg_close_button.fill_color.b=255;

                            //start animation once
                            msg_base_button.rect.x=WINDOW_WIDTH;
                            msg_title.text_rect.x=175+msg_base_button.rect.w;
                            msg_more_button.rect.x=175+msg_base_button.rect.w;
                            msg_send_button.rect.x=225+msg_base_button.rect.w;
                            msg_close_button.rect.x=475+msg_base_button.rect.w;
                            msg_input_button.rect.x=175+msg_base_button.rect.w;

                            msg_start=500;
                            cur_error.value=-999;
                    }
                    
                    if(temp!=NULL)
                        temp=temp->next;

                    if(temp2!=NULL)
                        temp2=temp2->next;

                    //clear content
                    memset(friend_content_button[i].text,0,sizeof(friend_content_button[i].text));
                    memset(group_content_button[i].text,0,sizeof(group_content_button[i].text));
                }

                if(SDL_PointInRect(&mouse_pt,&friend_control_last_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    std::cout<<"上一页"<<std::endl;
                    if(tcpc1.f_list.len>10&&cur_friend_page_first->last!=&(tcpc1.f_list.head))
                    {
                        for(int i=0;i<10;i++)
                            cur_friend_page_first=cur_friend_page_first->last;
                        friend_cur_page--;
                    }
                }

                if(SDL_PointInRect(&mouse_pt,&friend_control_next_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    std::cout<<"下一页"<<std::endl;
                    if(tcpc1.f_list.len>10*friend_cur_page&&cur_friend_page_first->next!=NULL)
                    {
                        for(int i=0;i<10;i++)
                            cur_friend_page_first=cur_friend_page_first->next;
                        friend_cur_page++;
                    }
                }

                if(SDL_PointInRect(&mouse_pt,&group_control_last_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    std::cout<<"上一页"<<std::endl;
                    if(tcpc1.g_list.len>10&&cur_group_page_first->last!=&(tcpc1.g_list.head))
                    {
                        for(int i=0;i<10;i++)
                            cur_group_page_first=cur_group_page_first->last;
                        group_cur_page--;
                    }
                }

                if(SDL_PointInRect(&mouse_pt,&group_control_next_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    std::cout<<"下一页"<<std::endl;
                    if(tcpc1.g_list.len>10*group_cur_page&&cur_group_page_first->next!=NULL)
                    {
                        for(int i=0;i<10;i++)
                            cur_group_page_first=cur_group_page_first->next;
                        group_cur_page++;
                    }
                }

                if(SDL_PointInRect(&mouse_pt,&apply_control_last_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&LEVEL_1_TS==LEVEL_1_FRIEND_APPLY)
                {
                    std::cout<<"上一页"<<std::endl;
                    if(tcpc1.fa_list.len>10&&cur_friend_apply_page_first->last!=&(tcpc1.fa_list.head))
                    {
                        for(int i=0;i<10;i++)
                            cur_friend_apply_page_first=cur_friend_apply_page_first->last;
                        friend_apply_cur_page--;
                    }
                }

                if(SDL_PointInRect(&mouse_pt,&apply_control_next_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&LEVEL_1_TS==LEVEL_1_FRIEND_APPLY)
                {
                    std::cout<<"下一页"<<std::endl;
                    if(tcpc1.fa_list.len>10*friend_apply_cur_page&&cur_friend_apply_page_first->next!=NULL)
                    {
                        for(int i=0;i<10;i++)
                        {
                            cur_friend_apply_page_first=cur_friend_apply_page_first->next;
                        }
                        friend_apply_cur_page++;
                    }
                }

                if(SDL_PointInRect(&mouse_pt,&apply_control_last_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&LEVEL_1_TS==LEVEL_1_GROUP_APPLY)
                {
                    std::cout<<"上一页"<<std::endl;
                    if(tcpc1.ga_list.len>10&&cur_group_apply_page_first->last!=&(tcpc1.ga_list.head))
                    {
                        for(int i=0;i<10;i++)
                            cur_group_apply_page_first=cur_group_apply_page_first->last;
                        group_apply_cur_page--;
                    }
                }

                if(SDL_PointInRect(&mouse_pt,&apply_control_next_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&LEVEL_1_TS==LEVEL_1_GROUP_APPLY)
                {
                    std::cout<<"下一页"<<std::endl;
                    if(tcpc1.ga_list.len>10*group_apply_cur_page&&cur_group_apply_page_first->next!=NULL)
                    {
                        for(int i=0;i<10;i++)
                            cur_group_apply_page_first=cur_group_apply_page_first->next;
                        group_apply_cur_page++;
                    }
                }

                if(LEVEL_1_TS!=LEVEL_1_NONE&&SDL_PointInRect(&mouse_pt,&close_page_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_NONE;
                    cur_error.value=-999;
                    clear_input_buffer();
                }

                if
                (
                    SDL_PointInRect(&mouse_pt,&commit_page_rect)&&(event.button.button==SDL_BUTTON_LEFT)
                    &&LEVEL_1_TS==LEVEL_1_ADD_FRIEND&&strlen(admin_buffer)>0
                )
                {
                    if(!strcmp(admin_buffer,cur_user.admin))
                    {
                        cur_error.value=13;
                    }
                    else
                    {
                        int type=3;
                        user_info ui;
                        strcpy(ui.admin,admin_buffer);

                        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                        send(tcpc1.GetSocket_fd(),(char*)&ui,sizeof(ui),0);
                    }

                    r_commit.fill_color.r=255;
                    r_commit.fill_color.g=255;
                    r_commit.fill_color.b=255;
                    clear_input_buffer();
                }

                if
                (
                    SDL_PointInRect(&mouse_pt,&commit_page_rect)&&(event.button.button==SDL_BUTTON_LEFT)
                    &&LEVEL_1_TS==LEVEL_1_JOIN_GROUP&&strlen(group_admin_buffer)>0
                )
                {
                    int type=15;
                    group_info gi;
                    strcpy(gi.admin,group_admin_buffer);

                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&gi,sizeof(gi),0);

                    r_commit.fill_color.r=255;
                    r_commit.fill_color.g=255;
                    r_commit.fill_color.b=255;
                    clear_input_buffer();
                }

                if
                (
                    SDL_PointInRect(&mouse_pt,&commit_page_rect)&&(event.button.button==SDL_BUTTON_LEFT)
                    &&LEVEL_1_TS==LEVEL_1_CREATE_GROUP&&strlen(group_admin_buffer)>0&&strlen(group_name_buffer)
                )
                {
                    int type=14;
                    group_info gi;
                    strcpy(gi.admin,group_admin_buffer);
                    strcpy(gi.name,group_name_buffer);
                    gi.manager_id=cur_user.id;

                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&gi,sizeof(gi),0);
                    
                    r_commit.fill_color.r=255;
                    r_commit.fill_color.g=255;
                    r_commit.fill_color.b=255;
                    clear_input_buffer();
                }

                if(SDL_PointInRect(&mouse_pt,&msg_send_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&strlen(message_buffer)>0)
                {
                    if(LEVEL_1_MS==LEVEL_1_FRIEND_MSG_CONTENT)
                    {
                        int type=4;
                        unicast_msg tmp;
                        tmp.sou_id=cur_user.id;
                        tmp.des_id=cur_view_friend->data.id;
                        strcpy(tmp.msg,message_buffer);

                        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                        send(tcpc1.GetSocket_fd(),(char*)&tmp,sizeof(tmp),0);
                        
                        tcpc1.InsertUnicastList(&tmp);
                        clear_input_buffer();
                    }

                    if(LEVEL_1_MS==LEVEL_1_GROUP_MSG_CONTENT)
                    {
                        int type=5;
                        multicast_msg tmp;
                        tmp.member_id=cur_user.id;
                        tmp.group_id=cur_view_group->data.id;
                        strcpy(tmp.msg,message_buffer);
                        strcpy(tmp.member_name,cur_user.name);

                        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                        send(tcpc1.GetSocket_fd(),(char*)&tmp,sizeof(tmp),0);

                        tcpc1.InsertMulticastList(&tmp);
                        clear_input_buffer();
                    }
                }

                if(LEVEL_1_TS==LEVEL_1_FRIEND_APPLY&&(event.button.button==SDL_BUTTON_LEFT)&&(tcpc1.fa_list.len>0))
                {
                    event_permit_friend_apply();
                    event_refuse_friend_apply();
                    event_clear_all_friend_apply();
                }
                            
                if(LEVEL_1_TS==LEVEL_1_GROUP_APPLY&&(event.button.button==SDL_BUTTON_LEFT)&&(tcpc1.ga_list.len>0))
                {
                    event_permit_group_apply();
                    event_refuse_group_apply();
                    event_clear_all_group_apply();
                }

                if(SDL_PointInRect(&mouse_pt,&tool_add_friend_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_ADD_FRIEND;
                    
                    //start animation once
                    tool_all_button.rect.x=WINDOW_WIDTH;
                    add_friend_text.text_rect.x=250+tool_all_button.rect.w;
                    add_friend_input_button.rect.x=350+tool_all_button.rect.w;
                    close_page_button.rect.x=175+tool_all_button.rect.w;
                    commit_page_button.rect.x=525+tool_all_button.rect.w;
                    add_friend_title.text_rect.x=325+tool_all_button.rect.w;

                    //close msg content
                    LEVEL_1_MS=LEVEL_1_NONE2;
                    cur_error.value=-999;

                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
                    clear_input_buffer();
                }

                if
                (
                   (LEVEL_1_MS==LEVEL_1_FRIEND_MSG_CONTENT||LEVEL_1_MS==LEVEL_1_GROUP_MSG_CONTENT)&&
                   SDL_PointInRect(&mouse_pt,&msg_input_rect)&&(event.button.button==SDL_BUTTON_LEFT)
                )
                {
                    msg_input_button.border_color.g=220;
                    SDL_Rect rect=msg_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);
                    message_editing=true;
                }
                else
                {
                    msg_input_button.border_color.g=0;
                    message_editing=false;
                }

                if(LEVEL_1_TS==LEVEL_1_ADD_FRIEND&&SDL_PointInRect(&mouse_pt,&add_friend_input_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    add_friend_input_button.border_color.g=220;

                    SDL_Rect rect=add_friend_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);
                    admin_editing=true;
                }
                else
                {
                    add_friend_input_button.border_color.g=0;
                    admin_editing=false;
                }

                if(SDL_PointInRect(&mouse_pt,&tool_join_group_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_JOIN_GROUP;

                    //start animation once
                    tool_all_button.rect.x=WINDOW_WIDTH;

                    join_group_text.text_rect.x=250+tool_all_button.rect.w;
                    join_group_input_button.rect.x=350+tool_all_button.rect.w;
                    close_page_button.rect.x=175+tool_all_button.rect.w;
                    commit_page_button.rect.x=525+tool_all_button.rect.w;
                    join_group_title.text_rect.x=325+tool_all_button.rect.w;

                    //close msg content
                    LEVEL_1_MS=LEVEL_1_NONE2;
                    cur_error.value=-999;

                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
                    clear_input_buffer();
                }

                if(LEVEL_1_TS==LEVEL_1_JOIN_GROUP&&SDL_PointInRect(&mouse_pt,&join_group_input_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    join_group_input_button.border_color.g=220;

                    SDL_Rect rect=join_group_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);
                    group_admin_editing1=true;
                }
                else
                {
                    join_group_input_button.border_color.g=0;
                    group_admin_editing1=false;
                }

                if(SDL_PointInRect(&mouse_pt,&tool_create_group_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_CREATE_GROUP;

                    //start animation once
                    tool_all_button.rect.x=WINDOW_WIDTH;

                    create_group_admin_text.text_rect.x=250+tool_all_button.rect.w;
                    create_group_name_text.text_rect.x=250+tool_all_button.rect.w;

                    create_group_admin_input_button.rect.x=350+tool_all_button.rect.w;
                    create_group_name_input_button.rect.x=350+tool_all_button.rect.w;

                    close_page_button.rect.x=175+tool_all_button.rect.w;
                    commit_page_button.rect.x=525+tool_all_button.rect.w;
                    create_group_title.text_rect.x=325+tool_all_button.rect.w;

                    //close msg content
                    LEVEL_1_MS=LEVEL_1_NONE2;
                    cur_error.value=-999;

                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
                    clear_input_buffer();
                }

                if(LEVEL_1_TS==LEVEL_1_CREATE_GROUP&&SDL_PointInRect(&mouse_pt,&create_group_admin_input_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    create_group_admin_input_button.border_color.g=220;

                    SDL_Rect rect=create_group_admin_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);
                    group_admin_editing2=true;
                }
                else
                {
                    create_group_admin_input_button.border_color.g=0;
                    group_admin_editing2=false;
                }

                if(LEVEL_1_TS==LEVEL_1_CREATE_GROUP&&SDL_PointInRect(&mouse_pt,&create_group_name_input_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    create_group_name_input_button.border_color.g=220;

                    SDL_Rect rect=create_group_name_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);
                    group_name_editing=true;
                }
                else
                {
                    create_group_name_input_button.border_color.g=0;
                    group_name_editing=false;
                }

                if(SDL_PointInRect(&mouse_pt,&tool_friend_apply_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_FRIEND_APPLY;
                    
                    //start animation once
                    tool_all_button.rect.x=WINDOW_WIDTH;

                    friend_apply_title.text_rect.x=325+tool_all_button.rect.w;
                    apply_control_last_button.rect.x=225+tool_all_button.rect.w;
                    apply_control_page_button.rect.x=275+tool_all_button.rect.w;
                    apply_control_next_button.rect.x=475+tool_all_button.rect.w;
                    close_page_button.rect.x=175+tool_all_button.rect.w;
                    clear_page_button.rect.x=525+tool_all_button.rect.w;

                    for(int i=0;i<10;i++)
                    {
                        friend_apply_button[i].rect.x=friend_apply_rect[i].x+tool_all_button.rect.w;
                        refuse_button[i].rect.x=refuse_apply_rect[i].x+tool_all_button.rect.w;
                        permit_button[i].rect.x=permit_apply_rect[i].x+tool_all_button.rect.w;
                    }

                    //close msg content
                    LEVEL_1_MS=LEVEL_1_NONE2;
                    cur_error.value=-999;

                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
                    clear_input_buffer();
                }

                if(SDL_PointInRect(&mouse_pt,&tool_group_apply_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_GROUP_APPLY;

                    //start animation once
                    tool_all_button.rect.x=WINDOW_WIDTH;

                    group_apply_title.text_rect.x=325+tool_all_button.rect.w;
                    apply_control_last_button.rect.x=225+tool_all_button.rect.w;
                    apply_control_page_button.rect.x=275+tool_all_button.rect.w;
                    apply_control_next_button.rect.x=475+tool_all_button.rect.w;
                    close_page_button.rect.x=175+tool_all_button.rect.w;
                    clear_page_button.rect.x=525+tool_all_button.rect.w;

                    for(int i=0;i<10;i++)
                    {
                        group_apply_button[i].rect.x=group_apply_rect[i].x+tool_all_button.rect.w;
                        refuse_button[i].rect.x=refuse_apply_rect[i].x+tool_all_button.rect.w;
                        permit_button[i].rect.x=permit_apply_rect[i].x+tool_all_button.rect.w;
                    }

                    //close msg content
                    LEVEL_1_MS=LEVEL_1_NONE2;
                    cur_error.value=-999;

                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
                }

                if(SDL_PointInRect(&mouse_pt,&tool_setting_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_SETTING;

                    //start animation once
                    tool_all_button.rect.x=WINDOW_WIDTH;
                    cur_admin_text.text_rect.x=175+tool_all_button.rect.w;
                    exit_login_button.rect.x=325+tool_all_button.rect.w;
                    setting_title.text_rect.x=325+tool_all_button.rect.w;
                    close_page_button.rect.x=175+tool_all_button.rect.w;

                    help_button.rect.x=325+tool_all_button.rect.w;
                    about_me_button.rect.x=325+tool_all_button.rect.w;
                    version_button.rect.x=325+tool_all_button.rect.w;
                    change_passwd_button.rect.x=325+tool_all_button.rect.w;
                    change_style_button.rect.x=325+tool_all_button.rect.w;

                    //close msg content
                    LEVEL_1_MS=LEVEL_1_NONE2;
                    cur_error.value=-999;

                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
                    clear_input_buffer();
                }

                if
                (
                    (LEVEL_1_MS==LEVEL_1_FRIEND_MSG_CONTENT||LEVEL_1_MS==LEVEL_1_GROUP_MSG_CONTENT)&&
                    SDL_PointInRect(&mouse_pt,&msg_close_rect)&&(event.button.button==SDL_BUTTON_LEFT)
                )
                {
                    LEVEL_1_MS=LEVEL_1_NONE2;
                    memset(msg_title.text,0,sizeof(msg_title.text));
                }

                //群主不能退出群聊
                if
                (
                    cur_view_group!=NULL&&msg_group_more_tool&&
                    ((cur_view_group->data).manager_id!=cur_user.id)&&(LEVEL_1_MS==LEVEL_1_GROUP_MSG_CONTENT)&&
                    SDL_PointInRect(&mouse_pt,&msg_more_exit_group_rect)&&(event.button.button==SDL_BUTTON_LEFT)
                )
                {
                    int type=21;
                    
                    ug_info ug;
                    strcpy(ug.gi.admin,cur_view_group->data.admin);
                    strcpy(ug.gi.name,cur_view_group->data.name);
                    strcpy(ug.ui.admin,cur_user.admin);
                    strcpy(ug.ui.name,cur_user.name);

                    ug.gi.id=cur_view_group->data.id;
                    ug.gi.manager_id=cur_view_group->data.manager_id;
                    ug.ui.id=cur_user.id;

                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);

                    group_node* g=cur_view_group;
                    if(cur_view_group==cur_group_page_first)
                    {
                        if(cur_group_page_first->next!=NULL)
                            cur_group_page_first=cur_group_page_first->next;
                        else if(tcpc1.g_list.len>10)
                        {
                            for(int i=0;i<10;i++)
                                cur_group_page_first=cur_group_page_first->last;
                            group_cur_page--;
                        }
                        else
                        {
                            cur_group_page_first=NULL;
                            group_cur_page=0;
                        }
                    }

                    tcpc1.DeleteGroupList(&(g->data));
                    LEVEL_1_MS=LEVEL_1_NONE2;
                }

                if
                (
                    LEVEL_1_MS==LEVEL_1_FRIEND_MSG_CONTENT&&msg_friend_more_tool&&
                    SDL_PointInRect(&mouse_pt,&msg_more_delete_friend_rect)
                    &&(event.button.button==SDL_BUTTON_LEFT)
                )
                {
                    int type=9;
                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&(cur_view_friend->data),sizeof(user_info),0);
                    
                    if(cur_friend_page_first->data.id==cur_view_friend->data.id)
                    {
                        if(cur_friend_page_first->next!=NULL)
                            cur_friend_page_first=cur_friend_page_first->next;
                        else if(cur_friend_page_first->last!=&(tcpc1.f_list.head)&&friend_cur_page>1)
                        {
                            for(int i=0;i<10;i++)
                                cur_friend_page_first=cur_friend_page_first->last;
                            friend_cur_page--;
                        }
                        else
                        {
                            cur_friend_page_first=NULL;
                            friend_cur_page=0;
                        }
                    }

                    tcpc1.DeleteFriendList(&(cur_view_friend->data));
                    LEVEL_1_MS=LEVEL_1_NONE2;
                    cur_view_friend=NULL;
                    cur_error.value=-999;
                }

                if
                (
                    LEVEL_1_MS==LEVEL_1_GROUP_MSG_CONTENT&&msg_group_more_tool&&
                    SDL_PointInRect(&mouse_pt,&msg_more_group_member_rect)
                    &&(event.button.button==SDL_BUTTON_LEFT)
                )
                {
                    tcpc1.DestroyGroupMemberList();
                    
                    int type=18;
                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&(cur_view_group->data),sizeof(group_info),0);
                }

                //check after msg_group_more_tool and msg_friend_more_tool
                if(LEVEL_1_MS==LEVEL_1_GROUP_MSG_CONTENT&&SDL_PointInRect(&mouse_pt,&msg_more_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                    msg_group_more_tool=true;
                else
                    msg_group_more_tool=false;

                if(LEVEL_1_MS==LEVEL_1_FRIEND_MSG_CONTENT&&SDL_PointInRect(&mouse_pt,&msg_more_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                    msg_friend_more_tool=true;
                else
                    msg_friend_more_tool=false;

                if
                (
                    LEVEL_1_MS==LEVEL_1_GROUP_MEMBER_MSG_CONTENT&&
                    cur_user.id==cur_view_group->data.manager_id&&
                    (event.button.button==SDL_BUTTON_LEFT)
                )
                {
                    for(int i=0;i<25;i++)
                    {
                        if(SDL_PointInRect(&mouse_pt,&(remove_member_rect[i]))&&strlen(group_member_button[i].text)>0&&LEVEL_1_TS==LEVEL_1_NONE)
                        {
                            group_member_node* find=cur_group_member_page_first;

                            if(find!=NULL)
                            {
                                for(int j=0;j<i;j++)
                                    find=find->next;
                                
                                if(cur_user.id==find->data.id)
                                {
                                    event_remove_manager=true;
                                    break;
                                }   
                            }

                            int type=22;
                            ug_info ug;
                            ug.gi.id=cur_view_group->data.id;
                            ug.gi.manager_id=cur_view_group->data.manager_id;
                            strcpy(ug.gi.admin,cur_view_group->data.admin);
                            strcpy(ug.gi.name,cur_view_group->data.name);

                            ug.ui.id=find->data.id;
                            strcpy(ug.ui.name,find->data.name);
                            strcpy(ug.ui.admin,find->data.admin);

                            if(cur_group_member_page_first->data.id==find->data.id)
                            {
                                if(cur_group_member_page_first->next!=NULL)
                                    cur_group_member_page_first=cur_group_member_page_first->next;
                                else if(cur_group_member_page_first->last!=&(tcpc1.gm_list.head)&&group_member_cur_page>1)
                                {
                                    for(int j=0;j<25;j++)
                                        cur_group_member_page_first=cur_group_member_page_first->last;
                                    group_member_cur_page--;
                                }
                                else
                                {
                                    cur_group_member_page_first=NULL;
                                    group_member_cur_page=0;
                                }
                            }
                            tcpc1.DeleteGroupMemberList(&(ug.ui));
                            //清除上一次的数据，当子线程接收完数据后才能更改页面状态
                            //要么加锁要或者在子线程中接受完数据再改状态，否则会竞争

                            send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                            send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);
                            break;
                        }
                    }
                }

                if
                (LEVEL_1_MS==LEVEL_1_GROUP_MEMBER_MSG_CONTENT&&cur_view_group!=NULL)
                {
                    if(SDL_PointInRect(&mouse_pt,&gm_last_page_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                    {
                        std::cout<<"上一页"<<std::endl;
                        if(tcpc1.gm_list.len>25&&cur_group_member_page_first->last!=&(tcpc1.gm_list.head))
                        {
                            for(int i=0;i<25;i++)
                                cur_group_member_page_first=cur_group_member_page_first->last;
                            group_member_cur_page--;
                        }
                    }

                    if(SDL_PointInRect(&mouse_pt,&gm_next_page_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                    {
                        std::cout<<"下一页"<<std::endl;
                        if(tcpc1.gm_list.len>25*group_member_cur_page&&cur_group_member_page_first->next!=NULL)
                        {
                            for(int i=0;i<25;i++)
                                cur_group_member_page_first=cur_group_member_page_first->next;
                            group_member_cur_page++;
                        }
                    }
                }

                if
                (
                   (LEVEL_1_MS==LEVEL_1_CHANGE_PASSWD)&&
                   SDL_PointInRect(&mouse_pt,&input_new_passwd1_rect)&&(event.button.button==SDL_BUTTON_LEFT)
                )
                {
                    input_new_passwd1_button.border_color.g=220;
                    SDL_Rect rect=msg_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);
                    new_passwd_editing1=true;
                }
                else
                {
                    input_new_passwd1_button.border_color.g=0;
                    new_passwd_editing1=false;
                }

                if
                (
                   (LEVEL_1_MS==LEVEL_1_CHANGE_PASSWD)&&
                   SDL_PointInRect(&mouse_pt,&input_new_passwd2_rect)&&(event.button.button==SDL_BUTTON_LEFT)
                )
                {
                    input_new_passwd2_button.border_color.g=220;
                    SDL_Rect rect=msg_input_rect;
                    rect.y+=20;
                    SDL_SetTextInputRect(&rect);
                    new_passwd_editing2=true;
                }
                else
                {
                    input_new_passwd2_button.border_color.g=0;
                    new_passwd_editing2=false;
                }

                if
                (
                    SDL_PointInRect(&mouse_pt,&commit_new_passwd_rect)&&strlen(passwd_buffer)>0&&
                    strlen(repasswd_buffer)>0&&(event.button.button==SDL_BUTTON_LEFT)&&LEVEL_1_MS==LEVEL_1_CHANGE_PASSWD
                )
                {
                    if(strcmp(passwd_buffer,repasswd_buffer))
                    {
                        cur_error.value=12;
                    }
                    else
                    {
                        int type=23;
                        user_info ui;
                        strcpy(ui.admin,cur_user.admin);
                        strcpy(ui.name,cur_user.name);
                        strcpy(ui.passwd,passwd_buffer);
                        ui.id=cur_user.id;

                        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                        send(tcpc1.GetSocket_fd(),(char*)&ui,sizeof(ui),0);
                    }

                    r_commit.fill_color.r=255;
                    r_commit.fill_color.g=255;
                    r_commit.fill_color.b=255;
                    clear_input_buffer();
                }

                break;
            case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_BACKSPACE) 
                    {
                        int len;
                        if(admin_editing)
                        {
                            len=strlen(admin_buffer);
                            cur_buffer=admin_buffer;
                        } 
                        else if(group_admin_editing1||group_admin_editing2)
                        {
                            len=strlen(group_admin_buffer);
                            cur_buffer=group_admin_buffer;
                        }
                        else if(group_name_editing)
                        {
                            len=strlen(group_name_buffer);
                            cur_buffer=group_name_buffer;
                        }
                        else if(message_editing)
                        {
                            len=strlen(message_buffer);
                            cur_buffer=message_buffer;
                        }
                        else if(new_passwd_editing1)
                        {
                            len=strlen(passwd_buffer);
                            cur_buffer=passwd_buffer;
                        }
                        else if(new_passwd_editing2)
                        {
                            len=strlen(repasswd_buffer);
                            cur_buffer=repasswd_buffer;
                        }

                        if(len == 0) return;
                    
                        if(len >= 3) 
                        {
                            unsigned char ch = cur_buffer[len-3];
                            //中文UTF-8首字节范围
                            if((ch & 0xE0) == 0xE0 && ch != 0xC0 && ch != 0xC1)
                            {
                                cur_buffer[len-3] = '\0';//删3字节
                                return;
                            }
                        }
                        cur_buffer[len-1] = '\0';//删1字节
                    }
                    break;
            case SDL_TEXTEDITING:
                    //std::cout<<event.edit.text<<std::endl;
                    break;
            case SDL_TEXTINPUT:
                     
                    if(admin_editing&&strlen(admin_buffer)+strlen(event.text.text)<=15)
                    {
                        strcat(admin_buffer,event.text.text);
                    }
                    if((group_admin_editing1||group_admin_editing2)&&strlen(group_admin_buffer)+strlen(event.text.text)<=15)
                    {
                        strcat(group_admin_buffer,event.text.text);
                    }
                    if((new_passwd_editing1)&&strlen(passwd_buffer)+strlen(event.text.text)<=15)
                    {
                        strcat(passwd_buffer,event.text.text);
                    }

                    if((new_passwd_editing2)&&strlen(repasswd_buffer)+strlen(event.text.text)<=15)
                    {
                       strcat(repasswd_buffer,event.text.text);
                    }

                    int len=0;
                    for(int i=0;i<strlen(group_name_buffer);i++)
                    {
                        if((group_name_buffer[i]&0xE0)==0xE0&&group_name_buffer[i]!=0xC0&&group_name_buffer[i]!=0xC1)
                        {
                            len+=3;//UTF-8中文统计为3个字节
                            i+=2;  //跳过一个完整的中文字符
                        }
                        else
                            len+=2;//数字字母其他统计为2个字节
                    }

                    for(int i=0;i<strlen(event.text.text);i++)
                    {
                        if((event.text.text[i]&0xE0)==0xE0&&event.text.text[i]!=0xC0&&event.text.text[i]!=0xC1)
                        {
                            len+=3;//UTF-8中文统计为3个字节
                            i+=2;  //跳过一个完整的中文字符
                        }
                        else
                            len+=2;//数字字母其他统计为2个字节
                    }
                    
                    if(group_name_editing&&len<=30)
                    {
                        strcat(group_name_buffer,event.text.text);
                    }

                    if(message_editing&&strlen(message_buffer)+strlen(event.text.text)<=435)
                    {
                        strcat(message_buffer,event.text.text);
                    }

                    break;
        }
    }
}

void level_1_check_status()
{
    if(cur_group_status_die)
    {
        LEVEL_1_MS=LEVEL_1_NONE2;
        cur_view_group=NULL;

        //还原标志位
        cur_group_status_die=false;
    }

    if(need_update_friend_list)
    {
        int type=11;
        tcpc1.DestroyFriendList();
        friend_cur_page=0;

        send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
        
        if(cur_friend_status_die)
        {
            LEVEL_1_MS=LEVEL_1_NONE2;
            cur_view_friend=NULL;
            cur_friend_status_die=false;
        }

        //wait for download friend list
        while(true)
        {
            if(download_friend_list)
            {
                download_friend_list=false;
                need_update_friend_list=false;
                break;
            }
        }
    }
}

void level_0_check_login_status()
{
    if
    (
        download_friend_list&&download_group_list&&download_friend_apply_list&&
        download_group_apply_list&&download_unicast_list
    )
    {
        //download group msg(must wait for group_list download finished)
        int type=13;
        group_node* find=tcpc1.g_list.head.next;
        while(find!=NULL)
        {
            send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
            send(tcpc1.GetSocket_fd(),(char*)&(find->data),sizeof(group_info),0);
            find=find->next;
        }

        if(find==NULL)
            download_multicast_list=true; 

        if(download_multicast_list)
        {
            //change window size
            SDL_SetWindowSize(win,WINDOW_WIDTH,WINDOW_HEIGHT);

            //等待所有下载完成后再切换状态
            CUR_STATUS=LEVEL_1;
            //确保下载完成再修改，否则可能会产生竞争
            cur_error.value=-999;

            download_multicast_list=false;
            download_friend_list=false;
            download_group_list=false;
            download_friend_apply_list=false;
            download_group_apply_list=false;
            download_unicast_list=false;
        }
    }
}
/*****************************************-----main modle-----*****************************************/

void init()
{   
    graphic_init();
    data_init();
    config_init();
}

void loop()
{
    std::thread recv_thread(RecvMessage);

    while(CUR_STATUS)
    {
        switch(CUR_STATUS)
        {
            case LEVEL_CONNECT_ERROR:
                draw_level_connect_error();
                event_loop_connect_error();
                break;
            case LEVEL_0_REGIST:
                draw_level_0_regist();
                event_loop_regist();
                break;
            case LEVEL_0_LOGIN:
                draw_level_0_login();
                event_loop_login();
                level_0_check_login_status();
                break;
            case LEVEL_1:
                draw_level_1_index();
                event_loop_index();
                //检测子线程的消息并修改状态
                level_1_check_status();
                break;
        }
    }
    recv_thread.join();
}

void destroy()
{
    SDL_DestroyRenderer(rdr);
    SDL_DestroyWindow(win);
    TTF_CloseFont(g_font);
    SDL_Quit();
}

/*****************************************-----main-----*****************************************/

int main(int argv,char* argc[])
{
    init();    
    loop();
    destroy();
    return 0;
}