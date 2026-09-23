#include "GraphicOutPut.h"
#include "TcpClient.h"
#include<mutex>
#include<fstream>
/******************************************------global data------******************************************/
#define LEVEL_0_WIDTH 500 //level 0
#define LEVEL_0_HEIGHT 700

#define WINDOW_WIDTH 575  //level 1
#define WINDOW_HEIGHT 775

#define TITLE_X (LEVEL_0_WIDTH/2-70)
#define TITLE_Y (LEVEL_0_HEIGHT/2-240)

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
TcpClient tcpc1("NULL",0);
char ip_str[32]={0};
char port_str[32]={0};
int port=1026;

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
char message_buffer[1025];

bool admin_editing=false;
bool passwd_editing=false;
bool name_editing=false;
bool repasswd_editing=false;
bool group_name_editing=false;
bool group_admin_editing1=false;//join group
bool group_admin_editing2=false;//create group
bool message_editing=false;

bool new_passwd_editing1=false;
bool new_passwd_editing2=false;
/******************************************------level0 data------******************************************/

SDL_Rect admin_input_rect={ADMIN_INPUT_X,ADMIN_INPUT_Y,220,30};
SDL_Rect passwd_input_rect={PASSWD_INPUT_X,PASSWD_INPUT_Y,220,30};
SDL_Rect name_input_rect={NAME_INPUT_X,NAME_INPUT_Y,220,30};
SDL_Rect repasswd_input_rect={REPASSWD_INPUT_X,REPASSWD_INPUT_Y,220,30};

SDL_Rect regist_rect={REGIST_BUTTON_X,REGIST_BUTTON_Y,50,30};
SDL_Rect login_rect={LOGIN_BUTTON_X,LOGIN_BUTTON_Y,50,30};
SDL_Rect commit_rect={COMMIT_BUTTON_X,COMMIT_BUTTON_Y,50,30};
SDL_Rect level_0_error_rect={LEVEL_0_WIDTH/6,LEVEL_0_HEIGHT/4*3,LEVEL_0_WIDTH/6*4,80};

SDL_Color black={0,0,0,255};
SDL_Color white={255,255,255,255};
SDL_Color white_low={245,250,245,255};
SDL_Color red={255,0,0,255};
SDL_Color green={0,220,0,255};
SDL_Color blue={0,0,255,255};
SDL_Color sky_bule={220,230,255,255};

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

SDL_Rect level_1_error_rect={250,400,0,0};

//friend ui
Text friend_title_text(30,5,red,20,"好友列表");
SDL_Rect friend_content_rect[10]={{0,25,150,25}};
SDL_Rect friend_control_last_rect={0,275,54,25};
SDL_Rect friend_control_page_rect={54,275,52,25};
SDL_Rect friend_control_next_rect={106,275,54,25};
RectButton friend_content_button[10];
RectButton friend_control_last_button(friend_control_last_rect,black,white,15,black,"上一页");
RectButton friend_control_page_button(friend_control_page_rect,black,white,15,black,"0 / 0");
RectButton friend_control_next_button(friend_control_next_rect,black,white,15,black,"下一页");
int friend_total_page=0;
int friend_cur_page=0;
friend_node* cur_friend_page_first=NULL;

//group ui
Text group_title_text(30,305,red,20,"群聊列表");
SDL_Rect group_content_rect[10]={{0,325,150,25}};
SDL_Rect group_control_last_rect={0,575,54,25};
SDL_Rect group_control_page_rect={54,575,52,25};
SDL_Rect group_control_next_rect={106,575,54,25};
RectButton group_content_button[10];
RectButton group_control_last_button(group_control_last_rect,black,white,15,black,"上一页");
RectButton group_control_page_button(group_control_page_rect,black,white,15,black,"0 / 0");
RectButton group_control_next_button(group_control_next_rect,black,white,15,black,"下一页");
int group_total_page=0;
int group_cur_page=0;
group_node* cur_group_page_first=NULL;

enum LEVEL_1_TOOL_STATUS
{
    LEVEL_1_NONE,
    LEVEL_1_ADD_FRIEND,
    LEVEL_1_FRIEND_APPLY,
    LEVEL_1_JOIN_GROUP,
    LEVEL_1_GROUP_APPLY,
    LEVEL_1_CREATE_GROUP,
    LEVEL_1_SETTING
};

