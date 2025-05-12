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
					qDebug() << "已获取进路权限";
				}
			});
			connect(ui.limitsButton, &QPushButton::clicked, [&]() {
				QByteArray btResult;
				if (Http::HttpClient::UpdataStaLimits(Station::Limits::RouteLimits, 0, btResult)) {
					Station::MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x70, 0x01, 0x00);
					qDebug() << "进路权限已释放";
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
				{ "车次", 96 },
				{ "股道", 72 },
				{ "自触", 56 , true},
				{ "方向", 96 },
				{ "开始", 72 },
				{ "计划", 72 },
				{ "状态", 96 },
				{ "进路描述", 120 },
				{ "", 0 }	//占位
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
				ui.trainRouteBox->setTitle("列车进路序列");
			}
			else {
				ui.trainRouteBox->setTitle("列车进路序列 只读(不可修改)");
			}
		}

		QVector<Control::TableViewHeadInfo> StaRoutePlanKSK::GetShuntRouteTableHeadInfo()
		{
			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				{ "勾序", 56 },
				{ "进路及方向", 80 },
				{ "自触", 64 },
				{ "状态", 64 },
				{ "触发时间", 80 },
				{ "占用时间", 80 },
				{ "", 0 }	//占位
			};
			return vecHeadInfo;
		}

		QVector<Control::TableViewHeadInfo> StaRoutePlanKSK::GetHookPlanTableHeadInfo()
		{
			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				{ "股道", 72 },
				{ "操作", 72 },
				{ "状态", 72 },
				{ "记事", 96 },
				{ "作业时间", 96 },
				{ "", 0 }	//占位
			};
			return vecHeadInfo;
		}
	}
}
