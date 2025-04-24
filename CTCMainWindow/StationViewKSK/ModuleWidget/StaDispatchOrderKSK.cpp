#include "StaDispatchOrderKSK.h"
#include <QTabBar>
#include <QMouseEvent>
#include <QDebug>
//#include <Global.cpp>
#include <QTreeWidget>
#include <Global.h>
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CASCO {

		StaDispatchOrderKSK::StaDispatchOrderKSK(QWidget* parent)
			: BaseWnd::StaDispatchOrder(parent)
		{
			ui.setupUi(this);
			ui.tabWidget->tabBar()->hide();
			connect(ui.toolBox, &QToolBox::currentChanged, [=](int index) {
				ui.tabWidget->setCurrentIndex(index);
				SetTableWidgetStation(index, 0);
				});
			connect(ui.pushButton_2, &QPushButton::clicked, [=]() {
				this->close();
				});
			connect(ui.pushButton_6, &QPushButton::clicked, [=]() {
				SignForDispatchOrder();
				DispatchOrderListUpData();

				});
			connect(ui.pushButton, &QPushButton::clicked, [=]() {
				if (ui.label_19->text().contains("预收令箱")) {
					m_pStationOrder->removeRow(m_nrow);
					m_vecStationAdvanceReceiptDispatchOrder.clear();
					//m_vecStationAdvanceReceiptDispatchOrder.removeAt(m_nrow);
				}
				else if (ui.label_21->text().contains("储令箱")) {
					m_pDispatcherCommand->removeRow(m_nrow);
				}
				else if ( ui.label_22->text().contains("储令箱")) {
					m_pLocomotiveOrder->removeRow(m_nrow);
				}
				SignForDispatchOrder();
				DispatchOrderListUpData();
				});
			InitTable();
			InitControl();
			DispatchOrderListUpData();

			for (Station::StaDispatchOrder* pCurDispatch : m_vecStationClosedDispatchOrder) {
				m_pCurDispatch = pCurDispatch;
				SetTableWidgetStation(0, 0);
			}
		}

		StaDispatchOrderKSK::~StaDispatchOrderKSK()
		{}

		void StaDispatchOrderKSK::SignForDispatchOrder()
		{
			if (m_pCurDispatch) {
				QByteArray btResult;
				if (Http::HttpClient::SignForDispatchOrder(m_pCurDispatch->m_nOrderId, btResult)) {
					m_pCurDispatch->m_nStateDisOrder = 1;
				}
			}
			CurDispatchOrderUpData();
		}
		QVector<QStringList> StaDispatchOrderKSK::GetDispatcherReceivingData(QString strTrainNum, QVector<Station::StaDispatchOrder*> vecDispatch, DataType type)//调度台数据
		{
			QVector<QStringList> TableDataList;
			switch (type)
			{
			case CTCWindows::CASCO::DataType::ReserveOrder:
				for (Station::StaDispatchOrder* pRoute : vecDispatch) {
					if (pRoute->m_nSendState == 3) {
						TableDataList.append({
							pRoute->m_strOrderNum,
							pRoute->m_strOrderTip,
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_strSignName,
							});
					}
				}
				return TableDataList;
			case CTCWindows::CASCO::DataType::PendingOrder:
				for (Station::StaDispatchOrder* pRoute : vecDispatch) {
					if (pRoute->m_nSendState == 3) {
						TableDataList.append({
							pRoute->m_strOrderNum,
							pRoute->m_strOrderTip,
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_strSignName,
							});
					}
				}
				return TableDataList;
			case CTCWindows::CASCO::DataType::IssueAnOrder:
				for (Station::StaDispatchOrder* pRoute : vecDispatch) {
					if (pRoute->m_nSendState == 3) {
						TableDataList.append({
							pRoute->m_strOrderNum,
							pRoute->m_strOrderTip,
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_strSignName,
							});
					}
				}
				return TableDataList;
			default:
				break;

			}
			return TableDataList;
		}
		QVector<QStringList> StaDispatchOrderKSK::GetTheLocomotiveCollectionData(QString strTrainNum, QVector<Station::StaDispatchOrder*> vecDispatch, DataType type)//机车数据
		{
			QVector<QStringList> TableDataList;
			QString Text;
			switch (type)
			{
			case CTCWindows::CASCO::DataType::ReserveOrder:
				for (Station::StaDispatchOrder* pRoute : vecDispatch) {
					if (pRoute->m_nSendState == 1) {
						TableDataList.append({
							pRoute->m_strOrderNum,
							pRoute->m_strType,
							pRoute->m_strSendAgency,
							pRoute->m_strSendName,
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_tSendTime.toString("hh:mm")
							});
					}
				}
				return TableDataList;
		
			case CTCWindows::CASCO::DataType::PendingOrder:
				for (Station::StaDispatchOrder* pRoute : vecDispatch) {
					if (pRoute->m_nSendState == 2) {

						TableDataList.append({
							pRoute->m_strOrderNum,
							pRoute->m_strType,
							pRoute->m_strSendAgency,
							pRoute->m_strSendName,
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_tSendTime.toString("hh:mm")
							});
					}
				}
				return TableDataList;
			case CTCWindows::CASCO::DataType::IssueAnOrder:
				for (Station::StaDispatchOrder* pRoute : vecDispatch) {
					if (pRoute->m_nSendState == 3) {
						TableDataList.append({
							pRoute->m_strOrderNum,
							pRoute->m_strType,
							pRoute->m_strSendAgency,
							pRoute->m_strSendName,
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_tSendTime.toString("hh:mm")
							});
					}
				}
				return TableDataList;

			default:
				break;

			}
			return TableDataList;
		}
		void StaDispatchOrderKSK::DispatchOrderListUpData()
		{
			m_vecStationAdvanceReceiptDispatchOrder.clear();
			m_vecStationClosedDispatchOrder.clear();
			m_vecConsoleHairDispatchOrder.clear();
			m_vecConsoleStorageImpDispatchOrder.clear();
			m_vecConsoleTobesentDispatchOrder.clear();
			m_vecLocomotiveHairDispatchOrder.clear();
			m_vecLocomotiveStorageImpDispatchOrder.clear();
			m_vecLocomotiveTobesentDispatchOrder.clear();
			int nReceiveNum = 0;
			QTreeWidgetItem* pItem = nullptr;
			for (Station::StaDispatchOrder* pDispatch : Station::MainStation()->DispatchOrderList()) {

				pItem = new QTreeWidgetItem();
				//pItem->setText(0, pDispatch->m_strOrderNum);
				pItem->setText(0, pDispatch->m_strOrderTip);
				if (pDispatch->m_nOrderType == 1) {//车站
					if (pDispatch->m_nStateDisOrder == 0) {
						m_vecStationAdvanceReceiptDispatchOrder.append(pDispatch);
					}
					else if (pDispatch->m_nStateDisOrder == 1|| pDispatch->m_nStateDisOrder == 2) {
						m_vecStationClosedDispatchOrder.append(pDispatch);
					}
				}
				else if (pDispatch->m_nOrderType == 2) {
					if (pDispatch->m_nSendState == 1) {
						m_vecConsoleHairDispatchOrder.append(pDispatch);
					}
					else if (pDispatch->m_nSendState == 2) {
						m_vecConsoleStorageImpDispatchOrder.append(pDispatch);
					}
					else if (pDispatch->m_nSendState == 3) {
						m_vecConsoleTobesentDispatchOrder.append(pDispatch);
					}
				}
				else if (pDispatch->m_nOrderType == 3) {
					if (pDispatch->m_nSendState == 1) {
						m_vecLocomotiveHairDispatchOrder.append(pDispatch);
					}
					else if (pDispatch->m_nSendState == 2) {
						m_vecLocomotiveStorageImpDispatchOrder.append(pDispatch);
					}
					else if (pDispatch->m_nSendState == 3) {
						m_vecLocomotiveTobesentDispatchOrder.append(pDispatch);
					}
				}

				if (pDispatch->m_nStateDisOrder == 1) {
					nReceiveNum++;
				}
			}
	
		}
		void StaDispatchOrderKSK::CurDispatchOrderUpData()
		{
		}
		void StaDispatchOrderKSK::TrainDispatchOrderListUpData()
		{
		}
		void StaDispatchOrderKSK::CurTrainDispatchOrderUpData()
		{
		}
		QVector<QStringList> StaDispatchOrderKSK::GetStationOrderData(QString strTrainNum, QVector<Station::StaDispatchOrder*> vecDispatch, DataType type)//车站收令数据
		{
			QVector<QStringList> TableDataList;
			QString Text;

			switch (type)
			{
			case CTCWindows::CASCO::DataType::RetakeOrder:
				for (Station::StaDispatchOrder* pRoute : vecDispatch) {
					if (pRoute->m_nStateDisOrder == 1 || pRoute->m_nStateDisOrder == 2) {
						QString statusText = (pRoute->m_nStateDisOrder == 1) ? "签收" : "拒签";
						TableDataList.append({
							pRoute->m_strOrderNum,
							pRoute->m_strType,
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_strSendAgency,
							pRoute->m_strSendName,
							statusText,
							pRoute->m_strSignName,
							pRoute->m_tSignTime.toString("hh:mm")
							});
					}
				}
				return TableDataList;
			case CTCWindows::CASCO::DataType::PrecollectionOrder:
				for (Station::StaDispatchOrder* pRoute : vecDispatch) {
					if (pRoute->m_nStateDisOrder == 0) {
						Text = "未签收";
						TableDataList.append({
							pRoute->m_strOrderNum,
							pRoute->m_strType,
							pRoute->m_tSendTime.toString("hh:mm"),
							pRoute->m_strSendAgency,
							pRoute->m_strSendName,
							Text,
							pRoute->m_strSignName,
							pRoute->m_tSignTime.toString("hh:mm")
							});
					}
				}
				return TableDataList;
			default:
				break;
			}
			return TableDataList;
		}
		bool StaDispatchOrderKSK::eventFilter(QObject * watched, QEvent * event)
		{
            if (watched == ui.widget) {
                if (event->type() == QEvent::MouseButtonPress) {
                    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                    if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_19->setText(QString("车站命令%1").arg(ui.label_2->text()));
						m_pStationOrder->RemoveAllRows();
						ui.tabWidget->setCurrentIndex(0);
						for (Station::StaDispatchOrder* pCurDispatch : m_vecStationClosedDispatchOrder) {
							m_pCurDispatch = pCurDispatch;
							SetTableWidgetStation(0, 0);
						}
                    }
                }

			}
			else if(watched == ui.widget_2){
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_19->setText(QString("车站命令%1").arg(ui.label_4->text()));
						ui.tabWidget->setCurrentIndex(0);
						m_pStationOrder->RemoveAllRows();
						for (Station::StaDispatchOrder* pCurDispatch : m_vecStationAdvanceReceiptDispatchOrder) {
							m_pCurDispatch = pCurDispatch;
							SetTableWidgetStation(0, 1);

						}

					}
				}
			}
			else if (watched == ui.widget_4) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_21->setText(QString("调度台命令%1").arg(ui.label_8->text()));
						m_pDispatcherCommand->RemoveAllRows();
						for (Station::StaDispatchOrder* pCurDispatch : m_vecConsoleHairDispatchOrder) {
							m_pCurDispatch = pCurDispatch;
							ui.tabWidget->setCurrentIndex(1);
							SetTableWidgetStation(1, 0);
						}
					}
				}
			}
			else if (watched == ui.widget_5) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_21->setText(QString("调度台命令%1").arg(ui.label_10->text()));
						m_pDispatcherCommand->RemoveAllRows();
						ui.tabWidget->setCurrentIndex(1);
						for (Station::StaDispatchOrder* pCurDispatch : m_vecConsoleStorageImpDispatchOrder) {
							m_pCurDispatch = pCurDispatch;
							SetTableWidgetStation(1, 1);
						}
					}
				}
			}
			else if (watched == ui.widget_6) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_21->setText(QString("调度台命令%1").arg(ui.label_12->text()));
						m_pDispatcherCommand->RemoveAllRows();
						for (Station::StaDispatchOrder* pCurDispatch : m_vecConsoleTobesentDispatchOrder) {
							m_pCurDispatch = pCurDispatch;
							ui.tabWidget->setCurrentIndex(1);
							SetTableWidgetStation(1, 2);
						}
					}
				}
			}
			else if (watched == ui.widget_7) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_22->setText(QString("机车命令%1").arg(ui.label_14->text()));
						m_pLocomotiveOrder->RemoveAllRows();
						ui.tabWidget->setCurrentIndex(2);
						for (Station::StaDispatchOrder* pCurDispatch : m_vecLocomotiveHairDispatchOrder) {
							m_pCurDispatch = pCurDispatch;

							SetTableWidgetStation(2, 0);
						}
					}
				}
			}
			else if (watched == ui.widget_8) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_22->setText(QString("机车命令%1").arg(ui.label_16->text()));
						m_pLocomotiveOrder->RemoveAllRows();
						ui.tabWidget->setCurrentIndex(2);
						for (Station::StaDispatchOrder* pCurDispatch : m_vecLocomotiveStorageImpDispatchOrder) {
							m_pCurDispatch = pCurDispatch;
							SetTableWidgetStation(2, 1);
						}
					}
				}
			}
			else if (watched == ui.widget_9) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_22->setText(QString("机车命令%1").arg(ui.label_18->text()));
						m_pLocomotiveOrder->RemoveAllRows();
						ui.tabWidget->setCurrentIndex(2);
						for (Station::StaDispatchOrder* pCurDispatch : m_vecLocomotiveTobesentDispatchOrder) {
							m_pCurDispatch = pCurDispatch;
							SetTableWidgetStation(2, 2);
						}
					}
				}
			}
            return StaDispatchOrder::eventFilter(watched, event);
		}

		void StaDispatchOrderKSK::InitControl()
		{
			ui.widget->installEventFilter(this);
			ui.widget_2->installEventFilter(this);
			ui.widget_4->installEventFilter(this);
			ui.widget_5->installEventFilter(this);
			ui.widget_6->installEventFilter(this);
			ui.widget_7->installEventFilter(this);
			ui.widget_8->installEventFilter(this);
			ui.widget_9->installEventFilter(this);
			ui.widget->setAttribute(Qt::WA_AcceptTouchEvents, false);
			ui.widget_2->setAttribute(Qt::WA_AcceptTouchEvents, false);
			ui.widget_4->setAttribute(Qt::WA_AcceptTouchEvents, false);
			ui.widget_5->setAttribute(Qt::WA_AcceptTouchEvents, false);
			ui.widget_6->setAttribute(Qt::WA_AcceptTouchEvents, false);
			ui.widget_7->setAttribute(Qt::WA_AcceptTouchEvents, false);
			ui.widget_8->setAttribute(Qt::WA_AcceptTouchEvents, false);
			ui.widget_9->setAttribute(Qt::WA_AcceptTouchEvents, false);
			ui.widget->setMouseTracking(true);
			ui.widget_2->setMouseTracking(true);
			ui.widget_4->setMouseTracking(true);
			ui.widget_5->setMouseTracking(true);
			ui.widget_6->setMouseTracking(true);
			ui.widget_7->setMouseTracking(true);
			ui.widget_8->setMouseTracking(true);
			ui.widget_9->setMouseTracking(true);


			m_pStationOrder->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_pDispatcherCommand->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_pLocomotiveOrder->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_pCopyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_pReadTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_pdispatchTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
			m_pLocomotiveTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

		}

		void StaDispatchOrderKSK::SetTableWidgetStation(int index, int flag)
		{

			if (m_pCurDispatch == nullptr) {
				return;
			}
			if (index == 0) {//车站
				if (flag == 0) {
					TableDataList = GetStationOrderData(m_pCurDispatch->m_strOrderNum, m_vecStationClosedDispatchOrder,DataType::RetakeOrder);
					m_pStationOrder->ResetTableRows(TableDataList);
				}
				else if (flag == 1) {

					TableDataList = GetStationOrderData(m_pCurDispatch->m_strOrderNum, m_vecStationAdvanceReceiptDispatchOrder, DataType::PrecollectionOrder);
					m_pStationOrder->ResetTableRows(TableDataList);
				}
			}
			else if (index == 1) {//调度台
				if (flag == 0) {//发送
	
					TableDataList = GetDispatcherReceivingData(m_pCurDispatch->m_strOrderNum, m_vecConsoleHairDispatchOrder, DataType::IssueAnOrder);
					m_pDispatcherCommand->ResetTableRows(TableDataList);
				}
				else if (flag == 1) {//储令
					TableDataList = GetDispatcherReceivingData(m_pCurDispatch->m_strOrderNum, m_vecConsoleStorageImpDispatchOrder, DataType::ReserveOrder);
					m_pDispatcherCommand->ResetTableRows(TableDataList);
				}
				
				else if (flag == 2) {//待发
					TableDataList = GetDispatcherReceivingData(m_pCurDispatch->m_strOrderNum, m_vecConsoleTobesentDispatchOrder, DataType::PendingOrder);
					m_pDispatcherCommand->ResetTableRows(TableDataList);
				}
			}
			else if (index == 2) {//机车
				if (flag == 0) {//发送
					
					TableDataList = GetTheLocomotiveCollectionData(m_pCurDispatch->m_strOrderNum, m_vecLocomotiveHairDispatchOrder, DataType::IssueAnOrder);
					m_pLocomotiveOrder->ResetTableRows(TableDataList);
				}
				else if (flag == 1) {//储令
					TableDataList = GetTheLocomotiveCollectionData(m_pCurDispatch->m_strOrderNum, m_vecLocomotiveStorageImpDispatchOrder, DataType::ReserveOrder);
					m_pLocomotiveOrder->ResetTableRows(TableDataList);
				}
				else if (flag == 2) {//待发
					TableDataList = GetTheLocomotiveCollectionData(m_pCurDispatch->m_strOrderNum, m_vecLocomotiveTobesentDispatchOrder, DataType::PendingOrder);
					m_pLocomotiveOrder->ResetTableRows(TableDataList);
				}
			}
		}

		void StaDispatchOrderKSK::InitTable()
		{
			m_pStationOrder = new Control::TableView(this);//车站
			m_pStationOrder->SetSectionText("序", 36);
			m_pStationOrder->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::station), HHead);
			ui.frame->layout()->addWidget(m_pStationOrder); 

			connect(m_pStationOrder, &CTCWindows::Control::TableView::clicked, [=](const QModelIndex& index) {
				qDebug() << "单击行:" << index.row()<<"车站"<< ui.label_19->text();
				m_nrow = index.row();
			});

			m_pDispatcherCommand = new Control::TableView(this);//调度台
			m_pDispatcherCommand->SetSectionText("序", 36);
			m_pDispatcherCommand->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::Dispatcher), HHead);
			ui.frame_4->layout()->addWidget(m_pDispatcherCommand);

			connect(m_pDispatcherCommand, &CTCWindows::Control::TableView::clicked, [=](const QModelIndex& index) {
				qDebug() << "单击行:" << index.row() << "调度台";
				m_nrow = index.row();
				});

			m_pLocomotiveOrder = new Control::TableView(this);//机车
			m_pLocomotiveOrder->SetSectionText("序", 36);
			m_pLocomotiveOrder->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::Locomot), HHead);
			ui.frame_7->layout()->addWidget(m_pLocomotiveOrder);

			connect(m_pLocomotiveOrder, &CTCWindows::Control::TableView::clicked, [=](const QModelIndex& index) {
				qDebug() << "单击行:" << index.row() << "机车";
				m_nrow = index.row();
				});

			m_pCopyTable = new Control::TableView(this);;	//抄知处所表格
			m_pCopyTable->SetSectionText("序", 36);
			m_pCopyTable->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::CopyTable), HHead);
			ui.frame_2->layout()->addWidget(m_pCopyTable);

			connect(m_pCopyTable, &CTCWindows::Control::TableView::clicked, [=](const QModelIndex& index) {
				qDebug() << "单击行:" << index.row() << "抄知处所";

				});

			m_pReadTable = new Control::TableView(this);;	//阅读表格
			m_pReadTable->SetSectionText("序", 36);
			m_pReadTable->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::ReadTable), HHead);
			ui.frame_3->layout()->addWidget(m_pReadTable);

			connect(m_pReadTable, &CTCWindows::Control::TableView::clicked, [=](const QModelIndex& index) {
				qDebug() << "单击行:" << index.row() << "阅读表格";
				});

			m_pdispatchTable = new Control::TableView(this);;	//调度台接收表格
			m_pdispatchTable->SetSectionText("序", 36);
			m_pdispatchTable->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::DispatchTable), HHead);
			ui.frame_5->layout()->addWidget(m_pdispatchTable);

			connect(m_pdispatchTable, &CTCWindows::Control::TableView::clicked, [=](const QModelIndex& index) {
				qDebug() << "单击行:" << index.row() << "调度台接收";
				});

			m_pLocomotiveTable = new Control::TableView(this);;	//机车接收表格
			m_pLocomotiveTable->SetSectionText("序", 36);
			m_pLocomotiveTable->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::LocomotTable), HHead);
			ui.frame_6->layout()->addWidget(m_pLocomotiveTable);

			connect(m_pLocomotiveTable, &CTCWindows::Control::TableView::clicked, [=](const QModelIndex& index) {
				qDebug() << "单击行:" << index.row() << "机车接收表格";
				});

		}

		QVector<Control::TableViewHeadInfo> StaDispatchOrderKSK::GetTrainRouteTableHeadInfo(TableSpecies m_intSpecies)
		{
			QVector<Control::TableViewHeadInfo> vecHeadInfo;

			switch (m_intSpecies)
			{
			case TableSpecies::station://车站
				vecHeadInfo = {
					{ "号码", 96 },
					{ "命令类型", 72 },
					{ "收令日期", 56 },
					{ "发令单位", 96 },
					{ "发令人", 72 },
					{ "签否", 72 },
					{ "签收人", 96 },
					{ "签收时刻", 120 },
					{ "", 0 }	//占位
				};
				return vecHeadInfo;
			case TableSpecies::Dispatcher://调度台
				vecHeadInfo = {
					{ "编号", 96 },
					{ "请求名称", 72 },
					{ "创建时间", 56 },
					{ "发送时间", 96 },
					{ "值班人", 72 },
					{ "", 0 }	//占位
				};
				return vecHeadInfo;
			case TableSpecies::Locomot://机车
				vecHeadInfo = {
					{ "编号", 96 },
					{ "命令种类", 72 },
					{ "命令类型", 56 },
					{ "发令单位", 96 },
					{ "发令人", 72 },
					{ "创建时间", 72 },
					{ "发送时间", 96 },
					{ "", 0 }	//占位
				};
			case TableSpecies::CopyTable://抄知处所
				vecHeadInfo = {
					{ "编号", 96 },
					{ "命令种类", 72 },
					{ "命令类型", 56 },
					{ "发令单位", 96 },
					{ "发令人", 72 },
					{ "创建时间", 72 },
					{ "发送时间", 96 },
					{ "", 0 }	//占位
				};
			case TableSpecies::ReadTable://阅读
				vecHeadInfo = {
					{ "编号", 96 },
					{ "命令种类", 72 },
					{ "命令类型", 56 },
					{ "发令单位", 96 },
					{ "发令人", 72 },
					{ "创建时间", 72 },
					{ "发送时间", 96 },
					{ "", 0 }	//占位
				};
			case TableSpecies::DispatchTable://调度台表格
				vecHeadInfo = {
					{ "编号", 96 },
					{ "命令种类", 72 },
					{ "命令类型", 56 },
					{ "发令单位", 96 },
					{ "发令人", 72 },
					{ "创建时间", 72 },
					{ "发送时间", 96 },
					{ "", 0 }	//占位
				};
			case TableSpecies::LocomotTable://机车表格
				vecHeadInfo = {
					{ "编号", 96 },
					{ "命令种类", 72 },
					{ "命令类型", 56 },
					{ "发令单位", 96 },
					{ "发令人", 72 },
					{ "创建时间", 72 },
					{ "发送时间", 96 },
					{ "", 0 }	//占位
				};
				return vecHeadInfo;
			default:
				break;
			}
			return vecHeadInfo;
		}
	}
}
