#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "widget.h"
#include <QDebug>
#include <QMessageBox>
#include <map>

struct ChatMessage{
    int sender;
    int receiver;
    QString message;
    int msg_type;
};

struct GroupChatMessage{
    QString sender;
    int receiver;
    QString message;
    int msg_type;
};

extern User g_currentUser;
extern vector<User> g_currentUserFriendList;
extern vector<Group> g_currentUserGroupList;
extern vector<string> g_currentUserOfflineMsgList;
extern string getCurrentTime();
map<int, vector<ChatMessage>> ChatHistory;
map<int, vector<GroupChatMessage>> GroupChatHistory;

ChatWidget::ChatWidget(QTcpSocket *socket, Widget* mainWidget, QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatWidget), client_socket_(socket)
{
    ui->setupUi(this);
    this->setLayout(ui->horizontalLayout_3);

    setCurrentTime();

    //流光背景特效
    background_widget_ = new FlowLightWidget(this);
    background_widget_->lower();
    background_widget_->setGeometry(this->rect());
    background_widget_->show();
    background_widget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //消息提示
    flash_manager_ = new FlashEffectManager(this);
    hideLayout(ui->verticalLayout_2);
    ui->upload_file->hide();
    ui->audio_input->hide();
    ui->picture->hide();
    ui->send->hide();
    setLayout(ui->horizontalLayout_3);

    connect(mainWidget, &Widget::chatMessageReceived, this, &ChatWidget::handleChatMessage);

    connect(mainWidget, &Widget::groupChatMessageReceived, this, &ChatWidget::handleGroupMessage);

    connect(mainWidget, &Widget::addFriendRequestReceived, this, &ChatWidget::handleAddFriendRequest);

    connect(mainWidget, &Widget::addFriendResponseReceived, this, &ChatWidget::handleAddFriendResponse);

    connect(mainWidget, &Widget::updateFriendListReceived, this, &ChatWidget::handleUpdateFriendResponse);

    connect(mainWidget, &Widget::createGroupReceived, this, &ChatWidget::handleCreateGroupResponse);

    connect(mainWidget, &Widget::updateGroupListReceived, this, &ChatWidget::handleUpdateGroupResponse);

    connect(mainWidget, &Widget::addGroupRequestReceived, this, &ChatWidget::handleAddGroupRequest);

    connect(mainWidget, &Widget::addGroupResponseReceived, this, &ChatWidget::handleAddGroupResponse);

    connect(ui->user, &QPushButton::clicked, this, [this]() {
        std::shared_ptr<User> userPtr = std::make_shared<User>(g_currentUser);
        this->on_user_clicked(userPtr);
    });

    init_side_bar();

    handleOfflineMessages();
    info_list_handler();
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::handleOfflineMessages()
{
    for (const string& msgStr : g_currentUserOfflineMsgList)
    {
        json js = json::parse(msgStr);

        int msgid = js["msgid"];
        if (msgid == ONE_CHAT_MSG)
        {
            handleChatMessage(js);
        }
        else if (msgid == GROUP_CHAT_MSG)
        {
            handleGroupMessage(js);
        }
        else if (msgid == ADD_FRIEND_REQUEST_MSG)
        {
            handleAddFriendRequest(js);
        }
        else if (msgid == ADD_FRIEND_RESPONSE_MSG)
        {
            handleAddFriendResponse(js);
        }
    }
    g_currentUserOfflineMsgList.clear();
}


void ChatWidget::clear_info_list()
{
    for (int i = 0; i < ui->info_list->count(); ++i)
    {
        QListWidgetItem* item = ui->info_list->item(i);
        QWidget* widget = ui->info_list->itemWidget(item);
        if (widget)
        {
            delete widget;  // 手动释放 InfoWidget，否则 clear 时重复释放会崩溃
        }
    }
    ui->info_list->clear();

    user_item_map_.clear();
    user_widget_map_.clear();
    group_item_map_.clear();
    group_widget_map_.clear();
}

void ChatWidget::info_list_handler()
{
    user_info_handler();
    group_info_handler();
}

void ChatWidget::user_info_handler()
{
    for (User& user : g_currentUserFriendList)
    {
        int userid = user.getId();
        std::shared_ptr<User> user_ptr = std::make_shared<User>(user);
        QString text = QString("%1 %2").arg(userid).arg(QString::fromStdString(user.getName()));

        QPointer<InfoWidget> info_widget = new InfoWidget(user_ptr, text);

        int scrollPos = ui->info_list->verticalScrollBar()->value();

        QListWidgetItem* item = new QListWidgetItem(ui->info_list);
        item->setSizeHint(QSize(ui->info_list->width(), 60));
        item->setHidden(true);

        ui->info_list->addItem(item);
        ui->info_list->setItemWidget(item, info_widget);

        ui->info_list->verticalScrollBar()->setValue(scrollPos);

        user_widget_map_[user_ptr->getId()] = info_widget;
        user_item_map_[user_ptr->getId()] = item;

        connect(info_widget, &InfoWidget::friendclicked, this,
                static_cast<void (ChatWidget::*)(std::shared_ptr<User>)>(&ChatWidget::on_infowidget_clicked));
    }
}

void ChatWidget::group_info_handler()
{
    for (Group& group : g_currentUserGroupList)
    {
        int groupid = group.getId();
        std::shared_ptr<Group> group_ptr = std::make_shared<Group>(group);
        QString text = QString("%1 %2").arg(groupid).arg(QString::fromStdString(group.getName()));

        QPointer<InfoWidget> info_widget = new InfoWidget(group_ptr, text);
        //记录当前滚轮的位置
        int scrollPos = ui->info_list->verticalScrollBar()->value();

        QListWidgetItem* item = new QListWidgetItem(ui->info_list);
        item->setSizeHint(QSize(ui->info_list->width(), 40));
        item->setHidden(true);

        ui->info_list->addItem(item);
        ui->info_list->setItemWidget(item, info_widget);
        //恢复滚轮的位置
        ui->info_list->verticalScrollBar()->setValue(scrollPos);

        group_widget_map_[group_ptr->getId()] = info_widget;
        group_item_map_[group_ptr->getId()] = item;

        connect(info_widget, &InfoWidget::groupclicked, this,
                static_cast<void (ChatWidget::*)(std::shared_ptr<Group>)>(&ChatWidget::on_infowidget_clicked));
    }
}

void ChatWidget::on_groups_clicked()
{
    flash_manager_->stop_flashing(ui->groups);

    for (auto& [_, item] : group_item_map_)
    {
        item->setHidden(false);
    }

    for (auto& [_, item] : user_item_map_)
    {
        item->setHidden(true);
    }
}

void ChatWidget::on_friends_clicked()
{
    flash_manager_->stop_flashing(ui->friends);

    for (auto& [_, item] : user_item_map_)
    {
        item->setHidden(false);
    }

    for (auto& [_, item] : group_item_map_)
    {
        item->setHidden(true);
    }
}

void ChatWidget::on_user_clicked(std::shared_ptr<User> user)
{
    UserInfoDialog* userinfo = new UserInfoDialog(user, this);
    userinfo->exec();
}

void ChatWidget::on_letters_clicked()
{
    flash_manager_->stop_flashing(ui->letters);
    if (side_bar_visible_ == false)
    {
        // 显示侧边栏
        side_bar_anim_->stop();
        side_bar_anim_->setStartValue(side_bar_->pos());
        side_bar_anim_->setEndValue(QPoint(ui->letters->width() + 2, 0));
        side_bar_anim_->start();
        side_bar_visible_ = true;
    }
    else
    {
        // 隐藏侧边栏
        side_bar_anim_->stop();
        side_bar_anim_->setStartValue(side_bar_->pos());
        side_bar_anim_->setEndValue(QPoint(-side_bar_->width(), 0));
        side_bar_anim_->start();
        side_bar_visible_ = false;
    }
}

void ChatWidget::init_side_bar()
{
    side_bar_ = new QWidget(this);
    side_bar_->setFixedWidth(200);
    side_bar_->setStyleSheet("background-color: rgb(0,200,0);");
    side_bar_->move(-side_bar_->width(), 0); // 初始隐藏
    side_bar_->setFixedHeight(this->height());

    side_bar_layout_ = new QVBoxLayout(side_bar_);
    side_bar_layout_->addWidget(new QLabel("历史通知", side_bar_));
    side_bar_layout_->addStretch();

    side_bar_anim_ = new QPropertyAnimation(side_bar_, "pos", this);
    side_bar_anim_->setDuration(300);
}

void ChatWidget::on_loginout_clicked()
{
    emit chatloginout();
    this->close();
}

void ChatWidget::on_infowidget_clicked(std::shared_ptr<Group> group)
{
    ui->chat_type->setText("群聊");
    showLayout(ui->verticalLayout_2);
    setBtnState();
    int groupid = group->getId();
    flash_manager_->stop_flashing(group_widget_map_[groupid]);
    flash_manager_->stop_flashing(ui->groups);
    ui->quit_group->setEnabled(true);
    ui->groupusers->clear();
    ui->chat_msg_menu->clear();
    ui->id->setText(QString::number(group->getId()));
    ui->group_friend_info->setText(QString::fromStdString(group->getName()));
    //记录最近访问的群组信息
    cur_group_.cur_group_id = ui->id->text();
    cur_group_.cur_group_name = ui->group_friend_info->text();


    if (!group) return;

    ui->cur_num_users->setText(QString::number(group->getNumUsers()));

    for (const auto& msg : GroupChatHistory[ui->id->text().toInt()])
    {
        if(msg.sender == QString::fromStdString(g_currentUser.getName()) && msg.receiver == ui->id->text().toInt())
        {
            set_groupchat_format(msg.sender, msg.message, 1, msg.msg_type);
        }
        if(msg.sender != QString::fromStdString(g_currentUser.getName()) && msg.receiver == ui->id->text().toInt())
        {
            set_groupchat_format(msg.sender, msg.message, 0, msg.msg_type);
        }
    }

    int scrollPos = ui->info_list->verticalScrollBar()->value();
    for (GroupUser &user : group->getUsers())
    {
        if(user.getId() == g_currentUser.getId() && user.getRole() != "creator")
        {
            ui->remove_group->hide();
            ui->publish_inform->hide();
        }
        QListWidgetItem* item = new QListWidgetItem(ui->groupusers);
        item->setSizeHint(QSize(ui->groupusers->width(), 30));
        InfoWidget* info_widget = new InfoWidget(group, QString("%1 %2 %3 %4").arg(user.getId())
                        .arg(QString::fromStdString(user.getName()))
                        .arg(QString::fromStdString(user.getRole()))
                        .arg(QString::fromStdString(user.getState())));
        ui->groupusers->setItemWidget(item, info_widget);
    }
    ui->info_list->verticalScrollBar()->setValue(scrollPos);
}

void ChatWidget::on_infowidget_clicked(std::shared_ptr<User> user)
{
    ui->chat_type->setText("好友");
    hideLayout(ui->verticalLayout_2);
    setBtnState();
    flash_manager_->stop_flashing(user_widget_map_[user->getId()]);
    flash_manager_->stop_flashing(ui->friends);
    ui->chat_msg_menu->clear();
    ui->id->setText(QString::number(user->getId()));
    ui->group_friend_info->setText(QString::fromStdString(user->getName()));

    //记录最近访问的好友信息
    cur_friend_.cur_friend_id = ui->id->text();
    cur_friend_.cur_friend_name = ui->group_friend_info->text();

    for (const auto& msg : ChatHistory[g_currentUser.getId()])
    {
        if(msg.sender == g_currentUser.getId() && msg.receiver == ui->id->text().toInt())
        {
            set_onechat_format(msg.message, 1, msg.msg_type);
        }
        else if(msg.receiver == g_currentUser.getId() && msg.sender == ui->id->text().toInt())
        {
            set_onechat_format(msg.message, 0, msg.msg_type);
        }
    }
}

void ChatWidget::on_upload_file_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "All Files (*)");
    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件打开失败";
        return;
    }
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    qint64 fileSize = file.size();

    json js;
    js["msgtype"] = FILE_MSG;
    js["sender"] = g_currentUser.getId();
    js["name"] = g_currentUser.getName();
    js["msg"] = fileName.toStdString();     //只向用户B传输文件的名称以及文件大小
    js["filesize"] = fileSize;
    if(ui->chat_type->text() == "好友")
    {
        js["msgid"] = ONE_CHAT_MSG;
        js["to"] = ui->id->text().toInt();
    }
    else
    {
        js["msgid"] = GROUP_CHAT_MSG;
        js["groupid"] = ui->id->text().toInt();
    }
    QByteArray meta_data = QByteArray::fromStdString(js.dump());
    meta_data.append("\n");

    client_socket_->write(meta_data);
    client_socket_->waitForBytesWritten();

    while (!file.atEnd())                   //发送文件内容
    {
        QByteArray buffer = file.read(4096);
        client_socket_->write(buffer);
        client_socket_->waitForBytesWritten();
    }
    file.close();
}

