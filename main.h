#include "GraphicOutPut.h"
#include "TcpClient.h"

/******************************************------global data------******************************************/
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 900

#define TITLE_X (WINDOW_WIDTH/2-80)
#define TITLE_Y (WINDOW_HEIGHT/2-240)

#define ADMIN_X (TITLE_X-80)
#define ADMIN_Y (TITLE_Y+120)
#define ADMIN_INPUT_X (ADMIN_X+55)
#define ADMIN_INPUT_Y (ADMIN_Y-5)

#define PASSWD_X (ADMIN_X)
#define PASSWD_Y (ADMIN_Y+60)
#define PASSWD_INPUT_X (PASSWD_X+55)
#define PASSWD_INPUT_Y (PASSWD_Y-5)

#define NAME_X (PASSWD_X)
#define NAME_Y (PASSWD_Y+60)
#define NAME_INPUT_X (NAME_X+55)
#define NAME_INPUT_Y (NAME_Y-5)

#define REPASSWD_X (NAME_X-40)
#define REPASSWD_Y (NAME_Y+60)
#define REPASSWD_INPUT_X (REPASSWD_X+95)
#define REPASSWD_INPUT_Y (REPASSWD_Y-5)

#define REGIST_BUTTON_X (NAME_X+20)
#define REGIST_BUTTON_Y (NAME_Y+120)

#define LOGIN_BUTTON_X (REGIST_BUTTON_X+100)
#define LOGIN_BUTTON_Y (REGIST_BUTTON_Y)

#define COMMIT_BUTTON_X (LOGIN_BUTTON_X+100)
#define COMMIT_BUTTON_Y (LOGIN_BUTTON_Y)

enum STATUS
{
    LEVEL_EXIT,
    LEVEL_0_REGIST,
    LEVEL_0_LOGIN,
    LEVEL_1,
    LEVEL_2,
    LEVEL_CONNECT_ERROR,
};

SDL_Event event;
SDL_Window* win;
SDL_Renderer* rdr;
SDL_Point mouse_pt;
SDL_Surface* win_surf;
STATUS CUR_STATUS=LEVEL_0_LOGIN;
TcpClient tcpc1("192.168.232.140",1026);

data_rule dr;
error_code cur_error={-999};
user_info cur_user;

char* cur_buffer;
char admin_buffer[256];
char passwd_buffer[256];
char name_buffer[256];
char repasswd_buffer[256];
char passwd_temp_buffer[256];
char group_name_buffer[256];
char group_admin_buffer[256];

bool admin_editing=false;
bool passwd_editing=false;
bool name_editing=false;
bool repasswd_editing=false;
bool group_name_editing=false;
bool group_admin_editing1=false;//join group
bool group_admin_editing2=false;//create group
/******************************************------level0 data------******************************************/

SDL_Rect admin_input_rect={ADMIN_INPUT_X,ADMIN_INPUT_Y,220,30};
SDL_Rect passwd_input_rect={PASSWD_INPUT_X,PASSWD_INPUT_Y,220,30};
SDL_Rect name_input_rect={NAME_INPUT_X,NAME_INPUT_Y,220,30};
SDL_Rect repasswd_input_rect={REPASSWD_INPUT_X,REPASSWD_INPUT_Y,220,30};

SDL_Rect regist_rect={REGIST_BUTTON_X,REGIST_BUTTON_Y,50,30};
SDL_Rect login_rect={LOGIN_BUTTON_X,LOGIN_BUTTON_Y,50,30};
SDL_Rect commit_rect={COMMIT_BUTTON_X,COMMIT_BUTTON_Y,50,30};
SDL_Rect level_0_error_rect={WINDOW_WIDTH/6,WINDOW_HEIGHT/4*3,WINDOW_WIDTH/6*4,80};

SDL_Color black={0,0,0,255};
SDL_Color white={255,255,255,255};
SDL_Color red={255,0,0,255};
SDL_Color green={0,220,0,255};
SDL_Color blue={0,0,255,255};

