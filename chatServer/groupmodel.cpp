#include "groupmodel.hpp"
#include "db.h"
#include <iostream>
// 创建群组
bool GroupModel::createGroup(Group &group, std::shared_ptr<MySQL> conn)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    snprintf(sql, sizeof(sql),
             "insert into AllGroup(groupname, groupdesc, groupusers) values('%s', '%s', %d)",
             group.getName().c_str(),
             group.getDesc().c_str(),
             group.getNumUsers());

    // MySQL mysql;
    // if (mysql.connect())
    // {
    //     if(mysql.update(sql))
    //     {
    //         group.setId(mysql_insert_id(mysql.getConnection()));
    //         return true;
    //     }
    // }
    if(conn->connect())
    {
        if(conn->update(sql))
        {
            group.setId(mysql_insert_id(conn->getConnection()));
            return true;
        }
    }
    else
    {
        std::cerr << "SQL执行失败: " << sql << std::endl;
    }
    return false;
}

void GroupModel::removeGroup(int groupid, int userid, string grouprole, std::shared_ptr<MySQL> conn)
{
    char sql[1024] = {0};

    if (grouprole == "creator")
        sprintf(sql, "delete from AllGroup where id = %d", groupid); // 设置groupid为id的外键
    else
    {
        printf("you have no authority to remove this group, because you are not the creator!");
    }

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

// 加入群组
void GroupModel::addGroup(int userid, int groupid, string role, std::shared_ptr<MySQL> conn)
{
    // 1.组装sql语句
    char sql[1024] = {0};

    //防止用户重复进群
    sprintf(sql, "insert into GroupUser (groupid, userid, grouprole) \
                select %d, %d, '%s' where not exists( select 1 from GroupUser where \
                groupid = %d and userid = %d)", groupid, userid, role.c_str(), groupid, userid);

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

void GroupModel::quitGroup(int groupid, int userid, string grouprole, std::shared_ptr<MySQL> conn)
{
    char sql[1024] = {0};
    sprintf(sql, "delete from GroupUser where groupid = %d and userid = %d", groupid, userid);

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

void GroupModel::changeRole(int userid, int groupid, std::shared_ptr<MySQL> conn)
{
    char sql[1024] = {0};

    sprintf(sql, "update GroupUser set grouprole = 'creator' where userid = %d and groupid = %d", userid, groupid);

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

Group GroupModel::query(int groupid, std::shared_ptr<MySQL> conn)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from AllGroup where id=%d", groupid);

    // MySQL mysql;
    // if(mysql.connect())
    // {
    //     MYSQL_RES *res = mysql.query(sql);
    //     if(res != nullptr)
    //     {
    //         MYSQL_ROW row = mysql_fetch_row(res);
    //         if(row != nullptr)
    //         {
    //             Group group;
    //             group.setId(atoi(row[0]));
    //             group.setName(row[1]);
    //             group.setDesc(row[2]);
    //             group.setNumUsers(atoi(row[3]));

    //             mysql_free_result(res);
    //             return group;
    //         }
    //     }
    // }
    if(conn->connect())
    {
        MYSQL_RES *res = conn->query(sql);
        if(res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row != nullptr)
            {
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                group.setNumUsers(atoi(row[3]));

                mysql_free_result(res);
                return group;
            }
        }
    }

    Group invalid_group;
    invalid_group.setId(-1);     // 群聊不存在
    return invalid_group;
}

// 查询用户所在的群组信息
vector<Group> GroupModel::queryGroups(int userid, std::shared_ptr<MySQL> conn)
{
    char sql[1024] = {0};
    sprintf(sql, "select a.id,a.groupname,a.groupdesc,a.groupusers from AllGroup a inner join GroupUser b on a.id = b.groupid where b.userid=%d", userid);

    vector<Group> groupVec;

    //MySQL mysql;
    if (conn->connect())
    {
        MYSQL_RES *res = conn->query(sql);
        if (res != nullptr)
        {
            //查出userid的所有群组信息
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                Group group;
                group.setId(atoi(row[0]));
                group.setName(row[1]);
                group.setDesc(row[2]);
                group.setNumUsers(atoi(row[3]));
                groupVec.push_back(group);
            }
            mysql_free_result(res);
        }
    }

    for (Group &group : groupVec)
    {
        sprintf(sql, "select a.id,a.name,a.state,b.grouprole from User a inner join \
            GroupUser b on b.userid = a.id where b.groupid=%d", group.getId());

        MYSQL_RES *res = conn->query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                GroupUser user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                user.setRole(row[3]);
                group.getUsers().push_back(user);
            }
            mysql_free_result(res);
        }
    }
    return groupVec;
}

// 根据指定的groupid查询群组用户id列表，除userid自己，用户通过群聊给群组其他成员群发消息
vector<int> GroupModel::queryGroupsUsers(int userid, int groupid, std::shared_ptr<MySQL> conn)
{
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select userid from GroupUser where groupid = %d and userid != %d", groupid, userid);

    vector<int> idVec;
    // MySQL mysql;
    // if(mysql.connect())
    // {
    //     MYSQL_RES *res = mysql.query(sql);
    //     if (res != nullptr)
    //     {
    //         // 把userid的离线消息放入vec中返回
    //         MYSQL_ROW row;
    //         while ((row = mysql_fetch_row(res)) != nullptr)
    //         {
    //             idVec.push_back(atoi(row[0]));
    //         }
    //         mysql_free_result(res);
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
                idVec.push_back(atoi(row[0]));
            }
            mysql_free_result(res);
        }
    }

    return idVec;
}

int GroupModel::searchGroupCreator(int groupid, std::shared_ptr<MySQL> conn)
{
    char sql[1024];
    sprintf(sql, "select a.userid from GroupUser a inner join AllGroup b on b.id=a.groupid \
                    where a.grouprole='creator' and a.groupid=%d", groupid);
    
    int creator = 0;
    // MySQL mysql;
    // if(mysql.connect())
    // {
    //     MYSQL_RES *res = mysql.query(sql);
    //     if(res != nullptr)
    //     {
    //         MYSQL_ROW row = mysql_fetch_row(res);
    //         if(row != nullptr)
    //         {
    //             creator = atoi(row[0]);
    //         }
    //         mysql_free_result(res);
    //     }
    // }
    if(conn->connect())
    {
        MYSQL_RES *res = conn->query(sql);
        if(res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row != nullptr)
            {
                creator = atoi(row[0]);
            }
            mysql_free_result(res);
        }
    }

    return creator;
}
