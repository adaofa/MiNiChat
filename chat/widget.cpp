#include "widget.h"
#include "./ui_widget.h"
#include "mybutton.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonArray>
#include <QTimer>

#include "json.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <functional>
using namespace std;
using json = nlohmann::json;

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <atomic>

#include "group.hpp"
#include "user.hpp"
#include "groupuser.hpp"
#include "public.hpp"
//#include "chatwidget.h"

// 记录当前系统登录的用户信息
User g_currentUser;
//  记录当前登录用户的好友列表信息
vector<User> g_currentUserFriendList;
// 记录当前登录用户的群组列表信息
vector<Group> g_currentUserGroupList;
// 记录当前登录用户的离线消息
vector<string> g_currentUserOfflineMsgList;
// 控制聊天页面程序
bool isMainMenuRunning = false;
// 用于读写线程之间的通信
sem_t rwsem;
// 记录登录状态是否成功
atomic_bool g_isLoginSuccess{false};
//
atomic_bool g_isRegSuccess{false};

// 接收线程
//void readTaskHandler();
// 获取系统时间
string getCurrentTime();
// 主聊天页面程序
void mainMenu(int);
// 显示当前登录成功用户的基本信息
void showCurrentUserData();
// 当有新加入或退出的成员时，更新g_currentUserGroupList
void updateCurUserGroupList(json &responsejs);
// 处理登录事件响应
void doLoginResponse(json &responsejs);
// 处理注册事件响应
void doRegResponse(json &responsejs);

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MiniChat)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout_3);

    //设置背景颜色为流光特效
    background_widget_ = new FlowLightWidget(this);
    background_widget_->lower();
    background_widget_->setGeometry(this->rect());
    background_widget_->show();

    client_socket_ = new QTcpSocket(this);
    connect(client_socket_, &QTcpSocket::readyRead, this, &Widget::readTaskHandler);
    connect(client_socket_, &QTcpSocket::errorOccurred, this, [](QAbstractSocket::SocketError socketError){
        qDebug() << "Socket Error: " << socketError;
    });

    ui->userid->setPlaceholderText("请输入账号");
    ui->password->setPlaceholderText("请输入密码");
    ui->password->setEchoMode(QLineEdit::Password);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_login_clicked()
{
    if (client_socket_->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "已连接至服务器!";
    }
    else
    {
        QStringList args = QCoreApplication::arguments();
        QString ip = args[1];
        uint16_t port = args[2].toInt();

        client_socket_->connectToHost(QHostAddress(ip), port);

        if (client_socket_->waitForConnected(3000)) {
            qDebug() << "Connected to server!";
        } else {
            qDebug() << "Connection failed:" << client_socket_->errorString();
            return;
        }
    }

    int userid = ui->userid->text().toInt();
    QString password = ui->password->text();

    json js;
    js["msgid"] = LOGIN_MSG;
    js["id"] = userid;
    js["password"] = password.toStdString();
    string request = js.dump();

    client_socket_->write(request.c_str());
    client_socket_->waitForBytesWritten();


    QEventLoop loop;
    connect(this, &Widget::loginResponseReceived, &loop, &QEventLoop::quit);
    loop.exec();

    if (g_isLoginSuccess)
    {
        QMessageBox::information(this, "登录成功", "欢迎 " + QString::number(userid) + " 号用户!");
        chatwidget_ = new ChatWidget(client_socket_, this);
        connect(chatwidget_, &ChatWidget::chatloginout, this, &Widget::onChatLogout);
        chatwidget_->show();
        this->hide();
    }
    else
    {
        QMessageBox::warning(this, "登录失败", "账号或密码错误!");
    }
}