enum LEVEL_1_MAIN_STATUS
{
    LEVEL_1_NONE2,
    LEVEL_1_FRIEND_MSG_CONTENT,
    LEVEL_1_GROUP_MSG_CONTENT,
    LEVEL_1_GROUP_MEMBER_MSG_CONTENT,
    LEVEL_1_HELP,
    LEVEL_1_ABOUT_ME,
    LEVEL_1_VERSION,
    LEVEL_1_CHANGE_PASSWD,
    LEVEL_1_STYLE
};

//功能状态机1
LEVEL_1_TOOL_STATUS LEVEL_1_TS=LEVEL_1_NONE;
//功能状态机2
LEVEL_1_MAIN_STATUS LEVEL_1_MS=LEVEL_1_NONE2;

//tool ui
Text tool_title_text(30,605,red,20,"工具列表");
SDL_Rect tool_add_friend_rect={0,625,160,25};
SDL_Rect tool_friend_apply_rect={0,650,160,25};
SDL_Rect tool_join_group_rect={0,675,160,25};
SDL_Rect tool_group_apply_rect={0,700,160,25};
SDL_Rect tool_create_group_rect={0,725,160,25};
SDL_Rect tool_setting_rect={0,750,160,25};

RectButton tool_add_friend_button(tool_add_friend_rect,black,white,15,black,"添加好友");
RectButton tool_friend_apply_button(tool_friend_apply_rect,black,white,15,black,"好友申请");
RectButton tool_join_group_button(tool_join_group_rect,black,white,15,black,"加入群聊");
RectButton tool_group_apply_button(tool_group_apply_rect,black,white,15,black,"群聊通知");
RectButton tool_create_group_button(tool_create_group_rect,black,white,15,black,"创建群聊");
RectButton tool_setting_button(tool_setting_rect,black,white,15,black,"更多设置");

//tool son ui
SDL_Rect tool_all_rect={175,225,400,275};
SDL_Rect close_page_rect={175,475,50,25};
SDL_Rect commit_page_rect={525,475,50,25};
SDL_Rect clear_page_rect={525,475,50,25};
RectButton tool_all_button={tool_all_rect,black,white,20,white,NULL};
RectButton close_page_button(close_page_rect,black,white,18,black,"关闭");
RectButton commit_page_button(commit_page_rect,black,white,18,black,"提交");
RectButton clear_page_button(commit_page_rect,black,white,18,black,"清空");

Text add_friend_title(325,200,red,25,"添加好友");
Text add_friend_text(250,305,black,20,"输入账号");
SDL_Rect add_friend_input_rect={350,300,150,30};
RectButton add_friend_input_button(add_friend_input_rect,black,white,20,white,NULL);

Text join_group_title(325,200,red,25,"加入群聊");
Text join_group_text(250,305,black,20,"输入账号");
SDL_Rect join_group_input_rect={350,300,150,30};
RectButton join_group_input_button(join_group_input_rect,black,white,20,white,NULL);

Text create_group_title(325,200,red,25,"创建群聊");
Text create_group_admin_text(250,305,black,20,"输入账号");
Text create_group_name_text(250,340,black,20,"输入群名");
SDL_Rect create_group_admin_input_rect={350,300,150,30};
RectButton create_group_admin_input_button(create_group_admin_input_rect,black,white,20,white,NULL);
SDL_Rect create_group_name_input_rect={350,335,150,30};
RectButton create_group_name_input_button(create_group_name_input_rect,black,white,20,white,NULL);

//more setting
SDL_Rect msg_setting_base_rect={175,25,400,750};
RectButton msg_setting_base_button(msg_setting_base_rect,black,white,20,black,""); 

Text setting_title(325,200,red,25,"更多设置");
Text cur_admin_text={175,230,blue,18,"当前账号:"};


SDL_Rect exit_login_rect={325,440,100,28};
RectButton exit_login_button={exit_login_rect,black,white,18,black," 退出登录"};


