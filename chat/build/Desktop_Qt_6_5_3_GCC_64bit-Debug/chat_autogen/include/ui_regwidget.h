/********************************************************************************
** Form generated from reading UI file 'regwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGWIDGET_H
#define UI_REGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <mybutton.h>

QT_BEGIN_NAMESPACE

class Ui_RegWidget
{
public:
    MyButton *confirm;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *reg_username;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *reg_password;

    void setupUi(QWidget *RegWidget)
    {
        if (RegWidget->objectName().isEmpty())
            RegWidget->setObjectName("RegWidget");
        RegWidget->resize(320, 240);
        RegWidget->setMinimumSize(QSize(320, 240));
        RegWidget->setMaximumSize(QSize(320, 240));
        confirm = new MyButton(RegWidget);
        confirm->setObjectName("confirm");
        confirm->setGeometry(QRect(110, 150, 89, 25));
        confirm->setStyleSheet(QString::fromUtf8(""));
        layoutWidget = new QWidget(RegWidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(40, 50, 223, 62));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(layoutWidget);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        reg_username = new QLineEdit(layoutWidget);
        reg_username->setObjectName("reg_username");

        horizontalLayout->addWidget(reg_username);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");

        horizontalLayout_2->addWidget(label_2);

        reg_password = new QLineEdit(layoutWidget);
        reg_password->setObjectName("reg_password");

        horizontalLayout_2->addWidget(reg_password);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(RegWidget);

        QMetaObject::connectSlotsByName(RegWidget);
    } // setupUi

    void retranslateUi(QWidget *RegWidget)
    {
        RegWidget->setWindowTitle(QCoreApplication::translate("RegWidget", "Form", nullptr));
        confirm->setText(QCoreApplication::translate("RegWidget", "\347\241\256\350\256\244", nullptr));
        label->setText(QCoreApplication::translate("RegWidget", "\347\224\250\346\210\267\345\220\215", nullptr));
        label_2->setText(QCoreApplication::translate("RegWidget", "\345\257\206    \347\240\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegWidget: public Ui_RegWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGWIDGET_H
