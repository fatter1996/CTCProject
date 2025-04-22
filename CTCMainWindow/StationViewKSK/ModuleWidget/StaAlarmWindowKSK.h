#pragma once

#include <QWidget>
#include "ui_StaAlarmWindowKSK.h"

#include "./CTCMainWindow/BaseWndClass/ModuleWidget/StaAlarmWindow.h"

class StaAlarmWindowKSK : public CTCWindows::BaseWnd::StaAlarmWindow
{
	Q_OBJECT

public:
	StaAlarmWindowKSK(QWidget *parent = nullptr);
	~StaAlarmWindowKSK();

private:
	Ui::StaAlarmWindowKSKClass ui;
};
