/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <mybutton.h>

QT_BEGIN_NAMESPACE

class Ui_MiniChat
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QVBoxLayout *verticalLayout;
    QLineEdit *userid;
    QLineEdit *password;
    QHBoxLayout *horizontalLayout;
    MyButton *login;
    MyButton *reg;
    MyButton *login_out;
    QLabel *title;

    void setupUi(QWidget *MiniChat)
    {
        if (MiniChat->objectName().isEmpty())
            MiniChat->setObjectName("MiniChat");
        MiniChat->resize(590, 410);
        layoutWidget = new QWidget(MiniChat);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(80, 120, 411, 250));
        layoutWidget->setMaximumSize(QSize(500, 250));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label = new QLabel(layoutWidget);
        label->setObjectName("label");
        label->setMaximumSize(QSize(90, 35));

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName("label_2");
        label_2->setMaximumSize(QSize(90, 35));

        verticalLayout_2->addWidget(label_2);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        userid = new QLineEdit(layoutWidget);
        userid->setObjectName("userid");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(userid->sizePolicy().hasHeightForWidth());
        userid->setSizePolicy(sizePolicy);
        userid->setMaximumSize(QSize(400, 35));

        verticalLayout->addWidget(userid);

        password = new QLineEdit(layoutWidget);
        password->setObjectName("password");
        sizePolicy.setHeightForWidth(password->sizePolicy().hasHeightForWidth());
        password->setSizePolicy(sizePolicy);
        password->setMaximumSize(QSize(400, 35));

        verticalLayout->addWidget(password);


        horizontalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2->setStretch(0, 2);
        horizontalLayout_2->setStretch(1, 8);

        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, -1, 0, -1);
        login = new MyButton(layoutWidget);
        login->setObjectName("login");
        sizePolicy.setHeightForWidth(login->sizePolicy().hasHeightForWidth());
        login->setSizePolicy(sizePolicy);
        login->setMaximumSize(QSize(180, 35));
        login->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(login);

        reg = new MyButton(layoutWidget);
        reg->setObjectName("reg");
        sizePolicy.setHeightForWidth(reg->sizePolicy().hasHeightForWidth());
        reg->setSizePolicy(sizePolicy);
        reg->setMaximumSize(QSize(180, 35));
        reg->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(reg);

        login_out = new MyButton(layoutWidget);
        login_out->setObjectName("login_out");
        sizePolicy.setHeightForWidth(login_out->sizePolicy().hasHeightForWidth());
        login_out->setSizePolicy(sizePolicy);
        login_out->setMaximumSize(QSize(180, 35));
        login_out->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(login_out);


        verticalLayout_3->addLayout(horizontalLayout);

        verticalLayout_3->setStretch(0, 1);
        verticalLayout_3->setStretch(1, 2);
        title = new QLabel(MiniChat);
        title->setObjectName("title");
        title->setGeometry(QRect(190, 40, 191, 61));
        title->setAlignment(Qt::AlignCenter);

        retranslateUi(MiniChat);

        QMetaObject::connectSlotsByName(MiniChat);
    } // setupUi

    void retranslateUi(QWidget *MiniChat)
    {
        MiniChat->setWindowTitle(QCoreApplication::translate("MiniChat", "miniCHat", nullptr));
        label->setText(QCoreApplication::translate("MiniChat", "\347\224\250\346\210\267id", nullptr));
        label_2->setText(QCoreApplication::translate("MiniChat", "\345\257\206   \347\240\201", nullptr));
        login->setText(QCoreApplication::translate("MiniChat", "\347\231\273\345\275\225", nullptr));
        reg->setText(QCoreApplication::translate("MiniChat", "\346\263\250\345\206\214", nullptr));
        login_out->setText(QCoreApplication::translate("MiniChat", "\351\200\200\345\207\272", nullptr));
        title->setText(QCoreApplication::translate("MiniChat", "MiNiChat", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MiniChat: public Ui_MiniChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
