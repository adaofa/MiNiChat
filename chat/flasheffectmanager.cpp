#include "flasheffectmanager.h"

FlashEffectManager::FlashEffectManager(QObject *parent)
    : QObject(parent)
{
}

void FlashEffectManager::start_flashing(QWidget *widget, const QColor &flashColor, int intervalMs)
{
    if (!widget || flashing_widgets_.contains(widget))
        return;

    // 创建闪烁特效
    auto *effect = new QGraphicsColorizeEffect(widget);
    effect->setColor(flashColor);
    effect->setStrength(0);

    widget->setGraphicsEffect(effect);

    auto *timer = new QTimer(this);
    FlashData data{timer, effect, false};
    flashing_widgets_.insert(widget, data);

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        auto &flashData = flashing_widgets_[widget];
        if (!widget || !effect)
            return;

        flashData.visible = !flashData.visible;
        effect->setStrength(flashData.visible ? 0.6 : 0.0);
    });

    timer->start(intervalMs);
}

void FlashEffectManager::stop_flashing(QWidget *widget)
{
    if (!flashing_widgets_.contains(widget))
        return;

    auto data = flashing_widgets_.value(widget);
    if (data.timer)
    {
        data.timer->stop();
        data.timer->deleteLater();
    }

    if (widget)
        widget->setGraphicsEffect(nullptr);

    flashing_widgets_.remove(widget);
}
