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
        strcpy(cur_admin_text.text,"当前账号：");
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

        CUR_STATUS=LEVEL_1;
        cur_error.value=-999;
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
    }
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
    }
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

        friend_apply_node* look=tcpc1.fa_list.head.next;
        while(look!=NULL)
        {
            std::cout<<look->data.admin<<" "<<look->data.name<<" 请求添加为好友"<<std::endl;
            look=look->next;
        }

        cur_friend_apply_page_first=tcpc1.fa_list.head.next;
    }
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
        
        std::cout<<"total size:"<<size<<std::endl;
        group_apply_node* look=tcpc1.ga_list.head.next;
        while(look!=NULL)
        {
            std::cout<<"user id:"<<look->data.ui.id<<std::endl;
            std::cout<<look->data.ui.admin<<" "<<look->data.ui.name<<"请求加入群聊 "<<look->data.gi.name<<std::endl;
            look=look->next;
        }

        cur_group_apply_page_first=tcpc1.ga_list.head.next;
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
    recv(tcpc1.GetSocket_fd(),(char*)&cur_error,sizeof(cur_error),0);
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
            case 0:
                get_data_rule_msg();
                break;
            case 1:
                get_regist_msg();
                break;
            case 2:
                get_login_msg();
                break;
            case 3:
                get_apply_friend_msg();
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                break;
            case 10:
                get_friend_apply_list_msg();
                break;
            case 11:
                get_friend_list_msg();
                break;
            case 12:
                break;
            case 13:
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
                break;
            case 19:
                break;
            case 20:
                break;
            case 21:
                break;
            case 22:
                break;
            case -1:
                break;
            case -2:
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
            case -8:
                break;
        }
    }
}

/*****************************************-----level0-----*****************************************/

void draw_level_0_error_0()
{
    RectButton r1(level_0_error_rect,black,white,18,red,"操作成功！");
    r1.Draw(rdr);
}

void draw_level_0_error_1()
{
    RectButton r1(level_0_error_rect,black,white,18,red,"账号格式错误！允许的字符：字母 数字 _ @ # & *");
    r1.Draw(rdr);
}

void draw_level_0_error_2()
{
    RectButton r1(level_0_error_rect,black,white,18,red,"账号已经存在！");
    r1.Draw(rdr);
}

void draw_level_0_error_3()
{
    RectButton r1(level_0_error_rect,black,white,18,red,"密码格式错误！允许的字符：字母 数字 _ @ # & *");
    r1.Draw(rdr);
}

void draw_level_0_error_4()
{
    RectButton r1(level_0_error_rect,black,white,18,red,"账号不存在！");
    r1.Draw(rdr);
}

void draw_level_0_error_5()
{
    RectButton r1(level_0_error_rect,black,white,18,red,"密码错误！");
    r1.Draw(rdr);
}

void draw_level_0_error_8()
{
    RectButton r1(level_0_error_rect,black,white,18,red,"账号正在使用中！");
    r1.Draw(rdr);
}

void draw_level_0_error_12()
{
    RectButton r1(level_0_error_rect,black,white,18,red,"密码不一致！");
    r1.Draw(rdr);
}

