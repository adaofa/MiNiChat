#include "friendmodel.hpp"
#include "db.h"

void FriendModel::insert(int userid, int friendid, std::shared_ptr<MySQL> conn)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into Friend values(%d, %d)", userid, friendid);

    // MySQL mysql;
    // if(mysql.connect())
    // {
    //     mysql.update(sql);
    // }
    if(conn->connect())
    {
        conn->update(sql);
    }
}

void FriendModel::remove(int userid, int friendid, std::shared_ptr<MySQL> conn)
{
    char sql[1024] = {0};
    sprintf(sql, "delete from Friend where (userid = %d and friendid = %d) \
                or (userid = %d and friendid = %d)", userid, friendid, friendid, userid);

    // MySQL mysql;
    // if(mysql.connect())
    // {
    //     mysql.update(sql);
    // }
    if(conn->connect())
    {
        conn->update(sql);
    }
}

bool FriendModel::select_friend(int userid, std::shared_ptr<MySQL> conn)
{
    char sql[1024] = {0};
    sprintf(sql, "select from User where id = %d", userid);

    // MySQL mysql;
    // if (mysql.connect())
    // {
    //     MYSQL_RES* res = mysql.query(sql);
    //     if (res != nullptr)
    //     {
    //         //如果能查找到一行数据，则表明该用户存在
    //         bool is_exists = mysql_num_rows(res) > 0;
    //         mysql_free_result(res);
    //         return true;
    //     }
    // }

    if(conn->connect())
    {
        MYSQL_RES* res = conn->query(sql);
        if(res != nullptr)
        {
            bool is_exists = mysql_num_rows(res) > 0;
            mysql_free_result(res);
            return true;
        }
    }
    return false;  // 查询失败或用户不存在
}

vector<User> FriendModel::query(int userid, std::shared_ptr<MySQL> conn)
{
    //1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select a.id, a.name, a.state from User a inner join Friend b on b. friendid =a.id where b.userid=%d",userid);

    // MySQL mysql;
    // vector<User> vec;
    // if(mysql.connect())
    // {
    //     MYSQL_RES *res = mysql.query(sql);
    //     if(res != nullptr)
    //     {
    //         //把userid的离线消息放入vec中返回
    //         MYSQL_ROW row;
    //         while ((row = mysql_fetch_row(res)) != nullptr)
    //         {
    //             User user;
    //             user.setId(atoi(row[0]));
    //             user.setName(row[1]);
    //             user.setState(row[2]);
    //             vec.push_back(user);
    //         }
    //         mysql_free_result(res);
    //         return vec;
    //     }
    // }

    vector<User> vec;
    if(conn->connect())
    {
        MYSQL_RES *res = conn->query(sql);
        if(res != nullptr)
        {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
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
