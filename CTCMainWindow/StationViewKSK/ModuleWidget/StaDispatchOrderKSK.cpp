#include "StaDispatchOrderKSK.h"
#include <QTabBar>
#include <QMouseEvent>
#include <QDebug>

#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CASCO {

		StaDispatchOrderKSK::StaDispatchOrderKSK(QWidget* parent)
			: BaseWnd::StaDispatchOrder(parent)
		{
			ui.setupUi(this);
			ui.tabWidget->tabBar()->hide();
			connect(ui.toolBox, &QToolBox::currentChanged, [=](int index) {
				qDebug() << "Switched to page index:" << index;
				ui.tabWidget->setCurrentIndex(index);
				SetTableWidgetStation(index, 0);
			});
			
			InitTable();
			InitControl();
			SetTableWidgetStation(0);
		}

		StaDispatchOrderKSK::~StaDispatchOrderKSK()
		{}
		QVector<QStringList> StaDispatchOrderKSK::GetDispatcherReceivingData(QString strTrainNum, Station::StaDispatchOrder* pRoute, DataType type)//����̨����
		{
			QVector<QStringList> TableDataList;
			if (!pRoute) {
				return QVector<QStringList>();
			}
			switch (type)
			{
			case CTCWindows::CASCO::DataType::ReserveOrder:
				if (pRoute->m_nSendState == 3) {
					TableDataList.append({
						strTrainNum,
						pRoute->m_strOrderTip,
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_strSignName,
						});
					return TableDataList;
				}
				break;
			case CTCWindows::CASCO::DataType::PendingOrder:
				if (pRoute->m_nSendState == 3) {
					TableDataList.append({
						strTrainNum,
						pRoute->m_strOrderTip,
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_strSignName,
						});
					return TableDataList;
				}
				break;
			case CTCWindows::CASCO::DataType::IssueAnOrder:
				if (pRoute->m_nSendState == 3) {
					TableDataList.append({
						strTrainNum,
						pRoute->m_strOrderTip,
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_strSignName,
						});
					return TableDataList;
				}
				break;
			default:
				break;

			}
			return TableDataList;
		}
		QVector<QStringList> StaDispatchOrderKSK::GetTheLocomotiveCollectionData(QString strTrainNum, Station::StaDispatchOrder* pRoute, DataType type)//��������
		{
			QVector<QStringList> TableDataList;
			QString Text;
			if (!pRoute) {
				return QVector<QStringList>();
			}
			switch (type)
			{
			case CTCWindows::CASCO::DataType::ReserveOrder:
				if (pRoute->m_nSendState == 1) {
					TableDataList.append({
						strTrainNum,
						pRoute->m_strType,
						pRoute->m_strSendAgency,
						pRoute->m_strSendName,
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_tSendTime.toString("hh:mm")
						});
					return TableDataList;
				}
				break;
		
			case CTCWindows::CASCO::DataType::PendingOrder:
				if (pRoute->m_nSendState == 2) {
					TableDataList.append({
						strTrainNum,
						pRoute->m_strType,
						pRoute->m_strSendAgency,
						pRoute->m_strSendName,
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_tSendTime.toString("hh:mm")
						});
					return TableDataList;
				}
				break;
			case CTCWindows::CASCO::DataType::IssueAnOrder:
				if (pRoute->m_nSendState == 3) {
					TableDataList.append({
						strTrainNum,
						pRoute->m_strType,
						pRoute->m_strSendAgency,
						pRoute->m_strSendName,
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_tSendTime.toString("hh:mm")
						});
					return TableDataList;
				}
				break;

			default:
				break;

			}
			return TableDataList;
		}
		QVector<QStringList> StaDispatchOrderKSK::GetStationOrderData(QString strTrainNum, Station::StaDispatchOrder* pRoute, DataType type)//��վ��������
		{
			QVector<QStringList> TableDataList;
			QString Text;
			if (!pRoute) {
				return QVector<QStringList>();
			}
			switch (type)
			{
			case CTCWindows::CASCO::DataType::RetakeOrder:
				if (pRoute->m_nStateDisOrder == 1 || pRoute->m_nStateDisOrder == 2) {
					QString statusText = (pRoute->m_nStateDisOrder == 1) ? "ǩ��" : "��ǩ";
					TableDataList.append({
						strTrainNum,
						pRoute->m_strType,
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_strSendAgency,
						pRoute->m_strSendName,
						statusText,
						pRoute->m_strSignName,
						pRoute->m_tSignTime.toString("hh:mm")
						});
					return TableDataList;
				}
				break;
			case CTCWindows::CASCO::DataType::PrecollectionOrder:
				if (pRoute->m_nStateDisOrder == 0) {
					Text = "δǩ��";
					TableDataList.append({
						strTrainNum,
						pRoute->m_strType,
						pRoute->m_tSendTime.toString("hh:mm"),
						pRoute->m_strSendAgency,
						pRoute->m_strSendName,
						Text,
						pRoute->m_strSignName,
						pRoute->m_tSignTime.toString("hh:mm")
						});
					return TableDataList;
				}
				break;
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
						ui.label_19->setText(QString("��վ����%1").arg(ui.label_2->text()));
						ui.tabWidget->setCurrentIndex(0);
						
						SetTableWidgetStation(0,0);
                    }
                }

			}
			else if(watched == ui.widget_2){
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_19->setText(QString("��վ����%1").arg(ui.label_4->text()));
						ui.tabWidget->setCurrentIndex(0);
						SetTableWidgetStation(0,1);
					}
				}
			}
			else if (watched == ui.widget_4) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_21->setText(QString("����̨����%1").arg(ui.label_8->text()));
						ui.tabWidget->setCurrentIndex(1);
						SetTableWidgetStation(1,0);
					}
				}
			}
			else if (watched == ui.widget_5) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_21->setText(QString("����̨����%1").arg(ui.label_10->text()));
						ui.tabWidget->setCurrentIndex(1);
						SetTableWidgetStation(1,1);
					}
				}
			}
			else if (watched == ui.widget_6) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_21->setText(QString("����̨����%1").arg(ui.label_12->text()));
						ui.tabWidget->setCurrentIndex(1);
						SetTableWidgetStation(1,2);
					}
				}
			}
			else if (watched == ui.widget_7) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_22->setText(QString("��������%1").arg(ui.label_14->text()));
						ui.tabWidget->setCurrentIndex(2);
						SetTableWidgetStation(2,0);
					}
				}
			}
			else if (watched == ui.widget_8) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_22->setText(QString("��������%1").arg(ui.label_16->text()));
						ui.tabWidget->setCurrentIndex(2);
						SetTableWidgetStation(2,1);
					}
				}
			}
			else if (watched == ui.widget_9) {
				if (event->type() == QEvent::MouseButtonPress) {
					QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
					if (mouseEvent->button() == Qt::LeftButton) {
						ui.label_22->setText(QString("��������%1").arg(ui.label_18->text()));
						ui.tabWidget->setCurrentIndex(2);
						SetTableWidgetStation(2,2);
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
		}

		void StaDispatchOrderKSK::SetTableWidgetStation(int index, int flag)
		{

			if (m_pCurDispatch == nullptr) {
				return;
			}
			QVector<QStringList> TableDataList;
			if (index == 0) {//��վ
				if (flag == 0) {
					TableDataList = GetStationOrderData(m_pCurDispatch->m_strOrderNum, m_pCurDispatch,DataType::RetakeOrder);
					m_pStationOrder->ResetTableRows(TableDataList);
				}
				else if (flag == 1) {
					TableDataList = GetStationOrderData(m_pCurDispatch->m_strOrderNum, m_pCurDispatch,DataType::PrecollectionOrder);

					m_pStationOrder->ResetTableRows(TableDataList);
				}
			}
			else if (index == 1) {//����̨
				if (flag == 0) {//����
					TableDataList = GetDispatcherReceivingData(m_pCurDispatch->m_strOrderNum, m_pCurDispatch, DataType::IssueAnOrder);
					m_pDispatcherCommand->ResetTableRows(TableDataList);
				}
				else if (flag == 1) {//����
					TableDataList = GetDispatcherReceivingData(m_pCurDispatch->m_strOrderNum, m_pCurDispatch, DataType::ReserveOrder);
				}
				else if (flag == 2) {//����
					TableDataList = GetDispatcherReceivingData(m_pCurDispatch->m_strOrderNum, m_pCurDispatch, DataType::PendingOrder);
					m_pDispatcherCommand->ResetTableRows(TableDataList);
				}
			}
			else if (index == 2) {//����
				if (flag == 0) {//����
					TableDataList = GetTheLocomotiveCollectionData(m_pCurDispatch->m_strOrderNum, m_pCurDispatch, DataType::IssueAnOrder);
					m_pLocomotiveOrder->ResetTableRows(TableDataList);
				}
				else if (flag == 1) {//����
					TableDataList = GetTheLocomotiveCollectionData(m_pCurDispatch->m_strOrderNum, m_pCurDispatch, DataType::ReserveOrder);
					m_pLocomotiveOrder->ResetTableRows(TableDataList);
				}
				else if (flag == 2) {//����
					TableDataList = GetTheLocomotiveCollectionData(m_pCurDispatch->m_strOrderNum, m_pCurDispatch, DataType::PendingOrder);
					m_pLocomotiveOrder->ResetTableRows(TableDataList);
				}
			}
		}

		void StaDispatchOrderKSK::InitTable()
		{
			m_pStationOrder = new Control::TableView(this);//��վ
			m_pStationOrder->SetSectionText("��", 36);
			m_pStationOrder->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::station), HHead);
			ui.frame->layout()->addWidget(m_pStationOrder); 

			m_pDispatcherCommand = new Control::TableView(this);//����̨
			m_pDispatcherCommand->SetSectionText("��", 36);
			m_pDispatcherCommand->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::Dispatcher), HHead);
			ui.frame_4->layout()->addWidget(m_pDispatcherCommand);

			m_pLocomotiveOrder = new Control::TableView(this);//����
			m_pLocomotiveOrder->SetSectionText("��", 36);
			m_pLocomotiveOrder->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::Locomot), HHead);
			ui.frame_7->layout()->addWidget(m_pLocomotiveOrder);

			m_pCopyTable = new Control::TableView(this);;	//��֪�������
			m_pCopyTable->SetSectionText("��", 36);
			m_pCopyTable->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::CopyTable), HHead);
			ui.frame_2->layout()->addWidget(m_pCopyTable);

			m_pReadTable = new Control::TableView(this);;	//�Ķ����
			m_pReadTable->SetSectionText("��", 36);
			m_pReadTable->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::ReadTable), HHead);
			ui.frame_3->layout()->addWidget(m_pReadTable);

			m_pdispatchTable = new Control::TableView(this);;	//����̨���ձ��
			m_pdispatchTable->SetSectionText("��", 36);
			m_pdispatchTable->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::DispatchTable), HHead);
			ui.frame_5->layout()->addWidget(m_pdispatchTable);

			m_pLocomotiveTable = new Control::TableView(this);;	//�������ձ��
			m_pLocomotiveTable->SetSectionText("��", 36);
			m_pLocomotiveTable->SetHeadData(GetTrainRouteTableHeadInfo(TableSpecies::LocomotTable), HHead);
			ui.frame_6->layout()->addWidget(m_pLocomotiveTable);

		}

		QVector<Control::TableViewHeadInfo> StaDispatchOrderKSK::GetTrainRouteTableHeadInfo(TableSpecies m_intSpecies)
		{
			QVector<Control::TableViewHeadInfo> vecHeadInfo;
			switch (m_intSpecies)
			{
			case TableSpecies::station://��վ
				vecHeadInfo = {
					{ "����", 96 },
					{ "��������", 72 },
					{ "��������", 56 },
					{ "���λ", 96 },
					{ "������", 72 },
					{ "ǩ��", 72 },
					{ "ǩ����", 96 },
					{ "ǩ��ʱ��", 120 },
					{ "", 0 }	//ռλ
				};
				break;
			case TableSpecies::Dispatcher://����̨
				vecHeadInfo = {
					{ "���", 96 },
					{ "��������", 72 },
					{ "����ʱ��", 56 },
					{ "����ʱ��", 96 },
					{ "ֵ����", 72 },
					{ "", 0 }	//ռλ
				};
				break;
			case TableSpecies::Locomot://����
				vecHeadInfo = {
					{ "���", 96 },
					{ "��������", 72 },
					{ "��������", 56 },
					{ "���λ", 96 },
					{ "������", 72 },
					{ "����ʱ��", 72 },
					{ "����ʱ��", 96 },
					{ "", 0 }	//ռλ
				};
			case TableSpecies::CopyTable://��֪����
				vecHeadInfo = {
					{ "���", 96 },
					{ "��������", 72 },
					{ "��������", 56 },
					{ "���λ", 96 },
					{ "������", 72 },
					{ "����ʱ��", 72 },
					{ "����ʱ��", 96 },
					{ "", 0 }	//ռλ
				};
			case TableSpecies::ReadTable://�Ķ�
				vecHeadInfo = {
					{ "���", 96 },
					{ "��������", 72 },
					{ "��������", 56 },
					{ "���λ", 96 },
					{ "������", 72 },
					{ "����ʱ��", 72 },
					{ "����ʱ��", 96 },
					{ "", 0 }	//ռλ
				};
			case TableSpecies::DispatchTable://����̨���
				vecHeadInfo = {
					{ "���", 96 },
					{ "��������", 72 },
					{ "��������", 56 },
					{ "���λ", 96 },
					{ "������", 72 },
					{ "����ʱ��", 72 },
					{ "����ʱ��", 96 },
					{ "", 0 }	//ռλ
				};
			case TableSpecies::LocomotTable://�������
				vecHeadInfo = {
					{ "���", 96 },
					{ "��������", 72 },
					{ "��������", 56 },
					{ "���λ", 96 },
					{ "������", 72 },
					{ "����ʱ��", 72 },
					{ "����ʱ��", 96 },
					{ "", 0 }	//ռλ
				};
				break;
			default:
				break;
			}
			return vecHeadInfo;
		}
	}
}
