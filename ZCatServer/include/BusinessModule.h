#include "DataBase.h"
#include "DataAnalysis.h"
#include<iostream>
#pragma once

bool CheckValidData(const char* str);
error_code RegistModule(user_info& u1,online_client_info& o1,DataBase& db1);
error_code LoginModule(user_info& u1,online_client_info& o1,int isonline,DataBase& db1);
error_code ApplyModule(int source_id,user_info& u1,DataBase& db1);

int GetApplyBuffer_len(const char* admin,DataBase& db1);
error_code GetApplyBufferModule(user_info** ptr,const char* admin,DataBase& db1);

int GetFriendBuffer_len(const char* admin,DataBase& db1);
error_code GetFriendBufferModule(user_info** ptr,const char* admin,DataBase& db1);

int GetUnicastMsgBuffer_len(int des_id,DataBase& db1);
error_code GetUnicastMsgBufferModule(unicast_msg** ptr,int id,DataBase& db1);

int GetMulticastMsgBuffer_len(int group_id,DataBase& db1);
error_code GetMulticastMsgBufferModule(multicast_msg** ptr,int group_id,DataBase& db1);

int GetGroupBuffer_len(int id,DataBase& db1);
error_code GetGroupBufferModule(group_info** ptr,int id,DataBase& db1);

int GetGroupApplyBuffer_len(int m_id,DataBase& db1);
error_code GetGroupApplyBufferModule(ug_info** ptr,int m_id,DataBase& db1);

int GetGroupMembers_len(int group_id,DataBase& db1);
error_code ViewGroupMembersModule(user_info** ptr,int group_id,DataBase& db1);

error_code PermitApplyModule(int sou_id,int des_id,DataBase& db1);
void RefuseApplyModule(int sou_id,int des_id,DataBase& db1);
error_code PermitGroupApplyModule(ug_info& ug,DataBase& db1);
void RefuseGroupApplyModule(ug_info& ug,DataBase& db1);

void DeleteFriendModule(int sou_id,int des_id,DataBase& db1);
void UnicastMsgModule(unicast_msg& um1,DataBase& db1);
void MulticastMsgModule(multicast_msg& mm1,DataBase& db1);

error_code CreateGroupModule(group_info& gi1,DataBase& db1);
error_code GroupApplyModule(int source_id,group_info& gi1,DataBase& db1);

void ExitGroupModule(ug_info& ug,DataBase& db1);
void RemoveGroupMemberModule(ug_info& ug,DataBase& db1);

int GetGroupMemberNumModule(int group_id,DataBase& db1);
void GetGroupMemberIDModule(int* ptr,int group_id,DataBase& db1);

error_code UpdatePasswdModule(user_info& u1,online_client_info& o1,DataBase& db1);
