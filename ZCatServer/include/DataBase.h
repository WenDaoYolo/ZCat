#include "DataAnalysis.h"
#include<pqxx/pqxx>
#include<iostream>
#include<cstring>
#include<iomanip>
#include<fstream>
#pragma once

class DataBase
{
    private:
        pqxx::connection* c1;
    public:
        DataBase();
        bool FindUser(const std::string& admin,user_info* u_info);
        void InsertUser(const char* admin,const char* passwd,const char* name);
        void InsertLog(const char* ip,const char* admin,const char* active);

        bool InsertApply(int sou_id,int des_id); //if insert error -> exists
        void RemoveApply(int sou_id,int des_id);
        void RemoveGroupApply(int sou_id,int group_id,int manager_id);

        bool FindFriend(int sou_id,int des_id);
        void InsertFriend(int sou_id,int des_id);
        void DeleteFriend(int sou_id,int des_id);
        
        int SearchApplyBuffer(int id);
        int SearchFriendBuffer(int id);
        int SearchUnicastMsgBuffer(int id);
        int SearchGroupBuffer(int id);
        int SearchGroupApplyBuffer(int m_id);
        int SearchGroupMembers(int group_id);
        int SearchMulticastMsgBuffer(int group_id);

        void DownLoadApplyBuffer(user_info** ptr,int id); 
        void DownLoadFriendBuffer(user_info** ptr,int id);
        void DownLoadUnicastMsgBuffer(unicast_msg** ptr,int id);
        void DownLoadGroupBuffer(group_info** ptr,int id);
        void DownLoadGroupApplyBuffer(ug_info** ptr,int m_id);
        void DownLoadGroupMembers(user_info** ptr,int group_id);
        void DownLoadMulticastMsgBuffer(multicast_msg** ptr,int group_id);

        void InsertUnicastMsg(unicast_msg* um1);
        void InsertMulticastMsg(multicast_msg* mm1);

        bool FindGroup(const char* admin,group_info* gi1);
        void InsertGroup(group_info& gi1);
        int CountGroupNums(int id);

        bool InsertGroupApply(int sou_id,int group_id,int manager_id);
        bool FindGroupMember(int sou_id,int group_id);
        void InsertGroupMember(int sou_id,int group_id,int manager_id);
        void RemoveGroupMember(int sou_id,int group_id,int manager_id);

        int FindGroupMemberNum(int group_id);
        void GetGroupMemberID(int* ptr,int group_id);


        void View_All_Users();
        bool IsBlackUser(const char* admin);
        bool InsertBlackList(int id);
        bool RemoveBlackList(int id);
        void ViewBlackList();
        void ViewActiveLog();

        void ClearActiveLog();
        bool DeleteOneActiveLog(int act_id);
        void UpdatePasswd(int id,const char* passwd);

        ~DataBase();
};