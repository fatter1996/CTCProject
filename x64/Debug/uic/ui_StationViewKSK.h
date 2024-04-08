/********************************************************************************
** Form generated from reading UI file 'StationViewKSK.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATIONVIEWKSK_H
#define UI_STATIONVIEWKSK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StationViewKSKClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QMainWindow *StationViewKSKClass)
    {
        if (StationViewKSKClass->objectName().isEmpty())
            StationViewKSKClass->setObjectName(QString::fromUtf8("StationViewKSKClass"));
        StationViewKSKClass->resize(1920, 1080);
        StationViewKSKClass->setAutoFillBackground(true);
        StationViewKSKClass->setStyleSheet(QString::fromUtf8("QMenuBar {\n"
"	background-color: rgb(250, 250, 250);\n"
"}\n"
"QMenu {\n"
"	background-color: rgb(250, 250, 250);\n"
"}\n"
"QMenu::item {\n"
"	background-color: rgb(250, 250, 250);\n"
"	padding: 4px 16px 4px 24px;\n"
"	margin: 2px 2px 2px 2px;\n"
"	color: black;\n"
"}\n"
"QMenu::item:!enabled {\n"
"	background-color: rgb(250, 250, 250);\n"
"	padding: 4px 16px 4px 24px;\n"
"	margin: 2px 2px 2px 2px;\n"
"	color: gray;\n"
"}\n"
"QMenu::item:selected {\n"
"	padding: 4px 16px 4px 24px;\n"
"	margin: 2px 2px 2px 2px;\n"
"	background-color: rgba(200, 200, 200, 100);\n"
"}\n"
"\n"
"\n"
"QMenu::item:checked\n"
", QMenu::item:unchecked\n"
"{\n"
"	padding-left: 8;\n"
"}\n"
" \n"
"QMenu::indicator\n"
"{\n"
"	width: 16px;\n"
"	height: 16px;\n"
"}\n"
"\n"
"QMenu::separator {\n"
"	margin: 2px 2px 2px 2px;\n"
"	background-color: rgba(180, 180, 180, 160);\n"
"	height: 1px;\n"
"}\n"
""));
        centralWidget = new QWidget(StationViewKSKClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(250, 200, 75, 23));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(260, 390, 75, 23));
        StationViewKSKClass->setCentralWidget(centralWidget);

        retranslateUi(StationViewKSKClass);

        QMetaObject::connectSlotsByName(StationViewKSKClass);
    } // setupUi

    void retranslateUi(QMainWindow *StationViewKSKClass)
    {
        StationViewKSKClass->setWindowTitle(QCoreApplication::translate("StationViewKSKClass", "StationViewKSK", nullptr));
        pushButton->setText(QCoreApplication::translate("StationViewKSKClass", "PushButton", nullptr));
        pushButton_2->setText(QCoreApplication::translate("StationViewKSKClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StationViewKSKClass: public Ui_StationViewKSKClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONVIEWKSK_H
