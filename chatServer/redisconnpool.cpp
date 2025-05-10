#include "redisconnpool.h"
#include <iostream>

using namespace std;

RedisConnPool::~RedisConnPool()
{
    closePool();
}

RedisConnPool* RedisConnPool::getInstance()
{
    static RedisConnPool instance;
    return &instance;
}

bool RedisConnPool::initPool(const string& ip, int port, const string& password, int poolSize)
{
    redis_ip_ = ip;
    redis_port_ = port;
    redis_password_ = password;
    max_pool_size_ = poolSize;

    for (int i = 0; i < max_pool_size_; ++i)
    {
        redisContext* conn = createConnection();
        if (conn) {
            connPool_.push(conn);
            current_size_++;
        } else {
            cerr << "Failed to create redis connection!" << endl;
            return false;
        }
    }
    return true;
}

redisContext* RedisConnPool::createConnection()
{
    redisContext* conn = redisConnect(redis_ip_.c_str(), redis_port_);
    if (conn == nullptr || conn->err)
    {
        if (conn) redisFree(conn);
        return nullptr;
    }

    if (!redis_password_.empty())
    {
        redisReply* reply = (redisReply*)redisCommand(conn, "AUTH %s", redis_password_.c_str());
        if (!reply || reply->type == REDIS_REPLY_ERROR)
        {
            cerr << "Redis AUTH failed: " << (reply ? reply->str : "null") << endl;
            if (reply) freeReplyObject(reply);
            redisFree(conn);
            return nullptr;
        }
        freeReplyObject(reply);
    }

    return conn;
}

redisContext* RedisConnPool::getConnection()
{
    unique_lock<mutex> lock(mutex_);
    while (connPool_.empty() && !stop_)
    {
        cond_.wait(lock);
    }

    if (stop_) return nullptr;

    redisContext* conn = connPool_.front();
    connPool_.pop();
    return conn;
}

void RedisConnPool::releaseConnection(redisContext* conn)
{
    if (conn == nullptr) return;

    unique_lock<mutex> lock(mutex_);
    connPool_.push(conn);
    cond_.notify_one();
}

void RedisConnPool::closePool()
{
    unique_lock<mutex> lock(mutex_);
    stop_ = true;
    while (!connPool_.empty())
    {
        redisContext* conn = connPool_.front();
        connPool_.pop();
        redisFree(conn);
    }
    current_size_ = 0;
}

