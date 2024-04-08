/********************************************************************************
** Form generated from reading UI file 'StationCtrlDispTKY.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATIONCTRLDISPTKY_H
#define UI_STATIONCTRLDISPTKY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StationCtrlDispTKYClass
{
public:
    QHBoxLayout *horizontalLayout;
    QScrollArea *scrollArea;
    QWidget *widget;

    void setupUi(QWidget *StationCtrlDispTKYClass)
    {
        if (StationCtrlDispTKYClass->objectName().isEmpty())
            StationCtrlDispTKYClass->setObjectName(QString::fromUtf8("StationCtrlDispTKYClass"));
        StationCtrlDispTKYClass->resize(1081, 751);
        horizontalLayout = new QHBoxLayout(StationCtrlDispTKYClass);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(StationCtrlDispTKYClass);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 1079, 749));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        scrollArea->setWidget(widget);

        horizontalLayout->addWidget(scrollArea);


        retranslateUi(StationCtrlDispTKYClass);

        QMetaObject::connectSlotsByName(StationCtrlDispTKYClass);
    } // setupUi

    void retranslateUi(QWidget *StationCtrlDispTKYClass)
    {
        StationCtrlDispTKYClass->setWindowTitle(QCoreApplication::translate("StationCtrlDispTKYClass", "StationCtrlDispTKY", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StationCtrlDispTKYClass: public Ui_StationCtrlDispTKYClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATIONCTRLDISPTKY_H
