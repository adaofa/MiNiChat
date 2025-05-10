#include "chatservice.hpp"
#include "public.hpp"
#include <muduo/base/Logging.h>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <ctime>
using namespace muduo;
using namespace std;
using namespace placeholders;


struct FileTransferContext{
    std::ofstream fileStream;
    int64_t bytesRemaining = 0;
    std::string fileName;
};

// 获取单例对象的接口函数
ChatService *ChatService::instance()
{
    static ChatService service;
    return &service;
}

// 注册消息以及对应的Handler回调操作
ChatService::ChatService()
{
    // 用户基本业务相关的事件处理回调
    msgHandlerMap_.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    msgHandlerMap_.insert({LOGINOUT_MSG, std::bind(&ChatService::loginOut, this, _1, _2, _3)});
    msgHandlerMap_.insert({REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3)});
    msgHandlerMap_.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    msgHandlerMap_.insert({ADD_FRIEND_REQUEST_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3)});
    msgHandlerMap_.insert({ADD_FRIEND_RESPONSE_MSG, std::bind(&ChatService::addFriendResponse, this, _1, _2, _3)});
    msgHandlerMap_.insert({REMOVE_FRIEND_MSG, std::bind(&ChatService::removeFriend, this, _1, _2, _3)});

    // 群组业务相关的事件处理回调
    msgHandlerMap_.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    msgHandlerMap_.insert({REMOVE_GROUP_MSG, std::bind(&ChatService::removeGroup, this, _1, _2, _3)});
    msgHandlerMap_.insert({ADD_GROUP_REQUEST_MSG, std::bind(&ChatService::addGroup, this, _1, _2, _3)});
    msgHandlerMap_.insert({ADD_GROUP_RESPONSE_MSG, std::bind(&ChatService::addGroupResponse, this, _1, _2, _3)});
    msgHandlerMap_.insert({QUIT_GROUP_MSG, std::bind(&ChatService::quitGroup, this, _1, _2, _3)});
    msgHandlerMap_.insert({GROUP_CHAT_MSG, std::bind(&ChatService::chatGroup, this, _1, _2, _3)});

    // 连接redis服务器
    if (redis_.connect())
    {
        // 设置上报消息的回调
        redis_.init_notify_handler(std::bind(&ChatService::handleRedisSubscribeMessage, this, _1, _2));
    }
}

void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int id = js["id"].get<int>();
    string pwd = js["password"];

    User user = usermodel_.query(id, mysqlConn);
    if (user.getId() == id && user.getPwd() == pwd)
    {
        if (user.getState() == "online")
        {
            //该用户已经登录，不允许重新登录
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "The user has logged in and is not allowed to log in again";
            conn->send(response.dump());
        }
        else
        {
            {
                lock_guard<mutex> lock(connMutex_);
                userConnMap_.insert({id, conn});
            }

            redis_.subscribe(id);

            user.setState("online");
            usermodel_.updateState(user, mysqlConn);

            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();
            response["state"] = user.getState();

            //查询该用户是否有离线消息
            vector<string> vec = offlinemsgmodel_.query(id, mysqlConn);
            if (!vec.empty())
            {
                response["offlinemsg"] = vec;
                offlinemsgmodel_.remove(id, mysqlConn);
            }
            //查询该用户的好友信息并返回
            vector<User> userVec = friendmodel_.query(id, mysqlConn);
            if (!userVec.empty())
            {
                vector<string> vec2;
                for (User &user : userVec)
                {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec2.push_back(js.dump());
                }
                response["friends"] = vec2;
            }
            // 查询用户的群组信息
            vector<Group> groupuserVec = groupmodel_.queryGroups(id, mysqlConn);
            if (!groupuserVec.empty())
            {
                vector<string> groupV;
                for (Group &group : groupuserVec)
                {
                    json grpjson;
                    grpjson["id"] = group.getId();
                    grpjson["groupname"] = group.getName();
                    grpjson["groupdesc"] = group.getDesc();
                    vector<string> userV;
                    for (GroupUser &user : group.getUsers())
                    {
                        json js;
                        js["id"] = user.getId();
                        js["name"] = user.getName();
                        js["state"] = user.getState();
                        js["role"] = user.getRole();
                        userV.push_back(js.dump());
                    }
                    grpjson["users"] = userV;
                    groupV.push_back(grpjson.dump());
                }

                response["groups"] = groupV;
            }
            conn->send(response.dump());
        }
    }
    else
    {
        //该用户不存在，用户存在但是密码错误
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "User name or password error";
        conn->send(response.dump());
    }
}

