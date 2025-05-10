#include "mybutton.h"
#include <QPainter>

MyButton::MyButton(QWidget *parent)
    : QPushButton(parent),
    m_hoverProgress_(0.0f)
{
    setMouseTracking(true);
    setMinimumSize(85, 35);

    // 动画：用于 hover 渐变
    m_hoverAnimation_ = new QPropertyAnimation(this, "hoverProgress", this);
    m_hoverAnimation_->setDuration(200);
    m_hoverAnimation_->setStartValue(0.0);
    m_hoverAnimation_->setEndValue(1.0);

    setStyleSheet("background: transparent; border: none;"); // 透明背景 & 无边框
}

// hover 动画进度 getter/setter
float MyButton::hoverProgress() const
{
    return m_hoverProgress_;
}

void MyButton::setHoverProgress(float value)
{
    m_hoverProgress_ = value;
    update();
}

void MyButton::enterEvent(QEnterEvent *event)
{
    m_hoverAnimation_->setDirection(QAbstractAnimation::Forward);
    m_hoverAnimation_->start();
    QPushButton::enterEvent(event);
}

void MyButton::leaveEvent(QEvent *event)
{
    m_hoverAnimation_->setDirection(QAbstractAnimation::Backward);
    m_hoverAnimation_->start();
    QPushButton::leaveEvent(event);
}

void MyButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect();
    QPainterPath path;
    path.addRoundedRect(rect.adjusted(1, 1, -1, -1), 8, 8);

    painter.setClipPath(path);

    if (m_hoverProgress_ > 0.0f)
    {
        QColor hoverColor = QColor(0, 200, 0); // 绿色
        hoverColor.setAlphaF(m_hoverProgress_); // 渐变出现
        painter.fillPath(path, hoverColor);
    }

    painter.setPen(Qt::black);
    painter.drawText(rect, Qt::AlignCenter, text());
}
