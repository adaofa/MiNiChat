#include "redis.hpp"
#include<iostream>
using namespace std;

Redis::Redis()
    : publish_context_(nullptr), subscribe_context_(nullptr)
{

}

Redis::~Redis()
{
    if(publish_context_ != nullptr)
    {
        redisFree(publish_context_);
    }

    if(subscribe_context_ != nullptr)
    {
        redisFree(subscribe_context_);
    }
}

bool Redis::connect()
{
    const char* redis_password = "123456";

    //负责publish发布消息的上下文连接
    publish_context_ = redisConnect("127.0.0.1", 6379);
    if(nullptr == publish_context_)
    {
        cerr << "connect redis failed!" << endl;
        return false;
    }

    // 发送 AUTH 命令
    redisReply* reply = (redisReply*)redisCommand(publish_context_, "AUTH %s", redis_password);
    if (!reply || reply->type == REDIS_REPLY_ERROR)
    {
        cerr << "publish_context_ AUTH failed: " << (reply ? reply->str : "no reply") << endl;
        if (reply) freeReplyObject(reply);
        redisFree(publish_context_);
        return false;
    }
    freeReplyObject(reply);

    //负责subscribe订阅消息的上下文连接
    subscribe_context_ = redisConnect("127.0.0.1", 6379);
    if(nullptr == subscribe_context_)
    {
        cerr << "connect redis failed!" << endl;
        return false;
    }

    // 发送 AUTH 命令
    reply = (redisReply*)redisCommand(subscribe_context_, "AUTH %s", redis_password);
    if (!reply || reply->type == REDIS_REPLY_ERROR)
    {
        cerr << "subscribe_context_ AUTH failed: " << (reply ? reply->str : "no reply") << endl;
        if (reply) freeReplyObject(reply);
        redisFree(subscribe_context_);
        return false;
    }
    freeReplyObject(reply);

    //在单独的线程中监听通道上的事件，有消息时给业务层上报
    thread t([&]() {
        observer_channel_message();
    });
    t.detach();

    cout << "connect redis-server success!" << endl;

    return true;
}

bool Redis::publish(int channel, string message)
{
    redisReply *reply = (redisReply *)redisCommand(publish_context_, "PUBLISH %d %s", channel, message.c_str());
    if(nullptr == reply)
    {
        cerr << "publish command failed!" << endl;
        return false;
    }
    freeReplyObject(reply);
    return true;
}

bool Redis::subscribe(int channel)
{
    //SUBSCRIBE命令本身会导致线程阻塞等待通道里面发送消息，这里只做了订阅通道，不接收通道消息
    //通道消息的接收专门在observer_channel_message函数中的独立线程中进行
    //只负责发送命令，不阻塞接收redis server响应消息，否则和notifyMsg线程抢占响应资源
    if(REDIS_ERR == redisAppendCommand(this->subscribe_context_, "SUBSCRIBE %d", channel))
    {
        cerr << "subscribe command failed!" << endl;
        return false;
    }

    int done = 0;
    while(!done)
    {
        if(REDIS_ERR == redisBufferWrite(this->subscribe_context_, &done))
        {
            cerr << "subscribe command failed!" << endl;
            return false;
        }
    }
    //redisGetReply

    return true;
}

bool Redis::unsubscribe(int channel)
{
    if(REDIS_ERR == redisAppendCommand(this->subscribe_context_, "UNSUBSCRIBE %d", channel))
    {
        cerr << "unsubscribe command failed!" << endl;
        return false;
    }
    //redisBufferWrite可以循环发送缓冲区，直到缓冲区数据发送完毕(done被置为1)
    int done = 0;
    while(!done)
    {
        if(REDIS_ERR == redisBufferWrite(this->subscribe_context_, &done))
        {
            cerr << "unsubscribe command failed!" << endl;
            return false;
        }
    }
    return true;
}

void Redis::observer_channel_message()
{
    redisReply *reply = nullptr;
    while(REDIS_OK == redisGetReply(this->subscribe_context_, (void **)&reply))
    {
        //订阅收到的消息是一个带三元素的数组
        if(reply != nullptr && reply->element[2] != nullptr && reply->element[2]->str != nullptr)
        {
            //给业务层上报通道上发生的消息
            notify_message_handler_(atoi(reply->element[1]->str), reply->element[2]->str);
        }
        freeReplyObject(reply);
    }

    cerr << ">>>>>>>>>>>>> observer_channel_message quit <<<<<<<<<<<<<<" << endl;
}

void Redis::init_notify_handler(function<void(int, string)> fn)
{
    this->notify_message_handler_ = fn;
}



