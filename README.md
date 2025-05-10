1--这是一个基于muoduo网络库以及QT6的多人在线聊天软件，基础架构代码参考https://github.com/1289148370/Chatserver。 我在该代码上进行了二次开发，支持更加丰富的功能，如图片发送、群聊的销毁、用户退出当前群聊等。

2--此外我还对服务端的代码进行了优化，在10000个用户同时发送登录请求时，QPS可以稳定在500-600之间（原版代码QPS在150-250之间）。

3--我后续还会对该聊天项目进行优化，包括但不限于音视频聊天、多媒体文件传输等功能。

4--chat是使用QT开发的一个用户端聊天界面，包括用户登录、注册、发送消息、添加好友、创建/销毁/加入/退出群聊等功能；

5--chatServer是基于muduo网络库、Redis以及nginx开发的一个高并发服务器，用于处理用户端发送过来的请求以及数据库与用户之间的数据交互；

运行方法

--在chatServer/build/Desktop_Qt_6_5_3_GCC_64bit-Debug目录中包含chatServer可执行文件，在命令行输入./chatServer 127.0.0.1 6000（或6002），具体端口设置是在nginx.conf文件中设置的，你也可以自行设置端口号，
  服务端没有ui界面；

--同理，在chat/build/Desktop_Qt_6_5_3_GCC_64bit-Debug中同样有chat可执行文件，也是用户进行聊天的程序，在命令行输入./chat 127.0.0.1 8000，同样，具体端口设置也可以在nginx.conf中自行设置；

#nginx tcp loadbalance config

stream {
    upstream MyServer {
        server 127.0.0.1:6000 weight=1 max_fails=3 fail_timeout=30s;
        server 127.0.0.1:6002 weight=1 max_fails=3 fail_timeout=30s;
    } 
    server {
        proxy_connect_timeout 1s;
        #proxy_timeout 3s;
        listen 8000;
        proxy_pass MyServer;
        tcp_nodelay on;
    }
}    

在nginx.conf中加入这一段代码，方式同https://github.com/1289148370/Chatserver。

--如果想重新运行这个项目，需要在redis.cpp以及db.cpp中的密码或用户名修改为自己的，

