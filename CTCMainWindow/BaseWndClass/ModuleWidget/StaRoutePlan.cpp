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
			policy.setVerticalPolicy(QSizePolicy::MinimumExpanding); // 允许在垂直方向上缩小到最小大小，但也可以扩展
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
			m_pTrainRouteTable->SetSectionText("序", 36);
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
				/*从左向右找比key大的值*/
				while (RouteOrderCompare(*order1, key)) {
					if (++order1 == high) {
						break;
					}
				}
				/*从右向左找比key小的值*/
				while (RouteOrderCompare(key, *order2)) {
					if (--order2 == low) {
						break;
					}
				}
				if (order1 >= order2) {
					break;
				}
				/*交换i,j对应的值*/
				std::iter_swap(order1, order2);
			}
			/*中枢值与j对应值交换*/
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
			QAction* pAction = new QAction("变更股道");
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
			QAction* pAction1 = new QAction("触发类型");
			pAction1->setEnabled(false);
			pAction1->setCheckable(true);
			pMenu->addAction(pAction1);
			pMenu->addSeparator();

			QAction* pAction2 = new QAction("设置自触");
			pAction2->setCheckable(true);
			pMenu->addAction(pAction2);
			connect(pAction2, &QAction::triggered, [=]() {
				OnTriggerTypeChange(pTrainRoute, true);
			});

			QAction* pAction3 = new QAction("取消自触");
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
						QMessageBox::information(this, MSGBOX_TITTLE, "进路状态不允许修改进路!", "确定");
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
			m_pShuntRouteTable->SetSectionText("顺序", 48);
			m_pShuntRouteTable->SetHeadData(GetShuntRouteTableHeadInfo(), HHead);
		}

		void StaRoutePlan::InitHookPlanTable()
		{
			m_pHookPlanTable = new Control::TableView(this);
			m_pHookPlanTable->SetSectionText("勾计划", 56);
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
			if (index.column() == m_nTrackRow) {	//更改股道
				InitTrackChangeMenu(index, pTrainRoute);
			}
			else if (index.column() == m_nTriggerRow) {	//设置自触
				InitTriggerMenu(index, pTrainRoute);
			}
			else if (index.column() == m_nRouteRow) {	//变更进路
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
				QAction* pAction1 = new QAction("人工触发");
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
					QString strTips = QString("确定要触发'股道:%1','%2'次列车,'%3','%4'进路?")
						.arg(pTrainRoute->m_strTrack)
						.arg(pTrain->m_strTrainNum)
						.arg(routeDescrip)
						.arg(pTrainRoute->m_bArrivaRoute ? "接车" : "发车");

					if (QMessageBox::question(nullptr, MSGBOX_TITTLE, strTips, "是", "否") == 0) {
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

				QAction* pAction2 = new QAction("明细");
				pMenu->addAction(pAction2);
				connect(pAction2, &QAction::triggered, [=]() {
					
				});
				QAction* pAction3 = new QAction("站场显示");
				pMenu->addAction(pAction3);
				pAction3->setEnabled(false);

				pMenu->addSeparator();
				QAction* pAction4 = new QAction("删除");
				pMenu->addAction(pAction4);
				connect(pAction4, &QAction::triggered, [=]() {
					StaTrain* pTrain = MainStation()->getStaTrainById(pTrainRoute->m_nTrainId);
					QString strTips = QString("删除第%1条列车进路('股道:%2','%3'次列车,'%4'进路)?")
						.arg(pTrainRoute->m_nRowIndex + 1)
						.arg(pTrainRoute->m_strTrack)
						.arg(pTrain->m_strTrainNum)
						.arg((pTrainRoute->m_bArrivaRoute == 0) ? "接车" : "发车");

					if (QMessageBox::question(nullptr, "信息对话框", strTips, "是", "否") == 0) {
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
					QMessageBox::information(this, MSGBOX_TITTLE, "进路状态不允许修改到达股道!", "确定");
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
				QMessageBox::information(this, MSGBOX_TITTLE, "进路状态不允许修改触发方式!", "确定");
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
			if (event->timerId() == m_nTimerId_15) {	//自动触发检查
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