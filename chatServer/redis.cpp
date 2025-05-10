#include "redis.hpp"
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <memory>
#include <hiredis/hiredis.h>
#include <iostream>
using namespace std;

Redis::Redis()
    : publish_context_(nullptr), subscribe_context_(nullptr)
{
}

Redis::~Redis()
{
    if (publish_context_ != nullptr)
    {
        redisFree(publish_context_);
    }

    if (subscribe_context_ != nullptr)
    {
        redisFree(subscribe_context_);
    }
}

// bool Redis::connect()
// {
//     publish_context_ = redisConnect("127.0.0.1", 6379);
//     if (publish_context_ == nullptr || publish_context_->err)
//     {
//         cerr << "Failed to connect to Redis for publish!" << endl;
//         return false;
//     }

//     const char* password = "123456";
//     redisReply* reply = (redisReply*)redisCommand(publish_context_, "AUTH %s", password);
//     if (reply == nullptr || reply->type == REDIS_REPLY_ERROR)
//     {
//         cerr << "Failed to authenticate with Redis for publish!" << endl;
//         redisFree(publish_context_);
//         return false;
//     }
//     freeReplyObject(reply);

//     subscribe_context_ = redisConnect("127.0.0.1", 6379);
//     if (subscribe_context_ == nullptr || subscribe_context_->err)
//     {
//         cerr << "Failed to connect to Redis for subscribe!" << endl;
//         redisFree(publish_context_);
//         return false;
//     }

//     reply = (redisReply*)redisCommand(subscribe_context_, "AUTH %s", password);
//     if (reply == nullptr || reply->type == REDIS_REPLY_ERROR)
//     {
//         cerr << "Failed to authenticate with Redis for subscribe!" << endl;
//         redisFree(publish_context_);
//         redisFree(subscribe_context_);
//         return false;
//     }
//     freeReplyObject(reply);

//     // 启动监听订阅消息的线程
//     thread t([this]() {
//         observer_channel_message();
//     });
//     t.detach();

//     cout << "connect redis-server success!" << endl;
//     return true;
// }

bool Redis::connect()
{
    const char* redis_password = "123456";

    // 正确初始化 publish_context_
    publish_context_ = redisConnect("127.0.0.1", 6379);
    if (publish_context_ == nullptr || publish_context_->err)
    {
        cerr << "connect redis failed!" << endl;
        return false;
    }

    redisReply* reply = (redisReply*)redisCommand(publish_context_, "AUTH %s", redis_password);
    if (!reply || reply->type == REDIS_REPLY_ERROR)
    {
        cerr << "publish_context_ AUTH failed: " << (reply ? reply->str : "no reply") << endl;
        if (reply) freeReplyObject(reply);
        redisFree(publish_context_);
        publish_context_ = nullptr;
        return false;
    }
    freeReplyObject(reply);

    // 正确初始化 subscribe_context_
    subscribe_context_ = redisConnect("127.0.0.1", 6379);
    if (subscribe_context_ == nullptr || subscribe_context_->err)
    {
        cerr << "connect redis failed!" << endl;
        return false;
    }

    reply = (redisReply*)redisCommand(subscribe_context_, "AUTH %s", redis_password);
    if (!reply || reply->type == REDIS_REPLY_ERROR)
    {
        cerr << "subscribe_context_ AUTH failed: " << (reply ? reply->str : "no reply") << endl;
        if (reply) freeReplyObject(reply);
        redisFree(subscribe_context_);
        subscribe_context_ = nullptr;
        return false;
    }
    freeReplyObject(reply);

    thread t([this]() {
        observer_channel_message();
    });
    t.detach();

    cout << "connect redis-server success!" << endl;
    return true;
}

bool Redis::publish(int channel, string message)
{
    //redisContext* conn = RedisConnPool::getInstance()->getConnection();
    redisContext* conn = Redis::getPublishContext();
    if (!conn)
    {
        cerr << "publish failed: no available Redis connection" << endl;
        return false;
    }

    redisReply *reply = (redisReply *)redisCommand(conn, "PUBLISH %d %s", channel, message.c_str());
    if(nullptr == reply)
    {
        cerr << "publish command failed!" << endl;
        RedisConnPool::getInstance()->releaseConnection(conn);
        return false;
    }
    freeReplyObject(reply);
    //RedisConnPool::getInstance()->releaseConnection(conn);
    Redis::releasePublishContext(conn);
    return true;
}

