#include<SDL.h>
#include<iostream>
#include<windows.h>
#include<SDL_ttf.h>
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 900

#define TITLE_X (WINDOW_WIDTH/2-60)
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

#define REGIST_BUTTON_X (NAME_X+10)
#define REGIST_BUTTON_Y (NAME_Y+120)

#define LOGIN_BUTTON_X (REGIST_BUTTON_X+100)
#define LOGIN_BUTTON_Y (REGIST_BUTTON_Y)

#define COMMIT_BUTTON_X (LOGIN_BUTTON_X+100)
#define COMMIT_BUTTON_Y (LOGIN_BUTTON_Y)
///////////////////////////////////////////////////---Position---////////////////////////////////////////////////////////////////////
enum STATUS
{
    LEVEL_EXIT,
    LEVEL_0_REGIST,
    LEVEL_0_LOGIN,
    LEVEL_1,
    LEVEL_2
};

struct rect_color
{
    Uint8 r;
    Uint8 g;
    Uint8 b;
};

SDL_Event event;
SDL_Window* win;
SDL_Renderer* rdr;
SDL_Point mouse_pt;
SDL_Surface* win_surf;
STATUS CUR_STATUS=LEVEL_0_LOGIN;

SDL_Rect left_up={0,0,125,275};
SDL_Rect left_down={0,0,125,25};
SDL_Rect bottom={125,800,575,100};

SDL_Rect regist_rect={REGIST_BUTTON_X,REGIST_BUTTON_Y,60,30};
SDL_Rect login_rect={LOGIN_BUTTON_X,LOGIN_BUTTON_Y,60,30};
SDL_Rect commit_rect={COMMIT_BUTTON_X,COMMIT_BUTTON_Y,60,30};

SDL_Rect admin_input_rect={ADMIN_INPUT_X,ADMIN_INPUT_Y,220,30};
SDL_Rect passwd_input_rect={PASSWD_INPUT_X,PASSWD_INPUT_Y,220,30};
SDL_Rect name_input_rect={NAME_INPUT_X,NAME_INPUT_Y,220,30};
SDL_Rect repasswd_input_rect={REPASSWD_INPUT_X,REPASSWD_INPUT_Y,220,30};

rect_color rect_default_color={0,0,0};
rect_color admin_input_fg_color={0,0,0};
rect_color passwd_input_fg_color={0,0,0};
rect_color name_input_fg_color={0,0,0};
rect_color repasswd_input_fg_color={0,0,0};

rect_color register_bg_color={255,255,255};
rect_color login_bg_color={255,255,255};
rect_color commit_bg_color={255,255,255};

char* cur_buffer;
char admin_buffer[256];
char passwd_buffer[256];
char name_buffer[256];
char repasswd_buffer[256];

bool admin_editing=false;
bool passwd_editing=false;
bool name_editing=false;
bool repasswd_editing=false;

void DrawRect(SDL_Rect& rect,rect_color color)
{
    SDL_SetRenderDrawColor(rdr,color.r,color.g,color.b,255);
    SDL_RenderDrawRect(rdr,&rect);
}

void DrawWindowBackground(Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    SDL_SetRenderDrawColor(rdr,r,g,b,a);
    SDL_RenderClear(rdr);
}

void draw_level_1()
{
    DrawWindowBackground(245,245,220,255);

    for(int i=0;i<3;i++)
    {
        left_up.y=i*(WINDOW_HEIGHT/3);
        DrawRect(left_up,rect_default_color);
        left_down.y=left_up.y+WINDOW_HEIGHT/3-WINDOW_HEIGHT/36;
        DrawRect(left_down,rect_default_color);
    }
    DrawRect(bottom,rect_default_color);

    SDL_RenderPresent(rdr);
    left_up.y=0;
    left_down.y=0;
}

void MyDrawText(int size,const char* txt,int x,int y,Uint8 r,Uint8 g,Uint8 b)
{
    TTF_Font* index_font=TTF_OpenFont("../resource/font/simkai.ttf",size);
    if(index_font==NULL)
    {
        SDL_Log("TTF_OpenFont:%s",TTF_GetError());
        return;
    }

    SDL_Surface* index_surf=TTF_RenderUTF8_Blended(index_font,txt,{r,g,b});
    SDL_Texture* index_tt=SDL_CreateTextureFromSurface(rdr,index_surf);
    SDL_Rect rect={x,y,index_surf->w,index_surf->h};
    SDL_RenderCopy(rdr,index_tt,NULL,&rect);

    TTF_CloseFont(index_font);
    SDL_FreeSurface(index_surf);
    SDL_DestroyTexture(index_tt);
}

