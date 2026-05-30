#include<iostream>
#include<easyx.h>
#include<windows.h>
#include<mmsystem.h>
#include<thread>
#pragma coment(lib,"winmm.lib");
#define W_WIDTH 1200
#define W_HEIGHT 700
using namespace std;

/*
    本项目的问题：
    在最初就没有设计好，类方法应该只用于实现最基础的单一的功能
    类与类之间有联系的功能应该定义单独的关联变量,然后用全局函数来实现
    否则各个某块之间依赖性太强,首先就是不利于维护和修改，其次不够通用
*/

class shoot{
    public:
        int x;
        int y;
        IMAGE surface_cur;
        IMAGE surfaces[5];
        int frame_count;
        int timer;
        bool IsOver;
    void SetXY(int x,int y){
        this->x=x;
        this->y=y;
    }

    shoot(){
        this->x=760;
        this->y=160;
        this->frame_count=0;
        this->timer=0;
        this->IsOver=true;
    }

    void Display(){
        putimage(this->x,this->y,&this->surfaces[this->frame_count]);
        if(this->timer++>=150&&(!this->IsOver)){
            this->timer=0;
            this->frame_count++;
            if(this->frame_count>5)
            {
                this->frame_count=0;
                this->IsOver=true;
            }
        }
    }
};

ExMessage e1;
shoot st,st2;                 //向前声明，采用头文件分离的方式就无需考虑声明顺序

class UI_Start{
    public:
        int x;
        int y;
        int w;
        int h;
        bool IsDie;
        IMAGE surface;
        IMAGE surfaces[10];

    UI_Start(int x,int y,int w,int h){
        this->IsDie=false;
        this->x=x;
        this->y=y;
        this->w=w;
        this->h=h;
    }

    bool CheckCollsion(int x,int y){
        if((x>=this->x&&y>=this->y)&&(x<=this->x+w&&y<=this->y+h))
            return true;
        else
            return false;
    }
};

class Bubble{
    public:
        int x;
        int y;
        IMAGE surface;
        void changexy(int x,int y){
            this->x=x;
            this->y=y;
        }
};

class Loadbank{
    public:
        int b_x;
        int b_y;
        int b_w;
        int b_h;
        int v_x;
        int v_y;
        int v_w;
        int v_h;
        int timer;

        bool onceflag;

    IMAGE bk_surface;
    IMAGE v_surface;
    Loadbank(){
        this->b_x=460;
        this->b_y=660;
        this->b_w=280;
        this->b_h=25;
        
        this->onceflag=true;
        this->timer=0;

        this->v_x=this->b_x+2;
        this->v_y=this->b_y+3;
        this->v_w=1;
        this->v_h=20;
    }

    void Display(){

        if(this->onceflag==true)          //一次性标识符，让其在循环中只执行一次
        {                                 //进度条背景在定时器之前先输出
            putimage(this->b_x,this->b_y,&this->bk_surface);
            this->onceflag=false;
        }

        if(this->timer==67)               //局部定时器
        {
            if(this->v_x>=this->b_x+this->b_w-1)
            {                             //进度条与烟花的定时关系
            st.IsOver=false;              //这样设计导致类方法与其他方法强制绑定
            st2.IsOver=false;  
            putimage(this->b_x,this->b_y,&this->bk_surface);
            this->v_x=this->b_x+2;
            }
            putimage(this->v_x,this->v_y,&this->v_surface);
            this->v_x+=1;
            this->timer=0;
        }
        this->timer++;
    }
};


IMAGE bk,clear_text;
UI_Start ui1(W_WIDTH/2-150,W_HEIGHT/2+35,300,150);
UI_Start ui2(ui1.x,ui1.y+155,300,150);
Bubble bb[4];
IMAGE Bubble_clear;
Loadbank lb;


