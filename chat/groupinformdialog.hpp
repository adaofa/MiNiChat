#ifndef GROUPINFORMDIALOG_H
#define GROUPINFORMDIALOG_H
#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <string>

class GroupInformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupInformDialog(QWidget* parent = nullptr)
        : QDialog(parent)
    {
        setWindowTitle("群通知");
        setFixedSize(400, 200);

        title_ = new QLineEdit(this);
        inform_ = new QTextEdit(this);
        title_->setPlaceholderText("请输入标题");
        inform_->setPlaceholderText("详细内容");

        QPushButton* confirmBtn = new QPushButton("确定", this);
        connect(confirmBtn, &QPushButton::clicked, this, [this]() {
            accept();
        });

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(title_);
        layout->addWidget(inform_);
        layout->addWidget(confirmBtn);
    }

    std::string getTitle()
    {
        return title_->text().toStdString();
    }

    std::string getInform()
    {
        return inform_->toPlainText().toStdString();
    }

private:
    QLineEdit* title_;
    QTextEdit* inform_;
};
#endif // GROUPINFORMDIALOG_H
