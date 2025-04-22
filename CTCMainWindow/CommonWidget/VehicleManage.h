#pragma once

#include <QWidget>
#include "ui_VehicleManage.h"
#include "CTCMainWindow/CustomControl/CarLocationEdit.h"
class VehicleManage : public QWidget
{
	Q_OBJECT
public:
	VehicleManage(QWidget *parent = nullptr);
	~VehicleManage();
	void timerEvent(QTimerEvent* event);
	void InitConnect();
	void InitRadio();

private:
	int TimerId = 0;
private:
	CarLocationEdit* m_pCarLocation = nullptr;
	Ui::VehicleManageClass ui;
};
