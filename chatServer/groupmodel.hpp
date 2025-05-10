#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "group.hpp"
#include "user.hpp"
#include <string>
#include <vector>
#include "mysqlconnpool.h"
using namespace std;

class GroupModel
{
public:
    //创建群组
    bool createGroup(Group &group, std::shared_ptr<MySQL> conn);
    //删除群组
    void removeGroup(int groupid, int userid, string grouprole, std::shared_ptr<MySQL> conn);
    //退出群组
    void quitGroup(int groupid, int userid, string grouprole, std::shared_ptr<MySQL> conn);
    //加入群组
    void addGroup(int userid, int groupid, string role, std::shared_ptr<MySQL> conn);
    //修改用户在群聊中的身份
    void changeRole(int userid, int groupid, std::shared_ptr<MySQL> conn);
    //群主将指定用户禁言
    //void changeMute(int groupid, int userid);
    //查询当前用户是否被禁言
    //void isMuted(int userid);
    //根据groupid查询指定群组
    Group query(int groupid, std::shared_ptr<MySQL> conn);
    //查询用户所在的群组信息
    vector<Group> queryGroups(int userid, std::shared_ptr<MySQL> conn);
    //根据指定的groupid查询群组用户id列表，除userid自己，主要用户群聊业务给群组其他成员群发消息
    vector<int> queryGroupsUsers(int userid, int groupid, std::shared_ptr<MySQL> conn);
    //根据groupid寻找群主
    int searchGroupCreator(int groupid, std::shared_ptr<MySQL> conn);
};


#endif
