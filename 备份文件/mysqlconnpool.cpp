#include "mysqlconnpool.h"

MysqlConnPool::MysqlConnPool()
{
    initPool();
}

bool MysqlConnPool::initPool(int poolSize)
{
    for (int i = 0; i < poolSize; ++i)
    {
        std::shared_ptr<MySQL> conn(new MySQL());
        if (conn->connect())
        {
            connQueue.push(conn);
        }
        else
        {
            std::cerr << "MySQL connection pool initialization failed!" << std::endl;
            return false;
        }
    }
    return true;
}

MysqlConnPool* MysqlConnPool::getInstance()
{
    static MysqlConnPool instance;
    return &instance;
}

std::shared_ptr<MySQL> MysqlConnPool::getConnection() {
    std::unique_lock<std::mutex> lock(mtx_);
    while (connQueue.empty()) {
        cv.wait(lock);
    }

    auto conn = connQueue.front();
    connQueue.pop();

    return conn;
}

void MysqlConnPool::releaseConnection(std::shared_ptr<MySQL> conn) {
    std::lock_guard<std::mutex> lock(mtx_);
    connQueue.push(conn);
    cv.notify_one();  // 通知等待线程有可用连接了
}
