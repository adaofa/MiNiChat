#ifndef OFFLINEMSGMODEL_H
#define OFFLINEMSGMODEL_H
#include<vector>
#include<string>
#include "mysqlconnpool.h"
using namespace std;
//提供离线消息表的操作接口方法
class OfflineMsgModel
{
public:
    //存储用户的离线消息
    void insert(int userid, string msg, std::shared_ptr<MySQL> conn);

    //删除用户的离线消息
    void remove(int userid, std::shared_ptr<MySQL> conn);

    //查询用户的离线消息
    vector<string> query(int userid, std::shared_ptr<MySQL> conn);

};

#endif
