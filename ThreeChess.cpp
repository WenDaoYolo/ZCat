#include<iostream>
#include<time.h>
#include<windows.h>
#define WIDTH 7
#define HEIGHT 7
using namespace std;

enum GameState{
    INDEX,
    GAME,
    RESULT,
    EXIT
};

class ChessBox{
    public:
        char surface[WIDTH][HEIGHT];

        void Init(){
            for(int i=0;i<HEIGHT;i++)
                for(int j=0;j<WIDTH;j++)
                    this->surface[i][j]=' ';
        }

        void Display(){
            for(int j=0;j<=WIDTH;j++)
            {
                if(j!=0)
                    cout<<" "<<j<<"  ";
                else
                    cout<<"    ";
            }
                
            for(int i=0;i<HEIGHT;i++)
            {
                cout<<"    ";
                for(int j=0;j<WIDTH;j++)
                {
                    if(i==0)
                        break;
                    if(j!=WIDTH-1)
                        cout<<"---|";
                    else
                        cout<<"---";
                }
                cout<<endl;

                cout<<" "<<i+1<<"  ";
                for(int j=0;j<WIDTH;j++)
                {
                    if(j!=WIDTH-1)
                        cout<<" "<<this->surface[i][j]<<" |";
                    else
                        cout<<" "<<this->surface[i][j]<<" ";
                }
                cout<<endl;
            }
        }

        void Update(char flag,int x,int y){
            this->surface[x][y]=flag;
        }
};

GameState g_GS;
ChessBox g_BX;
char g_result;
int g_round;

/******************************************************************************/
void PrintMenu(){
    cout<<"+=============================+"<<endl;
    cout<<"            五子棋             "<<endl;
    cout<<"          1.开始游戏           "<<endl;
    cout<<"          2.退出游戏           "<<endl;
    cout<<"+=============================+"<<endl;
}

void PrintResult(const char* winner){
    cout<<"+=============================+"<<endl;
    cout<<"             计分板             "<<endl;
    cout<<"           获胜方:"      <<winner<<endl;
    cout<<"           回合数:  "   <<g_round<<endl;
    cout<<"     1.重新开始    2.退出游戏    "<<endl;
    cout<<"+=============================+"<<endl;
}

void ClearBuffer(){
    int ch;
    while((ch=getchar())!='\n'&&ch!=EOF);
}

bool CheckAndClearInputError(){
    if(cin.fail())
    {
        cin.clear();
        ClearBuffer();
        return true;
    }
    return false;
}

bool CheckPonitError(int x,int y){
    if((x>=0&&x<=HEIGHT-1)&&(y>=0&&y<=WIDTH-1)&&g_BX.surface[x][y]==' ')
        return false;
    return true;
}

bool TryWin(int i,int j,int di,int dj,int nums,char flag){
    int ti=i,tj=j;
    ti+=di;
    tj+=dj;

    if(ti<0||ti>=HEIGHT||tj<0||tj>=WIDTH)
        return false;

    int count=1;
    while(g_BX.surface[ti][tj]==flag)
    {
        count++;
        if(count==nums+1)
            return true;

        ti+=di;
        tj+=dj;

        if(ti<0||ti>=HEIGHT||tj<0||tj>=WIDTH)
            return false;
    }
    return false;
}

//检测N连珠
char IsWin(char flag,int nums){
    //四个方向数组
    nums--;
    int dir[8][2]={{0,1},{1,0},{1,1},{1,-1}};
    for(int i=0;i<HEIGHT;i++)
    {
        for(int j=0;j<WIDTH;j++)
        {
            if(g_BX.surface[i][j]==flag)
            {
                for(int k=0;k<4;k++)
                {
                    if(TryWin(i,j,dir[k][0],dir[k][1],nums,flag))
                        return flag;
                }
            }
        }
    }

    for(int i=0;i<HEIGHT;i++)
    {
        for(int j=0;j<WIDTH;j++)
        {
            if(g_BX.surface[i][j]==' ')
                    return 'C';
        }
    }
    return 'S';
}

void PlayerMove(){
    int x,y;

    while(true){
        cout<<"请输入坐标>";
        cin>>x>>y;                   

        if(CheckAndClearInputError())
        {
            cout<<"输入无效!"<<endl;
            continue;
        }

        x--,y--;
        if(CheckPonitError(x,y))
            cout<<"输入无效!"<<endl;
        else
        {
            g_BX.Update('x',x,y);
            break;
        }
    }

    ClearBuffer();//输入后清空缓冲区以防错误数据遗留在缓冲区影响下次输入
                  //缓冲区中一定会有数据(\n)所以不会阻塞程序
}