void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state = usermodel_.insert(user, mysqlConn);
    if (state)
    {
        // 注册成功
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    else
    {
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

MsgHandler ChatService::getHandler(int msgid)
{
    // 记录错误日志，msgid没有对应的事件处理回调
    auto it = msgHandlerMap_.find(msgid);
    if (it == msgHandlerMap_.end())
    {
        // LOG_ERROR << "msgid: " << msgid << "can not find Handler!";
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp)
        {
            LOG_ERROR << "msgid: " << msgid << "can not find Handler!";
        };
    }
    else
    {
        return msgHandlerMap_[msgid];
    }
}

void ChatService::clientCloseExecption(const TcpConnectionPtr &conn)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    User user;
    {
        lock_guard<mutex> lock(connMutex_);
        for (auto it = userConnMap_.begin(); it != userConnMap_.end(); ++it)
        {
            if (it->second == conn)
            {
                // 从map表删除用户的连接信息
                user.setId(it->first);
                userConnMap_.erase(it);
                break;
            }
        }
    }

    // 用户注销后，在redis中取消订阅通道
    redis_.unsubscribe(user.getId());

    // 更新用户的状态信息
    if (user.getId() != -1)
    {
        user.setState("offline");
        usermodel_.updateState(user, mysqlConn);
    }
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::loginOut(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int userid = js["id"].get<int>();
    {
        lock_guard<mutex> lock(connMutex_);
        auto it = userConnMap_.find(userid);
        if (it != userConnMap_.end())
        {
            userConnMap_.erase(it);
        }
    }

    // 用户注销后，在redis中取消订阅通道
    redis_.unsubscribe(userid);

    // 更新用户状态
    User user(userid, "", "", "offline");
    usermodel_.updateState(user, mysqlConn);
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    cout << "find the toid" << endl;
    int toid = js["to"].get<int>();

    {
        lock_guard<mutex> lock(connMutex_);
        auto it = userConnMap_.find(toid);
        if (it != userConnMap_.end())
        {
            it->second->send(js.dump());
            cout << "find the toid" << endl;
            return;
        }
    }

    // 查询to是否在线 A和B不在同一个服务器上，但都在线
    User user = usermodel_.query(toid, mysqlConn);
    if (user.getState() == "online")
    {
        redis_.publish(toid, js.dump());
        return;
    }

    // toid不在线，存储离线消息
    offlinemsgmodel_.insert(toid, js.dump(), mysqlConn);
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::ChatMessage(const TcpConnectionPtr &conn, json &js, Timestamp)
{
    std::string fileName = js["msg"];
    int64_t fileSize = js["filesize"];
    int senderId = js["sender"];
    std::string senderName = js["name"];

    // 创建用于存储文件的路径
    std::string savePath = "./received_files/";
    std::string fullFilePath = savePath + fileName;

    // 确保目录存在
    std::filesystem::create_directories(savePath);

    // 打开文件准备写入
    std::ofstream outFile(fullFilePath, std::ios::binary);
    if (!outFile.is_open())
    {
        std::cerr << "无法打开文件用于写入：" << fullFilePath << std::endl;
        return;
    }

    // 设置一个接收缓冲区（假设你为 TcpConnectionPtr 设置了上下文来跟踪状态）
    auto context = std::make_shared<FileTransferContext>();
    context->fileStream = std::move(outFile);
    context->bytesRemaining = fileSize;
    context->fileName = fileName;

    conn->setContext(context);

    // 设置新的消息回调来处理文件流
    conn->setMessageCallback(std::bind(&ChatService::FileMessage, this, _1, _2, _3));
}

void ChatService::FileMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp)
{
    auto context = boost::any_cast<std::shared_ptr<FileTransferContext>>(conn->getContext());

    while (buffer->readableBytes() > 0 && context->bytesRemaining > 0)
    {
        size_t toWrite = std::min<int64_t>(buffer->readableBytes(), context->bytesRemaining);
        context->fileStream.write(buffer->peek(), toWrite);
        buffer->retrieve(toWrite);
        context->bytesRemaining -= toWrite;
    }

    if (context->bytesRemaining <= 0)
    {
        context->fileStream.close();
        std::cout << "文件接收完毕: " << context->fileName << std::endl;

        // 恢复消息回调为原始的 json 消息处理逻辑
        //conn->setMessageCallback(std::bind(&ChatService::oneChat, this, _1, _2, _3));
    }
}

void ChatService::reset()
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    // 把所有online状态的用户设置为offline
    usermodel_.resetState(mysqlConn);
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int userid = js["id"];
    int friendid = js["friendid"];

    User user = usermodel_.query(friendid, mysqlConn);
    if (user.getId() == -1)
    {
        // 用户不存在
        json response;
        response["msgid"] = ADD_FRIEND_RESPONSE_MSG;
        response["errno"] = 1;
        response["errmsg"] = "用户不存在, 请检查ID是否正确!";
        conn->send(response.dump());
        return;
    }

    {
        lock_guard<mutex> lock(connMutex_);
        auto it = userConnMap_.find(friendid);
        if (it != userConnMap_.end())
        {
            it->second->send(js.dump());
            return;
        }
    }

    if (user.getState() == "online")
    {
        js["msgid"] = ADD_FRIEND_REQUEST_MSG;
        redis_.publish(friendid, js.dump());
        return;
    }

    offlinemsgmodel_.insert(friendid, js.dump(), mysqlConn);
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::addFriendResponse(const TcpConnectionPtr &conn, json &js, Timestamp)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    bool accept = js["accept"];
    int sender = js["from"];    //  B
    int receiver = js["to"];    //  A

    // 通知发起者
    //redis_.publish(receiver, js.dump());
    //conn->send(js.dump());
    if(!accept)
    {
        conn->send(js.dump());
        MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
        return;
    }
    if (accept)
    {
        friendmodel_.insert(sender, receiver, mysqlConn);
        friendmodel_.insert(receiver, sender, mysqlConn);

        //auto it_sender = userConnMap_.find(receiver);
        //auto it_receiver = userConnMap_.find(sender);
        friendsyn(receiver);
        friendsyn(sender);
        MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
        return;
    }
}