SDL_Rect help_rect={325,412,100,28};
RectButton help_button={help_rect,black,white,18,black," 帮助文档"};
int help_padding=25;
Text help_title(310,55,red,30,"帮助文档");
Text help_c1(185,105,black,18,"1.账号密码规则：只允许字母 数字 _ @ # & *");
Text help_c2(help_c1.text_rect.x,help_c1.text_rect.y+help_padding,black,18,"2.为保证账号的安全，请提高设定密码的复杂性");
Text help_c3(help_c2.text_rect.x,help_c2.text_rect.y+help_padding,black,18,"3.账号若被封禁请联系：18271312836@163.com");
Text help_c4(help_c3.text_rect.x,help_c3.text_rect.y+help_padding,black,18,"4.鼠标点击输入框，当边框为绿时则为输入状态");
Text help_c5(help_c4.text_rect.x,help_c4.text_rect.y+help_padding,black,18,"5.退格用于删除单个字符，点击按钮以提交数据");
Text help_c6(help_c5.text_rect.x,help_c5.text_rect.y+help_padding,black,18,"6.当输入的数据达到最大限制时无法再进行输入");
Text help_c7(help_c6.text_rect.x,help_c6.text_rect.y+help_padding,black,18,"7.聊天页面使用鼠标滚轮来查看历史的对话信息");
Text help_c8(help_c7.text_rect.x,help_c7.text_rect.y+help_padding,black,18,"8.请不要随意修改软件的目录结构，否则会报错");
Text help_c9(help_c8.text_rect.x,help_c8.text_rect.y+help_padding,black,18,"9.若想从桌面启动，请创建执行文件的桌面快捷");
Text help_c10(help_c9.text_rect.x,help_c9.text_rect.y+help_padding,black,18,"10.卸载软件时删除整个文件，无需清理注册表");
Text help_c11(help_c10.text_rect.x,help_c10.text_rect.y+help_padding,black,18,"11.创建群聊的数量有最大限制，群主无法退群");
Text help_c12(help_c11.text_rect.x,help_c11.text_rect.y+help_padding,black,18,"12.SERVER_CONFIG.txt可配置服务器IP和端口");
Text help_c13(help_c12.text_rect.x,help_c12.text_rect.y+help_padding,black,18,"13.更多的功能正在开发中，详细请见相关通告");

SDL_Rect about_me_rect={325,384,100,28};
RectButton about_me_button={about_me_rect,black,white,18,black," 关于我们"};
int about_me_padding=25;
Text about_me_title(310,55,red,30,"关于我们");
Text about_me_c1(185,105,black,18,"我："); 
Text about_me_c2(about_me_c1.text_rect.x,about_me_c1.text_rect.y+about_me_padding,black,18,"计算机网络工程专业，27年毕业，目前就读大三"); 
Text about_me_c3(about_me_c2.text_rect.x,about_me_c2.text_rect.y+about_me_padding,black,18,"喜欢钻研技术和编程，爱好开发独立游戏和软件"); 
Text about_me_c4(about_me_c3.text_rect.x,about_me_c3.text_rect.y+about_me_padding*2,black,18,"项目："); 
Text about_me_c5(about_me_c4.text_rect.x,about_me_c4.text_rect.y+about_me_padding,black,18,"软件的定位为面向windows平台的即时通信软件"); 
Text about_me_c6(about_me_c5.text_rect.x,about_me_c5.text_rect.y+about_me_padding,black,18,"基于TCP/IP协议，整体采用C/S跨平台通信架构"); 
Text about_me_c7(about_me_c6.text_rect.x,about_me_c6.text_rect.y+about_me_padding,black,18,"Linux服务端，采用Epoll+ET+非阻塞IO的模式"); 
Text about_me_c8(about_me_c7.text_rect.x,about_me_c7.text_rect.y+about_me_padding,black,18,"项目开发周期约2个月，2026年5月至2026年7月"); 
Text about_me_c9(about_me_c8.text_rect.x,about_me_c8.text_rect.y+about_me_padding,black,18,"整个项目均由个人独立完成，且无任何借鉴抄袭"); 
Text about_me_c10(about_me_c9.text_rect.x,about_me_c9.text_rect.y+about_me_padding,black,18,"目前项目已在GitHub开源，以供参考，禁止商用"); 
Text about_me_c11(about_me_c10.text_rect.x,about_me_c10.text_rect.y+about_me_padding,black,18,"地址：https://github.com/WenDaoYolo/ZCat"); 

SDL_Rect version_rect={325,356,100,28};
RectButton version_button={version_rect,black,white,18,black," 版本迭代"};
int version_padding=25;
Text version_title(310,55,red,30,"版本迭代");
Text version_c1(185,105,black,18,"2026-7-04：ZCat正式上线");
Text version_c2(185,125,black,18,"2026-8-28：修复了焦点丢失时，UI的更新问题");
Text version_c3(185,145,black,18,"2026-9-12：可手动配置服务器的IP地址和端口");
Text version_c4(185,165,black,18,"2026-9-22：消息框UI的长度会随内容动态调整");

