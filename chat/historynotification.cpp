#include "historynotification.h"

HistoryNotification::HistoryNotification(NotificationType type,
    const QString &message, int sender, QWidget *parent)
    : QWidget(parent), type_(type), sender_id_(sender)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(message, this);
    layout->addWidget(label);

    if (type == NotificationType::FriendRequest ||
        type == NotificationType::GroupRequest) {
        QPushButton *btn_accept = new QPushButton("接受", this);
        QPushButton *btn_reject = new QPushButton("拒绝", this);
        connect(btn_accept, &QPushButton::clicked, this, &HistoryNotification::onAcceptClicked);
        connect(btn_reject, &QPushButton::clicked, this, &HistoryNotification::onRejectClicked);
        layout->addWidget(btn_accept);
        layout->addWidget(btn_reject);
    }

    // 其他类型直接显示文字
    setLayout(layout);
}

void HistoryNotification::onAcceptClicked()
{

}

void HistoryNotification::onRejectClicked()
{

}