void ChatWidget::on_picture_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择图片", "", "Images (*.png *.jpg *.jpeg *.bmp *.svg)");
    if (filePath.isEmpty()) return;

    QPixmap originalPixmap(filePath);

    const int maxWidth = 50;
    const int maxHeight = 100;
    QPixmap scaledPixmap = originalPixmap.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 将图片转换为 Base64 字符串
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    scaledPixmap.save(&buffer, "PNG");
    QString base64String = byteArray.toBase64();

    json js;
    js["id"] = g_currentUser.getId();
    js["name"] = g_currentUser.getName();
    js["msg"] = base64String.toStdString();
    js["time"] = getCurrentTime();
    js["msgtype"] = IMAGE_MSG;
    if(ui->chat_type->text() == "好友")
    {
        js["msgid"] = ONE_CHAT_MSG;
        js["to"] = ui->id->text().toInt();
    }
    else
    {
        js["msgid"] = GROUP_CHAT_MSG;
        js["groupid"] = ui->id->text().toInt();
    }

    if (!client_socket_)
    {
        qDebug() << "client_socket is nullptr!";
        return;
    }

    string request = js.dump();
    client_socket_->write(request.c_str(), request.size());
    client_socket_->waitForBytesWritten();

    // 显示在聊天框
    if (ui->chat_type->text() == "好友")
    {
        ChatMessage chatmessage = {
            js["id"],
            js["to"],
            base64String,
            IMAGE_MSG
        };
        ChatHistory[js["id"]].push_back(chatmessage);
        set_onechat_format(base64String, 1, IMAGE_MSG);
    }
    else
    {
        GroupChatMessage groupchatmessage = {
            QString::fromStdString(js["name"]),
            js["groupid"],
            base64String,
            IMAGE_MSG
        };
        GroupChatHistory[js["groupid"]].push_back(groupchatmessage);
        set_groupchat_format(QString::fromStdString(js["name"]), base64String, 1, IMAGE_MSG);
    }
}

void ChatWidget::on_send_clicked()
{
    qDebug() << "消息已经发送！";
    QString msg = ui->edit_msg_menu->toPlainText();
    if (msg.isEmpty()) return;

    if (!client_socket_)
    {
        qDebug() << "Error: client_socket is nullptr!";
        return;
    }
    
    if(ui->chat_type->text() == "好友")
    {
        json js;
        js["msgid"] = ONE_CHAT_MSG;
        js["msgtype"] = TEXT_MSG;
        js["id"] = g_currentUser.getId();
        js["name"] = g_currentUser.getName();
        js["to"] = ui->id->text().toInt();
        js["msg"] = msg.toStdString();
        js["time"] = getCurrentTime();

        ChatMessage chatmessage = {js["id"], js["to"], QString::fromStdString(js["msg"]), js["msgtype"]};
        ChatHistory[js["id"]].push_back(chatmessage);

        string request = js.dump();
        client_socket_->write(request.c_str(), request.size());
        client_socket_->waitForBytesWritten();
        set_onechat_format(msg, 1, TEXT_MSG);
    }
    if(ui->chat_type->text() == "群聊")
    {
        json js;
        js["msgid"] = GROUP_CHAT_MSG;
        js["msgtype"] = TEXT_MSG;
        js["id"] = g_currentUser.getId();
        js["name"] = g_currentUser.getName();
        js["groupid"] = ui->id->text().toInt();
        js["msg"] = msg.toStdString();
        js["time"] = getCurrentTime();

        GroupChatMessage groupchatmessage = {QString::fromStdString(js["name"]), js["groupid"],
                                             QString::fromStdString(js["msg"]), js["msgtype"]};
        GroupChatHistory[js["groupid"]].push_back(groupchatmessage);

        string request = js.dump();
        client_socket_->write(request.c_str(), request.size());
        client_socket_->waitForBytesWritten();
        set_groupchat_format(QString::fromStdString(g_currentUser.getName()), msg, 1, TEXT_MSG);
    }
}