RectButton r_regist(regist_rect,black,white,20,black,"注册");
RectButton r_login(login_rect,black,white,20,black,"登录");
RectButton r_commit(commit_rect,black,white,20,black,"提交");
RectButton r_admin(admin_input_rect,black,white,20,black,NULL);
RectButton r_passwd(passwd_input_rect,black,white,20,black,NULL);
RectButton r_name(name_input_rect,black,white,20,black,NULL);
RectButton r_repasswd(repasswd_input_rect,black,white,20,black,NULL);

SDL_Rect r1_rect={0,25,150,275};
SDL_Rect r2_rect={0,275,150,25};
SDL_Rect r3_rect={0,325,150,275};
SDL_Rect r4_rect={0,575,150,25};
SDL_Rect r5_rect={0,620,150,300};

RectButton r1(r1_rect,black,white,20,white,NULL);
RectButton r2(r2_rect,black,white,14,white,NULL);
RectButton r3(r3_rect,black,white,20,white,NULL);
RectButton r4(r4_rect,black,white,20,white,NULL);
RectButton r5(r5_rect,black,white,20,white,NULL);

Text t_title(TITLE_X,TITLE_Y,black,60,"ZCat");
Text t_admin(ADMIN_X,ADMIN_Y,black,20,"账号");
Text t_passwd(PASSWD_X,PASSWD_Y,black,20,"密码");
Text t_name(NAME_X,NAME_Y,black,20,"昵称");
Text t_repasswd(REPASSWD_X,REPASSWD_Y,black,20,"确认密码");

/******************************************------level1 data------******************************************/

SDL_Rect level_1_error_rect={300,500,0,0};

//friend ui
Text friend_title_text(30,5,red,20,"好友列表");
SDL_Rect friend_content_rect[10]={{0,25,150,25}};
SDL_Rect friend_control_last_rect={0,275,50,25};
SDL_Rect friend_control_page_rect={50,275,50,25};
SDL_Rect friend_control_next_rect={100,275,50,25};
RectButton friend_content_button[10];
RectButton friend_control_last_button(friend_control_last_rect,black,white,15,black,"上一页");
RectButton friend_control_page_button(friend_control_page_rect,black,white,15,black,"0 / 0");
RectButton friend_control_next_button(friend_control_next_rect,black,white,15,black,"下一页");
int friend_total_page=0;
int friend_cur_page=1;
friend_node* cur_friend_page_first=NULL;

//group ui
Text group_title_text(30,305,red,20,"群聊列表");
SDL_Rect group_content_rect[10]={{0,325,150,25}};
SDL_Rect group_control_last_rect={0,575,50,25};
SDL_Rect group_control_page_rect={50,575,50,25};
SDL_Rect group_control_next_rect={100,575,50,25};
RectButton group_content_button[10];
RectButton group_control_last_button(group_control_last_rect,black,white,15,black,"上一页");
RectButton group_control_page_button(group_control_page_rect,black,white,15,black,"0 / 0");
RectButton group_control_next_button(group_control_next_rect,black,white,15,black,"下一页");
int group_total_page=0;
int group_cur_page=1;
group_node* cur_group_page_first=NULL;

enum LEVEL_1_TOOL_STATUS
{
    LEVEL_1_NONE,
    LEVEL_1_ADD_FRIEND,
    LEVEL_1_FRIEND_APPLY,
    LEVEL_1_JOIN_GROUP,
    LEVEL_1_GROUP_APPLY,
    LEVEL_1_CREATE_GROUP
};

//功能状态机
LEVEL_1_TOOL_STATUS LEVEL_1_TS=LEVEL_1_NONE;

//tool ui
Text tool_title_text(30,605,red,20,"工具列表");
SDL_Rect tool_add_friend_rect={0,625,150,25};
SDL_Rect tool_friend_apply_rect={0,650,150,25};
SDL_Rect tool_join_group_rect={0,675,150,25};
SDL_Rect tool_group_apply_rect={0,700,150,25};
SDL_Rect tool_create_group_rect={0,725,150,25};
SDL_Rect tool_1_rect={0,750,150,25};//1
SDL_Rect tool_2_rect={0,775,150,25};//2
SDL_Rect tool_setting_rect={0,800,150,25};
SDL_Rect tool_help_rect={0,825,150,25};
SDL_Rect tool_version_rect={0,850,150,25};
SDL_Rect tool_more_information_rect={0,875,150,25};

