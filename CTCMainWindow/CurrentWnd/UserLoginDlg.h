#pragma once

#include <QDialog>
#include "ui_UserLoginDlg.h"

class UserLoginDlg : public QDialog
{
	Q_OBJECT

public:
	UserLoginDlg(QWidget *parent = nullptr);
	~UserLoginDlg();

signals:
	void UserLogin(QString, QString);

private:
	Ui::UserLoginDlgClass ui;
};