void ChatWidget::on_add_friend_clicked()
{
    AddFriendDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        int friendid = dialog.getFriendId();
        if (friendid <= 0) return;

        json js;
        js["msgid"] = ADD_FRIEND_REQUEST_MSG;
        js["id"] = g_currentUser.getId();
        js["name"] = g_currentUser.getName();
        js["friendid"] = friendid;
        js["time"] = getCurrentTime();

        string request = js.dump();
        client_socket_->write(request.c_str(), request.size());
        client_socket_->waitForBytesWritten();
    }
}

void ChatWidget::handleAddFriendRequest(const json &js)
{
    int sender = js["id"];
    QString sender_name = QString::fromStdString(js["name"]);

    flash_manager_->start_flashing(ui->letters);
    QLabel *label = new QLabel(QString("用户 %1(ID: %2) 请求添加您为好友").arg(sender_name).arg(sender));
    QPushButton *btn_acc = new QPushButton("接受", side_bar_);
    QPushButton *btn_rej = new QPushButton("拒绝", side_bar_);

    side_bar_layout_->addWidget(label);
    side_bar_layout_->addWidget(btn_acc);
    side_bar_layout_->addWidget(btn_rej);
    side_bar_->setLayout(side_bar_layout_);

    json reply;
    reply["msgid"] = ADD_FRIEND_RESPONSE_MSG;
    reply["errno"] = 0;
    reply["from"] = g_currentUser.getId();      //接收方id
    reply["name"] = g_currentUser.getName();    //接收方名字
    reply["to"] = sender;

    // 接受按钮点击
    connect(btn_acc, &QPushButton::clicked, this, [=]() mutable{
        reply["accept"] = true;
        string response = reply.dump();
        client_socket_->write(response.c_str(), response.size());
        client_socket_->waitForBytesWritten();
        label->deleteLater();
        btn_acc->deleteLater();
        btn_rej->deleteLater();
    });

    // 拒绝按钮点击
    connect(btn_rej, &QPushButton::clicked, this, [=]() mutable{
        reply["accept"] = false;
        string response = reply.dump();
        client_socket_->write(response.c_str(), response.size());
        client_socket_->waitForBytesWritten();
        label->deleteLater();
        btn_acc->deleteLater();
        btn_rej->deleteLater();
    });
}