SDL_Rect change_passwd_rect={325,328,100,28};
RectButton change_passwd_button={change_passwd_rect,black,white,18,black," 修改密码"};
Text change_passwd_title(310,55,red,30,"修改密码");
Text input_new_passwd1_title(250,180,black,20,"新的密码");
Text input_new_passwd2_title(250,230,black,20,"确认密码");
SDL_Rect input_new_passwd1_rect={345,175,150,30};
SDL_Rect input_new_passwd2_rect={345,225,150,30};
SDL_Rect commit_new_passwd_rect={340,350,70,30};
RectButton input_new_passwd1_button(input_new_passwd1_rect,black,white,18,black,"");
RectButton input_new_passwd2_button(input_new_passwd2_rect,black,white,18,black,"");
RectButton commit_new_passwd_button(commit_new_passwd_rect,black,white,20,black," 提交");
Text tt0(input_new_passwd1_title.text_rect.x,input_new_passwd1_title.text_rect.y+100,red,15,"修改成功!");
Text tt31(input_new_passwd1_title.text_rect.x,input_new_passwd1_title.text_rect.y+100,red,15,"密码格式错误!");
Text tt32(input_new_passwd1_title.text_rect.x,input_new_passwd1_title.text_rect.y+125,red,15,"允许的字符：字母 数字 _ @ # & *");
Text tt12(input_new_passwd1_title.text_rect.x,input_new_passwd1_title.text_rect.y+100,red,15,"密码不一致");

SDL_Rect change_style_rect={325,300,100,28};
RectButton change_style_button={change_style_rect,black,white,18,black," 等待完善"};

//friend_apply_list ui and group_apply_list ui
bool cur_group_status_die=false;
bool cur_friend_status_die=false;
bool download_friend_list=false;
bool download_friend_apply_list=false;
bool download_group_list=false;
bool download_group_apply_list=false;
bool download_unicast_list=false;
bool download_multicast_list=false;
bool need_update_friend_list=false;


int friend_apply_total_page=0;
int friend_apply_cur_page=0;
friend_apply_node* cur_friend_apply_page_first=NULL;

Text friend_apply_title(325,200,red,25,"好友申请");
SDL_Rect friend_apply_rect[10];
RectButton friend_apply_button[10];

Text group_apply_title(325,200,red,25,"群聊通知");
SDL_Rect group_apply_rect[10];
RectButton group_apply_button[10];

SDL_Rect permit_apply_rect[10];
SDL_Rect refuse_apply_rect[10];
RectButton permit_button[10];
RectButton refuse_button[10];

int group_apply_total_page=0;
int group_apply_cur_page=0;
group_apply_node* cur_group_apply_page_first=NULL;

SDL_Rect apply_control_last_rect={225,475,50,25};
SDL_Rect apply_control_page_rect={275,475,200,25};
SDL_Rect apply_control_next_rect={475,475,50,25};
RectButton apply_control_last_button(apply_control_last_rect,black,white,15,black,"上一页");
RectButton apply_control_page_button(apply_control_page_rect,black,white,15,black,"0 / 0");
RectButton apply_control_next_button(apply_control_next_rect,black,white,15,black,"下一页");

//notice ui
SDL_Rect notice_rect={175,0,350,25};
RectButton notice_button(notice_rect,black,white,18,black,"");

SDL_Rect close_notice_rect={525,0,50,25};
RectButton close_notice_button(close_notice_rect,black,white,18,black,"关闭");

