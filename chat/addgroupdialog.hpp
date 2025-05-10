#ifndef ADDGROUPDIALOG_H
#define ADDGROUPDIALOG_H

#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <string>

class AddGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddGroupDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("加入群聊");
        setFixedSize(400, 200);

        input_group_name_ = new QLineEdit(this);
        input_introduction_ = new QTextEdit(this);
        input_group_name_->setPlaceholderText("请输入群聊id");
        input_introduction_->setPlaceholderText("介绍一下自己!");

        QPushButton* confirmBtn = new QPushButton("确定", this);
        connect(confirmBtn, &QPushButton::clicked, this, [this]() {
            accept();
        });

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(input_group_name_);
        layout->addWidget(input_introduction_);
        layout->addWidget(confirmBtn);
    }

    int getGroupid()
    {
        return input_group_name_->text().toInt();
    }

    std::string getIntroduction()
    {
        return input_introduction_->toPlainText().toStdString();
    }

private:
    QLineEdit* input_group_name_;
    QTextEdit* input_introduction_;
};

#endif // ADDGROUP_HPP
