#include "regwidget.h"
#include "ui_regwidget.h"
#include "widget.h"

RegWidget::RegWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegWidget)
{
    ui->setupUi(this);

    //设置背景颜色为流光特效
    background_widget_ = new FlowLightWidget(this);
    background_widget_->lower();
    background_widget_->setGeometry(this->rect());
    background_widget_->show();

    ui->reg_username->setPlaceholderText("请输入您的用户名");
    ui->reg_password->setPlaceholderText("请输入密码");
    ui->reg_password->setEchoMode(QLineEdit::Password);
}

RegWidget::~RegWidget()
{
    delete ui;
}

void RegWidget::on_confirm_clicked()
{
    json js;
    js["msgid"] = REG_MSG;
    js["name"] = ui->reg_username->text().toStdString();
    js["password"] = ui->reg_password->text().toStdString();

    if("" == js["name"] || "" == js[""])
    {
        QMessageBox::warning(this, "注册失败", "未输入用户名或密码!");
        return;
    }

    std::string request = js.dump();

    emit regData(request);
    this->close();
}

void RegWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (background_widget_) {
        background_widget_->setGeometry(0, 0, this->width(), this->height());
    }
}