//按钮组件统一存储(指针存储其地址)
RectButton* buttons[]=
{
    &r_regist,
    &r_login,
    &r_commit,
    &r_admin,
    &r_passwd,
    &r_name,
    &r_repasswd,
    &r1,
    &r2,
    &r3,
    &r4,
    &r5,
    &friend_content_button[0],
    &friend_content_button[1],
    &friend_content_button[2],
    &friend_content_button[3],
    &friend_content_button[4],
    &friend_content_button[5],
    &friend_content_button[6],
    &friend_content_button[7],
    &friend_content_button[8],
    &friend_content_button[9],
    &friend_control_last_button,
    &friend_control_page_button,
    &friend_control_next_button,
    &group_content_button[0],
    &group_content_button[1],
    &group_content_button[2],
    &group_content_button[3],
    &group_content_button[4],
    &group_content_button[5],
    &group_content_button[6],
    &group_content_button[7],
    &group_content_button[8],
    &group_content_button[9],
    &group_control_last_button,
    &group_control_page_button,
    &group_control_next_button,
    &tool_add_friend_button,
    &tool_friend_apply_button,
    &tool_join_group_button,
    &tool_group_apply_button,
    &tool_create_group_button,
    &tool_setting_button,
    &tool_all_button,
    &close_page_button,
    &commit_page_button,
    &clear_page_button,
    &add_friend_input_button,
    &join_group_input_button,
    &create_group_admin_input_button,
    &create_group_name_input_button,
    &msg_setting_base_button,
    &exit_login_button,
    &help_button,
    &about_me_button,
    &version_button,
    &change_passwd_button,
    &input_new_passwd1_button,
    &input_new_passwd2_button,
    &commit_new_passwd_button,
    &change_style_button,
    &friend_apply_button[0],
    &friend_apply_button[1],
    &friend_apply_button[2],
    &friend_apply_button[3],
    &friend_apply_button[4],
    &friend_apply_button[5],
    &friend_apply_button[6],
    &friend_apply_button[7],
    &friend_apply_button[8],
    &friend_apply_button[9],
    &group_apply_button[0],
    &group_apply_button[1],
    &group_apply_button[2],
    &group_apply_button[3],
    &group_apply_button[4],
    &group_apply_button[5],
    &group_apply_button[6],
    &group_apply_button[7],
    &group_apply_button[8],
    &group_apply_button[9],
    &permit_button[0],
    &permit_button[1],
    &permit_button[2],
    &permit_button[3],
    &permit_button[4],
    &permit_button[5],
    &permit_button[6],
    &permit_button[7],
    &permit_button[8],
    &permit_button[9],
    &refuse_button[0],
    &refuse_button[1],
    &refuse_button[2],
    &refuse_button[3],
    &refuse_button[4],
    &refuse_button[5],
    &refuse_button[6],
    &refuse_button[7],
    &refuse_button[8],
    &refuse_button[9],
    &apply_control_last_button,
    &apply_control_page_button,
    &apply_control_next_button,
    &notice_button,
    &close_notice_button,
};

/******************************************------level 1 main data------******************************************/

//test

group_node* cur_view_group=NULL;
friend_node* cur_view_friend=NULL;

SDL_Rect msg_input_rect={175,625,400,150};
RectButton msg_input_button(msg_input_rect,black,white_low,18,black,"");

Text msg_title(175,30,black,18,"");

SDL_Rect msg_base_rect={175,25,400,625};
RectButton msg_base_button(msg_base_rect,black,white,20,black,""); 

SDL_Rect msg_more_rect={175,600,50,25};
RectButton msg_more_button(msg_more_rect,black,white,18,black,"更多");

SDL_Rect msg_send_rect={225,600,50,25};
RectButton msg_send_button(msg_send_rect,black,white,18,black,"发送");

SDL_Rect msg_close_rect={475,600,100,25};//650,75,50,25
RectButton msg_close_button(msg_close_rect,black,white,18,black," 关闭会话");

//msg_more_tool
bool msg_group_more_tool=false;
bool msg_friend_more_tool=false;

SDL_Rect msg_more_exit_group_rect={175,550,100,25};
SDL_Rect msg_more_group_member_rect={175,575,100,25};
SDL_Rect msg_more_delete_friend_rect={175,575,100,25};

RectButton msg_more_exit_group_button(msg_more_exit_group_rect,black,white,18,black,"退出群聊");
RectButton msg_more_group_member_button(msg_more_group_member_rect,black,white,18,black,"查看成员");
RectButton msg_more_delete_friend_button(msg_more_delete_friend_rect,black,white,18,black,"删除好友");

//group member page ui
int group_member_total_page=0;
int group_member_cur_page=0;
group_member_node* cur_group_member_page_first=NULL;

SDL_Rect group_member_base_rect={175,25,400,775};
RectButton group_member_base_button(group_member_base_rect,black,white,20,black,"");

SDL_Rect gm_last_page_rect={175,750,50,25};
SDL_Rect gm_pages_rect={225,750,300,25};
SDL_Rect gm_next_page_rect={525,750,50,25};

RectButton gm_last_page_button(gm_last_page_rect,black,white,15,black,"上一页");
RectButton gm_pages_button(gm_pages_rect,black,white,15,black,"0 / 0");
RectButton gm_next_page_button(gm_next_page_rect,black,white,15,black,"下一页");

SDL_Rect group_member_rect[25];
RectButton group_member_button[25];

SDL_Rect group_member_type_rect[25];
RectButton group_member_type_button[25];

bool event_remove_manager=false;
SDL_Rect remove_member_rect[25];
RectButton remove_member_button[25];

//friend msg and group msg