void draw_level_0_error()
{
    switch(cur_error.value)
    {
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

    Text t1(TITLE_X-210,TITLE_Y-20,red,40,"连接失败");
    Text t2(TITLE_X-210,TITLE_Y+55,black,20,"1.网络环境不佳，请稍后重试...");
    Text t3(TITLE_X-210,TITLE_Y+100,black,20,"2.服务器关闭，请联系管理员(18271312836@163.com)");
    
    SDL_Rect rect1={TITLE_X-220,TITLE_Y+40,485,100};
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
                r_regist.InteractResponse_green(mouse_pt,regist_rect);
                r_login.InteractResponse_green(mouse_pt,login_rect);
                r_commit.InteractResponse_green(mouse_pt,commit_rect);

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

                if(SDL_PointInRect(&mouse_pt,&commit_rect)&&(event.button.button==SDL_BUTTON_LEFT))
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

                    std::cout<<"已提交："<<std::endl;
                    std::cout<<"账号 "<<admin_buffer<<std::endl;
                    std::cout<<"密码 "<<passwd_buffer<<std::endl;
                    std::cout<<"昵称 "<<name_buffer<<std::endl;
                    std::cout<<"确认密码 "<<repasswd_buffer<<std::endl;

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
                if(name_editing&&strlen(name_buffer)+strlen(event.text.text)<=30)
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
                r_regist.InteractResponse_green(mouse_pt,regist_rect);
                r_login.InteractResponse_green(mouse_pt,login_rect);
                r_commit.InteractResponse_green(mouse_pt,commit_rect);
    
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

                if(SDL_PointInRect(&mouse_pt,&commit_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    int type=2;
                    user_info ug;
                    strcpy(ug.admin,admin_buffer);
                    strcpy(ug.passwd,passwd_buffer);

                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&ug,sizeof(ug),0);

                    std::cout<<"已提交："<<std::endl;
                    std::cout<<"账号 "<<admin_buffer<<std::endl;
                    std::cout<<"密码 "<<passwd_buffer<<std::endl;

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

void draw_level_1_error_3()
{
    
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

void draw_level_1_error_7()
{
    
}

void draw_level_1_error_9()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"已经是好友!");
    t1.Draw(rdr);
}

void draw_level_1_error_11()
{
    Text t1(level_1_error_rect.x,level_1_error_rect.y,red,16,"已在群聊中!");
    t1.Draw(rdr);
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
        case 11:
            draw_level_1_error_11();
            break;
        case 12:
            ;
            break;
    }
}

void draw_level_1_add_friend()
{
    tool_all_button.Draw(rdr);
    add_friend_title.Draw(rdr);

    add_friend_text.Draw(rdr);
    add_friend_input_button.Draw(rdr);

    close_page_button.Draw(rdr);
    commit_page_button.Draw(rdr);
}

void draw_level_1_join_group()
{
    tool_all_button.Draw(rdr);
    join_group_title.Draw(rdr);

    join_group_text.Draw(rdr);
    join_group_input_button.Draw(rdr);

    close_page_button.Draw(rdr);
    commit_page_button.Draw(rdr);
}

void draw_level_1_create_group()
{
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
    tool_all_button.Draw(rdr);
    friend_apply_title.Draw(rdr);

    for(int i=0;i<10;i++)
    {
        if(strlen(friend_apply_button[i].text)>0)
        {
            friend_apply_button[i].Draw(rdr);
            permit_button[i].Draw(rdr);
            refuse_button[i].Draw(rdr);
        }
    }
    
    apply_control_last_button.Draw(rdr);
    apply_control_page_button.Draw(rdr);
    apply_control_next_button.Draw(rdr);

    close_page_button.Draw(rdr);
    commit_page_button.Draw(rdr);
}

void draw_level_1_group_apply_list()
{
    tool_all_button.Draw(rdr);
    group_apply_title.Draw(rdr);

    for(int i=0;i<10;i++)
    {
        if(strlen(group_apply_button[i].text)>0)
        {
            group_apply_button[i].Draw(rdr);
            permit_button[i].Draw(rdr);
            refuse_button[i].Draw(rdr);
        }
    }

    apply_control_last_button.Draw(rdr);
    apply_control_page_button.Draw(rdr);
    apply_control_next_button.Draw(rdr);

    close_page_button.Draw(rdr);
    commit_page_button.Draw(rdr);
}

void GetListPageToButton(int cur,int total,RectButton& des_button)
{
    char str1[32];
    const char* str2=" / ";
    char str3[32];

    snprintf(str1, sizeof(str1),"%d",cur);
    snprintf(str3, sizeof(str3),"%d",total);
    strcat(str1,str2);
    strcat(str1,str3);
    strcpy(des_button.text,str1);
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
        memset(friend_content_button[i].text,0,sizeof(friend_content_button[i].text));
    }
    
    friend_total_page=tcpc1.f_list.len/10;
    if(tcpc1.f_list.len-friend_total_page*10>0)
        friend_total_page++;
    GetListPageToButton(friend_cur_page,friend_total_page,friend_control_page_button);

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
        memset(group_content_button[i].text,0,sizeof(group_content_button[i].text));
    }

    group_total_page=tcpc1.g_list.len/10;
    if(tcpc1.g_list.len-group_total_page*10>0)
        group_total_page++;
    GetListPageToButton(group_cur_page,group_total_page,group_control_page_button);

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
    tool__1_button.Draw(rdr);
    tool__2_button.Draw(rdr);
    tool_setting_button.Draw(rdr);
    tool_help_button.Draw(rdr);
    tool_version_button.Draw(rdr);
    tool_more_information_button.Draw(rdr);
    //main_content_mod
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
    }

    //exit_mod
    cur_admin_text.Draw(rdr);
    exit_login_button.Draw(rdr);

    //input_mod
    if(strlen(admin_buffer)>0)
    {
        Text text(393,408,black,14,admin_buffer);
        text.Draw(rdr);
    }

    if(strlen(group_admin_buffer)>0)
    {
        Text text(393,408,black,14,group_admin_buffer);
        text.Draw(rdr);
    }

    if(strlen(group_name_buffer)>0)
    {
        Text text(393,443,black,14,group_name_buffer);
        text.Draw(rdr);
    }

    draw_level_1_error();

    SDL_RenderPresent(rdr);
}

