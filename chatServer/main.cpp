#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include<signal.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "redisconnpool.h"
using namespace std;

//处理服务器Ctrl+C结束后，重置user的状态信息
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char **argv)
{

    if(argc < 3)
    {
        cerr << "command invalid! example ./ChatServer 127.0.0.1 6000" << endl;
        exit(-1);
    }

    RedisConnPool::getInstance()->initPool("127.0.0.1", 6379, "123456", 15);
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);
    signal(SIGINT, resetHandler);

    EventLoop loop;
    InetAddress addr(ip, port);
    ChatServer server(&loop, addr, "chatServer");

    server.start();
    loop.loop();
    return 0;
}
