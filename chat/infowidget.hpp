#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QEnterEvent>
#include <QEvent>
#include <QMouseEvent>

#include "group.hpp"
#include "user.hpp"
#include "groupuser.hpp"
#include "public.hpp"

class InfoWidget : public QWidget {
    Q_OBJECT

public:
    enum Type { GroupType, FriendType };

    InfoWidget(std::shared_ptr<Group> group, const QString& text, QWidget* parent = nullptr)
        : QWidget(parent), type_(GroupType), group_(std::move(group))
    {
        setupLayout(text);
    }

    InfoWidget(std::shared_ptr<User> user, const QString& text, QWidget* parent = nullptr)
        : QWidget(parent), type_(FriendType), friend_(std::move(user))
    {
        setupLayout(text);
    }

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (current_selected_ && current_selected_ != this)
            current_selected_->setNormalStyle();

        setSelectedStyle();
        current_selected_ = this;

        if(type_ == GroupType)
            emit groupclicked(group_);
        else
            emit friendclicked(friend_);

        QWidget::mousePressEvent(event);
    }

signals:
    void groupclicked(std::shared_ptr<Group> group);
    void friendclicked(std::shared_ptr<User> user);

private:
    void setupLayout(const QString& text)
    {
        label_ = new QLabel(text);
        label_->setStyleSheet("background-color: #E6A7A7;border-bottom: 1px solid gray;");

        QFont font = label_->font();
        font.setPointSize(12);
        label_->setFont(font);

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->addWidget(label_);
        layout->setContentsMargins(0, 0, 0, 0);
        setLayout(layout);
    }

    void setSelectedStyle()
    {
        if (label_)
            label_->setStyleSheet("background-color: #E0FFFF;border-bottom: 1px solid gray;");
    }

    void setNormalStyle()
    {
        if (label_)
            label_->setStyleSheet("background-color: #E6A7A7;border-bottom: 1px solid gray;");
    }

private:
    QLabel* label_ = nullptr;
    Type type_;
    std::shared_ptr<Group> group_;
    std::shared_ptr<User> friend_;
    static InfoWidget* current_selected_;
};

#endif // INFOWIDGET_H
