#include "StaRoutePlanTH.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace TH {
		StaRoutePlanTH::StaRoutePlanTH(QWidget* parent)
			: StaRoutePlan(parent)
		{
			ui.setupUi(this);
			InitTrainRouteTable();
			ui.frame->layout()->addWidget(m_pTrainRouteTable);
			ui.stationCombo->addItem(Station::MainStation()->getStationName());
		}

		StaRoutePlanTH::~StaRoutePlanTH()
		{
		}

		QVector<Control::TableViewHeadInfo> StaRoutePlanTH::GetTrainRouteTableHeadInfo()
		{
			m_nTrackRow = 2;
			m_nTriggerRow = 4;
			m_nRouteRow = 8;
			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				//{ "顺序", 48 },
				{ "车站", 120 },
				{ "车次", 120 },
				{ "股道", 96 },
				{ "类型", 108 },
				{ "自触", 72 },
				{ "状态", 96 },
				{ "计划时间", 160 },
				{ "方向", 120 },
				{ "进路序列描述", 180 },
				{ "进路来源", 120 },
				{ "进路触发失败时间", 180 },
				{ "", 0}
			};
			return vecHeadInfo;
		}

		QStringList StaRoutePlanTH::GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute)
		{
			if (!pRoute) {
				return QStringList();
			}
			QStringList strList = QStringList()
				<< Station::MainStation()->getStationName()
				<< strTrainNum
				<< pRoute->m_strTrack
				<< (pRoute->m_bArrivaRoute ? "接车" : "发车")
				<< (pRoute->m_vecSubRouteId.size() ? "" : (pRoute->m_bAutoTouch ? "√" : ""))
				<< (pRoute->m_vecSubRouteId.size() ? pRoute->getStateStr() : "")
				<< pRoute->m_tPlanTime.toString("hh:MM")
				<< pRoute->m_strDirection
				<< pRoute->m_strRouteDescrip
				<< "计划生成" 
				<< "";
			return strList;
		}
	}
}
