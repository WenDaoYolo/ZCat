#include "../include/UserBase.h"

UserBase::UserBase()
{
    this->state=Wait;
    strcpy(this->data.admin,"NULL");
    strcpy(this->data.password,"NULL");
}

void UserBase::PrintMenu()
{
    std::cout<<"==================================================="<<std::endl;
    std::cout<<"                   数据库登录                       "<<std::endl;
    std::cout<<"                     1.登录                        "<<std::endl;
    std::cout<<"                     2.注册                        "<<std::endl;
    std::cout<<"                     3.退出                        "<<std::endl;
    std::cout<<"==================================================="<<std::endl;
}

void UserBase::FlushScreen()
{
    system("cls");
    this->PrintMenu();
}

bool UserBase::CheckInputError()
{
    if(std::cin.fail())
        return true;
    return false;
}

void UserBase::ClearBuffer()
{
    std::cin.clear();
    std::cin.ignore(64,'\n');
}

UserBase::~UserBase()
{   
    ;
}

bool UserBase::Input(char* admin,char* password)
{
        char temp_admin[16];
        char temp_password[16];
        std::cout<<"[账号(11) 密码(11)]>";
        std::cin>>temp_admin>>temp_password;

        if(strlen(temp_admin)!=ADMIN_LENGTH-1||strlen(temp_password)!=PASSWORD_LENGTH-1)
            std::cout<<"账号或密码长度无效"<<std::endl;
        else
        {
            strcpy(admin,temp_admin);
            strcpy(password,temp_password);
            return true;
        }

        return false;
}

bool UserBase::FindUser(std::ifstream& ifs,UserData& ap,char* admin)
{
    while(ifs.read((char*)&ap,sizeof(ap)))
    {
        if(!strcmp(ap.admin,admin))
            return true;
    }
    return false;
}

void UserBase::Login(char* g_filename)
{
    std::ifstream ifs("src/userbase.txt",std::ios::in|std::ios::binary);
    if(!ifs.is_open())
    {
        std::cout<<"userbase打开失败"<<std::endl;
        this->state=Exit;
        return;
    }

    char admin[ADMIN_LENGTH];
    char password[PASSWORD_LENGTH];
    if(this->Input(admin,password)==false)
    {
        this->state=Wait;
        ifs.close();
        return;
    }

    UserData ap;
    if(this->FindUser(ifs,ap,admin))
    {
        if(!strcmp(ap.password,password))
        {
            strcpy(g_filename,"src/data/");
            strcat(g_filename,admin);
            strcat(g_filename,".txt");
            this->state=Success;
            std::cout<<"登录成功!"<<std::endl;
            ifs.close();
            return;
        }
        else
            std::cout<<"密码错误!"<<std::endl;
    }
    else
        std::cout<<"用户不存在!"<<std::endl;

    this->state=Wait;
    ifs.close();
}

void UserBase::Regisiter()
{
    std::ifstream ifs("src/userbase.txt",std::ios::in|std::ios::binary);
    if(!ifs.is_open())
    {
        std::cout<<"userbase打开失败"<<std::endl;
        this->state=Exit;
        return;
    }

    char admin[ADMIN_LENGTH];
    char password[PASSWORD_LENGTH];
    char re_admin[ADMIN_LENGTH];
    char re_password[PASSWORD_LENGTH];

    if(this->Input(admin,password)==false)
    {
        this->state=Wait;
        ifs.close();
        return;
    }

    UserData ap;
    if(this->FindUser(ifs,ap,admin))
    {
        this->state=Wait;
        std::cout<<"账号已存在!"<<std::endl;
    }
    else
    {
        this->ClearBuffer();
        std::cout<<"再次确定,";
        if(this->Input(re_admin,re_password)==false)
        {
            this->state=Wait;
            ifs.close();
            return;
        }
        
        if(!strcmp(re_admin,admin)&&!strcmp(re_password,password))
        {
            UserData tmp;
            strcpy(tmp.admin,re_admin);
            strcpy(tmp.password,re_password);

            char filename[64]="src/data/";
            strcat(filename,re_admin);
            strcat(filename,".txt");
            
            std::ofstream ofs1(filename,std::ios::out);
            std::ofstream ofs2("src/userbase.txt",std::ios::app);
            ofs2.write((char*)&tmp,sizeof(UserData));

            std::cout<<"注册成功!"<<std::endl;
            ofs1.close();
            ofs2.close();
        }
        else
            std::cout<<"账号或密码不一致!"<<std::endl;
    }

    this->state=Wait;
    ifs.close();
}