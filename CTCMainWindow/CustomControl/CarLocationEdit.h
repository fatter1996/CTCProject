#pragma once

#include <QWidget>
#include "ui_CarLocationEdit.h"

class CarLocationEdit : public QWidget
{
	Q_OBJECT

public:
	CarLocationEdit(QWidget *parent = nullptr);
	~CarLocationEdit();
	void InitConnect();
private:
	Ui::CarLocationEditClass ui;
};
