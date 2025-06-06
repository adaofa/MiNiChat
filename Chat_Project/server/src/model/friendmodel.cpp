#include "friendmodel.hpp"
#include "db.h"

void FriendModel::insert(int userid, int friendid)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into Friend values(%d, %d)", userid, friendid);

    MySQL mysql;
    if(mysql.connect())
    {
        mysql.update(sql);
    }
}

vector<User> FriendModel::query(int userid)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select a.id, a.name, a.state from User a inner join Friend b on b. friendid =a.id where b.userid=%d",userid);

    MySQL mysql;
    vector<User> vec;
    if(mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr)
        {
            //把userid的离线消息放入vec中返回
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.push_back(user);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}