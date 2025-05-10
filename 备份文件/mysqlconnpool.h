// MysqlConnPool.h
#pragma once
#include "db.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>

class MysqlConnPool {
public:
    static MysqlConnPool* getInstance();

    // 获取一个可用的数据库连接
    std::shared_ptr<MySQL> getConnection();

    // 归还连接
    void releaseConnection(std::shared_ptr<MySQL> conn);

    bool initPool(int poolSize = 10);
private:
    MysqlConnPool();

    std::queue<std::shared_ptr<MySQL>> connQueue;
    std::mutex mtx_;
    std::condition_variable cv;
    const int maxPoolSize = 10; // 设置连接池最大连接数
};
