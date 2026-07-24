#include<iostream>
#include<windows.h>
#include<ctime>
using namespace std;

enum GameState{
    level_0_s,
    level_1_s,
    level_2_s,
    exit_s
};//场景状态机

GameState g_GameState;
int g_MinCount,t_CurCount;

void PrintMenu(){
    cout<<"+=============================+"<<endl;
    cout<<"            猜数字             "<<endl;
    cout<<"          1.开始游戏           "<<endl;
    cout<<"          2.退出游戏           "<<endl;
    cout<<"+=============================+"<<endl;
}

void PrintResult(){
    cout<<"+=============================+"<<endl;
    cout<<"            计分板              "<<endl;
    cout<<"           当前次数:"<<t_CurCount<<endl;
    cout<<"           历史最佳:"<<g_MinCount<<endl;
    cout<<"     1.重新开始    2.退出游戏    "<<endl;
    cout<<"+=============================+"<<endl;
}

void Check(int rand_num,int input){
    if(input>rand_num)
        cout<<"数字大啦!"<<endl;
    else if(input <rand_num)
        cout<<"数字小啦!"<<endl;
    else
    {
        cout<<"猜对啦!"<<endl;
        g_GameState=level_2_s;
    }
    t_CurCount++;
}

void ClearBuffer(){
    int ch;
    while((ch=getchar())!='\n'&&ch!=EOF);
}

//输入检测与错误处理
bool CheckInputError(){
    if(cin.fail())
    {
        cin.clear();
        ClearBuffer();

        return true;
    }
    return false;
}

void level_0(){
    int choose=0;
    system("cls");
    PrintMenu();

    while(g_GameState==level_0_s)
    {
        cout<<"请输入>";
        cin>>choose;
        CheckInputError();

        if(choose==1)
            g_GameState=GameState::level_1_s;
        else if(choose==2)
            g_GameState=GameState::exit_s;
        else
            cout<<"选择无效!"<<endl;
    }
    ClearBuffer();
}

void level_1(){
    int rand_num=rand()%100;
    int input;
    t_CurCount=0;
    system("cls");

    while(g_GameState==GameState::level_1_s)
    {
        cout<<"请输入数字>";
        cin>>input;
        
        if(CheckInputError())
        {
            cout<<"输入错误!"<<endl;
            continue;
        }
        Check(rand_num,input);
    }
    ClearBuffer();//输入后清空缓冲区，将缓冲区中错误的数据移除
}

void level_2(){
    if(t_CurCount<g_MinCount)
        g_MinCount=t_CurCount;

    int choose=0;
    system("cls");
    PrintResult();

    while(g_GameState==level_2_s)
    {
        cout<<"请输入>";
        cin>>choose;
        CheckInputError();

        if(choose==1)
            g_GameState=GameState::level_1_s;
        else if(choose==2)
            g_GameState=GameState::exit_s;
        else
            cout<<"选择无效!"<<endl;
    }
    ClearBuffer();
};

void Init(){
    srand((unsigned int)time(NULL));
    g_MinCount=999;
    t_CurCount=0;
    g_GameState=level_0_s;
}

void loop(){
    while(g_GameState!=GameState::exit_s)
    {
        switch(g_GameState)//场景的切换
        {
            case GameState::level_0_s:
                level_0();
                break;
            case GameState::level_1_s:
                level_1();
                break;
            case GameState::level_2_s:
                level_2();
                break;
        }
    }
}

void Destroy(){
    cout<<"程序已退出!"<<endl;
}

int main(){
    Init();     //初始化
    loop();     //游戏主循环
    Destroy();  //反初始化

    return 0;
}