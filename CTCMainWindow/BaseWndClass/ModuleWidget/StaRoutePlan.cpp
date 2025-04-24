#include "StaRoutePlan.h"
#include "Global.h"
#include <QMessageBox>

#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace BaseWnd {
		using namespace Station;
		StaRoutePlan::StaRoutePlan(QWidget* parent)
		{
			this->setMinimumHeight(164);
			QSizePolicy policy = this->sizePolicy();
			policy.setVerticalPolicy(QSizePolicy::MinimumExpanding); // �����ڴ�ֱ��������С����С��С����Ҳ������չ
			this->setSizePolicy(policy);
			m_nTimerId_15 = startTimer(15 * 1000);
		}

		StaRoutePlan::~StaRoutePlan()
		{
			killTimer(m_nTimerId_15);
		}

		void StaRoutePlan::InitTrainRouteTable()
		{
			m_pTrainRouteTable = new Control::TableView(this);
			m_pTrainRouteTable->SetSectionText("��", 36);
			m_pTrainRouteTable->SetHeadData(GetTrainRouteTableHeadInfo(), HHead);
			connect(m_pTrainRouteTable, &Control::TableView::clicked, this, &StaRoutePlan::TrainRouteTableLeftMenu);
			connect(m_pTrainRouteTable, &Control::TableView::customContextMenuRequested, this, &StaRoutePlan::TrainRouteTableRightMenu);
		}

		void StaRoutePlan::RouteOrderSort(QVector<StaTrainRoute*>::iterator low, QVector<StaTrainRoute*>::iterator high)
		{
			QVector<StaTrainRoute*>::iterator order1 = low;
			QVector<StaTrainRoute*>::iterator order2 = high;
			if (high <= low) {
				return;
			}
			StaTrainRoute* key = *low;
			while (1) {
				/*���������ұ�key���ֵ*/
				while (RouteOrderCompare(*order1, key)) {
					if (++order1 == high) {
						break;
					}
				}
				/*���������ұ�keyС��ֵ*/
				while (RouteOrderCompare(key, *order2)) {
					if (--order2 == low) {
						break;
					}
				}
				if (order1 >= order2) {
					break;
				}
				/*����i,j��Ӧ��ֵ*/
				std::iter_swap(order1, order2);
			}
			/*����ֵ��j��Ӧֵ����*/
			*low = *order2;
			*order2 = key;
			RouteOrderSort(low, order2 - 1);
			RouteOrderSort(order2 + 1, high);
		}

		bool StaRoutePlan::RouteOrderCompare(StaTrainRoute* pRoute1, StaTrainRoute* pRoute2)
		{
			if (pRoute1->m_nTrainId == pRoute2->m_nTrainId) {
				if (pRoute1->m_bArrivaRoute == pRoute2->m_bArrivaRoute) {
					return pRoute1->m_tPlanTime.toTime_t() <= pRoute2->m_tPlanTime.toTime_t();
				}
				else {
					return pRoute1->m_bArrivaRoute;
				}
			}
			else {
				return pRoute1->m_tPlanTime.toTime_t() <= pRoute2->m_tPlanTime.toTime_t();
			}
		}

		void StaRoutePlan::OnTrainRouteUpData()
		{
			QVector<StaTrainRoute*> vecStaTrainRoute = MainStation()->TrainRouteList();
			QVector<QStringList> vecTableData;
			StaTrafficLog* pTrafficLog = nullptr;
			StaTrain* pTrain = nullptr;
			QString strTrainNum;
			
			if (vecStaTrainRoute.size() > 0) {
				RouteOrderSort(vecStaTrainRoute.begin(), vecStaTrainRoute.end() - 1);
			}
			
			int nRowIndex = 0;
			for (StaTrainRoute* pRoute : vecStaTrainRoute) {
				if (pRoute->m_bSunTrainRoute) {
					continue;
				}
				pTrafficLog = MainStation()->getStaTrafficLogById(pRoute->m_nLogId);
				pTrain = MainStation()->getStaTrainById(pRoute->m_nTrainId);
				if (pTrafficLog) {
					strTrainNum = pRoute->m_bArrivaRoute ? pTrafficLog->m_strArrivalTrainNum : pTrafficLog->m_strDepartTrainNum;
				}
				else if(pTrain) {
					strTrainNum = pTrain->m_strTrainNum;
				}
				vecTableData.append(GetTrainRouteTableDataByRoute(pRoute->m_strTrainNum, pRoute));
				pRoute->m_nRowIndex = nRowIndex++;
				for (int nRouteId : pRoute->m_vecSubRouteId) {
					if (nRouteId != 0) {
						vecTableData.append(GetTrainRouteTableDataByRoute(strTrainNum, MainStation()->getStaTrainRouteById(nRouteId)));
						pRoute->m_nRowIndex = nRowIndex++;
					}
				}
			}
			m_pTrainRouteTable->ResetTableRows(vecTableData);
			TrainRouteColorUpData();
		}

		void StaRoutePlan::TrainRouteColorUpData()
		{
			QVector<StaTrainRoute*> vecStaTrainRoute = MainStation()->TrainRouteList();
			StaTrain* pTrain = nullptr;
			for (StaTrainRoute* pRoute : MainStation()->TrainRouteList()) {
				if (pRoute->m_nRouteState == 1) {
					QFont font = m_pTrainRouteTable->font();
					font.setBold(true);
					font.setItalic(true);
					m_pTrainRouteTable->SetRowItemsTextFont(pRoute->m_nRowIndex, font);
				}

				if (pRoute->m_bTempRoute) {
					m_pTrainRouteTable->SetRowItemsColor(pRoute->m_nRowIndex, Qt::gray, Qt::black);
					continue;
				}

				pTrain = MainStation()->getStaTrainById(pRoute->m_nTrainId);

				QColor textColor;
				if (pTrain) {
					textColor = pTrain->m_bFreightTrain ? Qt::black : Qt::red;
				}
				else {
					textColor = Qt::black;
				}

				QColor bkColor;
				if (pRoute->m_vecSubRouteId.size()) {
					bkColor = Qt::yellow;
				}
				else {

					switch (pRoute->m_nRouteState) {
					case 0:
					case 5: bkColor = Qt::yellow; break;
					case 1:
					case 2:
					case 3: bkColor = Qt::green; break;
					case 4: bkColor = Qt::gray; break;
					}
				}

				m_pTrainRouteTable->SetRowItemsColor(pRoute->m_nRowIndex, bkColor, textColor);
			}
		}

		void StaRoutePlan::InitTrackChangeMenu(const QModelIndex& index, StaTrainRoute* pTrainRoute)
		{
			if (pTrainRoute->m_bSunTrainRoute) {
				return;
			}
			QMenu* pMenu = new QMenu();
			pMenu->setAttribute(Qt::WA_DeleteOnClose);
			QAction* pAction = new QAction("����ɵ�");
			pAction->setEnabled(false);
			pMenu->addAction(pAction);
			pMenu->addSeparator();
			Device::StaTrack* pTrack = nullptr;
			for (Device::DeviceBase* pDevice : MainStation()->getDeviceVectorByType(TRACK)) {
				pTrack = dynamic_cast<Device::StaTrack*>(pDevice);
				if (pTrack->TrackType() == "ZX_GD" || pTrack->TrackType() == "GD_QD") {
					QAction* pAction = new QAction(pTrack->getName());
					pAction->setCheckable(true);
					pMenu->addAction(pAction);
					if (pTrack->getName() == index.data()) {
						pAction->setChecked(true);
						pAction->setEnabled(false);
					}
					connect(pAction, &QAction::triggered, [=]() {
						OnTrackChange(pTrainRoute, pAction->text());
					});
				}
			}
			pMenu->exec(QCursor::pos());
		}

		void StaRoutePlan::InitTriggerMenu(const QModelIndex& index, StaTrainRoute* pTrainRoute)
		{
			if (pTrainRoute->m_vecSubRouteId.size()) {
				return;
			}
			QMenu* pMenu = new QMenu();
			pMenu->setAttribute(Qt::WA_DeleteOnClose);
			QAction* pAction1 = new QAction("��������");
			pAction1->setEnabled(false);
			pAction1->setCheckable(true);
			pMenu->addAction(pAction1);
			pMenu->addSeparator();

			QAction* pAction2 = new QAction("�����Դ�");
			pAction2->setCheckable(true);
			pMenu->addAction(pAction2);
			connect(pAction2, &QAction::triggered, [=]() {
				OnTriggerTypeChange(pTrainRoute, true);
			});

			QAction* pAction3 = new QAction("ȡ���Դ�");
			pAction3->setCheckable(true);
			pMenu->addAction(pAction3);
			connect(pAction3, &QAction::triggered, [=]() {
				OnTriggerTypeChange(pTrainRoute, false);
			});

			if (pTrainRoute->m_bAutoTouch) {
				pAction2->setChecked(true);
				pAction2->setEnabled(false);
			}
			else {
				pAction3->setChecked(true);
				pAction3->setEnabled(false);
			}
			pMenu->exec(QCursor::pos());
		}

		void StaRoutePlan::InitRouteChangeMenu(const QModelIndex& index, StaTrainRoute* pTrainRoute)
		{
			if (pTrainRoute->m_vecSubRouteId.size()) {
				return;
			}

			QMenu* pMenu = new QMenu();
			QString strRouteTemp;
			for (QString strRoute : pTrainRoute->m_vecFlexibleRoute) {
				strRouteTemp = strRoute;
				strRouteTemp.replace(',', '-');
				QAction* pAction = new QAction(strRouteTemp);
				pAction->setCheckable(true);
				pMenu->addAction(pAction);
				if (strRouteTemp == index.data()) {
					pAction->setChecked(true);
					pAction->setEnabled(false);
				}
				strRoute.replace(',', '-');
				connect(pAction, &QAction::triggered, [=]() {
					if (pTrainRoute->m_nRouteState != 0) {
						QMessageBox::information(this, MSGBOX_TITTLE, "��·״̬�������޸Ľ�·!", "ȷ��");
						return;
					}
					QByteArray btResult;
					if (Http::HttpClient::ChangeRoute(pTrainRoute->m_nRouteId, strRoute, btResult)) {
						pTrainRoute->m_strRouteDescrip = strRoute;
					}
					OnTrainRouteUpData();
				});
			}
			pMenu->exec(QCursor::pos());
		}

		void StaRoutePlan::InitShuntRouteTable()
		{
			m_pShuntRouteTable = new Control::TableView(this);
			m_pShuntRouteTable->SetSectionText("˳��", 48);
			m_pShuntRouteTable->SetHeadData(GetShuntRouteTableHeadInfo(), HHead);
		}

		void StaRoutePlan::InitHookPlanTable()
		{
			m_pHookPlanTable = new Control::TableView(this);
			m_pHookPlanTable->SetSectionText("���ƻ�", 56);
			m_pHookPlanTable->SetHeadData(GetHookPlanTableHeadInfo(), HHead);
		}

		void StaRoutePlan::OnFontSizeChange(int nIndex)
		{
			QFont font = m_pTrainRouteTable->font();
			int size = font.pointSize() + nIndex;
			font.setPointSize(9 + nIndex);
			m_pTrainRouteTable->setFont(font);
			m_pShuntRouteTable->setFont(font);
			m_pHookPlanTable->setFont(font);
			m_pTrainRouteTable->update();
		}

		void StaRoutePlan::TrainRouteTableLeftMenu(const QModelIndex& index)
		{
			if (MainStation()->IsAllowStaOperation()) {
				return;
			}
			StaTrainRoute* pTrainRoute = MainStation()->getStaTrainRouteByRowIndex(index.row());
			if (!pTrainRoute) {
				return;
			}
			if (index.column() == m_nTrackRow) {	//���Ĺɵ�
				InitTrackChangeMenu(index, pTrainRoute);
			}
			else if (index.column() == m_nTriggerRow) {	//�����Դ�
				InitTriggerMenu(index, pTrainRoute);
			}
			else if (index.column() == m_nRouteRow) {	//�����·
				InitRouteChangeMenu(index, pTrainRoute);
			}
		}

		void StaRoutePlan::TrainRouteTableRightMenu(const QPoint& pt)
		{
			if (MainStation()->IsAllowStaOperation()) {
				return;
			}
			QModelIndex index = m_pTrainRouteTable->indexAt(pt);
			if (index.row() >= 0 && index.column() >= 0) {
				StaTrainRoute* pTrainRoute = MainStation()->getStaTrainRouteByRowIndex(index.row());
				if (!pTrainRoute) {
					return;
				}
				QMenu* pMenu = new QMenu();
				QAction* pAction1 = new QAction("�˹�����");
				pMenu->addAction(pAction1);
				connect(pAction1, &QAction::triggered, [=]() {
					QString routeDescrip;
					if (pTrainRoute->m_vecSubRouteId.size()) {
						StaTrainRoute* pSubTrainRoute = nullptr;
						for (int nSubRouteId : pTrainRoute->m_vecSubRouteId) {
							pSubTrainRoute = MainStation()->getStaTrainRouteById(nSubRouteId);
							routeDescrip += pSubTrainRoute->m_strRouteDescrip;
							routeDescrip += ",";
						}
						routeDescrip = routeDescrip.left(routeDescrip.length() - 1);
					}
					else {
						routeDescrip = pTrainRoute->m_strRouteDescrip;
					}
					StaTrain* pTrain = MainStation()->getStaTrainById(pTrainRoute->m_nTrainId);
					QString strTips = QString("ȷ��Ҫ����'�ɵ�:%1','%2'���г�,'%3','%4'��·?")
						.arg(pTrainRoute->m_strTrack)
						.arg(pTrain->m_strTrainNum)
						.arg(routeDescrip)
						.arg(pTrainRoute->m_bArrivaRoute ? "�ӳ�" : "����");

					if (QMessageBox::question(nullptr, MSGBOX_TITTLE, strTips, "��", "��") == 0) {
						if (pTrainRoute->m_vecSubRouteId.size()) {
							StaTrainRoute* pSubTrainRoute = nullptr;
							QByteArray btResult;
							for (int nSubRouteId : pTrainRoute->m_vecSubRouteId) {
								pSubTrainRoute = MainStation()->getStaTrainRouteById(nSubRouteId);
								MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x61, pSubTrainRoute->m_nRouteId, 0x02);
								
								pSubTrainRoute->m_nRouteState = 1;
								if (Http::HttpClient::UpdataRouteState(pTrainRoute->m_nRouteId, pSubTrainRoute->m_nRouteState, btResult)) {
									//MainStation()->TrainRouteList().removeOne(pTrainRoute);
								}
							}
						}
						else {
							MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x61, pTrainRoute->m_nRouteId, 0x02);
							QByteArray btResult;
							pTrainRoute->m_nRouteState = 1;
							if (Http::HttpClient::UpdataRouteState(pTrainRoute->m_nRouteId, pTrainRoute->m_nRouteState, btResult)) {
								//MainStation()->TrainRouteList().removeOne(pTrainRoute);
							}
						}
					}
				});

				QAction* pAction2 = new QAction("��ϸ");
				pMenu->addAction(pAction2);
				connect(pAction2, &QAction::triggered, [=]() {
					
				});
				QAction* pAction3 = new QAction("վ����ʾ");
				pMenu->addAction(pAction3);
				pAction3->setEnabled(false);

				pMenu->addSeparator();
				QAction* pAction4 = new QAction("ɾ��");
				pMenu->addAction(pAction4);
				connect(pAction4, &QAction::triggered, [=]() {
					StaTrain* pTrain = MainStation()->getStaTrainById(pTrainRoute->m_nTrainId);
					QString strTips = QString("ɾ����%1���г���·('�ɵ�:%2','%3'���г�,'%4'��·)?")
						.arg(pTrainRoute->m_nRowIndex + 1)
						.arg(pTrainRoute->m_strTrack)
						.arg(pTrain->m_strTrainNum)
						.arg((pTrainRoute->m_bArrivaRoute == 0) ? "�ӳ�" : "����");

					if (QMessageBox::question(nullptr, "��Ϣ�Ի���", strTips, "��", "��") == 0) {
						QByteArray btResult;
						if (pTrainRoute->m_vecSubRouteId.size()) {
							StaTrainRoute* pSubTrainRoute = nullptr;
							for (int nSubRouteId : pTrainRoute->m_vecSubRouteId) {
								if (Http::HttpClient::DeleteStaTrainRoute(nSubRouteId, btResult)) {
									MainStation()->TrainRouteList().removeOne(MainStation()->getStaTrainRouteById(nSubRouteId));
								}
							}
						}
						if (Http::HttpClient::DeleteStaTrainRoute(pTrainRoute->m_nRouteId, btResult)) {
							MainStation()->TrainRouteList().removeOne(pTrainRoute);
						}
					}
				});
				pMenu->exec(QCursor::pos());
			}
		}

		void StaRoutePlan::OnTrackChange(StaTrainRoute* pTrainRoute, QString strTrack)
		{
			QVector<StaTrainRoute*> vecTempRouteOrder = MainStation()->getStaTrainRouteByTrain(pTrainRoute->m_nTrainId);
			for (StaTrainRoute* pRoute : vecTempRouteOrder) {
				if (pRoute->m_nRouteState != 0) {
					QMessageBox::information(this, MSGBOX_TITTLE, "��·״̬�������޸ĵ���ɵ�!", "ȷ��");
					return;
				}
			}
			Device::DeviceBase* pTrack = MainStation()->getDeviceByName(strTrack);
			if (pTrack) {
				QByteArray btResult;
				for (StaTrainRoute* pRoute : vecTempRouteOrder) {
					if (Http::HttpClient::ChangeRouteTrack(pRoute->m_nRouteId, pTrack->getCode(), btResult)) {
						pRoute->ChangeTrack(pTrack->getCode(), pTrack->getName());
						MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x61, pRoute->m_nRouteId, 0x04);
					}
				}
				OnTrainRouteUpData();
			}
		}

		void StaRoutePlan::OnTriggerTypeChange(StaTrainRoute* pTrainRoute, bool bAutoTouch)
		{
			if (pTrainRoute->m_nRouteState != 0) {
				QMessageBox::information(this, MSGBOX_TITTLE, "��·״̬�������޸Ĵ�����ʽ!", "ȷ��");
				return;
			}

			QByteArray btResult;
			if (Http::HttpClient::ChangeTriggerType(pTrainRoute->m_nRouteId, bAutoTouch ? 1 : 2, btResult)) {
				pTrainRoute->m_bAutoTouch = bAutoTouch;
			}
			OnTrainRouteUpData();
		}

		void StaRoutePlan::timerEvent(QTimerEvent* event)
		{
			if (event->timerId() == m_nTimerId_15) {	//�Զ��������
				QByteArray btResult;
				for (StaTrainRoute* pTrainRoute : MainStation()->TrainRouteList()) {
					if (pTrainRoute->m_vecSubRouteId.size() == 0 && pTrainRoute->m_bAutoTouch) {
						if (pTrainRoute->m_tPlanTime.toTime_t() >= QDateTime::currentDateTime().toTime_t()) {
							pTrainRoute->m_tTriggerTime = QDateTime::currentDateTime();
							MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x61, pTrainRoute->m_nRouteId, 0x02);
						}
					}
				}
			}
			return QWidget::timerEvent(event);
		}
	}
}