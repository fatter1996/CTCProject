#pragma once

#include "BaseWndClass/ModuleWidget/StaRoutePlan.h"
#include "ui_StaRoutePlanTH.h"

namespace CTCWindows {
	namespace TH {
		class StaRoutePlanTH : public BaseWnd::StaRoutePlan
		{
			Q_OBJECT

		public:
			StaRoutePlanTH(QWidget* parent = nullptr);
			~StaRoutePlanTH();

		public:
			QVector<Control::TableViewHeadInfo> GetTrainRouteTableHeadInfo();
			QStringList GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute);

		private:
			Ui::StaRoutePlanTH ui;
		};
	}
}
