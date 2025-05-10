#include "mysqlconnpool.h"

MysqlConnPool::MysqlConnPool()
{
    initPool();
}

bool MysqlConnPool::initPool(int poolSize)
{
    for (int i = 0; i < poolSize; ++i)
    {
        auto conn = std::make_shared<MySQL>();
        if (conn->connect())
        {
            connQueue_.push(conn);
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
    while (connQueue_.empty()) {
        cv.wait(lock);
    }

    auto conn = connQueue_.front();
    connQueue_.pop();

    return std::shared_ptr<MySQL>(
        conn.get(),
        [this, conn](MySQL* ptr) {
            std::lock_guard<std::mutex> lock(mtx_);
            connQueue_.push(conn);
            cv.notify_one();
        }
        );
}

void MysqlConnPool::releaseConnection(std::shared_ptr<MySQL> conn) {
    std::lock_guard<std::mutex> lock(mtx_);
    connQueue_.push(conn);
    cv.notify_one();  // 通知等待线程有可用连接了
}
