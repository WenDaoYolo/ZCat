#include<iostream>
#include<fstream>
#include<windows.h>
#include<cstring>
#include<iomanip>
#pragma once
#define DELIMITER_SPACE 4
#define ID_LENGTH 10
#define AGE_LENGTH 6

#define NAME_BUFFER_S 12
#define SEX_BUFFER 6
#define PHONENUMBER_BUFFER 14
#define EMAIL_BUFFER 24
#define ADDRESS_BUFFER 40
#define TOTAL_BUFFER NAME_BUFFER_S+SEX_BUFFER+PHONENUMBER_BUFFER+EMAIL_BUFFER+ADDRESS_BUFFER

void SetCursor(int flag);  //设置控制台光标的可见性，0不可见1可见

enum BaseState
{
    wait,           //0
    add,            //1
    del,            //2
    find,           //3
    change,         //4
    sort,           //5
    display,        //6
    exit_s          //7
};

enum CmpOrder
{
    Up,             //0
    Down            //1
};

struct Data{
    int id;
    char name[NAME_BUFFER_S];
    char sex[SEX_BUFFER];
    int age;
    char phonenumber[PHONENUMBER_BUFFER];
    char email[EMAIL_BUFFER];
    char address[ADDRESS_BUFFER];
};

struct Node{
    Data data;
    Node* next;
};

class DataBase{
    public:
        int length;
        Node* base;
        BaseState state;

        DataBase();
        ~DataBase();
        bool LoadFile(const char* filename);
        void SaveFile(const char* filename);

        void PrintBorder(char border_flag);
        void PrintText(const char* string);
        void PrintTable();
        void PrintDataValue(Node* find);

        void DisplayMenu();
        void DisplayInformation();
        void FlushScreen();

        bool CheckEmpty();
        void ClearBuffer();
        bool CheckInputError();
        bool CheckRepeatData(int id);
        void Input(Node* tmp);

        void AddNode(Node* tmp);
        void DeleteNode();
        void SortNode(CmpOrder sys,int(*cmp)(void*,void*,CmpOrder));
        void FindNode();
        void ChangeNode();
};

int CmpById(void* e1,void* e2,CmpOrder sys);
int CmpByName(void* e1,void* e2,CmpOrder sys);
int CmpBySex(void* e1,void* e2,CmpOrder sys);
int CmpByAge(void* e1,void* e2,CmpOrder sys);
void SwapNode(Node* n1,Node* n2);