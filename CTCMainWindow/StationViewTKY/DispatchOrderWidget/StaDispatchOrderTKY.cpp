#include "StaDispatchOrderTKY.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CARS {
		StaDispatchOrderTKY::StaDispatchOrderTKY(QWidget* parent)
			: StaDispatchOrder(parent)
		{
			ui.setupUi(this);
			ui.receiveTree->setColumnCount(2);
			ui.receiveTree->setHeaderHidden(true);
			ui.importantTree->setColumnCount(2);
			ui.importantTree->setHeaderHidden(true);

			QVector<Control::TableViewHeadInfo> vecHeadInfo = {
				{ "车次号", 64 },
				{ "机车号", 64 },
				{ "状态", 96 }
			};
			ui.forwardTable->SetHeadData(vecHeadInfo, HHead);
			ui.forwardTable->verticalHeader()->hide();
			DispatchOrderListUpData();
			
			connect(ui.receiveTree, &QTreeWidget::clicked, [&](const QModelIndex& index) {
				if (index.row() < m_vecDispatchOrder.size()) {
					m_pCurDispatch = m_vecDispatchOrder.at(index.row());
					if (m_pCurDispatch->m_nStateDisOrder == 0) {
						ui.signBtn->setEnabled(true);
					}
					else {
						ui.signBtn->setEnabled(false);
					}
					CurDispatchOrderUpData();
					TrainDispatchOrderListUpData();
				}
			});

			connect(ui.importantTree, &QTreeWidget::clicked, [&](const QModelIndex& index) {
				if (index.row() < m_vecImpDispatchOrder.size()) {
					m_pCurDispatch = m_vecImpDispatchOrder.at(index.row());
					if (m_pCurDispatch->m_nStateDisOrder == 0) {
						ui.signBtn->setEnabled(true);
					}
					else {
						ui.signBtn->setEnabled(false);
					}
					CurDispatchOrderUpData();
					TrainDispatchOrderListUpData();
				}
			});

			connect(ui.importantBtn, &QPushButton::clicked, [&]() {
				if (m_pCurDispatch) {
					m_pCurDispatch->m_bImportant = true;
					DispatchOrderListUpData();
				}
			});
			connect(ui.cancelBtn, &QPushButton::clicked, [&]() {
				if (m_pCurDispatch) {
					m_pCurDispatch->m_bImportant = false;
					DispatchOrderListUpData();
				}
			});
			connect(ui.forwardCheckBox, &QCheckBox::stateChanged, [&](int nState) {
				if (nState) {
					ui.trainWidget->show();
					this->setFixedWidth(920);
				}
				else {
					ui.trainWidget->hide();
					this->setFixedWidth(712);
				}
			});
			connect(ui.signBtn, &QPushButton::clicked, this, &StaDispatchOrderTKY::SignForDispatchOrder);
			connect(ui.quitBtn, &QPushButton::clicked, [&]() { this->close(); });

			connect(ui.addBtn, &QPushButton::clicked, this, &StaDispatchOrderTKY::AddNewTrainDispatch);
			connect(ui.changeBtn, &QPushButton::clicked, this, &StaDispatchOrderTKY::ModifyNewTrainDispatch);
			connect(ui.deleteBtn, &QPushButton::clicked, this, &StaDispatchOrderTKY::DeleteNewTrainDispatch);

			connect(ui.forwardTable, &Control::TableView::clicked, [&](const QModelIndex& index) {
				if (index.row() < m_pCurDispatch->m_vecTrainDispatch.size()) {
					m_pCurTrainDispatch = m_pCurDispatch->m_vecTrainDispatch.at(index.row());
					CurTrainDispatchOrderUpData();
				}
			});
			
		}

		StaDispatchOrderTKY::~StaDispatchOrderTKY()
		{

		}

		void StaDispatchOrderTKY::DispatchOrderListUpData()
		{
			ui.receiveTree->clear();
			ui.importantTree->clear();

			int nReceiveNum = 0;
			QTreeWidgetItem* pItem = nullptr;
			for (Station::StaDispatchOrder* pDispatch : Station::MainStation()->DispatchOrderList()) {

				pItem = new QTreeWidgetItem();
				pItem->setText(0, pDispatch->m_strOrderNum);
				pItem->setText(1, pDispatch->m_strOrderTip);

				if (pDispatch->m_bImportant) {
					ui.importantTree->addTopLevelItem(pItem);
					m_vecImpDispatchOrder.append(pDispatch);
				}
				else {
					ui.receiveTree->addTopLevelItem(pItem);
					m_vecDispatchOrder.append(pDispatch);
				}

				if (pDispatch->m_nStateDisOrder == 1) {
					nReceiveNum++;
				}
			}
			ui.receiveLabel->setText(QString("已接收的命令数 : %1").arg(nReceiveNum));
			ui.unreceiveLabel->setText(QString("未接受命令数 : %1").arg(Station::MainStation()->DispatchOrderList().size() - nReceiveNum));
		}

		void StaDispatchOrderTKY::CurDispatchOrderUpData()
		{
			ui.orderNumLabel->setText(QString("命令号码: %1").arg(m_pCurDispatch->m_strOrderNum));
			ui.sendAgencyLabel->setText(QString("发令机构: %1").arg(m_pCurDispatch->m_strSendAgency));
			ui.sendTimeLabel->setText(QString("发令时间: %1").arg(m_pCurDispatch->m_tSendTime.toString("yyyy/MM/dd hh:mm:ss")));
			ui.sendNameLabel->setText(QString("发令人: %1").arg(m_pCurDispatch->m_strSendName));
			ui.signNameLabel->setText(QString("受令人: %1").arg(m_pCurDispatch->m_strSignName));
			ui.orderedUnitLabel->setText(m_pCurDispatch->m_strOrderedUnit);
			ui.contentLabel->setText(m_pCurDispatch->m_strContent);
		}

		void StaDispatchOrderTKY::TrainDispatchOrderListUpData()
		{
			QVector<QStringList> vecTableData;
			for (Station::StaTrainDispatch* pTrainDispatch : m_pCurDispatch->m_vecTrainDispatch) {
				vecTableData.append(QStringList()
					<< pTrainDispatch->m_strTrainNum
					<< pTrainDispatch->m_strLocomotive
					<< QString("%1[%2]").arg(pTrainDispatch->getStateStr()).arg(pTrainDispatch->m_timRecv.toString("hh:mm")));
			}
			ui.forwardTable->ResetTableRows(vecTableData);
		}

		void StaDispatchOrderTKY::CurTrainDispatchOrderUpData()
		{
			ui.trainNumEdit->clear();
			ui.locomotiveEdit->clear();

			if (m_pCurTrainDispatch) {
				ui.trainNumEdit->setText(m_pCurTrainDispatch->m_strTrainNum);
				ui.locomotiveEdit->setText(m_pCurTrainDispatch->m_strLocomotive);
			}
		}

		void StaDispatchOrderTKY::GetTrainInfo(Station::StaTrainDispatch* pTrainDispatch)
		{
			pTrainDispatch->m_strTrainNum = ui.trainNumEdit->text();//车次
			pTrainDispatch->m_strLocomotive = ui.locomotiveEdit->text();//机车
		}
	}
}
