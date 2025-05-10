#ifndef REDISCONNPOOL_H
#define REDISCONNPOOL_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
#include <hiredis/hiredis.h>

class RedisConnPool {
public:
    static RedisConnPool* getInstance();

    // 初始化连接池，最多 poolSize 个连接
    bool initPool(const std::string& ip, int port, const std::string& password = "", int poolSize = 10);

    redisContext* getConnection();
    void releaseConnection(redisContext* conn);
    void closePool();

private:
    RedisConnPool() = default;
    ~RedisConnPool();

    redisContext* createConnection();

private:
    std::queue<redisContext*> connPool_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::string redis_ip_;
    int redis_port_;
    std::string redis_password_;
    int max_pool_size_ = 10;
    int current_size_ = 0;
    bool stop_ = false;
};

#endif // REDISCONNPOOL_H