void ChatWidget::handleAddFriendResponse(const json &js)
{
    if(1 == js["errno"])
    {
        QMessageBox::information(this, "添加好友失败", QString(QString::fromStdString(js["errmsg"])));
    }
    else if(0 == js["errno"])
    {
        flash_manager_->start_flashing(ui->letters);
        if(true == js["accept"])
        {
            QLabel *label = new QLabel(QString("用户 %1(ID: %2) 同意了您的好友申请!")
                                           .arg(QString::fromStdString(js["name"])).arg(js["from"].get<int>()));
            side_bar_layout_->addWidget(label);
            side_bar_->setLayout(side_bar_layout_);
        }
        if(false == js["accept"])
        {
            QLabel *label = new QLabel(QString("用户 %1(ID: %2) 拒绝了您的好友申请!")
                                           .arg(QString::fromStdString(js["name"])).arg(js["from"].get<int>()));
            side_bar_layout_->addWidget(label);
            side_bar_->setLayout(side_bar_layout_);
        }
    }
}

void ChatWidget::handleUpdateFriendResponse(const json &js)
{
    qDebug() << g_currentUserFriendList.size();
    if(js.contains("friends"))
    {
        g_currentUserFriendList.clear();

        for (const auto &responsejs : js["friends"])
        {
            User user;
            user.setId(responsejs["id"].get<int>());
            user.setName(responsejs["name"].get<string>());
            user.setState(responsejs["state"].get<string>());
            g_currentUserFriendList.push_back(user);
        }
    }
    qDebug() << g_currentUserFriendList.size();
    clear_info_list();
    info_list_handler();
    on_friends_clicked();
}

void ChatWidget::on_create_group_clicked()
{
    CreateGroupDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString groupname = dialog.getGroupName();
        if (groupname.size() == 0) return;
        QString intro = dialog.getGroupDesc();

        json js;
        js["msgid"] = CREATE_GROUP_MSG;
        js["id"] = g_currentUser.getId();
        js["username"] = g_currentUser.getName();
        js["groupname"] = groupname.toUtf8().constData();
        js["groupdesc"] = intro.toUtf8().constData();

        string request = js.dump();
        client_socket_->write(request.c_str(), request.size());
        client_socket_->waitForBytesWritten();
    }
}

void ChatWidget::handleCreateGroupResponse(const json &js)
{

    if(js["errno"] == 0)
    {
        int groupid = js["groupid"];
        Group new_group;
        new_group.setId(groupid);
        new_group.setName((js["groupname"]));
        new_group.setDesc(js["groupdesc"]);
        new_group.setNumUsers(1);

        GroupUser creator;
        vector<GroupUser> users;

        creator.setId(js["userid"]);
        creator.setName(js["username"]);
        creator.setRole("creator");
        creator.setState("online");
        users.push_back(creator);
        new_group.addUser(creator);

        g_currentUserGroupList.push_back(new_group);
        QMessageBox::information(this, "创建群聊", QString("群聊创建成功,当前群聊id为 %1。").arg(groupid));
        clear_info_list();
        group_info_handler();
    }
    else
    {
        QMessageBox::information(this, "创建群聊", QString("群聊创建失败!"));
    }

}

void ChatWidget::on_addgroup_clicked()
{
    AddGroupDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted)
    {
        int groupid = dialog.getGroupid();
        if (groupid <= 0) return;

        string intro = dialog.getIntroduction();
        json js;
        js["msgid"] = ADD_GROUP_REQUEST_MSG;
        js["id"] = g_currentUser.getId();
        js["username"] = g_currentUser.getName();
        js["groupid"] = groupid;
        //js["introduction"] = intro.empty() ? "" : intro;
        js["introduction"] = intro;

        string request = js.dump();
        client_socket_->write(request.c_str(), request.size());
        client_socket_->waitForBytesWritten();
    }
}

void ChatWidget::handleAddGroupRequest(const json &js)
{
    int sender = js["id"];
    int groupid = js["groupid"];
    QString sender_name = QString::fromStdString(js["username"]);
    flash_manager_->start_flashing(ui->letters);

    QLabel *label = new QLabel(QString("用户 %1(ID: %2) 请求加入群聊 %3").arg(sender_name).arg(sender).arg(groupid));
    QPushButton *btn_acc = new QPushButton("接受", side_bar_);
    QPushButton *btn_rej = new QPushButton("拒绝", side_bar_);

    side_bar_layout_->addWidget(label);
    side_bar_layout_->addWidget(btn_acc);
    side_bar_layout_->addWidget(btn_rej);
    side_bar_->setLayout(side_bar_layout_);

    json reply;
    reply["msgid"] = ADD_GROUP_RESPONSE_MSG;
    reply["errno"] = 0;
    reply["from"] = g_currentUser.getId();
    reply["name"] = g_currentUser.getName();
    reply["groupid"] = groupid;
    reply["to"] = sender;

    connect(btn_acc, &QPushButton::clicked, this, [=]() mutable{
        reply["accept"] = true;
        string response = reply.dump();
        client_socket_->write(response.c_str(), response.size());
        client_socket_->waitForBytesWritten();
        label->deleteLater();
        btn_acc->deleteLater();
        btn_rej->deleteLater();
    });

    connect(btn_rej, &QPushButton::clicked, this, [=]() mutable{
        reply["accept"] = false;
        string response = reply.dump();
        client_socket_->write(response.c_str(), response.size());
        client_socket_->waitForBytesWritten();
        label->deleteLater();
        btn_acc->deleteLater();
        btn_rej->deleteLater();
    });
}

