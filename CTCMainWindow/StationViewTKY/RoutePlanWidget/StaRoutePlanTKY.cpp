#include "StaRoutePlanTKY.h"
#include "Global.h"
#include <QDebug>

namespace CTCWindows {
	namespace CARS {
		StaRoutePlanTKY::StaRoutePlanTKY(QWidget* parent)
			: StaRoutePlan(parent)
		{
			ui.setupUi(this);
			InitTrainRouteTable();
			ui.widget->layout()->addWidget(m_pTrainRouteTable);
			connect(ui.refreshBtn, &QPushButton::clicked, [&]() {
				Station::StaStagePlan plan;
				plan.m_strPlanNum = "512"; //计划编号
				plan.m_nPlanType = 1; //计划类型 (接发-0x01, 始发-0x02, 终到-0x03, 通过-0x04)
				plan.m_strArrivalTrainNum = "T1234"; //到达车次号
				plan.m_nArrivalTrackCode = 0x37; //到达股道编号
				plan.m_strArrivalTrack = "IG"; //到达股道
				plan.m_nEntrySignalCode = 0x1d; //进站信号机编号
				plan.m_strEntrySignal = "X"; //进站信号机
				plan.m_tArrivalTime = QDateTime::currentDateTime(); //到达时间
				plan.m_strDepartTrainNum = "T1234"; //出发车次号
				plan.m_nDepartTrackCode = 0x37; //出发股道编号
				plan.m_strDepartTrack = "IG"; //出发股道
				plan.m_nExitSignalCode = 0x24; //出站信号机编号
				plan.m_strExitSignal = "SN"; //出站信号机
				plan.m_tDepartTime = QDateTime::currentDateTime(); //出发时间
				Station::MainStation()->AddNewStagePlan(&plan);
			});
		}

		StaRoutePlanTKY::~StaRoutePlanTKY()
		{

		}
	}
}
