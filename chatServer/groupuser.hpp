#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

class GroupUser : public User
{
public:
    void setRole(string role)    {this->role_ = role;}
    void setChatState(string chatstate){this->chat_state_ = chatstate;}

    string getRole()    {return this->role_;}
    string getChatState()   {return this->chat_state_;}

private:
    string role_;
    string chat_state_;

};
#endif