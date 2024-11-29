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

			QVector<Control::TableViewHeadInfo> GetTrainRouteTableHeadInfo();
			QStringList GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute);

		private:
			Ui::StaRoutePlanTKY ui;
		};
	}
}
