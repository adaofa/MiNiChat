#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QPropertyAnimation>
#include <QEnterEvent>
#include <QPainterPath>

class MyButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(float hoverProgress READ hoverProgress WRITE setHoverProgress)

public:
    enum Shape { Rectangle, RoundedRect, Circle };

    explicit MyButton(QWidget *parent = nullptr);

    void setShape(Shape shape);
    void setGradientStyle(bool enable);
    void setGlassStyle(bool enable);
    void setShadowEffect(bool enable);

    float hoverProgress() const;
    void setHoverProgress(float value);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    Shape m_shape_;         //按钮形状
    bool m_gradient_;       //颜色渐变风格
    bool m_glass_;          //玻璃风格
    bool m_shadow_;         //阴影特效
    float m_hoverProgress_; //控制hover动画进度
    QPropertyAnimation *m_hoverAnimation_;
};

#endif // MYBUTTON_H