RectButton tool_add_friend_button(tool_add_friend_rect,black,white,15,black,"添加好友");
RectButton tool_friend_apply_button(tool_friend_apply_rect,black,white,15,black,"好友申请");
RectButton tool_join_group_button(tool_join_group_rect,black,white,15,black,"加入群聊");
RectButton tool_group_apply_button(tool_group_apply_rect,black,white,15,black,"群聊通知");
RectButton tool_create_group_button(tool_create_group_rect,black,white,15,black,"创建群聊");
RectButton tool__1_button(tool_1_rect,black,white,15,black,"...");//1
RectButton tool__2_button(tool_2_rect,black,white,15,black,"联系客服");//2
RectButton tool_setting_button(tool_setting_rect,black,white,15,black,"更多设置");
RectButton tool_help_button(tool_help_rect,black,white,15,black,"帮助文档");
RectButton tool_version_button(tool_version_rect,black,white,15,black,"版本迭代");
RectButton tool_more_information_button(tool_more_information_rect,black,white,15,black,"关于我们");

Text cur_admin_text={310,10,blue,20,"当前账号："};
SDL_Rect exit_login_rect={WINDOW_WIDTH-95,5,90,30};
RectButton exit_login_button={exit_login_rect,black,white,20,red,"退出登录"};

//tool son ui
SDL_Rect tool_all_rect={225,325,400,275};
SDL_Rect close_page_rect={225,575,50,25};
SDL_Rect commit_page_rect={575,575,50,25};
SDL_Rect clear_page_rect={575,575,50,25};
RectButton tool_all_button={tool_all_rect,black,white,20,white,NULL};
RectButton close_page_button(close_page_rect,black,white,18,black,"关闭");
RectButton commit_page_button(commit_page_rect,black,white,18,black,"提交");
RectButton clear_page_button(commit_page_rect,black,white,18,black,"清空");

Text add_friend_title(365,300,red,25,"添加好友");
Text add_friend_text(300,405,black,20,"输入账号");
SDL_Rect add_friend_input_rect={390,400,150,30};
RectButton add_friend_input_button(add_friend_input_rect,black,white,20,white,NULL);

Text join_group_title(365,300,red,25,"加入群聊");
Text join_group_text(300,405,black,20,"输入账号");
SDL_Rect join_group_input_rect={390,400,150,30};
RectButton join_group_input_button(join_group_input_rect,black,white,20,white,NULL);

Text create_group_title(365,300,red,25,"创建群聊");
Text create_group_admin_text(300,405,black,20,"输入账号");
Text create_group_name_text(300,440,black,20,"输入群名");
SDL_Rect create_group_admin_input_rect={390,400,150,30};
RectButton create_group_admin_input_button(create_group_admin_input_rect,black,white,20,white,NULL);
SDL_Rect create_group_name_input_rect={390,435,150,30};
RectButton create_group_name_input_button(create_group_name_input_rect,black,white,20,white,NULL);


Text friend_apply_title(365,300,red,25,"好友申请");
SDL_Rect friend_apply_rect[10]={{225,325,300,25}};
RectButton friend_apply_button[10];

Text group_apply_title(365,300,red,25,"群聊通知");
SDL_Rect group_apply_rect[10]={225,325,300,25};
RectButton group_apply_button[10];

SDL_Rect permit_apply_rect[10]={{525,325,50,25}};
SDL_Rect refuse_apply_rect[10]={{575,325,50,25}};
RectButton permit_button[10];
RectButton refuse_button[10];

//friend_apply_list ui and group_apply_list ui
int friend_apply_total_page=0;
int friend_apply_cur_page=1;
friend_apply_node* cur_friend_apply_page_first=NULL;

int group_apply_total_page=0;
int group_apply_cur_page=1;
group_apply_node* cur_group_apply_page_first=NULL;