void ChatService::removeFriend(const TcpConnectionPtr &conn, json &js, Timestamp)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();

    // 删除好友信息
    friendmodel_.remove(userid, friendid, mysqlConn);
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int userid = js["id"].get<int>();
    string username = js["username"];
    string groupname = js["groupname"];
    string desc = js["groupdesc"];

    json response;
    response["msgid"] = CREATE_GROUP_MSG;
    // 存储新创建的群组信息
    Group group(-1, groupname, desc);
    if (groupmodel_.createGroup(group, mysqlConn))
    {
        // 存储群组创建人的信息
        groupmodel_.addGroup(userid, group.getId(), "creator", mysqlConn);

        response["errno"] = 0;
        response["userid"] = userid;
        response["username"] = username;
        response["groupname"] = groupname;
        response["groupdesc"] = desc;
        response["groupid"] = group.getId();
    }
    //redis_.publish(userid, response.dump());
    conn->send(response.dump());
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::removeGroup(const TcpConnectionPtr &conn, json &js, Timestamp)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    string grouprole = js["grouprole"];
    vector<int> users = groupmodel_.queryGroupsUsers(userid, groupid, mysqlConn);

    groupmodel_.removeGroup(groupid, userid, grouprole, mysqlConn);

    for(int user : users)
    {
        groupsyn(user);
    }
    groupsyn(userid);
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int groupid = js["groupid"].get<int>();

    Group group = groupmodel_.query(groupid, mysqlConn);
    if(group.getId() == -1)
    {
        json response;
        response["msgid"] = ADD_GROUP_RESPONSE_MSG;
        response["errno"] = 1;
        response["errmsg"] = "群聊不存在, 请检查ID是否正确!";
        conn->send(response.dump());
        return;
    }
    //根据群号寻找群主id
    int creator = groupmodel_.searchGroupCreator(groupid, mysqlConn);
    {
        lock_guard<mutex> lock(connMutex_);
        auto it = userConnMap_.find(creator);
        if (it != userConnMap_.end())
        {
            it->second->send(js.dump());
            return;
        }
    }
    //向群主发送进入群聊请求
    User user = usermodel_.query(creator, mysqlConn);
    if (user.getState() == "online")
    {
        js["msgid"] = ADD_GROUP_REQUEST_MSG;
        redis_.publish(creator, js.dump());
        return;
    }
    offlinemsgmodel_.insert(creator, js.dump(), mysqlConn);
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::addGroupResponse(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int groupid = js["groupid"];
    bool accept = js["accept"];
    int sender = js["from"];    //  B 群主
    int receiver = js["to"];    //  A
    vector<int> groupusers = groupmodel_.queryGroupsUsers(sender, groupid, mysqlConn);

    // 通知发起者
    if(!accept)
    {
        conn->send(js.dump());
        //redis_.publish(receiver, js.dump());
        MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
        return;
    }
    if (accept)
    {
        groupmodel_.addGroup(receiver, groupid, "normal", mysqlConn);
        for(int i = 0; i < groupusers.size(); ++i)
        {
            groupsyn(groupusers[i]);
        }
        groupsyn(receiver);
        groupsyn(sender);
        MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
        return;
    }
}

