#pragma once

#include <QWidget>
#include "ui_QuickReportKSK.h"
#include "Global.h"
class QuickReportKSK : public QWidget
{
	Q_OBJECT

public:
	QuickReportKSK(Station::StaTrafficLog* m_pCurTrafficLog,QWidget *parent = nullptr);
	~QuickReportKSK();
	QVector<CTCWindows::Control::TableViewHeadInfo> GetTrainRouteTableHeadInfo();
private:
	int m_nTrackRow = -1;
	int m_nTriggerRow = -1;
	int m_nRouteRow = -1;

	CTCWindows::Control::TableView* m_pTrainList = nullptr;
	Ui::QuickReportKSKClass ui;
};
