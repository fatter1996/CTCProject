#pragma once

#include <QMainWindow>
#include "ui_EditingInterfaceTKY.h"
#include "Global.h"
class EditingInterfaceTKY : public QMainWindow
{
	Q_OBJECT

public:
	EditingInterfaceTKY(QWidget *parent = nullptr);
	~EditingInterfaceTKY();
	void setLinEditText(QString Text, Station::Device::StaTextSign* TextSign);
private:
	Ui::EditingInterfaceTKYClass ui;
	Station::Device::StaTextSign* pTextSign = nullptr;
};
