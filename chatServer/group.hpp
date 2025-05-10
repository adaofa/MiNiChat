#ifndef GROUP_H
#define GROUP_H

#include "groupuser.hpp"
#include <string>
#include <vector>
using namespace std;

class Group
{
public:
    Group(int id = -1, string name ="", string desc = "", int num_users = 0)
    {
        this->id_ = id;
        this->name_ = name;
        this->desc_ = desc;
        this->num_users_ = num_users;
    }

    void setId(int id)  {this->id_ = id;}
    void setName(string name) {this->name_ = name;}
    void setDesc(string desc) {this->desc_ = desc;}
    void setNumUsers(int num_users) {this->num_users_ = num_users;}

    int getId() {return this->id_;}
    string getName()    {return this->name_;}
    string getDesc()    {return this->desc_;}
    int getNumUsers() {return this->num_users_;}
    vector<GroupUser> &getUsers()    {return this->users_;}

private:
    int id_;
    int num_users_;
    string name_;
    string desc_;
    vector<GroupUser> users_;

};



#endif