void Widget::on_reg_clicked()
{
    if (client_socket_->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "已连接至服务器!";
    }
    else
    {
        QStringList args = QCoreApplication::arguments();
        QString ip = args[1];
        uint16_t port = args[2].toInt();

        // 连接服务器
        client_socket_->connectToHost(QHostAddress(ip), port);

        if (client_socket_->waitForConnected(3000)) {
            qDebug() << "成功连接至服务器!";
        } else {
            qDebug() << "连接失败:" << client_socket_->errorString();
            return;
        }
    }
    //点击注册按钮后只显示注册界面，而不进行消息的接收
    regwindow_ = new RegWidget();
    connect(regwindow_, &RegWidget::regData, this, &Widget::acceptRegData);
    regwindow_->show();

    QEventLoop loop;
    connect(this, &Widget::regResponseReceived, &loop, &QEventLoop::quit);
    loop.exec();
}

void Widget::acceptRegData(const string &request)
{
    //当用户点击了确定按钮后，再将用户信息返回给主界面Widget，并向服务器发送注册请求
    if (client_socket_->state() == QAbstractSocket::ConnectedState)
    {
        client_socket_->write(request.c_str());
        client_socket_->waitForBytesWritten();
        qDebug() << "注册请求已发送!";
    }
    else
    {
        qDebug() << "无法连接至服务器!";
    }
    emit regResponseReceived();
}

void Widget::onChatLogout()
{
    json js;
    js["msgid"] = LOGINOUT_MSG;
    js["id"] = g_currentUser.getId();
    string request = js.dump();

    client_socket_->write(request.c_str());
    client_socket_->waitForBytesWritten();

    this->show();
    ui->userid->clear();
    ui->password->clear();
}

void Widget::readTaskHandler()
{

    QByteArray buffer = client_socket_->readAll();
    json js = json::parse(buffer.data());
    int msgtype = js["msgid"].get<int>();

    if (LOGIN_MSG_ACK == msgtype)
    {
        processLoginResponse(js);
        emit loginResponseReceived();
        qDebug() << "Login response received, emitting signal.";
        return;
    }

    if (REG_MSG_ACK == msgtype)
    {
        processRegResponse(js);
        emit regResponseReceived();
        return;
    }

    if (ONE_CHAT_MSG == msgtype)
    {
        emit chatMessageReceived(js);
    }

    if(ADD_FRIEND_REQUEST_MSG == msgtype)
    {
        emit addFriendRequestReceived(js);  //A->server->B
    }

    if(ADD_FRIEND_RESPONSE_MSG == msgtype)  //B->server->A
    {
        emit addFriendResponseReceived(js);
    }

    if(UPDATE_FRIEND_MSG == msgtype)
    {
        emit updateFriendListReceived(js);
    }

    if(CREATE_GROUP_MSG == msgtype)
    {
        emit createGroupReceived(js);
        return;
    }

    if(ADD_GROUP_REQUEST_MSG == msgtype)
    {
        emit addGroupRequestReceived(js);
    }

    if(ADD_GROUP_RESPONSE_MSG ==msgtype)
    {
        emit addGroupResponseReceived(js);
    }

    if(GROUP_CHAT_MSG == msgtype)
    {
        emit groupChatMessageReceived(js);
    }

    if(UPDATE_GROUP_MSG == msgtype)
    {
        emit updateGroupListReceived(js);
        return;
    }
}