TTF_Font* g_font=NULL;
/*
    notice:
    use to calculate the char width px
    init in main,must after ttf_init()
    remember to use TTF_CloseFont to destroy
*/

int msg_start=500;
int msg_pos=msg_start;
int msg_total_len=0;

SDL_Rect mine_msg_rect={325,500,240,300};
Message mine_msg(mine_msg_rect,black,sky_bule,15,black,"");

SDL_Rect target_msg_rect={185,500,240,300};
Message target_msg(target_msg_rect,black,sky_bule,15,black,"");

Text target_msg_title(185,0,red,15,"");
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

    memset(message_buffer,0,sizeof(message_buffer));
}

void ResetButton()
{
    int len=sizeof(buttons)/sizeof(RectButton*);
    for(int i=0;i<len;i++)
    {
        buttons[i]->fill_color=white;
    }
}

void clear_last_admin_data()
{
    //clear last remain status
    exit_login_button.fill_color.r=255;
    exit_login_button.fill_color.g=255;
    exit_login_button.fill_color.b=255;

    //clear last admin data1
    memset(&cur_user,0,sizeof(cur_user));
    memset(&cur_admin_text.text,0,sizeof(cur_admin_text.text));
    memset(&(notice_button.text),0,sizeof(notice_button.text));
    cur_error.value=-999;

    //clear last admin data2
    tcpc1.DestroyFriendList();
    tcpc1.DestroyGroupList();
    tcpc1.DestroyFriendApplyList();
    tcpc1.DestroyGroupApplyList();
    tcpc1.DestroyGroupMemberList();
    tcpc1.DestroyUnicastList();
    tcpc1.DestroyMulticastList();

    //clear last admin data3
    friend_total_page=0;
    group_total_page=0;
    friend_cur_page=0;
    group_cur_page=0;
    cur_friend_page_first=NULL;
    cur_group_page_first=NULL;

    friend_apply_total_page=0;
    group_apply_total_page=0;
    group_member_total_page=0;
    friend_apply_cur_page=0;
    group_apply_cur_page=0;
    group_member_cur_page=0;
    cur_friend_apply_page_first=NULL;
    cur_group_apply_page_first=NULL;
    cur_group_member_page_first=NULL;

    //clear last admin data4
    LEVEL_1_MS=LEVEL_1_NONE2;
    cur_view_friend=NULL;
    cur_view_group=NULL;

    //clear last admin data5
    LEVEL_1_TS=LEVEL_1_NONE;

    //change window size
    SDL_SetWindowSize(win,LEVEL_0_WIDTH,LEVEL_0_HEIGHT);
}

void data_init()
{

    //init data
    friend_content_rect[0]={0,25,160,25};
    group_content_rect[0]={0,325,160,25};
    friend_apply_rect[0]={175,225,300,25};
    group_apply_rect[0]={175,225,300,25};

    permit_apply_rect[0]={475,225,50,25};
    refuse_apply_rect[0]={525,225,50,25};

    group_member_rect[0]={225,75,350,25};
    group_member_type_rect[0]={175,75,50,25};
    remove_member_rect[0]={525,75,50,25};

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

    for(int i=0;i<25;i++)
    {
        if(i!=0)
        {
            group_member_rect[i]=group_member_rect[i-1];
            group_member_rect[i].y+=25;

            group_member_type_rect[i]=group_member_type_rect[i-1];
            group_member_type_rect[i].y+=25;

            remove_member_rect[i]=remove_member_rect[i-1];
            remove_member_rect[i].y+=25;
        }

        group_member_button[i].SetValue(group_member_rect[i],black,white,18,black,"");
        group_member_type_button[i].SetValue(group_member_type_rect[i],black,white,18,black,"成员");
        remove_member_button[i].SetValue(remove_member_rect[i],black,white,18,black,"移除");
    }
}

void LoadConfig()
{
    std::fstream f1;
    f1.open("../SERVER_CONFIG.txt",std::ios::in);
    if(f1.is_open())
    {
        f1.getline(ip_str,32,'\n');
        f1.getline(port_str,32,'\n');
        f1.close();
        port=std::stoi(port_str);

        tcpc1.SetIp(ip_str);
        tcpc1.SetPORT(port);
    }
}

void config_init()
{
    LoadConfig();

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
        LEVEL_0_WIDTH,
        LEVEL_0_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    g_font=TTF_OpenFont("../resource/font/simyou.ttf",15);

    rdr=SDL_CreateRenderer(win,-1,0);
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}