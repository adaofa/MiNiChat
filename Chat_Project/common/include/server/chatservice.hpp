#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include<muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
#include<memory>
#include "json.hpp"
#include "usermodel.hpp"
#include "offlinemsgmodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"
using json = nlohmann::json;
using namespace muduo::net;
using namespace muduo;
using namespace std;

//处理消息的事件回调方法类型
using MsgHandler = std::function<void(const TcpConnectionPtr& conn, json &js, Timestamp)>;

//聊天服务器业务类
class ChatService
{
public:
    //获取单例对象的接口函数
    static ChatService* instance();
    //处理登录业务
    void login(const TcpConnectionPtr& conn, json &js, Timestamp);
    //处理注册业务
    void reg(const TcpConnectionPtr& conn, json &js, Timestamp);
    //一对一聊天服务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp);
    //获取消息对应的处理器
    MsgHandler getHandler(int msgid);
    //处理客户端异常退出
    void clientCloseExecption(const TcpConnectionPtr &conn);
    //添加好友业务处理
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp);
    //创建群组
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp);
    //加入群组
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp);
    //群组聊天
    void chatGroup(const TcpConnectionPtr &conn, json &js, Timestamp);
    //处理注销业务
    void loginOut(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //服务器异常退出后，业务重置方法
    void reset();
    //从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);
    
private:
    ChatService();

    //存储消息id和其对应的业务处理方法
    unordered_map<int, MsgHandler> msgHandlerMap_;

    //存储在线用户的通信连接
    unordered_map<int, TcpConnectionPtr> userConnMap_;

    //定义互斥锁，保证userConnMap_的线程安全
    mutex connMutex_;

    //数据操作类对象
    UserModel usermodel_;
    OfflineMsgModel offlinemsgmodel_;
    FriendModel friendmodel_;
    GroupModel groupModel_;

    Redis redis_;

};

#endif