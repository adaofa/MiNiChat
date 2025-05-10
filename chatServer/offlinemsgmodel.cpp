#include "offlinemsgmodel.hpp"
#include "db.h"
#include <iostream>
using namespace std;

void OfflineMsgModel::insert(int userid, string msg, std::shared_ptr<MySQL> conn)
{
    //MySQL mysql;
    if (conn->connect())
    {
        string escaped_msg;
        escaped_msg.resize(msg.length() * 2 + 1);
        mysql_real_escape_string(conn->getConnection(), &escaped_msg[0], msg.c_str(), msg.length());
        escaped_msg.resize(strlen(escaped_msg.c_str()));

        cout << "Escaped message: " << escaped_msg << endl;  // Debugging line

        string sql = "INSERT INTO OfflineMessage(userid, message) VALUES(" +
                     to_string(userid) + ", '" + escaped_msg + "')";

        if (!conn->update(sql))
        {
            cerr << "Failed to execute SQL: " << sql << endl;
            return;
        }

        cout << "Offline message inserted successfully." << endl;
    }
    else
    {
        cerr << "Failed to connect to database!" << endl;
    }
}

void OfflineMsgModel::remove(int userid, std::shared_ptr<MySQL> conn)
{
    char sql[1024] = {0};
    sprintf(sql, "delete from OfflineMessage where userid=%d", userid);

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

vector<string> OfflineMsgModel::query(int userid, std::shared_ptr<MySQL> conn)
{
    //组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select message from OfflineMessage where userid = %d", userid);

    vector<string> vec;
    // MySQL mysql;
    // if (mysql.connect())
    // {
    //     MYSQL_RES *res = mysql.query(sql);
    //     if(res != nullptr)
    //     {
    //         MYSQL_ROW row;
    //         while((row = mysql_fetch_row(res)) != nullptr)
    //         {
    //             vec.push_back(row[0]);
    //         }
    //         mysql_free_result(res);
    //         return vec;
    //     }
    // }
    if(conn->connect())
    {
        MYSQL_RES *res = conn->query(sql);
        if(res != nullptr)
        {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                vec.push_back(row[0]);
            }
            mysql_free_result(res);
            return vec;
        }
    }

    return vec;
}
