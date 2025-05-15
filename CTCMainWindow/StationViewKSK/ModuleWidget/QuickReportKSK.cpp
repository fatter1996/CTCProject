#include "QuickReportKSK.h"
#pragma execution_character_set("utf-8")


namespace CTCWindows {
	namespace CASCO {
		QuickReportKSK::QuickReportKSK(Station::StaTrafficLog* m_pCurTrafficLog, QWidget* parent)
			: QWidget(parent)
		{
			ui.setupUi(this);
			m_pTrainList = new CTCWindows::Control::TableView(this);
			m_pTrainList->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_pTrainList->SetSectionText("序", 36);
			m_pTrainList->SetHeadData(GetTrainRouteTableHeadInfo(), HHead);
			ui.tablewidget->layout()->addWidget(m_pTrainList);
		}
		QVector<CTCWindows::Control::TableViewHeadInfo> QuickReportKSK::GetTrainRouteTableHeadInfo()
		{
			m_nTrackRow = 1;
			m_nTriggerRow = 2;
			m_nRouteRow = 12;
			QVector<CTCWindows::Control::TableViewHeadInfo> vecHeadInfo = {
				{ "列车车次", 96 },
				{ "所属调度台", 72 },
				{ "发送/收到", 56 , true},
				{ "牵引机车", 96 },
				{ "超限", 72 },
				{ "司机", 72 },
				{ "辆数", 56 },
				{ "计长", 56 },
				{ "车长", 56 },
				{ "超限级", 56 },
				{ "机车类型", 72 },
				{ "机车号", 56 },
				{ "", 0 }	//占位
			};
			return vecHeadInfo;
		}
		QuickReportKSK::~QuickReportKSK()
		{}
	}
}