void draw_level_0_regist()
{
    DrawWindowBackground(245,245,235,255);

    MyDrawText(60,"ZCat",TITLE_X,TITLE_Y,0,0,0);
    MyDrawText(20,"账号 ",ADMIN_X,ADMIN_Y,0,0,0);
    MyDrawText(20,"密码 ",PASSWD_X,PASSWD_Y,0,0,0);
    MyDrawText(20,"昵称 ",NAME_X,NAME_Y,0,0,0);
    MyDrawText(20,"确认密码",REPASSWD_X,REPASSWD_Y,0,0,0);

    SDL_SetRenderDrawColor(rdr,admin_input_fg_color.r,admin_input_fg_color.g,admin_input_fg_color.b,255);
    DrawRect(admin_input_rect,admin_input_fg_color);
    if(strlen(admin_buffer)>0)
    {
        MyDrawText(14,admin_buffer,ADMIN_INPUT_X+3,ADMIN_INPUT_Y+8,0,0,0);
    }

    SDL_SetRenderDrawColor(rdr,passwd_input_fg_color.r,passwd_input_fg_color.g,passwd_input_fg_color.b,255);
    DrawRect(passwd_input_rect,passwd_input_fg_color);
    if(strlen(passwd_buffer)>0)
    {
        char tmp_buffer[256]={0};
        for(int i=0;i<strlen(passwd_buffer);i++)
            tmp_buffer[i]='*';
        MyDrawText(14,tmp_buffer,PASSWD_INPUT_X+3,PASSWD_INPUT_Y+8,0,0,0);
    }

    SDL_SetRenderDrawColor(rdr,name_input_fg_color.r,name_input_fg_color.g,name_input_fg_color.b,255);
    DrawRect(name_input_rect,name_input_fg_color);
    if(strlen(name_buffer)>0)
    {
        MyDrawText(14,name_buffer,NAME_INPUT_X+3,NAME_INPUT_Y+8,0,0,0);
    }

    SDL_SetRenderDrawColor(rdr,repasswd_input_fg_color.r,repasswd_input_fg_color.g,repasswd_input_fg_color.b,255);
    DrawRect(repasswd_input_rect,repasswd_input_fg_color);
    if(strlen(repasswd_buffer)>0)
    {
        char tmp_buffer[256]={0};
        for(int i=0;i<strlen(repasswd_buffer);i++)
            tmp_buffer[i]='*';
        MyDrawText(14,tmp_buffer,REPASSWD_INPUT_X+3,REPASSWD_INPUT_Y+8,0,0,0);
    }

    SDL_SetRenderDrawColor(rdr,register_bg_color.r,register_bg_color.g,register_bg_color.b,255);
    SDL_RenderFillRect(rdr,&regist_rect);
    DrawRect(regist_rect,rect_default_color);
    MyDrawText(20,"注册",REGIST_BUTTON_X+10,REGIST_BUTTON_Y+5,0,0,0);

    SDL_SetRenderDrawColor(rdr,login_bg_color.r,login_bg_color.g,login_bg_color.b,255);
    SDL_RenderFillRect(rdr,&login_rect);
    DrawRect(login_rect,rect_default_color);
    MyDrawText(20,"登录",LOGIN_BUTTON_X+10,LOGIN_BUTTON_Y+5,0,0,0);

    SDL_SetRenderDrawColor(rdr,commit_bg_color.r,commit_bg_color.g,commit_bg_color.b,255);
    SDL_RenderFillRect(rdr,&commit_rect);
    DrawRect(commit_rect,rect_default_color);
    MyDrawText(20,"提交",COMMIT_BUTTON_X+10,COMMIT_BUTTON_Y+5,0,0,0);

    SDL_RenderPresent(rdr);
}

