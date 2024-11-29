#pragma once

#include <QWidget>
#include "ui_TrafficMsgLog.h"

namespace CTCWindows {
	class TrafficMsgLog : public QWidget
	{
		Q_OBJECT

	public:
		TrafficMsgLog(QWidget* parent = nullptr);
		~TrafficMsgLog();

	public:
		void AddDispatchDeskMsg(QString strText);
		void AddAdjacentStationMsg(QString strText);

	private:
		Ui::TrafficMsgLogClass ui;
	};
}
