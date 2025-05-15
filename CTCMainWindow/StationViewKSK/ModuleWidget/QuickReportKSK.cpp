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
			m_pTrainList->SetSectionText("��", 36);
			m_pTrainList->SetHeadData(GetTrainRouteTableHeadInfo(), HHead);
			ui.tablewidget->layout()->addWidget(m_pTrainList);
		}
		QVector<CTCWindows::Control::TableViewHeadInfo> QuickReportKSK::GetTrainRouteTableHeadInfo()
		{
			m_nTrackRow = 1;
			m_nTriggerRow = 2;
			m_nRouteRow = 12;
			QVector<CTCWindows::Control::TableViewHeadInfo> vecHeadInfo = {
				{ "�г�����", 96 },
				{ "��������̨", 72 },
				{ "����/�յ�", 56 , true},
				{ "ǣ������", 96 },
				{ "����", 72 },
				{ "˾��", 72 },
				{ "����", 56 },
				{ "�Ƴ�", 56 },
				{ "����", 56 },
				{ "���޼�", 56 },
				{ "��������", 72 },
				{ "������", 56 },
				{ "", 0 }	//ռλ
			};
			return vecHeadInfo;
		}
		QuickReportKSK::~QuickReportKSK()
		{}
	}
}