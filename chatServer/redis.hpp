#ifndef REDIS_H
#define REDIS_H
#include<hiredis/hiredis.h>
#include<thread>
#include<string>
#include<functional>
#include <unordered_map>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "redisconnpool.h"

using namespace std;

class Redis
{
public:
    Redis();
    ~Redis();

    //连接redis服务器
    bool connect();

    //向redis指定的通道channel发送消息
    bool publish(int channel, string message);

    //向redis指定的通道channel订阅消息
    bool subscribe(int channel);

    //向redis指定的通道unsubscribe取消订阅消息
    bool unsubscribe(int channel);

    //初始化向业务层上报通道消息的回调对象
    void init_notify_handler(function<void(int, string)> fn);

    //设置当前用户的状态
    //void set(const std::string& key, const std::string& value, int expireSeconds = 0);

    //获取当前用户的状态
    //string get(const string& key);

    //搜寻用户状态
    //vector<string> getKeysWithPrefix(const string& prefix);

private:
    //在独立线程中接收订阅通道中的消息
    void observer_channel_message();

    redisContext* getPublishContext();

    void releasePublishContext(redisContext* ctx);

    // 枚举类型表示任务类型
    enum class TaskType { SUBSCRIBE, UNSUBSCRIBE };

    // 定义任务结构体
    struct SubscribeTask{
        TaskType type;
        int channel;
    };

private:
    //hiredis同步上下文对象，负责publish消息
    redisContext *publish_context_;

    //hiredis同步上下文对象，负责subsrcibe消息
    redisContext *subscribe_context_;

    //回调操作，收到订阅信息，给service层上报
    function<void(int, string)> notify_message_handler_;

    //任务队列
    std::queue<SubscribeTask> task_queue_;

    //连接池队列
    std::queue<redisContext*> publish_pool_;

    //队列互斥锁
    mutable std::mutex redis_mutex_;

    //条件变量
    std::condition_variable task_cv_;
};

#endif