void ChatService::quitGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    string grouprole = js["grouprole"];
    vector<int> users = groupmodel_.queryGroupsUsers(userid, groupid, mysqlConn);
    //群主退出群聊后，随机选择一名群聊成员为新任群主
    if (grouprole == "creator" && users.size() != 0)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, users.size() - 1);

        int idx = dis(gen);
        int rand_user = users[idx];

        groupmodel_.changeRole(rand_user, groupid, mysqlConn);
    }
    if (grouprole == "creator" && users.size() == 0)
    {
        groupmodel_.removeGroup(groupid, userid, grouprole, mysqlConn);
    }
    groupmodel_.quitGroup(groupid, userid, grouprole, mysqlConn);
    for(int user : users)
    {
        groupsyn(user);
    }
    groupsyn(userid);
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::friendsyn(int userid)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    json response;
    response["msgid"] = UPDATE_FRIEND_MSG;
    vector<User> userVec = friendmodel_.query(userid, mysqlConn);
    if(!userVec.empty())
    {
        json userV = json::array();
        for (User &user : userVec)
        {
            json userjson;
            userjson["id"] = user.getId();
            userjson["name"] = user.getName();
            userjson["state"] = user.getState();
            userV.push_back(userjson);
        }
        response["friends"] = userV;
    }
    redis_.publish(userid, response.dump());
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
    cout << userid << " redis_ published!" << endl;
}

void ChatService::groupsyn(int userid)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    json response;
    response["msgid"] = UPDATE_GROUP_MSG;
    vector<Group> groupuserVec = groupmodel_.queryGroups(userid, mysqlConn);
    if (!groupuserVec.empty())
    {
        json groupV = json::array();
        for (Group &group : groupuserVec)
        {
            json grpjson;
            grpjson["id"] = group.getId();
            grpjson["groupname"] = group.getName();
            grpjson["groupdesc"] = group.getDesc();
            grpjson["groupusers"] = group.getNumUsers();

            json userV = json::array();
            for (GroupUser &user : group.getUsers())
            {
                json js;
                js["id"] = user.getId();
                js["name"] = user.getName();
                js["state"] = user.getState();
                js["role"] = user.getRole();
                userV.push_back(js);
            }
            grpjson["users"] = userV;
            groupV.push_back(grpjson);
        }
        response["groups"] = groupV;
    }
    redis_.publish(userid, response.dump());
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::chatGroup(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    vector<int> useridVec = groupmodel_.queryGroupsUsers(userid, groupid, mysqlConn);

    lock_guard<mutex> lock(connMutex_);
    for (int id : useridVec)
    {
        auto it = userConnMap_.find(id);
        if (it != userConnMap_.end())
        {
            // 转发群消息
            it->second->send(js.dump());
        }
        else
        {
            // 查询to是否在线 A和B不在同一个服务器上，但都在线
            User user = usermodel_.query(id, mysqlConn);
            if (user.getState() == "online")
            {
                redis_.publish(id, js.dump());
            }
            else
            {
                // 存储离线消息
                offlinemsgmodel_.insert(id, js.dump(), mysqlConn);
            }
        }
    }
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

void ChatService::handleRedisSubscribeMessage(int userid, string msg)
{
    auto mysqlConn = MysqlConnPool::getInstance()->getConnection();
    lock_guard<mutex> lock(connMutex_);
    auto it = userConnMap_.find(userid);
    if (it != userConnMap_.end())
    {
        it->second->send(msg);
        return;
    }
    offlinemsgmodel_.insert(userid, msg, mysqlConn);
    MysqlConnPool::getInstance()->releaseConnection(mysqlConn);
}

