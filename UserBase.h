#include<iostream>
#include<windows.h>
#include<fstream>
#include<cstring>
#define ADMIN_LENGTH 12
#define PASSWORD_LENGTH 12
#pragma once

struct UserData{
    char admin[ADMIN_LENGTH];
    char password[PASSWORD_LENGTH];
};

enum UserState{
    Wait,
    Login,             //1
    Regisiter,         //2
    Exit,              //3
    Success           
};

class UserBase{
    public:
        UserData data;
        UserState state;
        
        UserBase();
        ~UserBase();
        void ClearBuffer();
        bool CheckInputError();
        void PrintMenu();
        void FlushScreen();

        bool Input(char* admin,char* password);
        bool FindUser(std::ifstream& ifs,UserData& ap,char* admin);
        void Login(char* g_filename);
        void Regisiter();
};

