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

		public:
			QVector<Control::TableViewHeadInfo> GetTrainRouteTableHeadInfo() override;
			QStringList GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute) override;
			void UpDataRouteLimits() override;

			QVector<Control::TableViewHeadInfo> GetShuntRouteTableHeadInfo() override;
			QVector<Control::TableViewHeadInfo> GetHookPlanTableHeadInfo() override;
			

		private:
			Ui::StaRoutePlanKSK ui;
		};
	}
}
