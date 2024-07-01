#include "StaRoutePlan.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace BaseWnd {
		StaRoutePlan::StaRoutePlan(QWidget* parent)
		{
			this->setMinimumHeight(164);
			QSizePolicy policy = this->sizePolicy();
			policy.setVerticalPolicy(QSizePolicy::MinimumExpanding); // 允许在垂直方向上缩小到最小大小，但也可以扩展
			this->setSizePolicy(policy);
		}

		StaRoutePlan::~StaRoutePlan()
		{
			
		}

		void StaRoutePlan::InitTrainRouteTable()
		{
			m_pTrainRouteTable = new Control::TableView(this);
			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				//{ "序号", 80 },
				{ "车次", 120 },
				{ "计划时间", 120 },
				{ "触发条件", 120 },
				{ "股道", 80 },
				{ "进路类型", 120 },
				{ "进路名称", 120 },
				{ "触发方式", 120 },
				{ "进路方向", 120 },
				{ "状态", 120 },
				{ "", 0 }	//占位
			};
			m_pTrainRouteTable->SetHeadData(vecHeadInfo, HHead);
		}

		void StaRoutePlan::TrainRouteUpData()
		{
			QVector<QStringList> vecTableData;
			Station::StaTrain* pTrain = nullptr;
			for (Station::StaTrainRoute* pRoute : Station::MainStation()->TrainRouteList()) {
				pTrain = Station::MainStation()->getStaTrainById(pRoute->m_nTrainId);
				if (pTrain) {
					vecTableData.append(QStringList()
						<< (pRoute->m_bArrivaRoute ? pTrain->m_strArrivalTrainNum : pTrain->m_strDepartTrainNum)
						<< pRoute->m_tPlanTime.toString(Qt::ISODate)
						<< pRoute->m_tTriggerTime.toString(Qt::ISODate)
						<< pRoute->m_strTrack
						<< (pRoute->m_bArrivaRoute ? "接车" : "发车")
						<< pRoute->m_strRouteDescrip
						<< (pRoute->m_nAutoTouch ? "自动触发" : "人工触发")
						<< pRoute->m_strDirection
						<< pRoute->getStateStr());
				}
			}
			m_pTrainRouteTable->ResetTableRows(vecTableData);
		}

		void StaRoutePlan::InitShuntRouteTable()
		{
			m_pShuntRouteTable = new Control::TableView(this);
		}

		void StaRoutePlan::InitHookPlanTable()
		{
			m_pHookPlanTable = new Control::TableView(this);
		}
	}
}