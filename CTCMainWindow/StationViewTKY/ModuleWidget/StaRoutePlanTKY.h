#pragma once
#include "BaseWndClass/ModuleWidget/StaRoutePlan.h"
#include "ui_StaRoutePlanTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StaRoutePlanTKY : public BaseWnd::StaRoutePlan
		{
			Q_OBJECT
		public:
			StaRoutePlanTKY(QWidget* parent = nullptr);
			~StaRoutePlanTKY();

		public:
			QVector<Control::TableViewHeadInfo> GetTrainRouteTableHeadInfo() override;
			QStringList GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute) override;
			void UpDataRouteLimits() override {};

		private:
			Ui::StaRoutePlanTKY ui;
		};
	}
}
