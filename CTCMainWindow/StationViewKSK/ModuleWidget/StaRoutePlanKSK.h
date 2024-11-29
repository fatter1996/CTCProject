#pragma once

#include "BaseWndClass/ModuleWidget/StaRoutePlan.h"
#include "ui_StaRoutePlanKSK.h"
namespace CTCWindows {
	namespace CASCO {
		class StaRoutePlanKSK : public BaseWnd::StaRoutePlan
		{
			Q_OBJECT
		public:
			StaRoutePlanKSK(QWidget* parent = nullptr);
			~StaRoutePlanKSK();

			QVector<Control::TableViewHeadInfo> GetTrainRouteTableHeadInfo();
			QStringList GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute);
			QVector<Control::TableViewHeadInfo> GetShuntRouteTableHeadInfo();
			QVector<Control::TableViewHeadInfo> GetHookPlanTableHeadInfo();

		private:
			Ui::StaRoutePlanKSK ui;
		};
	}
}