void ChatWidget::handleAddGroupResponse(const json &js)
{
    if(1 == js["errno"])
    {
        QMessageBox::information(this, "加入群聊失败", QString(QString::fromStdString(js["errmsg"])));
    }
    else if(0 == js["errno"])
    {
        flash_manager_->start_flashing(ui->letters);
        if(true == js["accept"])
        {
            QLabel *label = new QLabel(QString("用户 %1(ID: %2) 同意您加入群聊 %3!")
                            .arg(QString::fromStdString(js["name"])).arg(js["from"].get<int>()).arg(js["groupid"].get<int>()));
            side_bar_layout_->addWidget(label);
            side_bar_->setLayout(side_bar_layout_);
        }
        if(false == js["accept"])
        {
            QLabel *label = new QLabel(QString("用户 %1(ID: %2) 拒绝您加入群聊 %3!")
                            .arg(QString::fromStdString(js["name"])).arg(js["from"].get<int>()).arg(js["groupid"].get<int>()));
            side_bar_layout_->addWidget(label);
            side_bar_->setLayout(side_bar_layout_);
        }
    }
}

void ChatWidget::on_remove_group_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "解散群聊", "确定要解散该群聊吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        json js;
        js["msgid"] = REMOVE_GROUP_MSG;
        js["id"] = g_currentUser.getId();
        js["username"] = g_currentUser.getName();
        js["groupid"] = ui->id->text().toInt();
        js["grouprole"] = researchUserRole(ui->id->text().toInt(), g_currentUser.getId());

        string request = js.dump();
        client_socket_->write(request.c_str(), request.size());
        client_socket_->waitForBytesWritten();
    }
}

void ChatWidget::on_quit_group_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "退出群聊", "确定要退出该群聊吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        json js;
        js["msgid"] = QUIT_GROUP_MSG;
        js["id"] = g_currentUser.getId();
        js["username"] = g_currentUser.getName();
        js["groupid"] = ui->id->text().toInt();
        js["grouprole"] = researchUserRole(ui->id->text().toInt(), g_currentUser.getId());

        string request = js.dump();
        client_socket_->write(request.c_str(), request.size());
        client_socket_->waitForBytesWritten();
    }
}

void ChatWidget::on_publish_inform_clicked()
{
    // GroupInformDialog dialog(this);

    // if (dialog.exec() == QDialog::Accepted)
    // {
    //     string title = dialog.getTitle();
    //     string inform = dialog.getInform();
    //     if (title.empty() || inform.empty())
    //     {
    //         QMessageBox::warning(this, "输入消息错误", "标题或通知消息为空!");
    //     }

    //     ui->inform->setText(QString::fromStdString(title) + "\n" + QString::fromStdString(inform));

    //     json js;
    //     js["msgid"] = GROUP_CHAT_MSG;
    //     js["msgtype"] = TEXT_MSG;
    //     js["id"] = g_currentUser.getId();
    //     js["username"] = g_currentUser.getName();
    //     js["groupid"] = ui->id->text().toInt();
    //     js["title"] = title;
    //     js["msg"] = inform;

    //     string request = js.dump();
    //     client_socket_->write(request.c_str(), request.size());
    //     client_socket_->waitForBytesWritten();
    // }
}

void ChatWidget::handleUpdateGroupResponse(const json &js)
{
    if (js.contains("groups"))
    {
        // 初始化
        g_currentUserGroupList.clear();

        for (const auto &grpjs : js["groups"])
        {
            Group group;
            group.setId(grpjs["id"].get<int>());
            group.setName(grpjs["groupname"].get<string>());
            group.setDesc(grpjs["groupdesc"].get<string>());
            group.setNumUsers(grpjs["groupusers"].get<int>());

            for (const auto &js : grpjs["users"])
            {
                GroupUser user;
                user.setId(js["id"].get<int>());
                user.setName(js["name"].get<string>());
                user.setState(js["state"].get<string>());
                user.setRole(js["role"].get<string>());

                group.getUsers().push_back(user);
            }
            g_currentUserGroupList.push_back(group);
        }
    }
    clear_info_list();
    info_list_handler();
    on_groups_clicked();
}