//level1_event_loop

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
                exit_login_button.InteractResponse_green(mouse_pt,exit_login_rect);
                
                for(int i=0;i<10;i++)
                    friend_content_button[i].InteractResponse_green(mouse_pt,friend_content_rect[i]);

                friend_control_last_button.InteractResponse_green(mouse_pt,friend_control_last_rect);
                friend_control_next_button.InteractResponse_green(mouse_pt,friend_control_next_rect);

                for(int i=0;i<10;i++)
                    group_content_button[i].InteractResponse_green(mouse_pt,group_content_rect[i]);

                if(LEVEL_1_TS==LEVEL_1_FRIEND_APPLY||LEVEL_1_TS==LEVEL_1_GROUP_APPLY)
                {
                    for(int i=0;i<10;i++)
                        permit_button[i].InteractResponse_green(mouse_pt,permit_apply_rect[i]);

                    for(int i=0;i<10;i++)
                        refuse_button[i].InteractResponse_red(mouse_pt,refuse_apply_rect[i]);
                }

                group_control_last_button.InteractResponse_green(mouse_pt,group_control_last_rect);
                group_control_next_button.InteractResponse_green(mouse_pt,group_control_next_rect);

                tool_add_friend_button.InteractResponse_green(mouse_pt,tool_add_friend_rect);
                tool_friend_apply_button.InteractResponse_green(mouse_pt,tool_friend_apply_rect);
                tool_join_group_button.InteractResponse_green(mouse_pt,tool_join_group_rect);
                tool_group_apply_button.InteractResponse_green(mouse_pt,tool_group_apply_rect);
                tool_create_group_button.InteractResponse_green(mouse_pt,tool_create_group_rect);
                tool__1_button.InteractResponse_green(mouse_pt,tool_1_rect);
                tool__2_button.InteractResponse_green(mouse_pt,tool_2_rect);
                tool_setting_button.InteractResponse_green(mouse_pt,tool_setting_rect);
                tool_help_button.InteractResponse_green(mouse_pt,tool_help_rect);
                tool_version_button.InteractResponse_green(mouse_pt,tool_version_rect);
                tool_more_information_button.InteractResponse_green(mouse_pt,tool_more_information_rect);

                if(LEVEL_1_TS!=LEVEL_1_NONE)
                {
                    close_page_button.InteractResponse_green(mouse_pt,close_page_rect);
                    commit_page_button.InteractResponse_green(mouse_pt,commit_page_rect);
                }

                if(LEVEL_1_TS==LEVEL_1_FRIEND_APPLY||LEVEL_1_TS==LEVEL_1_GROUP_APPLY)
                {
                    apply_control_last_button.InteractResponse_green(mouse_pt,apply_control_last_rect);
                    apply_control_next_button.InteractResponse_green(mouse_pt,apply_control_next_rect);
                }

                break;
                
                case SDL_MOUSEBUTTONDOWN:
                mouse_pt.x=event.motion.x,mouse_pt.y=event.motion.y;
                if(SDL_PointInRect(&mouse_pt,&exit_login_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    int type=6;
                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    
                    CUR_STATUS=LEVEL_0_LOGIN;
                    std::cout<<"已退出登录"<<std::endl;
                    clear_last_admin_data();
                }

                for(int i=0;i<10;i++)
                {
                    if(SDL_PointInRect(&mouse_pt,&(friend_content_rect[i]))&&(event.button.button==SDL_BUTTON_LEFT))
                    {
                        if(temp!=NULL)
                        {
                            std::cout<<temp->data.id<<" "<<temp->data.name<<std::endl;
                        }
                    }

                    if(SDL_PointInRect(&mouse_pt,&(group_content_rect[i]))&&(event.button.button==SDL_BUTTON_LEFT))
                    {
                        if(temp2!=NULL)
                        {
                            std::cout<<temp2->data.id<<" "<<temp2->data.name<<std::endl;
                        }
                    }
                    
                    if(temp!=NULL)
                        temp=temp->next;

                    if(temp2!=NULL)
                        temp2=temp2->next;
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
                    if(tcpc1.f_list.len>10*friend_cur_page&&cur_friend_page_first->next!=&(tcpc1.f_list.head))
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
                    if(tcpc1.g_list.len>10*group_cur_page&&cur_group_page_first->next!=&(tcpc1.g_list.head))
                    {
                        for(int i=0;i<10;i++)
                            cur_group_page_first=cur_group_page_first->next;
                        group_cur_page++;
                    }
                }

                if(LEVEL_1_TS!=LEVEL_1_NONE&&SDL_PointInRect(&mouse_pt,&close_page_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_NONE;
                    cur_error.value=-999;
                    clear_input_buffer();
                }

                //commit_page_rect&&...
                /////////////////////////////////////////////////////////////////////////////////////////////////////
                if(SDL_PointInRect(&mouse_pt,&commit_page_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&LEVEL_1_TS==LEVEL_1_ADD_FRIEND)
                {
                    int type=3;
                    user_info ui;
                    strcpy(ui.admin,admin_buffer);

                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&ui,sizeof(ui),0);

                    std::cout<<"已提交："<<std::endl;
                    std::cout<<"账号 "<<admin_buffer<<std::endl;

                    r_commit.fill_color.r=255;
                    r_commit.fill_color.g=255;
                    r_commit.fill_color.b=255;
                    clear_input_buffer();
                }

                if(SDL_PointInRect(&mouse_pt,&commit_page_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&LEVEL_1_TS==LEVEL_1_JOIN_GROUP)
                {
                    int type=15;
                    group_info gi;
                    strcpy(gi.admin,group_admin_buffer);

                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&gi,sizeof(gi),0);

                    std::cout<<"已提交："<<std::endl;
                    std::cout<<"账号 "<<group_admin_buffer<<std::endl;

                    r_commit.fill_color.r=255;
                    r_commit.fill_color.g=255;
                    r_commit.fill_color.b=255;
                    clear_input_buffer();
                }

                if(SDL_PointInRect(&mouse_pt,&commit_page_rect)&&(event.button.button==SDL_BUTTON_LEFT)&&LEVEL_1_TS==LEVEL_1_CREATE_GROUP)
                {
                    int type=14;
                    group_info gi;
                    strcpy(gi.admin,group_admin_buffer);
                    strcpy(gi.name,group_name_buffer);
                    gi.manager_id=cur_user.id;

                    send(tcpc1.GetSocket_fd(),(char*)&type,sizeof(type),0);
                    send(tcpc1.GetSocket_fd(),(char*)&gi,sizeof(gi),0);
                    
                    std::cout<<"已提交："<<std::endl;
                    std::cout<<"账号 "<<group_admin_buffer<<std::endl;
                    std::cout<<"群名 "<<group_name_buffer<<std::endl;
                    
                    r_commit.fill_color.r=255;
                    r_commit.fill_color.g=255;
                    r_commit.fill_color.b=255;
                    clear_input_buffer();
                }
                /////////////////////////////////////////////////////////////////////////////////////////////////////

                if(LEVEL_1_TS==LEVEL_1_NONE&&SDL_PointInRect(&mouse_pt,&tool_add_friend_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_ADD_FRIEND;
                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
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

                if(LEVEL_1_TS==LEVEL_1_NONE&&SDL_PointInRect(&mouse_pt,&tool_join_group_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_JOIN_GROUP;
                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
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

                if(LEVEL_1_TS==LEVEL_1_NONE&&SDL_PointInRect(&mouse_pt,&tool_create_group_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_CREATE_GROUP;
                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
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


                if(LEVEL_1_TS==LEVEL_1_NONE&&SDL_PointInRect(&mouse_pt,&tool_friend_apply_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_FRIEND_APPLY;
                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
                }

                if(LEVEL_1_TS==LEVEL_1_NONE&&SDL_PointInRect(&mouse_pt,&tool_group_apply_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    LEVEL_1_TS=LEVEL_1_GROUP_APPLY;
                    //clear last status
                    close_page_button.fill_color.r=255;
                    close_page_button.fill_color.g=255;
                    close_page_button.fill_color.b=255;
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
                    if(group_name_editing&&strlen(group_name_buffer)+strlen(event.text.text)<=30)
                    {
                        strcat(group_name_buffer,event.text.text);
                    }

                    break;
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
                break;
            case LEVEL_1:
                draw_level_1_index();
                event_loop_index();
                break;
        }
    }
    recv_thread.join();
}

void destroy()
{
    SDL_DestroyRenderer(rdr);
    SDL_DestroyWindow(win);
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