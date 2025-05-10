#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "user.hpp"
#include "mysqlconnpool.h"
#include<vector>
using namespace std;

//维护好友信息的操作接口
class FriendModel
{
public:
    //添加好友关系
    void insert(int userid, int friendid, std::shared_ptr<MySQL> conn);

    //删除好友关系
    void remove(int userid, int friendid, std::shared_ptr<MySQL> conn);

    //判断指定用户是否存在
    bool select_friend(int userid, std::shared_ptr<MySQL> conn);

    //返回用户好友列表
    vector<User> query(int userid, std::shared_ptr<MySQL> conn);
};


#endif
