#ifndef FLASHEFFECTMANAGER_H
#define FLASHEFFECTMANAGER_H

#include <QObject>
#include <QHash>
#include <QPointer>
#include <QPushButton>
#include <QTimer>
#include <QGraphicsOpacityEffect>

class FlashEffectManager : public QObject
{
    Q_OBJECT
public:
    explicit FlashEffectManager(QObject *parent = nullptr);

    void start_flashing(QWidget *widget, const QColor &flashColor = Qt::yellow, int intervalMs = 500);
    void stop_flashing(QWidget *widget);

private:
    struct FlashData
    {
        QPointer<QTimer> timer;
        QPointer<QGraphicsColorizeEffect> effect;
        bool visible = true;
    };

    QHash<QWidget*, FlashData> flashing_widgets_;
};

#endif // FLASHEFFECTMANAGER_H
