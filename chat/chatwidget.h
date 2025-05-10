#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QKeyEvent>
#include <QTimer>
#include <unordered_map>
#include <QInputDialog>
#include <QPropertyAnimation>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QBuffer>
#include<QTextBlock>
#include <QScrollBar>
#include <QListWidgetItem>
#include <QResizeEvent>
#include "infowidget.hpp"
#include "userinfodialog.h"
#include "group.hpp"
#include "user.hpp"
#include "groupuser.hpp"
#include "public.hpp"
#include "infowidget.hpp"
#include "flasheffectmanager.h"
#include "json.hpp"
#include "addfrienddialog.hpp"
#include "creategroupdialog.hpp"
#include "addgroupdialog.hpp"
#include "flowlightwidget.h"
#include "groupinformdialog.hpp"
using json = nlohmann::json;

class Widget;

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QTcpSocket *socket, Widget *mainWidget, QWidget *parent = nullptr);
    ~ChatWidget();
    //隐藏右侧边栏布局
    void hideLayout(QLayout* layout);
    //显示右侧边栏布局
    void showLayout(QLayout* layout);
    //改变按钮状态
    void setBtnState();

signals:
    void chatloginout();

private slots:
    void on_groups_clicked();

    void on_friends_clicked();
    //群聊消息栏点击
    void on_infowidget_clicked(std::shared_ptr<Group> group);
    //用户消息栏点击
    void on_infowidget_clicked(std::shared_ptr<User> user);
    //发送消息
    void on_send_clicked();
    //对发送来的一对一聊天消息进行处理
    void handleChatMessage(const json &js);
    //对发送来的群聊消息进行处理
    void handleGroupMessage(const json &js);
    //显示当前用户的基本信息
    void on_user_clicked(std::shared_ptr<User> user);
    //用户退出到登录界面
    void on_loginout_clicked();
    //添加好友
    void on_add_friend_clicked();
    //发送添加好友请求
    void handleAddFriendRequest(const json &js);
    //好友请求响应处理
    void handleAddFriendResponse(const json &js);
    //更新好友列表并反馈到UI界面
    void handleUpdateFriendResponse(const json &js);
    //创建群聊消息响应处理
    void handleCreateGroupResponse(const json &js);
    //更新群聊列表并反馈到UI界面
    void handleUpdateGroupResponse(const json &js);
    //加入群聊请求
    void handleAddGroupRequest(const json &js);
    //群主对加入群聊请求进行响应
    void handleAddGroupResponse(const json &js);
    //其它的回复消息(添加好友是否成功、加入群聊是否成功等)
    void on_letters_clicked();

    void on_create_group_clicked();

    void on_addgroup_clicked();

    void on_picture_clicked();

    void on_upload_file_clicked();

    void on_quit_group_clicked();

    void on_remove_group_clicked();

    void on_publish_inform_clicked();

private:
    //设置一对一聊天的消息显示 当前用户自己发送的消息右对齐，对方用户发送的消息左对齐
    void set_onechat_format(QString msg, bool is_sender, int msg_type);
    //设置群聊的消息显示，格式同上
    void set_groupchat_format(QString name, QString msg, bool is_sender, int msg_type);
    //好友信息展示
    void user_info_handler();
    //群组信息展示
    void group_info_handler();
    //登录时显示用户当前的信息
    void info_list_handler();
    //用户按下ctrl+enter后发送消息
    void keyPressEvent(QKeyEvent *event);
    //处理用户的离线消息
    void handleOfflineMessages();
    //清除消息栏
    void clear_info_list();
    //设置侧边栏，用于接受其它消息
    void init_side_bar();
    //寻找当前用户在特定群聊中的身份
    string researchUserRole(int groupid, int userid);
    //设置时间
    void setCurrentTime();
    //设置OpenGL渲染范围
    void resizeEvent(QResizeEvent *event);

private:
    struct CurrentFriend{
        QString cur_friend_name;
        QString cur_friend_id;
    };

    struct CurrentGroup{
        QString cur_group_name;
        //QString cur_group_state;
        QString cur_group_id;
    };

    CurrentFriend cur_friend_;
    CurrentGroup cur_group_;

    Ui::ChatWidget *ui;
    QTcpSocket* client_socket_;
    FlashEffectManager *flash_manager_;
    InfoWidget *info_widget_;
    QWidget *side_bar_ = nullptr;
    FlowLightWidget * background_widget_;
    QVBoxLayout * side_bar_layout_;
    QPropertyAnimation *side_bar_anim_ = nullptr;
    bool side_bar_visible_ = false;
    QTimer *timer_;

    std::unordered_map<int, InfoWidget*> user_widget_map_;
    std::unordered_map<int, QListWidgetItem*> user_item_map_;
    std::unordered_map<int, InfoWidget*> group_widget_map_;
    std::unordered_map<int, QListWidgetItem*> group_item_map_;
};

#endif // CHATWIDGET_H
