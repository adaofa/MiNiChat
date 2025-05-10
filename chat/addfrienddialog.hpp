#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

// AddFriendDialog.h

#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("添加好友");
        setFixedSize(250, 100);

        input_ = new QLineEdit(this);
        input_->setPlaceholderText("请输入好友ID");

        QPushButton* confirmBtn = new QPushButton("确定", this);
        connect(confirmBtn, &QPushButton::clicked, this, [this]() {
            accept();
        });

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(input_);
        layout->addWidget(confirmBtn);
    }

    int getFriendId() const
    {
        return input_->text().toInt();
    }

private:
    QLineEdit* input_;
};


#endif // ADDFRIENDDIALOG_H
