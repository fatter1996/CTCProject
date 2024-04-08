/********************************************************************************
** Form generated from reading UI file 'StationViewTKY.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATIONVIEWTKY_H
#define UI_STATIONVIEWTKY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StationViewTKYClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;

    void setupUi(QMainWindow *StationViewTKYClass)
    {
        if (StationViewTKYClass->objectName().isEmpty())
            StationViewTKYClass->setObjectName(QString::fromUtf8("StationViewTKYClass"));
        StationViewTKYClass->resize(1920, 1080);
        StationViewTKYClass->setStyleSheet(QString::fromUtf8("QMenuBar {\n"
"	background-color: rgb(250, 250, 250);\n"
"}\n"
"QMenu {\n"
"	background-color: rgb(250, 250, 250);\n"
"	border: 1px solid rgba(100,100, 100, 100);\n"
"}\n"
"QMenu::item {\n"
"	background-color: rgb(250, 250, 250);\n"
"	padding: 4px 16px 4px 16px;\n"
"	margin: 2px 2px 2px 2px;\n"
"	color: black;\n"
"}\n"
"QMenu::item:!enabled {\n"
"	background-color: rgb(250, 250, 250);\n"
"	padding: 4px 16px 4px 16px;\n"
"	margin: 2px 2px 2px 2px;\n"
"	color: gray;\n"
"}\n"
"QMenu::item:selected {\n"
"	padding: 4px 16px 4px 16px;\n"
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
        centralWidget = new QWidget(StationViewTKYClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        StationViewTKYClass->setCentralWidget(centralWidget);

        retranslateUi(StationViewTKYClass);

        QMetaObject::connectSlotsByName(StationViewTKYClass);
    } // setupUi

    void retranslateUi(QMainWindow *StationViewTKYClass)
    {
        StationViewTKYClass->setWindowTitle(QCoreApplication::translate("StationViewTKYClass", "StationViewTKY", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StationViewTKYClass: public Ui_StationViewTKYClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONVIEWTKY_H
