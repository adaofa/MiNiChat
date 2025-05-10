#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

class GroupUser : public User
{
public:
    void setRole(string role)    {this->role_ = role;}
    string getRole()    {return this->role_;}

private:
    string role_;

};
#endif