#include "StaRoutePlanTKY.h"
#include "Global.h"
#include <QDebug>
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CARS {
		StaRoutePlanTKY::StaRoutePlanTKY(QWidget* parent)
			: StaRoutePlan(parent)
		{
			ui.setupUi(this);
			InitTrainRouteTable();
			ui.widget->layout()->addWidget(m_pTrainRouteTable);
			ui.label_2->setText(Station::MainStation()->getStationName());
			connect(ui.refreshBtn, &QPushButton::clicked, [&]() {});
		}

		StaRoutePlanTKY::~StaRoutePlanTKY()
		{

		}

		QVector<Control::TableViewHeadInfo> StaRoutePlanTKY::GetTrainRouteTableHeadInfo()
		{
			m_nTrackRow = 3;
			m_nTriggerRow = 6;
			m_nRouteRow = 5;
			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				//{ "序号", 80 },
				{ "车次", 120 },
				{ "计划时间", 140 },
				{ "触发条件", 140 },
				{ "股道", 80 },
				{ "进路类型", 120 },
				{ "进路名称", 120 },
				{ "触发方式", 120 },
				{ "进路方向", 120 },
				{ "状态", 120 },
				{ "", 0 }	//占位
			};
			return vecHeadInfo;
		}

		QStringList StaRoutePlanTKY::GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute)
		{
			QStringList strList = QStringList()
				<< strTrainNum
				<< pRoute->m_tPlanTime.toString("yyyy-MM-dd hh:mm")
				<< pRoute->m_tTriggerTime.toString("yyyy-MM-dd hh:mm")
				<< pRoute->m_strTrack
				<< (pRoute->m_bArrivaRoute ? "接车" : "发车")
				<< pRoute->m_strCurRouteDescrip
				<< (pRoute->m_bAutoTouch ? "自动触发" : "人工触发")
				<< pRoute->m_strDirection
				<< pRoute->getStateStr();
			return strList;
		}
	}
}