void Widget::processLoginResponse(const json &responsejs)
{
    // 登录响应处理逻辑
    if(responsejs["errno"].get<int>() != 0)
    {
        g_isLoginSuccess = false;
    }
    else
    {
        g_currentUser.setId(responsejs["id"].get<int>());
        g_currentUser.setName(responsejs["name"]);
        g_currentUser.setState(responsejs["state"]);

        // 记录当前用户的好友列表信息
        if (responsejs.contains("friends"))
        {
            // 初始化
            g_currentUserFriendList.clear();

            vector<string> vec = responsejs["friends"];
            for (string &str : vec)
            {
                json js = json::parse(str);
                User user;
                user.setId(js["id"].get<int>());
                user.setName(js["name"]);
                user.setState(js["state"]);
                g_currentUserFriendList.push_back(user);
            }
        }

        // 记录当前用户的群组列表信息
        if (responsejs.contains("groups"))
        {
            // 初始化
            g_currentUserGroupList.clear();

            vector<string> vec1 = responsejs["groups"];
            for (string &groupstr : vec1)
            {
                json grpjs = json::parse(groupstr);
                Group group;
                group.setId(grpjs["id"].get<int>());
                group.setName(grpjs["groupname"]);
                group.setDesc(grpjs["groupdesc"]);

                vector<string> vec2 = grpjs["users"];
                for (string &userstr : vec2)
                {
                    GroupUser user;
                    json js = json::parse(userstr);
                    user.setId(js["id"].get<int>());
                    user.setName(js["name"]);
                    user.setState(js["state"]);
                    user.setRole(js["role"]);
                    group.getUsers().push_back(user);
                }

                g_currentUserGroupList.push_back(group);
            }
        }

        // 显示当前用户的离线消息  个人聊天信息或者群组消息
        if (responsejs.contains("offlinemsg"))
        {
            g_currentUserOfflineMsgList = responsejs["offlinemsg"];
        }

        qDebug() << "[登录延迟加载完成]";
        g_isLoginSuccess = true;
    }
}

void Widget::processLoginExtraInfo(const json &js)
{
    // 好友信息
    if (js.contains("friends"))
    {
        g_currentUserFriendList.clear();
        for (const auto &fstr : js["friends"])
        {
            json fjs = json::parse(fstr.get<string>());
            User user;
            user.setId(fjs["id"].get<int>());
            user.setName(fjs["name"].get<string>());
            user.setState(fjs["state"].get<string>());
            g_currentUserFriendList.push_back(user);
        }
    }

    // 群组信息
    if (js.contains("groups"))
    {
        g_currentUserGroupList.clear();
        for (const auto &grpjs : js["groups"])
        {
            Group group;
            group.setId(grpjs["id"].get<int>());
            group.setName(grpjs["groupname"].get<string>());
            group.setDesc(grpjs["groupdesc"].get<string>());
            group.setNumUsers(grpjs["groupusers"].get<int>());

            for (const auto &usr : grpjs["users"])
            {
                GroupUser user;
                user.setId(usr["id"].get<int>());
                user.setName(usr["name"].get<string>());
                user.setState(usr["state"].get<string>());
                user.setRole(usr["role"].get<string>());
                group.getUsers().push_back(user);
            }
            g_currentUserGroupList.push_back(group);
        }
    }

    // 离线消息
    if (js.contains("offlinemsg"))
    {
        g_currentUserOfflineMsgList = js["offlinemsg"];
        for (string &str : g_currentUserOfflineMsgList)
        {
            json js = json::parse(str);
            if (ONE_CHAT_MSG == js["msgid"].get<int>())
            {
                cout << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                << " said: " << js["msg"].get<string>() << endl;
            }
            else if (GROUP_CHAT_MSG == js["msgid"].get<int>())
            {
                cout << "群消息[" << js["groupid"] << "]: " << js["time"].get<string>() << " [" << js["id"] << "]" << js["name"].get<string>()
                    << " said: " << js["msg"].get<string>() << endl;
            }
        }
    }
    qDebug() << "[登录延迟加载完成]";
}


void Widget::processRegResponse(const json &responsejs)
{
    int userid = responsejs["id"];
    if(1 == responsejs["errno"].get<int>()) // 注册失败
    {
        QMessageBox::warning(this, "注册失败", "该用户名已经存在!");
    }
    else // 注册成功
    {
        QMessageBox::information(this, "注册成功", "您的userid为: " + QString::number(userid) + " !");
    }
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (background_widget_) {
        background_widget_->setGeometry(0, 0, this->width(), this->height());
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        ui->login->click();
    }
    QWidget::keyPressEvent(event);
}

string getCurrentTime()
{
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm ltm = *localtime(&now_c);

    ostringstream oss;
    oss << put_time(&ltm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

