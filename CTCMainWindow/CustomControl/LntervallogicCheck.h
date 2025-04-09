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
	StationZoningSelection* m_pSectionOccupation = nullptr;//�����޳�ռ��
	StationZoningSelection* m_pSectionTotalUnlock = nullptr;//�����߼�״̬�ܽ���
	StationZoningSelection* m_pSectionLogicalCheckOn = nullptr;//�߼���鿪��
	StationZoningSelection* m_pSectionLogicalCheckOff  = nullptr;//�߼����ر�
	Ui::LntervallogicCheckClass ui;
};