void draw_level_0_login()
{
    DrawWindowBackground(245,245,235,255);

    MyDrawText(60,"ZCat",TITLE_X,TITLE_Y,0,0,0);
    MyDrawText(20,"账号 ",ADMIN_X,ADMIN_Y,0,0,0);
    MyDrawText(20,"密码 ",PASSWD_X,PASSWD_Y,0,0,0);

    SDL_SetRenderDrawColor(rdr,admin_input_fg_color.r,admin_input_fg_color.g,admin_input_fg_color.b,255);
    DrawRect(admin_input_rect,admin_input_fg_color);
    if(strlen(admin_buffer)>0)
    {
        MyDrawText(14,admin_buffer,ADMIN_INPUT_X+3,ADMIN_INPUT_Y+8,0,0,0);
    }

    SDL_SetRenderDrawColor(rdr,passwd_input_fg_color.r,passwd_input_fg_color.g,passwd_input_fg_color.b,255);
    DrawRect(passwd_input_rect,passwd_input_fg_color);
    if(strlen(passwd_buffer)>0)
    {
        char tmp_buffer[256]={0};
        for(int i=0;i<strlen(passwd_buffer);i++)
            tmp_buffer[i]='*';
        MyDrawText(14,tmp_buffer,PASSWD_INPUT_X+3,PASSWD_INPUT_Y+8,0,0,0);
    }

    SDL_SetRenderDrawColor(rdr,register_bg_color.r,register_bg_color.g,register_bg_color.b,255);
    SDL_RenderFillRect(rdr,&regist_rect);
    DrawRect(regist_rect,rect_default_color);
    MyDrawText(20,"注册",REGIST_BUTTON_X+10,REGIST_BUTTON_Y+5,0,0,0);

    SDL_SetRenderDrawColor(rdr,login_bg_color.r,login_bg_color.g,login_bg_color.b,255);
    SDL_RenderFillRect(rdr,&login_rect);
    DrawRect(login_rect,rect_default_color);
    MyDrawText(20,"登录",LOGIN_BUTTON_X+10,LOGIN_BUTTON_Y+5,0,0,0);

    SDL_SetRenderDrawColor(rdr,commit_bg_color.r,commit_bg_color.g,commit_bg_color.b,255);
    SDL_RenderFillRect(rdr,&commit_rect);
    DrawRect(commit_rect,rect_default_color);
    MyDrawText(20,"提交",COMMIT_BUTTON_X+10,COMMIT_BUTTON_Y+5,0,0,0);

    SDL_RenderPresent(rdr);
}