void loadsource(){
    loadimage(&bk,_T("./img/bk.png"),W_WIDTH,W_HEIGHT,true);

    loadimage(&ui1.surfaces[0],_T("./img/startgame_0.png"),300,150,true);
    loadimage(&ui1.surfaces[1],_T("./img/startgame_1.png"),300,150,true);
    ui1.surface=ui1.surfaces[0];

    loadimage(&ui2.surfaces[0],_T("./img/exitgame_0.png"),300,150,true);
    loadimage(&ui2.surfaces[1],_T("./img/exitgame_1.png"),300,150,true);
    ui2.surface=ui2.surfaces[0];

    loadimage(&bb[0].surface,_T("./img/bubble_0.png"),70,150,true);
    loadimage(&bb[1].surface,_T("./img/bubble_1.png"),70,150,true);
    loadimage(&bb[2].surface,_T("./img/bubble_2.png"),70,150,true);
    loadimage(&bb[3].surface,_T("./img/bubble_3.png"),70,150,true);
    loadimage(&Bubble_clear,_T("./img/bubble_clear.png"),70,150,true);

    loadimage(&lb.bk_surface,_T("./img/load_bk.png"),lb.b_w,lb.b_h,true);
    loadimage(&lb.v_surface,_T("./img/load_value.png"),lb.v_w,lb.v_h,true);

    //当展示大小大于实际大小可能会出现黑边的情况
    loadimage(&clear_text,_T("./img/clear_text.png"),560,200,true); 
      
    loadimage(&st.surfaces[0],_T("./img/shoot_0.png"),70,170,true);
    loadimage(&st.surfaces[1],_T("./img/shoot_1.png"),70,170,true);
    loadimage(&st.surfaces[2],_T("./img/shoot_2.png"),70,170,true);
    loadimage(&st.surfaces[3],_T("./img/shoot_3.png"),70,170,true);
    loadimage(&st.surfaces[4],_T("./img/shoot_4.png"),70,170,true);

    loadimage(&st2.surfaces[0],_T("./img/shoot_0.png"),70,170,true);
    loadimage(&st2.surfaces[1],_T("./img/shoot_1.png"),70,170,true);
    loadimage(&st2.surfaces[2],_T("./img/shoot_2.png"),70,170,true);
    loadimage(&st2.surfaces[3],_T("./img/shoot_3.png"),70,170,true);
    loadimage(&st2.surfaces[4],_T("./img/shoot_4.png"),70,170,true);
}

void CheckUI(UI_Start& ui){
    peekmessage(&e1,EX_MOUSE,true);
    if(ui.CheckCollsion(e1.x,e1.y))
        ui.surface=ui.surfaces[1];
    else
        ui.surface=ui.surfaces[0];
}

void DisplayUI(UI_Start& ui){
    putimage(ui.x,ui.y,&ui.surface);
}

bool program_state=true;
bool index_0_state=false;

void CheckExit(){
        peekmessage(&e1,EX_MOUSE,true);
        if(e1.lbutton==true&&ui2.CheckCollsion(e1.x,e1.y))
            program_state=false;
}

void CheckStart(){
    peekmessage(&e1,EX_MOUSE,true);
        if(e1.lbutton==true&&ui1.CheckCollsion(e1.x,e1.y))
            index_0_state=true;
}

int bubble_timer=0;

void DisplayBK(){
    putimage(0,0,&bk);
}

void bubble_clear(){
    putimage(bb[0].x,bb[0].y,&Bubble_clear);
    putimage(bb[1].x,bb[1].y,&Bubble_clear);
    putimage(bb[2].x,bb[2].y,&Bubble_clear);
    putimage(bb[3].x,bb[3].y,&Bubble_clear);
}

void DisplayBubble(){
    bubble_clear();
    
    bubble_timer++;

    if(bubble_timer==10)
    {
    bubble_timer=0;
    bb[0].y-=1;
    bb[1].y-=1;
    bb[2].y-=1;
    bb[3].y-=1;

    if(bb[0].y<=-100)
         bb[0].y=W_HEIGHT+100;
    if(bb[1].y<=-(3*100))
         bb[1].y=W_HEIGHT+100;
    if(bb[2].y<=-(2*100))
        bb[2].y=W_HEIGHT+100;
    if(bb[3].y<=-(4*100))
        bb[3].y=W_HEIGHT+100;
    }
    
    putimage(bb[0].x,bb[0].y,&bb[0].surface);
    putimage(bb[1].x,bb[1].y,&bb[1].surface);
    putimage(bb[2].x,bb[2].y,&bb[2].surface);
    putimage(bb[3].x,bb[3].y,&bb[3].surface);
}

void InitBullle(){
    bb[0].changexy(30,W_HEIGHT+100);
    bb[1].changexy(130,W_HEIGHT+500);
    bb[2].changexy(W_WIDTH-180,W_HEIGHT+800);
    bb[3].changexy(W_WIDTH-90,W_HEIGHT+400);
}

void bkmusic(){
    mciSendString(_T("set bgm buffer time 3000"),NULL,0,NULL);
    mciSendString(_T("play ./media/BK_MUSIC.mp3 repeat"),NULL,0,NULL);
    while(true){
        Sleep(100);
    }
}

void Index_0(){            //menu

    DisplayBK();
    
    while(program_state&&index_0_state==false){

        CheckExit();
        CheckStart();
        //cleardevice();
        CheckUI(ui1);
        CheckUI(ui2);
        //DisplayBubble();

        st.Display();
        st2.Display();
        DisplayUI(ui2);
        DisplayUI(ui1);
        
        FlushBatchDraw();
    }  
}

