#pragma once

#include <QDialog>
#include "ui_PointReport.h"
#include "StationObject/GlobalStruct.h" 

namespace CTCWindows {
	class PointReport : public QDialog
	{
		Q_OBJECT

	public:
		PointReport(QWidget* parent = nullptr);
		~PointReport();

	public:
		QDateTime getDateTimeByTime(const QString& strTime);
	public slots:
		void AddNewTraffic();

	private:
		Ui::PointReportClass ui;
		Station::StaTrafficLog* pCurTraffic = nullptr;
	};
}
