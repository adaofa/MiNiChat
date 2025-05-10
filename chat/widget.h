#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QResizeEvent>
#include "json.hpp"
#include "regwidget.h"
#include "chatwidget.h"
#include "flowlightwidget.h"
using namespace std;
using json = nlohmann::json;

class ChatWidget;

QT_BEGIN_NAMESPACE
namespace Ui {
class MiniChat;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QTcpSocket* getSocket() {return client_socket_;}

    //任务处理器，接收到不同的任务时，调用对应的处理函数
    void readTaskHandler();

signals:
    void loginResponseReceived();                   //登录响应信号
    void loginExtraInfoReceived();                  //延迟加载用户信息
    void regResponseReceived();                     //注册响应信号
    void chatMessageReceived(const json &js);       //一对一聊天消息响应信号
    void groupChatMessageReceived(const json &js);  //群聊消息响应信号
    void addFriendMessageReceived(const json &js);  //添加好友响应信号      A->服务端
    void addFriendRequestReceived(const json &js);  //添加好友请求响应信号  服务端->B
    void addFriendResponseReceived(const json &js); //添加好友回复响应信号  B->服务端
    void updateFriendListReceived(const json &js);  //更新好友列表响应信号
    void createGroupReceived(const json &js);       //创建群聊响应信号
    void addGroupRequestReceived(const json &js);   //申请加入群聊请求信号
    void addGroupResponseReceived(const json &js);  //申请加入群聊响应信号
    void updateGroupListReceived(const json &js);   //更新群组列表响应消息

private slots:

    void on_login_clicked();

    void on_reg_clicked();

    //将用户在RegWidget的注册信息返回到Widget中
    void acceptRegData(const std::string &resquest);

    void onChatLogout();

private:
    void processLoginResponse(const json &response);    // 处理登录响应
    void processLoginExtraInfo(const json &response);   // 延迟加载用户信息
    void processRegResponse(const json &respose);       // 处理注册相应
    //设置OpenGL渲染范围
    void resizeEvent(QResizeEvent *event);
    //重写按钮点击事件
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MiniChat *ui;
    QTcpSocket *client_socket_;
    RegWidget *regwindow_;
    string reg_request_;
    ChatWidget *chatwidget_;
    FlowLightWidget *background_widget_;


};
#endif // WIDGET_H
