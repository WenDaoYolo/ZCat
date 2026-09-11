#include "DataBase.h"
#include "DataAnalysis.h"
#include<iostream>
#pragma once

bool CheckValidData(const char* str);
error_code RegistModule(user_info& u1,online_client_info& o1);
error_code LoginModule(user_info& u1,online_client_info& o1,int isonline);
error_code ApplyModule(int source_id,user_info& u1);

int GetApplyBuffer_len(const char* admin);
error_code GetApplyBufferModule(user_info** ptr,const char* admin);

int GetFriendBuffer_len(const char* admin);
error_code GetFriendBufferModule(user_info** ptr,const char* admin);

int GetUnicastMsgBuffer_len(int des_id);
error_code GetUnicastMsgBufferModule(unicast_msg** ptr,int id);

int GetMulticastMsgBuffer_len(int group_id);
error_code GetMulticastMsgBufferModule(multicast_msg** ptr,int group_id);

int GetGroupBuffer_len(int id);
error_code GetGroupBufferModule(group_info** ptr,int id);

int GetGroupApplyBuffer_len(int m_id);
error_code GetGroupApplyBufferModule(ug_info** ptr,int m_id);

int GetGroupMembers_len(int group_id);
error_code ViewGroupMembersModule(user_info** ptr,int group_id);

error_code PermitApplyModule(int sou_id,int des_id);
void RefuseApplyModule(int sou_id,int des_id);
error_code PermitGroupApplyModule(ug_info& ug);
void RefuseGroupApplyModule(ug_info& ug);

void DeleteFriendModule(int sou_id,int des_id);
void UnicastMsgModule(unicast_msg& um1);
void MulticastMsgModule(multicast_msg& mm1);

error_code CreateGroupModule(group_info& gi1);
error_code GroupApplyModule(int source_id,group_info& gi1);

void ExitGroupModule(ug_info& ug);
void RemoveGroupMemberModule(ug_info& ug);

int GetGroupMemberNumModule(int group_id);
void GetGroupMemberIDModule(int* ptr,int group_id);

error_code UpdatePasswdModule(user_info& u1,online_client_info& o1);
