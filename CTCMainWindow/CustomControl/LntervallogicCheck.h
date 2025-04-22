#pragma once

#include <QMainWindow>
#include "ui_LntervallogicCheck.h"
#include "./CTCMainWindow/BaseWndClass/CTCMainWindow.h"
#include "StationZoningSelection.h"
class LntervallogicCheck : public QMainWindow
{
	Q_OBJECT

public:
	LntervallogicCheck(QWidget *parent = nullptr);
	~LntervallogicCheck();
	void setTitle(int index);

private:
	StationZoningSelection* m_pSectionOccupation = nullptr;//分区无车占用
	StationZoningSelection* m_pSectionTotalUnlock = nullptr;//区间逻辑状态总解锁
	StationZoningSelection* m_pSectionLogicalCheckOn = nullptr;//逻辑检查开启
	StationZoningSelection* m_pSectionLogicalCheckOff  = nullptr;//逻辑检查关闭
	Ui::LntervallogicCheckClass ui;
};
