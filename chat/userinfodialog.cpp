#include "userinfodialog.h"
#include <QHBoxLayout>
#include <QPixmap>

UserInfoDialog::UserInfoDialog(std::shared_ptr<User> user, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("用户详细信息");
    setFixedSize(300, 200);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // avatarLabel = new QLabel(this);
    // QPixmap avatar(":/resources/default_avatar.png"); // 默认头像路径
    // avatarLabel->setPixmap(avatar.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // avatarLabel->setAlignment(Qt::AlignCenter);

    user_name_ = new QLabel("用户名: " + QString::fromStdString(user->getName()), this);
    user_id_ = new QLabel("用户ID: " + QString::number(user->getId()), this);
    state_ = new QLabel("状态: " + QString::fromStdString(user->getState()), this);

    user_name_->setAlignment(Qt::AlignCenter);
    user_id_->setAlignment(Qt::AlignCenter);
    state_->setAlignment(Qt::AlignCenter);

    close_ = new QPushButton("关闭", this);
    connect(close_, &QPushButton::clicked, this, &QDialog::accept);

    // 布局
    //mainLayout->addWidget(avatar_);
    mainLayout->addWidget(user_name_);
    mainLayout->addWidget(user_id_);
    mainLayout->addWidget(state_);
    mainLayout->addStretch();
    mainLayout->addWidget(close_);
}
