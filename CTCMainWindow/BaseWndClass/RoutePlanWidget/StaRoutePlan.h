#pragma once
#include <QWidget>
#include "CTCMainWindow/CustomControl/TableView.h"

namespace CTCWindows {
	namespace BaseWnd {
		class StaRoutePlan : public QWidget
		{
			Q_OBJECT
		public:
			StaRoutePlan(QWidget* parent = nullptr);
			~StaRoutePlan();

		public:
			//列车进路序列
			void InitTrainRouteTable();
			void TrainRouteUpData();
			//调车进路序列
			void InitShuntRouteTable();

			//勾计划
			void InitHookPlanTable();

		protected:
			Control::TableView* m_pTrainRouteTable = nullptr;	//列车进路表格
			Control::TableView* m_pShuntRouteTable = nullptr;	//调车进路表格
			Control::TableView* m_pHookPlanTable = nullptr;		//勾计划表格
		};
	}
}

