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
			connect(ui.refreshBtn, &QPushButton::clicked, [&]() {});
		}

		StaRoutePlanTKY::~StaRoutePlanTKY()
		{

		}

		QVector<Control::TableViewHeadInfo> StaRoutePlanTKY::GetTrainRouteTableHeadInfo()
		{
			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				//{ "���", 80 },
				{ "����", 120 },
				{ "�ƻ�ʱ��", 120 },
				{ "��������", 120 },
				{ "�ɵ�", 80 },
				{ "��·����", 120 },
				{ "��·����", 120 },
				{ "������ʽ", 120 },
				{ "��·����", 120 },
				{ "״̬", 120 },
				{ "", 0 }	//ռλ
			};
			return vecHeadInfo;
		}

		QStringList StaRoutePlanTKY::GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute)
		{
			QStringList strList = QStringList()
				<< strTrainNum
				<< pRoute->m_tPlanTime.toString(Qt::ISODate)
				<< pRoute->m_tTriggerTime.toString(Qt::ISODate)
				<< pRoute->m_strTrack
				<< (pRoute->m_bArrivaRoute ? "�ӳ�" : "����")
				<< pRoute->m_strRouteDescrip
				<< (pRoute->m_bAutoTouch ? "�Զ�����" : "�˹�����")
				<< pRoute->m_strDirection
				<< pRoute->getStateStr();
			return strList;
		}
	}
}