bool tryDirection(int i, int j, int di, int dj) {
    int ni = i + di;
    int nj = j + dj;
    while(ni >= 0 && ni < HEIGHT && nj >= 0 && nj < WIDTH && g_BX.surface[ni][nj] != 'x') {
        if(g_BX.surface[ni][nj] == ' ') {
            g_BX.Update('o', ni, nj);
            return true;
        }
        else if(g_BX.surface[ni][nj] == 'o') {
            ni += di;
            nj += dj;
        }
        else {
            break;
        }
    }
    return false;
}

bool DefendPlayer(int level){

    for(int i=0;i<HEIGHT;i++)
    {
        for(int j=0;j<WIDTH;j++)
        {
            if(g_BX.surface[i][j]==' ')
            {
                g_BX.Update('x',i,j);
                if(IsWin('x',level)=='x')     
                {
                        g_BX.Update('o',i,j);     //模拟成功则落子该处
                        return true;
                }
                else
                    g_BX.Update(' ',i,j);         //否则撤销还原
            }
        }
    }
    return false;
}

void ComputerMove() {
    //简单防守策略 -> 模拟玩家下棋 -> 检测5连珠  防守>进攻
    if(DefendPlayer(5))
        return;

    // 简单进攻策略,八个方向数组
    int dirs[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {-1,-1}, {-1,1}, {1,-1}}; //用二维数组表示x和y的偏移方向

    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            if(g_BX.surface[i][j] == 'o') {
                for(int d = 0; d < 8; d++) {                                         //依次检查8个方向
                    if(tryDirection(i, j, dirs[d][0], dirs[d][1]))
                        return;
                }
            }
        }
    }
    
    // 随机落子
    while(true) {
        int x = rand() % HEIGHT;
        int y = rand() % WIDTH;
        if(g_BX.surface[x][y] == ' ') {
            g_BX.Update('o', x, y);
            return;
        }
    }
}

void level_0(){
    system("cls");
    PrintMenu();
    int choose;
    
    while(g_GS==INDEX){
        cout<<"请选择>";            
        cin>>choose;

        if(CheckAndClearInputError())
        {
            cout<<"输入无效!"<<endl;
            continue;
        }

        if(choose==1)
            g_GS=GAME;
        else if(choose==2)
            g_GS=EXIT;
        else
            cout<<"输入无效!"<<endl;
    }
    ClearBuffer();
}

void level_1(){
    int x,y;
    g_BX.Init();
    g_round=1;
    
    system("cls");
    g_BX.Display();

    while(g_GS==GAME)
    {
        //电脑移动
        ComputerMove();
        cout<<"电脑移动中..."<<endl;
        Sleep(2500);

        //检测
        g_result=IsWin('o',5);
        if(g_result=='o'||g_result=='S')
        {
            g_GS=RESULT;
            continue;
        }

        //更新画面
        system("cls");
        g_BX.Display();

        //玩家移动
        PlayerMove();

        //检测
        g_result=IsWin('x',5);
        if(g_result=='x'||g_result=='S')
        {
            g_GS=RESULT;
            continue;
        }

        //更新画面
        system("cls");
        g_BX.Display();

        //记录回合数
        g_round++;
    }
}

void level_2(){
    system("cls");

    g_BX.Display();
    if(g_result=='x')
        PrintResult("玩家");
    else if(g_result=='o')
        PrintResult("电脑");
    else
        PrintResult("平局");

    int choose;
    while(g_GS==RESULT)
    {
        cout<<"请选择>";
        cin>>choose;

        if(CheckAndClearInputError())
        {
            cout<<"输入无效!"<<endl;
            continue;
        }

        if(choose==1)
            g_GS=GAME;
        else if(choose==2)
            g_GS=EXIT;
        else
            cout<<"输入无效!"<<endl;
    }
    ClearBuffer();
}

void Init(){
    srand((unsigned int)time(NULL));
    g_BX.Init();
    g_GS=INDEX;
    g_result='C';
    g_round=1;
}

void loop(){
    while(g_GS!=EXIT)
    {
        switch(g_GS)
        {
        case INDEX:
            level_0(); 
            break;
        case GAME:         
            level_1();
            break;
        case RESULT:
            level_2();
            break;
        }
    }
}

void Destroy(){
    cout<<"程序已退出!"<<endl;
}

int main(){
    Init();
    loop();
    Destroy();

    return 0;
}