SDL_Rect apply_control_last_rect={275,575,50,25};
SDL_Rect apply_control_page_rect={325,575,200,25};
SDL_Rect apply_control_next_rect={525,575,50,25};
RectButton apply_control_last_button(apply_control_last_rect,black,white,15,black,"上一页");
RectButton apply_control_page_button(apply_control_page_rect,black,white,15,black,"0 / 0");
RectButton apply_control_next_button(apply_control_next_rect,black,white,15,black,"下一页");

/******************************************------global function------******************************************/

void clear_input_buffer()
{
    cur_buffer=NULL;
    memset(admin_buffer,0,sizeof(admin_buffer));
    memset(passwd_buffer,0,sizeof(passwd_buffer));
    memset(name_buffer,0,sizeof(name_buffer));
    memset(repasswd_buffer,0,sizeof(repasswd_buffer));
    
    memset(passwd_temp_buffer,0,sizeof(passwd_temp_buffer));

    memset(group_name_buffer,0,sizeof(group_name_buffer));
    memset(group_admin_buffer,0,sizeof(group_admin_buffer));
}

void clear_last_admin_data()
{
    //clear last remain status
    exit_login_button.fill_color.r=255;
    exit_login_button.fill_color.g=255;
    exit_login_button.fill_color.b=255;

    //clear last admin data1
    memset(&cur_user,0,sizeof(cur_user));
    memset(&cur_admin_text,0,sizeof(cur_admin_text));
    cur_error.value=-999;

    //clear last admin data2
    tcpc1.DestroyFriendList();
    tcpc1.DestroyGroupList();
    tcpc1.DestroyFriendApplyList();
    tcpc1.DestroyGroupApplyList();

    //clear last admin data3
    friend_total_page=0;
    group_total_page=0;
    friend_cur_page=1;
    group_cur_page=1;
    cur_friend_page_first=NULL;
    cur_group_page_first=NULL;

    friend_apply_total_page=0;
    group_apply_total_page=0;
    friend_apply_cur_page=1;
    group_apply_cur_page=1;
    cur_friend_apply_page_first=NULL;
    cur_group_apply_page_first=NULL;

    //clear last admin data4
    LEVEL_1_TS=LEVEL_1_NONE;
}

void data_init()
{
    //init data
    friend_content_rect[0]={0,25,150,25};
    group_content_rect[0]={0,325,150,25};
    friend_apply_rect[0]={225,325,300,25};
    group_apply_rect[0]={225,325,300,25};

    permit_apply_rect[0]={525,325,50,25};
    refuse_apply_rect[0]={575,325,50,25};

    for(int i=0;i<10;i++)
    {
        if(i!=0)
        {
            friend_content_rect[i]=friend_content_rect[i-1];
            friend_content_rect[i].y+=25;

            group_content_rect[i]=group_content_rect[i-1];
            group_content_rect[i].y+=25;

            friend_apply_rect[i]=friend_apply_rect[i-1];
            friend_apply_rect[i].y+=25;

            group_apply_rect[i]=group_apply_rect[i-1];
            group_apply_rect[i].y+=25;

            permit_apply_rect[i]=permit_apply_rect[i-1];
            permit_apply_rect[i].y+=25;

            refuse_apply_rect[i]=refuse_apply_rect[i-1];
            refuse_apply_rect[i].y+=25;
        }

        friend_content_button[i].SetValue(friend_content_rect[i],black,white,15,black,NULL);
        group_content_button[i].SetValue(group_content_rect[i],black,white,15,black,NULL);
        friend_apply_button[i].SetValue(friend_apply_rect[i],black,white,15,black,NULL);
        group_apply_button[i].SetValue(group_apply_rect[i],black,white,15,black,NULL);

        permit_button[i].SetValue(permit_apply_rect[i],black,white,15,black,"同意");
        refuse_button[i].SetValue(refuse_apply_rect[i],black,white,15,black,"拒绝");
    }
}

void config_init()
{
    if(!tcpc1.Connect())
    {
        CUR_STATUS=LEVEL_CONNECT_ERROR;
    }

    if(CUR_STATUS!=LEVEL_CONNECT_ERROR)
    {
        tcpc1.get_data_rule();
    }

    win=SDL_CreateWindow
    (
        "Zcat",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    rdr=SDL_CreateRenderer(win,-1,0);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}