void ClearAllBuffer()
{
    memset(admin_buffer,0,sizeof(admin_buffer));
    memset(passwd_buffer,0,sizeof(passwd_buffer));
    memset(name_buffer,0,sizeof(name_buffer));
    memset(repasswd_buffer,0,sizeof(repasswd_buffer));
}

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
                if(SDL_PointInRect(&mouse_pt,&regist_rect))
                {
                    register_bg_color.r=0;
                    register_bg_color.g=220;
                    register_bg_color.b=0;
                }
                else
                {
                    register_bg_color.r=255;
                    register_bg_color.g=255;
                    register_bg_color.b=255;
                }

                if(SDL_PointInRect(&mouse_pt,&login_rect))
                {
                    login_bg_color.r=0;
                    login_bg_color.g=220;
                    login_bg_color.b=0;
                }
                else
                {
                    login_bg_color.r=255;
                    login_bg_color.g=255;
                    login_bg_color.b=255;
                }

                if(SDL_PointInRect(&mouse_pt,&commit_rect))
                {
                    commit_bg_color.r=0;
                    commit_bg_color.g=220;
                    commit_bg_color.b=0;
                }
                else
                {
                    commit_bg_color.r=255;
                    commit_bg_color.g=255;
                    commit_bg_color.b=255;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_pt.x=event.motion.x,mouse_pt.y=event.motion.y;
                if(SDL_PointInRect(&mouse_pt,&regist_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    ClearAllBuffer();
                    CUR_STATUS=LEVEL_0_REGIST;
                    std::cout<<"已切换为注册模块"<<std::endl;
                }

                if(SDL_PointInRect(&mouse_pt,&login_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    ClearAllBuffer();
                    CUR_STATUS=LEVEL_0_LOGIN;
                    std::cout<<"已切换为登录模块"<<std::endl;
                }

                if(SDL_PointInRect(&mouse_pt,&commit_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    std::cout<<"已提交："<<std::endl;
                    std::cout<<"账号 "<<admin_buffer<<std::endl;
                    std::cout<<"密码 "<<passwd_buffer<<std::endl;
                    std::cout<<"昵称 "<<name_buffer<<std::endl;
                    std::cout<<"确认密码 "<<repasswd_buffer<<std::endl;
                    ClearAllBuffer();
                }

                if(SDL_PointInRect(&mouse_pt,&admin_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    admin_editing=true;
                    admin_input_fg_color.g=220;
                }
                else
                {
                    admin_editing=false;
                    admin_input_fg_color.g=0;
                }

                if(SDL_PointInRect(&mouse_pt,&passwd_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    passwd_editing=true;
                    passwd_input_fg_color.g=220;
                }
                else
                {
                    passwd_editing=false;
                    passwd_input_fg_color.g=0;
                }
                    

                if(SDL_PointInRect(&mouse_pt,&name_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    name_editing=true;
                    name_input_fg_color.g=220;
                }
                else
                {
                    name_editing=false;
                    name_input_fg_color.g=0;
                }

                if(SDL_PointInRect(&mouse_pt,&repasswd_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    repasswd_editing=true;
                    repasswd_input_fg_color.g=220;
                }
                else
                {
                    repasswd_editing=false;
                    repasswd_input_fg_color.g=0;
                }

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
                        // 中文 UTF-8 首字节范围
                        if((ch & 0xE0) == 0xE0 && ch != 0xC0 && ch != 0xC1)
                        {
                            cur_buffer[len-3] = '\0';  // 删 3 字节
                            return;
                        }
                    }
                    cur_buffer[len-1] = '\0';  // 删 1 字节
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
                if(SDL_PointInRect(&mouse_pt,&regist_rect))
                {
                    register_bg_color.r=0;
                    register_bg_color.g=220;
                    register_bg_color.b=0;
                }
                else
                {
                    register_bg_color.r=255;
                    register_bg_color.g=255;
                    register_bg_color.b=255;
                }

                if(SDL_PointInRect(&mouse_pt,&login_rect))
                {
                    login_bg_color.r=0;
                    login_bg_color.g=220;
                    login_bg_color.b=0;
                }
                else
                {
                    login_bg_color.r=255;
                    login_bg_color.g=255;
                    login_bg_color.b=255;
                }

                if(SDL_PointInRect(&mouse_pt,&commit_rect))
                {
                    commit_bg_color.r=0;
                    commit_bg_color.g=220;
                    commit_bg_color.b=0;
                }
                else
                {
                    commit_bg_color.r=255;
                    commit_bg_color.g=255;
                    commit_bg_color.b=255;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse_pt.x=event.motion.x,mouse_pt.y=event.motion.y;
                if(SDL_PointInRect(&mouse_pt,&regist_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    ClearAllBuffer();
                    CUR_STATUS=LEVEL_0_REGIST;
                    std::cout<<"已切换为注册模块"<<std::endl;
                }

                if(SDL_PointInRect(&mouse_pt,&login_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    ClearAllBuffer();
                    CUR_STATUS=LEVEL_0_LOGIN;
                    std::cout<<"已切换为登录模块"<<std::endl;
                }

                if(SDL_PointInRect(&mouse_pt,&commit_rect)&&(event.button.button==SDL_BUTTON_LEFT))
                {
                    std::cout<<"已提交："<<std::endl;
                    std::cout<<"账号 "<<admin_buffer<<std::endl;
                    std::cout<<"密码 "<<passwd_buffer<<std::endl;
                    ClearAllBuffer();
                }

                if(SDL_PointInRect(&mouse_pt,&admin_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    admin_editing=true;
                    admin_input_fg_color.g=220;
                }
                else
                {
                    admin_editing=false;
                    admin_input_fg_color.g=0;
                }

                if(SDL_PointInRect(&mouse_pt,&passwd_input_rect)&&event.button.button==SDL_BUTTON_LEFT)
                {
                    passwd_editing=true;
                    passwd_input_fg_color.g=220;
                }
                else
                {
                    passwd_editing=false;
                    passwd_input_fg_color.g=0;
                }

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
                        // 中文 UTF-8 首字节范围
                        if((ch & 0xE0) == 0xE0 && ch != 0xC0 && ch != 0xC1)
                        {
                            cur_buffer[len-3] = '\0';  // 删 3 字节
                            return;
                        }
                    }
                    cur_buffer[len-1] = '\0';  // 删 1 字节
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

/********************************----------main----------********************************/
void init()
{
    SDL_SetHint(SDL_HINT_IME_SHOW_UI,"1");
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    win=SDL_CreateWindow
    (
        "Zcat",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if(win==NULL)
    {
        SDL_Log("SDL_CreateWindow:%s",SDL_GetError());
        CUR_STATUS=LEVEL_EXIT;
        return;
    }

    rdr=SDL_CreateRenderer(win,-1,0);

    if(rdr==NULL)
    {
        SDL_Log("SDL_CreateRenderer:%s",SDL_GetError());
        CUR_STATUS=LEVEL_EXIT;
        return;
    }

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void loop()
{
    
    while(CUR_STATUS)
    {
        switch(CUR_STATUS)
        {
            case LEVEL_0_LOGIN:
                draw_level_0_login();
                event_loop_login();
                break;
            case LEVEL_0_REGIST:
                draw_level_0_regist();
                event_loop_regist();
                break;
        }
        //draw_level_1();
        SDL_Delay(10);
    }
}

void destory()
{
    SDL_Quit();
}

int main(int argc,char* argv[])
{
    init();
    loop();
    destory();
    
    return 0;
}