void ChatWidget::handleChatMessage(const json &js)
{
    flash_manager_->start_flashing(ui->friends);
    int sender = js["id"];
    int receiver = js["to"];
    int msg_type = js["msgtype"];
    flash_manager_->start_flashing(user_widget_map_[sender]);

    QString msg = QString::fromStdString(js["msg"]);
    ChatMessage chatmessage = {sender, receiver, msg, msg_type};
    ChatHistory[receiver].push_back(chatmessage);
    if(ui->id->text().toInt() == sender)
        set_onechat_format(msg, 0, msg_type);
}

void ChatWidget::handleGroupMessage(const json &js)
{
    flash_manager_->start_flashing(ui->groups);
    string sender = js["name"];
    int groupid = js["groupid"];
    int msg_type = js["msgtype"];
    flash_manager_->start_flashing(group_widget_map_[groupid]);
    // if(js.contains("title"))        //群通知
    // {
    //     QString title = QString::fromStdString(js["title"]);
    //     QString inform = QString::fromStdString(js["msg"]);
    //     ui->inform->setText(title + "\n" + inform);
    //     return;
    // }
    QString msg = QString::fromStdString(js["msg"]);

    GroupChatMessage groupchatmessage = {QString::fromStdString(sender), groupid, msg, msg_type};
    GroupChatHistory[groupid].push_back(groupchatmessage);

    if(ui->id->text().toInt() == groupid)
        set_groupchat_format(QString::fromStdString(sender), msg, 0, msg_type);
}

void ChatWidget::set_onechat_format(QString msg, bool is_sender, int msg_type)
{
    if (msg_type == TEXT_MSG)
    {
        QTextCursor cursor = ui->chat_msg_menu->textCursor();

        // 设置名字格式
        QTextCharFormat nameFormat;
        nameFormat.setFontWeight(QFont::Bold);
        nameFormat.setForeground(is_sender == 1 ? QColor("blue") : QColor("red"));

        // 设置段落格式（对齐和外边距）
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(is_sender == 1 ? Qt::AlignRight : Qt::AlignLeft);
        //blockFormat.setTopMargin(20);
        blockFormat.setBottomMargin(20);
        blockFormat.setLeftMargin(10);
        blockFormat.setRightMargin(10);
        cursor.insertBlock(blockFormat);  // 插入新段落块

        // 设置消息文本格式
        QTextCharFormat msgFormat;
        msgFormat.setBackground(is_sender == 1 ? QColor(Qt::green) : QColor(Qt::cyan));
        msgFormat.setFontPointSize(12);
        msgFormat.setFont(QFont("华文楷体"));

        // 插入一些空格模拟padding
        QString paddedMsg = "  " + msg + "  ";
        cursor.insertText(paddedMsg, msgFormat);

        ui->chat_msg_menu->setTextCursor(cursor);
        ui->edit_msg_menu->clear();
    }
    if(msg_type == IMAGE_MSG)
    {
        QByteArray imageData = QByteArray::fromBase64(msg.toUtf8());

        QImage image;
        image.loadFromData(imageData);
        if (image.isNull())
        {
            qDebug() << "图片解码失败";
            return;
        }

        QPixmap pixmap = QPixmap::fromImage(image);
        const int maxWidth = 50;
        const int maxHeight = 100;
        QPixmap scaledPixmap = pixmap.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QTextCursor cursor = ui->chat_msg_menu->textCursor();
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(is_sender == 1 ? Qt::AlignRight : Qt::AlignLeft);
        blockFormat.setBottomMargin(20);
        cursor.insertBlock(blockFormat);

        // 插入图片
        QTextImageFormat imageFormat;
        imageFormat.setWidth(scaledPixmap.width());
        imageFormat.setHeight(scaledPixmap.height());
        QString imageName = QString("img_%1").arg(QDateTime::currentMSecsSinceEpoch());
        ui->chat_msg_menu->document()->addResource(QTextDocument::ImageResource,
                                                   QUrl(imageName),
                                                   scaledPixmap);
        imageFormat.setName(imageName);

        cursor.insertImage(imageFormat);
        ui->chat_msg_menu->setTextCursor(cursor);
    }
}