// 向redis指定的通道subscribe订阅消息
// bool Redis::subscribe(int channel)
// {
//     // SUBSCRIBE命令本身会造成线程阻塞等待通道里面发生消息，这里只做订阅通道，不接收通道消息
//     // 通道消息的接收专门在observer_channel_message函数中的独立线程中进行
//     // 只负责发送命令，不阻塞接收redis server响应消息，否则和notifyMsg线程抢占响应资源
//     if (REDIS_ERR == redisAppendCommand(this->subscribe_context_, "SUBSCRIBE %d", channel))
//     {
//         cerr << "subscribe command failed!" << endl;
//         return false;
//     }
//     // redisBufferWrite可以循环发送缓冲区，直到缓冲区数据发送完毕（done被置为1）
//     int done = 0;
//     while (!done)
//     {
//         if (REDIS_ERR == redisBufferWrite(this->subscribe_context_, &done))
//         {
//             cerr << "subscribe command failed!" << endl;
//             return false;
//         }
//     }
//     return true;
// }
bool Redis::subscribe(int channel)
{
    {
        lock_guard<mutex> lock(redis_mutex_);
        task_queue_.push({TaskType::SUBSCRIBE, channel});
    }
    task_cv_.notify_one();
    return true;
}

// 向redis指定的通道unsubscribe取消订阅消息
// bool Redis::unsubscribe(int channel)
// {
//     if (REDIS_ERR == redisAppendCommand(this->subscribe_context_, "UNSUBSCRIBE %d", channel))
//     {
//         cerr << "unsubscribe command failed!" << endl;
//         return false;
//     }
//     // redisBufferWrite可以循环发送缓冲区，直到缓冲区数据发送完毕（done被置为1）
//     int done = 0;
//     while (!done)
//     {
//         if (REDIS_ERR == redisBufferWrite(this->subscribe_context_, &done))
//         {
//             cerr << "unsubscribe command failed!" << endl;
//             return false;
//         }
//     }
//     return true;
// }
bool Redis::unsubscribe(int channel)
{
    {
        lock_guard<mutex> lock(redis_mutex_);
        task_queue_.push({TaskType::UNSUBSCRIBE, channel});
    }
    task_cv_.notify_one();
    return true;
}

// 在独立线程中接收订阅通道中的消息
void Redis::observer_channel_message()
{
    redisReply* reply = nullptr;

    while (true)
    {
        {
            unique_lock<mutex> lock(redis_mutex_);
            while (!task_queue_.empty())
            {
                SubscribeTask task = task_queue_.front();
                task_queue_.pop();

                const char* cmd = (task.type == TaskType::SUBSCRIBE) ? "SUBSCRIBE" : "UNSUBSCRIBE";

                if (REDIS_ERR == redisAppendCommand(subscribe_context_, "%s %d", cmd, task.channel))
                {
                    cerr << cmd << " command failed!" << endl;
                    continue;
                }

                int done = 0;
                while (!done)
                {
                    if (REDIS_ERR == redisBufferWrite(subscribe_context_, &done))
                    {
                        cerr << cmd << " buffer write failed!" << endl;
                        break;
                    }
                }
            }
        }

        // 接收订阅消息
        if (REDIS_OK == redisGetReply(this->subscribe_context_, (void**)&reply))
        {
            if (reply && reply->type == REDIS_REPLY_ARRAY && reply->elements == 3)
            {
                if (notify_message_handler_)
                {
                    notify_message_handler_(atoi(reply->element[1]->str), reply->element[2]->str);
                }
            }
            freeReplyObject(reply);
        } else {
            cerr << "observer_channel_message quit (redisGetReply failed)!" << endl;
            break;
        }

        this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

redisContext* Redis::getPublishContext()
{
    unique_lock<mutex> lock(redis_mutex_);
    task_cv_.wait(lock, [this]() { return !publish_pool_.empty(); });

    redisContext* ctx = publish_pool_.front();
    publish_pool_.pop();
    return ctx;
}

void Redis::releasePublishContext(redisContext* ctx)
{
    lock_guard<mutex> lock(redis_mutex_);
    publish_pool_.push(ctx);
    task_cv_.notify_one();
}

// void Redis::set(const std::string& key, const std::string& value, int expireSeconds)
// {
//     redisContext* conn = RedisConnPool::getInstance()->getConnection();
//     if (expireSeconds > 0)
//         redisCommand(conn, "SETEX %s %d %s", key.c_str(), expireSeconds, value.c_str());
//     else
//         redisCommand(conn, "SET %s %s", key.c_str(), value.c_str());
//     RedisConnPool::getInstance()->releaseConnection(conn);
// }

// string Redis::get(const string& key)
// {
//     redisContext* conn = RedisConnPool::getInstance()->getConnection();
//     redisReply* reply = (redisReply*)redisCommand(conn, "GET %s", key.c_str());
//     std::string result;
//     if (reply && reply->type == REDIS_REPLY_STRING)
//     {
//         result = reply->str;
//     }
//     freeReplyObject(reply);
//     RedisConnPool::getInstance()->releaseConnection(conn);
//     return result;
// }

void Redis::init_notify_handler(function<void(int,string)> fn)
{
    this->notify_message_handler_ = fn;
}


