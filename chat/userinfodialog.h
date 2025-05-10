#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <memory>
#include "user.hpp"

class UserInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserInfoDialog(std::shared_ptr<User> user, QWidget *parent = nullptr);

private:
    QLabel* user_name_;
    QLabel* user_id_;
    QLabel* state_;
    QLabel* avatar_;
    QPushButton* close_;
};

#endif // USERINFODIALOG_H
