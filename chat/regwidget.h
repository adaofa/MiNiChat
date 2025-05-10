#ifndef REGWIDGET_H
#define REGWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QResizeEvent>
#include "json.hpp"
#include "public.hpp"
#include "flowlightwidget.h"
#include <QMessageBox>

namespace Ui {
class RegWidget;
}

class RegWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegWidget(QWidget *parent = nullptr);
    ~RegWidget();


signals:
    void regData(const std::string &jsonData);

private slots:
    void on_confirm_clicked();

private:
    void resizeEvent(QResizeEvent *event);

private:
    QTcpSocket *client_socket_;
    Ui::RegWidget *ui;
    FlowLightWidget *background_widget_;
};

#endif // REGWIDGET_H
