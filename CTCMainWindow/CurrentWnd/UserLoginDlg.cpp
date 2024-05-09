#include "UserLoginDlg.h"
#include <QDebug>

#pragma execution_character_set("utf-8")

UserLoginDlg::UserLoginDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.loginBtn, &QPushButton::clicked, [=]() {
		if (ui.nameEdit->text() == "" || ui.passwordEdit->text() == "") {
			qDebug() << "ÕËºÅÃÜÂë²»ÄÜÎª¿Õ";
			return;
		}
		emit UserLogin(ui.nameEdit->text(), ui.passwordEdit->text());
		this->done(Accepted);
		this->close();
	});

	connect(ui.cancelBtn, &QPushButton::clicked, [=]() {
		this->done(Rejected);
		this->close();
	});
}

UserLoginDlg::~UserLoginDlg()
{

}
