#include "StaRoutePlanKSK.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CASCO {
		StaRoutePlanKSK::StaRoutePlanKSK(QWidget* parent)
			: StaRoutePlan(parent)
		{
			ui.setupUi(this);
			InitTrainRouteTable();
			ui.trainRouteBox->layout()->addWidget(m_pTrainRouteTable);
			InitShuntRouteTable();
			ui.shuntRouteBox->layout()->addWidget(m_pShuntRouteTable);
			InitHookPlanTable();
			ui.hookPlanBox->layout()->addWidget(m_pHookPlanTable);
			ui.stationCombo->addItem(Station::MainStation()->getStationName());
			
			connect(ui.fontSizeCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, &StaRoutePlanKSK::OnFontSizeChange);
			connect(ui.stationCombo, qOverload<int>(&QComboBox::activated), [&]() {
				QByteArray btResult;
				if (Http::HttpClient::UpdataStaLimits(Station::Limits::RouteLimits, 1, btResult)) {
					Station::MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x70, 0x01, 0x01);
					qDebug() << "�ѻ�ȡ��·Ȩ��";
				}
			});
			connect(ui.limitsButton, &QPushButton::clicked, [&]() {
				QByteArray btResult;
				if (Http::HttpClient::UpdataStaLimits(Station::Limits::RouteLimits, 0, btResult)) {
					Station::MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x70, 0x01, 0x00);
					qDebug() << "��·Ȩ�����ͷ�";
				}
			});
		} 

		StaRoutePlanKSK::~StaRoutePlanKSK()
		{
		}

		QVector<Control::TableViewHeadInfo> StaRoutePlanKSK::GetTrainRouteTableHeadInfo()
		{
			m_nTrackRow = 1;
			m_nTriggerRow = 2;
			m_nRouteRow = 7;
			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				{ "����", 96 },
				{ "�ɵ�", 72 },
				{ "�Դ�", 56 , true},
				{ "����", 96 },
				{ "��ʼ", 72 },
				{ "�ƻ�", 72 },
				{ "״̬", 96 },
				{ "��·����", 120 },
				{ "", 0 }	//ռλ
			};
			return vecHeadInfo;
		}

		QStringList StaRoutePlanKSK::GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute)
		{
			if (!pRoute) {
				return QStringList();
			}
			QStringList strList = QStringList()
				<< strTrainNum
				<< pRoute->m_strTrack
				<< (pRoute->m_vecSubRouteId.size() ?  "" : (pRoute->m_bAutoTouch ? "true" : "false") )
				<< pRoute->m_strDirection
				<< pRoute->m_tTriggerTime.toString("hh:mm")
				<< pRoute->m_tPlanTime.toString("hh:mm")
				<< (pRoute->m_vecSubRouteId.size() ? "" : pRoute->getStateStr())
				<< pRoute->m_strCurRouteDescrip;
			return strList;
		}

		void StaRoutePlanKSK::UpDataRouteLimits()
		{
			if (Station::MainStation()->getStaLimits(Station::Limits::RouteLimits)) {
				ui.trainRouteBox->setTitle("�г���·����");
			}
			else {
				ui.trainRouteBox->setTitle("�г���·���� ֻ��(�����޸�)");
			}
		}

		QVector<Control::TableViewHeadInfo> StaRoutePlanKSK::GetShuntRouteTableHeadInfo()
		{
			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				{ "����", 56 },
				{ "��·������", 80 },
				{ "�Դ�", 64 },
				{ "״̬", 64 },
				{ "����ʱ��", 80 },
				{ "ռ��ʱ��", 80 },
				{ "", 0 }	//ռλ
			};
			return vecHeadInfo;
		}

		QVector<Control::TableViewHeadInfo> StaRoutePlanKSK::GetHookPlanTableHeadInfo()
		{
			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				{ "�ɵ�", 72 },
				{ "����", 72 },
				{ "״̬", 72 },
				{ "����", 96 },
				{ "��ҵʱ��", 96 },
				{ "", 0 }	//ռλ
			};
			return vecHeadInfo;
		}
	}
}
