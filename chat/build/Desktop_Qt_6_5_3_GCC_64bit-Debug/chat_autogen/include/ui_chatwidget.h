/********************************************************************************
** Form generated from reading UI file 'chatwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWIDGET_H
#define UI_CHATWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <mybutton.h>

QT_BEGIN_NAMESPACE

class Ui_ChatWidget
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *cur_time_label;
    MyButton *user;
    MyButton *friends;
    MyButton *groups;
    MyButton *letters;
    MyButton *loginout;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_5;
    MyButton *add_friend;
    MyButton *create_group;
    MyButton *addgroup;
    QListWidget *info_list;
    QVBoxLayout *verticalLayout_3;
    QLabel *chat_type;
    QLabel *group_friend_info;
    QLabel *id;
    QTextBrowser *chat_msg_menu;
    QHBoxLayout *horizontalLayout;
    MyButton *upload_file;
    QSpacerItem *horizontalSpacer;
    MyButton *audio_input;
    QSpacerItem *horizontalSpacer_2;
    MyButton *picture;
    QSpacerItem *horizontalSpacer_3;
    MyButton *send;
    QTextEdit *edit_msg_menu;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *inform_verticalLayout;
    MyButton *remove_group;
    MyButton *quit_group;
    MyButton *publish_inform;
    QLabel *label_2;
    QLabel *inform;
    QHBoxLayout *members_horizontalLayout;
    QLabel *label_3;
    QLabel *cur_num_users;
    QListWidget *groupusers;

    void setupUi(QWidget *ChatWidget)
    {
        if (ChatWidget->objectName().isEmpty())
            ChatWidget->setObjectName("ChatWidget");
        ChatWidget->resize(920, 490);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ChatWidget->sizePolicy().hasHeightForWidth());
        ChatWidget->setSizePolicy(sizePolicy);
        ChatWidget->setMinimumSize(QSize(0, 0));
        ChatWidget->setMaximumSize(QSize(1920, 1080));
        ChatWidget->setStyleSheet(QString::fromUtf8(""));
        layoutWidget = new QWidget(ChatWidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(0, 0, 920, 490));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(-1, -1, -1, 0);
        cur_time_label = new QLabel(layoutWidget);
        cur_time_label->setObjectName("cur_time_label");
        cur_time_label->setMaximumSize(QSize(200, 80));
        cur_time_label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(cur_time_label);

        user = new MyButton(layoutWidget);
        user->setObjectName("user");
        user->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white\n"
"}\n"
"\n"
""));

        verticalLayout->addWidget(user);

        friends = new MyButton(layoutWidget);
        friends->setObjectName("friends");
        friends->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white\n"
"}"));

        verticalLayout->addWidget(friends);

        groups = new MyButton(layoutWidget);
        groups->setObjectName("groups");
        groups->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white\n"
"}"));

        verticalLayout->addWidget(groups);

        letters = new MyButton(layoutWidget);
        letters->setObjectName("letters");
        letters->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white\n"
"}"));

        verticalLayout->addWidget(letters);

        loginout = new MyButton(layoutWidget);
        loginout->setObjectName("loginout");
        loginout->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white\n"
"}"));

        verticalLayout->addWidget(loginout);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(-1, -1, -1, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(3);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, -1, 0, -1);
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName("verticalLayout_5");
        add_friend = new MyButton(layoutWidget);
        add_friend->setObjectName("add_friend");
        add_friend->setMaximumSize(QSize(200, 30));
        add_friend->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_5->addWidget(add_friend);

        create_group = new MyButton(layoutWidget);
        create_group->setObjectName("create_group");
        create_group->setMaximumSize(QSize(200, 30));
        create_group->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_5->addWidget(create_group);

        addgroup = new MyButton(layoutWidget);
        addgroup->setObjectName("addgroup");
        addgroup->setMaximumSize(QSize(200, 30));
        addgroup->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_5->addWidget(addgroup);


        horizontalLayout_2->addLayout(verticalLayout_5);


        verticalLayout_4->addLayout(horizontalLayout_2);

        info_list = new QListWidget(layoutWidget);
        info_list->setObjectName("info_list");
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(info_list->sizePolicy().hasHeightForWidth());
        info_list->setSizePolicy(sizePolicy1);
        info_list->setMaximumSize(QSize(200, 16777215));
        info_list->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white;\n"
"}"));

        verticalLayout_4->addWidget(info_list);

        verticalLayout_4->setStretch(0, 100);
        verticalLayout_4->setStretch(1, 800);

        horizontalLayout_3->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(-1, -1, 0, 0);
        chat_type = new QLabel(layoutWidget);
        chat_type->setObjectName("chat_type");
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(chat_type->sizePolicy().hasHeightForWidth());
        chat_type->setSizePolicy(sizePolicy2);
        chat_type->setMaximumSize(QSize(16777215, 30));
        chat_type->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"font: 700 10pt \"Chandas\";\n"
"border: 1px solid #00CED1;\n"
"}"));
        chat_type->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(chat_type);

        group_friend_info = new QLabel(layoutWidget);
        group_friend_info->setObjectName("group_friend_info");
        sizePolicy2.setHeightForWidth(group_friend_info->sizePolicy().hasHeightForWidth());
        group_friend_info->setSizePolicy(sizePolicy2);
        group_friend_info->setMaximumSize(QSize(16777215, 30));
        group_friend_info->setStyleSheet(QString::fromUtf8("font: 700 10pt \"Chandas\";"));
        group_friend_info->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(group_friend_info);

        id = new QLabel(layoutWidget);
        id->setObjectName("id");
        sizePolicy2.setHeightForWidth(id->sizePolicy().hasHeightForWidth());
        id->setSizePolicy(sizePolicy2);
        id->setMaximumSize(QSize(16777215, 30));
        id->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"font: 700 10pt \"Chandas\";\n"
"border: 1px solid #00CED1;\n"
"}"));
        id->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(id);

        chat_msg_menu = new QTextBrowser(layoutWidget);
        chat_msg_menu->setObjectName("chat_msg_menu");
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(chat_msg_menu->sizePolicy().hasHeightForWidth());
        chat_msg_menu->setSizePolicy(sizePolicy3);
        chat_msg_menu->setMaximumSize(QSize(16777215, 16777215));
        chat_msg_menu->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white;\n"
"}"));

        verticalLayout_3->addWidget(chat_msg_menu);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        upload_file = new MyButton(layoutWidget);
        upload_file->setObjectName("upload_file");
        upload_file->setEnabled(false);
        upload_file->setMaximumSize(QSize(150, 16777215));
        upload_file->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(upload_file);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        audio_input = new MyButton(layoutWidget);
        audio_input->setObjectName("audio_input");
        audio_input->setEnabled(false);
        audio_input->setMaximumSize(QSize(150, 16777215));
        audio_input->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(audio_input);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        picture = new MyButton(layoutWidget);
        picture->setObjectName("picture");
        picture->setEnabled(false);
        picture->setMaximumSize(QSize(150, 16777215));
        picture->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(picture);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        send = new MyButton(layoutWidget);
        send->setObjectName("send");
        send->setEnabled(false);
        send->setMaximumSize(QSize(150, 16777215));
        send->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(send);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(4, 1);
        horizontalLayout->setStretch(6, 1);

        verticalLayout_3->addLayout(horizontalLayout);

        edit_msg_menu = new QTextEdit(layoutWidget);
        edit_msg_menu->setObjectName("edit_msg_menu");
        edit_msg_menu->setMaximumSize(QSize(16777215, 16777215));
        edit_msg_menu->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white\n"
"}"));

        verticalLayout_3->addWidget(edit_msg_menu);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 1);
        verticalLayout_3->setStretch(2, 1);
        verticalLayout_3->setStretch(3, 12);
        verticalLayout_3->setStretch(4, 1);
        verticalLayout_3->setStretch(5, 6);

        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        inform_verticalLayout = new QVBoxLayout();
        inform_verticalLayout->setSpacing(5);
        inform_verticalLayout->setObjectName("inform_verticalLayout");
        inform_verticalLayout->setContentsMargins(2, -1, 0, -1);
        remove_group = new MyButton(layoutWidget);
        remove_group->setObjectName("remove_group");
        remove_group->setMaximumSize(QSize(220, 30));
        remove_group->setStyleSheet(QString::fromUtf8(""));

        inform_verticalLayout->addWidget(remove_group);

        quit_group = new MyButton(layoutWidget);
        quit_group->setObjectName("quit_group");
        quit_group->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(quit_group->sizePolicy().hasHeightForWidth());
        quit_group->setSizePolicy(sizePolicy4);
        quit_group->setMaximumSize(QSize(220, 30));
        quit_group->setStyleSheet(QString::fromUtf8(""));

        inform_verticalLayout->addWidget(quit_group);

        publish_inform = new MyButton(layoutWidget);
        publish_inform->setObjectName("publish_inform");

        inform_verticalLayout->addWidget(publish_inform);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMaximumSize(QSize(220, 25));
        label_2->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white;\n"
"border: 1px solid #00CED1;\n"
"}"));
        label_2->setAlignment(Qt::AlignCenter);

        inform_verticalLayout->addWidget(label_2);

        inform = new QLabel(layoutWidget);
        inform->setObjectName("inform");
        inform->setMaximumSize(QSize(16777215, 16777215));
        inform->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white;\n"
"border: 1px solid #00CED1;\n"
"}"));
        inform->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        inform_verticalLayout->addWidget(inform);


        verticalLayout_2->addLayout(inform_verticalLayout);

        members_horizontalLayout = new QHBoxLayout();
        members_horizontalLayout->setObjectName("members_horizontalLayout");
        members_horizontalLayout->setContentsMargins(0, -1, 0, -1);
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName("label_3");
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMaximumSize(QSize(60, 16777215));
        label_3->setStyleSheet(QString::fromUtf8(""));

        members_horizontalLayout->addWidget(label_3);

        cur_num_users = new QLabel(layoutWidget);
        cur_num_users->setObjectName("cur_num_users");
        sizePolicy1.setHeightForWidth(cur_num_users->sizePolicy().hasHeightForWidth());
        cur_num_users->setSizePolicy(sizePolicy1);
        cur_num_users->setMaximumSize(QSize(160, 30));
        cur_num_users->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"border:none;\n"
"}"));
        cur_num_users->setAlignment(Qt::AlignCenter);

        members_horizontalLayout->addWidget(cur_num_users);

        members_horizontalLayout->setStretch(1, 2);

        verticalLayout_2->addLayout(members_horizontalLayout);

        groupusers = new QListWidget(layoutWidget);
        groupusers->setObjectName("groupusers");
        sizePolicy1.setHeightForWidth(groupusers->sizePolicy().hasHeightForWidth());
        groupusers->setSizePolicy(sizePolicy1);
        groupusers->setMaximumSize(QSize(220, 215));
        groupusers->setStyleSheet(QString::fromUtf8("QWidget\n"
"{\n"
"background-color: white;\n"
"}"));

        verticalLayout_2->addWidget(groupusers);

        verticalLayout_2->setStretch(0, 11);
        verticalLayout_2->setStretch(1, 1);
        verticalLayout_2->setStretch(2, 6);

        horizontalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 2);
        horizontalLayout_3->setStretch(2, 8);
        horizontalLayout_3->setStretch(3, 3);

        retranslateUi(ChatWidget);

        send->setDefault(false);


        QMetaObject::connectSlotsByName(ChatWidget);
    } // setupUi

    void retranslateUi(QWidget *ChatWidget)
    {
        ChatWidget->setWindowTitle(QCoreApplication::translate("ChatWidget", "ChatMenu", nullptr));
        cur_time_label->setText(QString());
        user->setText(QCoreApplication::translate("ChatWidget", "\347\224\250\346\210\267\344\277\241\346\201\257", nullptr));
        friends->setText(QCoreApplication::translate("ChatWidget", "\345\245\275\345\217\213\345\210\227\350\241\250", nullptr));
        groups->setText(QCoreApplication::translate("ChatWidget", "\347\276\244\347\273\204\345\210\227\350\241\250", nullptr));
        letters->setText(QCoreApplication::translate("ChatWidget", "\345\216\206\345\217\262\344\277\241\346\201\257", nullptr));
        loginout->setText(QCoreApplication::translate("ChatWidget", "\351\200\200\345\207\272\350\264\246\345\217\267", nullptr));
        add_friend->setText(QCoreApplication::translate("ChatWidget", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        create_group->setText(QCoreApplication::translate("ChatWidget", "\345\210\233\345\273\272\347\276\244\350\201\212", nullptr));
        addgroup->setText(QCoreApplication::translate("ChatWidget", "\345\212\240\345\205\245\347\276\244\350\201\212", nullptr));
        chat_type->setText(QString());
        group_friend_info->setText(QString());
        id->setText(QString());
        upload_file->setText(QCoreApplication::translate("ChatWidget", "\344\270\212\344\274\240", nullptr));
        audio_input->setText(QCoreApplication::translate("ChatWidget", "\350\257\255\351\237\263", nullptr));
        picture->setText(QCoreApplication::translate("ChatWidget", "\345\233\276\347\211\207", nullptr));
        send->setText(QCoreApplication::translate("ChatWidget", "\345\217\221\351\200\201", nullptr));
        remove_group->setText(QCoreApplication::translate("ChatWidget", "\350\247\243\346\225\243\347\276\244\350\201\212", nullptr));
        quit_group->setText(QCoreApplication::translate("ChatWidget", "\351\200\200\345\207\272\345\275\223\345\211\215\347\276\244\350\201\212", nullptr));
        publish_inform->setText(QCoreApplication::translate("ChatWidget", "\345\217\221\345\270\203\347\276\244\351\200\232\347\237\245", nullptr));
        label_2->setText(QCoreApplication::translate("ChatWidget", "\347\276\244\351\200\232\347\237\245", nullptr));
        inform->setText(QString());
        label_3->setText(QCoreApplication::translate("ChatWidget", "\347\276\244\350\201\212\344\272\272\346\225\260", nullptr));
        cur_num_users->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ChatWidget: public Ui_ChatWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWIDGET_H
