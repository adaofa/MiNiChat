#ifndef HISTORYNOTIFICATION_H
#define HISTORYNOTIFICATION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

enum class NotificationType
{
    FriendRequest,
    FriendResponse,
    GroupRequest,
    GroupResponse,
    GroupDisbanded,
    FriendDeleted
};


class HistoryNotification : public QWidget
{
    Q_OBJECT
public:
    //explicit HistoryNotification(QWidget *parent = nullptr);
    explicit HistoryNotification(NotificationType type, const QString &message,
                        int sender = -1, QWidget *parent = nullptr);

signals:
    void friendRequestResponded(int senderId, bool accepted);
    void groupRequestResponded(int senderId, bool accepted);

private slots:
    void onAcceptClicked();
    void onRejectClicked();

private:
    NotificationType type_;
    int sender_id_;
};

#endif // HISTORYNOTIFICATION_H