void Index_1(){            

    int x=W_WIDTH/3-80,y=W_HEIGHT/2+40;
    int count_0=0,count=0,count2=0;
    const TCHAR* s[54];

    s[0]=_T("3...");
    s[1]=_T("2...");
    s[2]=_T("1...");
    s[3]=_T("0...");
    s[4]=_T("关于我家的小娥宝宝:");
    s[5]=_T("摩羯座,很焦虑,喜欢乱想,没有安全感");
    s[6]=_T("胆子超级小,连路边的小狗都害怕 (有点小憨 ^_^)");
    s[7]=_T("是个小馋猫,特别喜欢吃辣 (太可怕啦 O_o)");
    s[8]=_T("最大的兴趣爱好就是追剧！！！");
    
    s[9]=_T("很独立很自强,人美心善,能力一流！！！");
    s[10]=_T("宝宝对我很好,我很爱我的宝宝");
    s[11]=_T("你很专一，总是能给足我安全感");
    s[12]=_T("每天都有你陪着，我觉得很开心");
    s[13]=_T("虽然我们发生了一些争吵,但是还是坚持了下来...");
    s[14]=_T("我希望这些不愉快不会成为我们心中的一道隔阂");
    s[15]=_T("相反会让我们之间的感情更加坚定");
    s[16]=_T("和你在一起的这段时间,我真的很开心");
    s[17]=_T("我想一直陪着你,也希望你能一直陪着我...");

    s[18]=_T("我的天空里没有太阳");
    s[19]=_T("总是黑夜，但并不暗");
    s[20]=_T("因为有东西代替了太阳");
    s[21]=_T("虽然没有太阳那么明亮");
    s[22]=_T("但对我来说已经足够...");
    s[23]=_T("凭借这份光,我便能把黑夜当成白天");
    s[24]=_T("困难如同连绵的雨季");
    s[25]=_T("无论风雨多大");
    s[26]=_T("只要你在身边，我就不会感到孤单");

    s[27]=_T("两个人能相遇,本来就已经很不容易了");
    s[28]=_T("我会好好珍惜这份缘分的");
    s[29]=_T("力的作用是相互，感情也应该如此");
    s[30]=_T("两个人彼此都互相重视、互相在意");
    s[31]=_T("这段感情才能走得更远...");
    s[32]=_T("不需要天天黏在一起,也不需要轰轰烈烈");
    s[33]=_T("平平淡淡、细水长流就足够了...");
    s[34]=_T("不期而遇的惊喜,也是命中注定的缘分...");
    s[35]=_T("我们都要珍惜这来之不易的缘分...");

    s[36]=_T("深情不及久伴,厚爱无需多言");
    s[37]=_T("未来的路虽然还很漫长...");
    s[38]=_T("但我想和你一直走下去");
    s[39]=_T("今年是你...");
    s[40]=_T("明年是你...");
    s[41]=_T("希望今后的生活也是你...");
    s[42]=_T("相遇时间：2025 年 10 月 2 日...");
    s[43]=_T("直到今天已过去：71 天 12 小时 45 分 54 秒...");
    s[44]=_T("距离跨年还有：19 天 11 小时 14 分 6 秒...");

    s[45]=_T("欢愉皆胜意,万事皆可期");
    s[46]=_T("愿你:");
    s[47]=_T("所行皆坦途");
    s[48]=_T("所遇皆温暖");
    s[49]=_T("所念皆如愿");
    s[50]=_T("生活善待你");
    s[51]=_T("世间的温柔都奔向你");
    s[52]=_T("多喜乐，长安宁");
    s[53]=_T("致：我最爱的小娥宝宝");
    
    std::thread t1(bkmusic);
    t1.detach();
    DisplayBK();
    while(program_state&&index_0_state)
    {
        
        if(GetAsyncKeyState(27))
            break;

        DisplayBubble();
        if(count_0==2700)
        {
        count_0=0;
        if(count==9)
        {
            count=0;                         
            putimage(x,y,&clear_text);  //不要使用全局性质的清屏，会导致局部对象的更新错误
        }

        outtextxy(x,y,s[count2]);
        if(count2++,count2==54)
            count2=0;

        if(count++,y+=22,y>=W_HEIGHT/2+230)
            y=W_HEIGHT/2+40;
        }
        count_0++;
        lb.Display();
        st.Display();
        st2.Display();
        FlushBatchDraw();
    }
}

int main(){
    loadsource();
    initgraph(W_WIDTH,W_HEIGHT,1);
    settextstyle(20,12,_T("楷体"));
    st2.SetXY(370,170);
    mciSendString(_T("open ./media/BK_MUSIC.mp3 alias bgm"),NULL,0,NULL);

    BeginBatchDraw();
    InitBullle();
    Index_0();
    InitBullle();
    Index_1();
    mciSendString(_T("stop ./media/BK_MUSIC.mp3"),NULL,0,NULL);
    EndBatchDraw();

    closegraph();
    return 0;
}

