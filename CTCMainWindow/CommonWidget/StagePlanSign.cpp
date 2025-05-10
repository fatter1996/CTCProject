#include "StagePlanSign.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	StagePlanSign::StagePlanSign(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		setWindowIcon(QIcon("icon/icon.ico"));
		setWindowTitle("阶段计划签收");
		m_pStagePlanSignTable = new Control::TableView(this);
		m_pStagePlanSignTable->SetSectionText("序", 36);
		QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				{ "到达车次", 72 },
				{ "出发车次", 72 },
				{ "调度台", 72 },
				{ "类型", 48 },
				{ "到达股道", 64 },
				{ "到达时间", 72 },
				{ "出发股道", 64 },
				{ "出发时间", 72 },
				{ "签收", 48 },
				{ "", 0 }	//占位
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
				<< "新增"
				<< pStaStagePlan->m_strDepartTrainNum
				<< pStaStagePlan->m_tArrivalTime.toString("hh:mm")
				<< pStaStagePlan->m_strDepartTrack
				<< pStaStagePlan->m_tDepartTime.toString("hh:mm")
			);
		}
		m_pStagePlanSignTable->ResetTableRows(vecTableData);
	}
}
