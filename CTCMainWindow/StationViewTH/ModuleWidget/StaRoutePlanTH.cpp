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
				//{ "˳��", 48 },
				{ "��վ", 120 },
				{ "����", 120 },
				{ "�ɵ�", 96 },
				{ "����", 108 },
				{ "�Դ�", 72 },
				{ "״̬", 96 },
				{ "�ƻ�ʱ��", 160 },
				{ "����", 120 },
				{ "��·��������", 180 },
				{ "��·��Դ", 120 },
				{ "��·����ʧ��ʱ��", 180 },
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
				<< (pRoute->m_bArrivaRoute ? "�ӳ�" : "����")
				<< (pRoute->m_vecSubRouteId.size() ? "" : (pRoute->m_bAutoTouch ? "��" : ""))
				<< (pRoute->m_vecSubRouteId.size() ? pRoute->getStateStr() : "")
				<< pRoute->m_tPlanTime.toString("hh:MM")
				<< pRoute->m_strDirection
				<< pRoute->m_strRouteDescrip
				<< "�ƻ�����" 
				<< "";
			return strList;
		}
	}
}
