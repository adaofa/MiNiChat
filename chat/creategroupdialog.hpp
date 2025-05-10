#ifndef CREATEGROUPDIALOG_H
#define CREATEGROUPDIALOG_H

#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <string>

class CreateGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateGroupDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("创建群聊");
        setFixedSize(400, 200);

        input_group_name_ = new QLineEdit(this);
        input_group_desc_ = new QTextEdit(this);
        input_group_name_->setPlaceholderText("请输入群聊名称");
        input_group_desc_->setPlaceholderText("介绍一下该群聊吧!");

        QPushButton* confirmBtn = new QPushButton("确定", this);
        connect(confirmBtn, &QPushButton::clicked, this, [this]() {
            accept();
        });

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(input_group_name_);
        layout->addWidget(input_group_desc_);
        layout->addWidget(confirmBtn);
    }

    QString getGroupName()
    {
        return input_group_name_->text();
    }

    QString getGroupDesc()
    {
        return input_group_desc_->toPlainText();
    }

private:
    QLineEdit* input_group_name_;
    QTextEdit* input_group_desc_;
};

#endif // CREATEGROUP_HPP