void ChatWidget::set_groupchat_format(QString name, QString msg, bool is_sender, int msg_type)
{
    if (msg_type == TEXT_MSG)
    {
        QTextCursor cursor = ui->chat_msg_menu->textCursor();

        // 设置名字格式
        QTextCharFormat nameFormat;
        nameFormat.setFontWeight(QFont::Bold);
        nameFormat.setForeground(is_sender == 1 ? QColor("blue") : QColor("red"));

        // 设置段落格式
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(is_sender == 1 ? Qt::AlignRight : Qt::AlignLeft);
        blockFormat.setBottomMargin(20);
        cursor.insertBlock(blockFormat);
        cursor.insertText(QString("%1").arg(name), nameFormat);

        // 设置消息正文格式
        QTextCharFormat msgFormat;
        if (is_sender == 1)
            msgFormat.setBackground(QColor(Qt::green));
        else
            msgFormat.setBackground(QColor(Qt::cyan));
        msgFormat.setFontPointSize(12);
        msgFormat.setFont(QFont("微软雅黑"));

        cursor.insertBlock(blockFormat);  // 保持对齐
        cursor.insertText(msg, msgFormat);

        ui->chat_msg_menu->setTextCursor(cursor);
        ui->edit_msg_menu->clear();
    }

    if(msg_type == IMAGE_MSG)
    {
        QByteArray imageData = QByteArray::fromBase64(msg.toUtf8());

        QImage image;
        image.loadFromData(imageData);
        if (image.isNull())
        {
            qDebug() << "图片解码失败";
            return;
        }

        QTextCursor cursor = ui->chat_msg_menu->textCursor();
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(is_sender == 1 ? Qt::AlignRight : Qt::AlignLeft);
        blockFormat.setBottomMargin(5);

        QTextCharFormat nameFormat;
        nameFormat.setFontWeight(QFont::Bold);
        nameFormat.setForeground(is_sender == 1 ? QColor("blue") : QColor("red"));

        cursor.insertBlock(blockFormat);
        cursor.insertText(name, nameFormat);

        // 插入图片
        QPixmap pixmap = QPixmap::fromImage(image);
        const int maxWidth = 150;
        const int maxHeight = 150;
        QPixmap scaledPixmap = pixmap.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QString imageName = QString("img_%1").arg(QDateTime::currentMSecsSinceEpoch());
        ui->chat_msg_menu->document()->addResource(QTextDocument::ImageResource, QUrl(imageName), scaledPixmap);

        QTextImageFormat imageFormat;
        imageFormat.setWidth(scaledPixmap.width());
        imageFormat.setHeight(scaledPixmap.height());
        imageFormat.setName(imageName);

        QTextBlockFormat imageBlockFormat;
        imageBlockFormat.setAlignment(is_sender == 1 ? Qt::AlignRight : Qt::AlignLeft);
        imageBlockFormat.setBottomMargin(20);

        cursor.insertBlock(imageBlockFormat);
        cursor.insertImage(imageFormat);

        ui->chat_msg_menu->setTextCursor(cursor);
    }

}

void ChatWidget::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        && event->modifiers() == Qt::ControlModifier) 
    {
        on_send_clicked();
    }
}

void ChatWidget::hideLayout(QLayout *layout)
{
    if (!layout) return;
    for (int i = 0; i < layout->count(); ++i)
    {
        QLayoutItem* item = layout->itemAt(i);
        if (!item) continue;

        if (QWidget* widget = item->widget())
        {
            widget->hide();
        } else if (QLayout* childLayout = item->layout())
        {
            hideLayout(childLayout);
        }
    }
}

void ChatWidget::showLayout(QLayout* layout)
{
    if (!layout) return;

    for (int i = 0; i < layout->count(); ++i)
    {
        QLayoutItem* item = layout->itemAt(i);
        if (!item) continue;

        if (QWidget* widget = item->widget())
        {
            widget->show();
        }
        else if (QLayout* childLayout = item->layout())
        {
            showLayout(childLayout);
        }
    }
}

void ChatWidget::setBtnState()
{
    ui->upload_file->show();
    ui->audio_input->show();
    ui->picture->show();
    ui->send->show();

    ui->upload_file->setEnabled(true);
    ui->audio_input->setEnabled(true);
    ui->picture->setEnabled(true);
    ui->send->setEnabled(true);
}

string ChatWidget::researchUserRole(int groupid, int userid)
{
    string grouprole;
    for (Group &group : g_currentUserGroupList)
    {
        if (groupid == group.getId())
        {
            for (GroupUser &user : group.getUsers())
            {
                if (userid == user.getId())
                {
                    grouprole = user.getRole();
                    break;
                }
            }
        }
    }
    return grouprole;
}

void ChatWidget::setCurrentTime()
{
    //显示当前时间
    int id = QFontDatabase::addApplicationFont(":/fonts/digital-7 (mono).ttf");
    if (id != -1)
    {
        QStringList fontList = QFontDatabase::applicationFontFamilies(id);
        if (!fontList.isEmpty()) {
            QFont digitalFont(fontList.at(0), 16);
            ui->cur_time_label->setFont(digitalFont);
        } else {
            qDebug() << "字体加载后没有获取到字体名称！";
        }
    }
    else
    {
        qDebug() << "字体加载失败！";
    }
    ui->cur_time_label->setStyleSheet(R"(
    QLabel {
        color: #00FF00;
        background-color: black;
        padding: 0px;
        border-radius: 5px;
    }
    )");

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, [=]() {
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");
        ui->cur_time_label->setText(currentTime);
    });
    timer_->start(1000);
}

void ChatWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if(side_bar_)
    {
        side_bar_->setFixedHeight(this->height());
    }
    if (background_widget_)
    {
        background_widget_->setGeometry(0, 0, this->width(), this->height());
    }
}

