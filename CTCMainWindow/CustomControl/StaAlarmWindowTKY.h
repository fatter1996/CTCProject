#pragma once

#include <QMainWindow>
#include "ui_StaAlarmWindowTKY.h"

#include "./CTCMainWindow/BaseWndClass/ModuleWidget/StaAlarmWindow.h"

class StaAlarmWindowTKY : public CTCWindows::BaseWnd::StaAlarmWindow
{
	Q_OBJECT

public:
	StaAlarmWindowTKY(QWidget *parent = nullptr);
	~StaAlarmWindowTKY();

private:
	Ui::StaAlarmWindowTKYClass ui;
};
