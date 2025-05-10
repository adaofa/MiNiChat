#include "widget.h"
#include "chatwidget.h"
#include "regwidget.h"
#include "infowidget.hpp"
#include <QApplication>

InfoWidget* InfoWidget::current_selected_ = nullptr;

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    qApp->setStyleSheet(R"(
    QMessageBox
    {
        background-color: white;
        color: white;
    }
)");
    Widget w;
    w.show();

    return a.exec();
}




