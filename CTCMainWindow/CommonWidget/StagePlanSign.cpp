#include "StagePlanSign.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	StagePlanSign::StagePlanSign(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		setWindowIcon(QIcon("icon/icon.ico"));
		setWindowTitle("�׶μƻ�ǩ��");
		m_pStagePlanSignTable = new Control::TableView(this);
		m_pStagePlanSignTable->SetSectionText("��", 36);
		QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				{ "���ﳵ��", 72 },
				{ "��������", 72 },
				{ "����̨", 72 },
				{ "����", 48 },
				{ "����ɵ�", 64 },
				{ "����ʱ��", 72 },
				{ "�����ɵ�", 64 },
				{ "����ʱ��", 72 },
				{ "ǩ��", 48 },
				{ "", 0 }	//ռλ
		};
		m_pStagePlanSignTable->SetHeadData(vecHeadInfo, HHead);
		ui.tableFrame->layout()->addWidget(m_pStagePlanSignTable);

		connect(ui.closeBtn, &QPushButton::clicked, this, &StagePlanSign::close);
		connect(ui.signBtn, &QPushButton::clicked, [=]() {
			for (Station::StaStagePlan* pStaStagePlan : Station::MainStation()->StagePlanList()) {
				pStaStagePlan->m_nOrderState = 1;
			}
			Station::MainStation()->ClearNewStagePlan();
			this->close();
		});
	}

	StagePlanSign::~StagePlanSign()
	{
	}

	void StagePlanSign::InitStagePlan()
	{
		Station::StaStagePlan* pNewStagePlan = Station::MainStation()->NewStagePlan();
		if (pNewStagePlan) {
			ui.timeLabel->setText(pNewStagePlan->m_tOrderedTime.toString("yyyy-MM-dd hh:mm"));
		}

		QVector<QStringList> vecTableData;
		for (Station::StaStagePlan* pStaStagePlan : Station::MainStation()->StagePlanList()) {
			vecTableData.append(QStringList() 
				<< pStaStagePlan->m_strArrivalTrainNum
				<< pStaStagePlan->m_strDepartTrainNum
				<< ""
				<< "����"
				<< pStaStagePlan->m_strDepartTrainNum
				<< pStaStagePlan->m_tArrivalTime.toString("hh:mm")
				<< pStaStagePlan->m_strDepartTrack
				<< pStaStagePlan->m_tDepartTime.toString("hh:mm")
			);
		}
		m_pStagePlanSignTable->ResetTableRows(vecTableData);
	}
}
