#include "DispatchOrderSign.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	DispatchOrderSign::DispatchOrderSign(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		setWindowIcon(QIcon("icon/icon.ico"));
		setWindowTitle("��������ǩ��");
		m_pOrderedUnitTable = new Control::TableView(this);
		m_pOrderedUnitTable->SetSectionText("���", 48);
		QVector<Control::TableViewHeadInfo> vecHeadInfo = {
			{ "���λ", 96 },
			{ "��֪����", 96 },
			{ "", 0 }	//ռλ
		};
		m_pOrderedUnitTable->SetHeadData(vecHeadInfo, HHead);
		ui.tableFrame->layout()->addWidget(m_pOrderedUnitTable);
	}

	DispatchOrderSign::~DispatchOrderSign()
	{
	
	}

	void DispatchOrderSign::InitNewDispatchOrder(Station::StaDispatchOrder* pNewDispatchOrder)
	{
		ui.orderTypeLabel->setText(pNewDispatchOrder->m_strOrderTip);
		ui.orderNumLabel->setText(pNewDispatchOrder->m_strOrderNum);
		ui.orderDateLabel->setText(pNewDispatchOrder->m_tSendTime.toString("yyyy��MM��dd��"));
		ui.orderTimeLabel->setText(pNewDispatchOrder->m_tSendTime.toString("hh:mm"));
		ui.sendAgencyLabel->setText(pNewDispatchOrder->m_strSendAgency);
		ui.sendNameLabel->setText(pNewDispatchOrder->m_strSendName);
		ui.contentEdit->setText(pNewDispatchOrder->m_strContent);

		QVector<QStringList> vecTableData;
		vecTableData.append(QStringList() << pNewDispatchOrder->m_strOrderedUnit << "");
		m_pOrderedUnitTable->ResetTableRows(vecTableData);

		connect(ui.closeBtn, &QPushButton::clicked,[=] {
			Station::MainStation()->ClearNewDispatchOrder();
			this->close();
			});
		connect(ui.signBtn, &QPushButton::clicked, [=]() {
			pNewDispatchOrder->m_nStateDisOrder = 1;
			Station::MainStation()->ClearNewDispatchOrder();
			this->close();
		});

		connect(ui.refuseSignBtn, &QPushButton::clicked, [=]() {
			pNewDispatchOrder->m_nStateDisOrder = 2;
			Station::MainStation()->ClearNewDispatchOrder();
			this->close();
		});
	}
}
