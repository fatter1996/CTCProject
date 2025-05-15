#include "StaAddNewTrainKSK.h"
#include "Global.h"
#include "StationObject/GlobalStruct.h" 
#include <QMessageBox>
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CASCO {
		StaAddNewTrainKSK::StaAddNewTrainKSK(QWidget* parent)
			: QWidget(parent)
		{
			ui.setupUi(this);
			setAttribute(Qt::WA_DeleteOnClose, true);
			setWindowTitle("新增列车信息");
			ConnectEvent();
		}

		StaAddNewTrainKSK::~StaAddNewTrainKSK()
		{
		}

		void StaAddNewTrainKSK::InitAddView()
		{
			m_bAddNew = true;
			m_pTrafficLog = new Station::StaTrafficLog;
			ui.stationname->addItem(Station::MainStation()->getStationName());

			QMap<int, QString> mapPassengeTrain = Station::MainStation()->getPassengeTrain();
			QMap<int, QString> mapFreighTrain = Station::MainStation()->getFreighTrain();
			QMap<int, QString> mapTrainType = Station::MainStation()->getTrainType();
			for (int nIndex : mapPassengeTrain.keys()) {
				ui.traintype->insertItem(nIndex, mapPassengeTrain[nIndex]);
			}
			for (int nIndex : mapFreighTrain.keys()) {
				ui.traintype->insertItem(nIndex, mapFreighTrain[nIndex]);
			}
			for (int nIndex : mapTrainType.keys()) {
				ui.runtype->insertItem(nIndex, mapTrainType[nIndex]);
			}

			ui.pickupTrack->addItem("");
			ui.hairtrack->addItem("");
			Station::Device::StaTrack* pTrack = nullptr;
			Station::Device::StaSignal* pSignal = nullptr;
			for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(TRACK)) {
				pTrack = dynamic_cast<Station::Device::StaTrack*>(pDevice);
				if (pTrack->TrackType() == "GD_QD") {
					ui.pickupTrack->addItem(pDevice->getName());
					ui.hairtrack->addItem(pDevice->getName());
				}
			}

			ui.hair->addItem("");
			ui.pickup->addItem("");
			ui.Come->addItem("");
			ui.Head->addItem("");
			for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(SIGNALLAMP)) {
				pSignal = dynamic_cast<Station::Device::StaSignal*>(pDevice);
				if (pSignal->getXHDTYpe() == "JZ_XHJ") {
					ui.hair->addItem(pSignal->getName());
					ui.pickup->addItem(pSignal->getName());
					ui.Come->addItem(pSignal->getDirection());
					ui.Head->addItem(pSignal->getDirection());
				}

			}
			// 设置日期和时间
			ui.arrivaltime->setDateTime(QDateTime::currentDateTime());
			ui.departuretime->setDateTime(QDateTime::currentDateTime());
		}

		void StaAddNewTrainKSK::InitAddView(Station::StaTrafficLog* m_pCurTrafficLog)
		{
			m_bAddNew = true;
			m_pTrafficLog = m_pCurTrafficLog;
			QVector<Station::StaTrain*> pt = Station::MainStation()->TrainList();
			ui.stationname->addItem(Station::MainStation()->getStationName());
			ui.Arrival->setText(m_pCurTrafficLog->m_strArrivalTrainNum);	//到达车次号
			ui.setoff->setText(m_pCurTrafficLog->m_strDepartTrainNum);		//发车车次号
			Station::StaTrain* pTrain = Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId);	//车次信息修改车次号
			ui.traintype->setCurrentText(pTrain->m_strTrainType);	//列车类型
			ui.runtype->setCurrentText(pTrain->m_strOperationType);	//运行类型
			ui.Reach->setCurrentIndex(m_pCurTrafficLog->m_nArrivalLimit);	//到达超限
			ui.Departure->setCurrentIndex(m_pCurTrafficLog->m_nDepartLimit);	//出发超限
			ui.pickup->setCurrentText(m_pCurTrafficLog->m_strArrivalSignal);		//接车口
			ui.Come->setCurrentIndex(ui.pickup->findText(m_pCurTrafficLog->m_strArrivalSignal));	//来向车站
			ui.hair->setCurrentText(m_pCurTrafficLog->m_strDepartSignal);	//发车口
			ui.Head->setCurrentIndex(ui.hair->findText(m_pCurTrafficLog->m_strDepartSignal));	//去向车站
			ui.pickupTrack->setCurrentText(m_pCurTrafficLog->m_strArrivalTrack);	//接车股道
			ui.hairtrack->setCurrentText(m_pCurTrafficLog->m_strDepartTrack);		//发车股道

			if (m_pCurTrafficLog->m_nPlanType == 0x02) {
				ui.startbegin->setCheckState(Qt::Checked);
			}
			else if (m_pCurTrafficLog->m_nPlanType == 0x03) {
				ui.theend->setCheckState(Qt::Checked);
			}

			ui.imp->setCheckState(pTrain->m_bImportant ? Qt::Checked : Qt::Unchecked);	//重点
			ui.electricity->setCheckState(pTrain->m_bElectric ? Qt::Checked : Qt::Unchecked);	//电力
			ui.army->setCheckState(pTrain->m_bArmy ? Qt::Checked : Qt::Unchecked);	//军运
			ui.passenger->setCheckState(m_pCurTrafficLog->m_bPassenger ? Qt::Checked : Qt::Unchecked);	//客运
			ui.railwaytrack->setCheckState(m_pCurTrafficLog->m_bAllowTrackNotMatch ? Qt::Checked : Qt::Unchecked);	//股道不一致
			ui.exit->setCheckState(m_pCurTrafficLog->m_bAllowSignalNotMatch ? Qt::Checked : Qt::Unchecked);	//出入口不一致
			ui.arrivaltime->setDateTime(m_pCurTrafficLog->m_tProvArrivalTime);	//到达时间
			ui.departuretime->setDateTime(m_pCurTrafficLog->m_tProvDepartTime);	//发车时间
		}

		void StaAddNewTrainKSK::ConnectEvent()
		{
			ui.Come->setDisabled(true);
			ui.Head->setDisabled(true);
			connect(ui.startbegin, &QCheckBox::stateChanged, [=](int nState) {
				if (nState) {
					ui.Arrival->setDisabled(true);
					ui.Reach->setDisabled(true);
					ui.pickup->setDisabled(true);
					ui.pickupTrack->setDisabled(true);
					ui.Arrival->clear();
					ui.pickup->setCurrentIndex(0);
					ui.pickupTrack->setCurrentIndex(0);
				}
				else {
					ui.Arrival->setDisabled(false);
					ui.Reach->setDisabled(false);
					ui.pickup->setDisabled(false);
					ui.pickupTrack->setDisabled(false);
				}
			});

			connect(ui.theend, &QCheckBox::stateChanged, [=](int nState) {
				if (nState) {
					ui.setoff->setDisabled(true);
					ui.Departure->setDisabled(true);
					ui.hair->setDisabled(true);
					ui.hairtrack->setDisabled(true);
					ui.setoff->clear();
					ui.hair->setCurrentIndex(0);
					ui.hairtrack->setCurrentIndex(0);
				}
				else {
					ui.setoff->setDisabled(false);
					ui.Departure->setDisabled(false);
					ui.hair->setDisabled(false);
					ui.hairtrack->setDisabled(false);
				}
			});

			connect(ui.sure, &QPushButton::clicked, [=]() {
				AddNewTrafficLog();
			});

			connect(ui.cancel, &QPushButton::clicked, this, &StaAddNewTrainKSK::close);
		}

		void StaAddNewTrainKSK::AddNewTrafficLog()
		{
			Station::StaTrain* pTrain = nullptr;
			if (m_bAddNew) {
				pTrain = new Station::StaTrain(m_pTrafficLog);
			}
			else {
				pTrain = Station::MainStation()->getStaTrainById(m_pTrafficLog->m_nTrainId);
				if (!pTrain) {
					return;
				}
			}

			//判断不能同时终到始发
			if (ui.startbegin->checkState() && ui.theend->checkState()) {
				QMessageBox::warning(this, tr("CTC"), tr("不能同时选择始发终到！"), tr("确定"), "", 0);
				return;
			}

			if ((ui.hair->currentIndex() == 0 && ui.theend->checkState() == Qt::Unchecked) ||
				(ui.pickup->currentIndex() == 0 && ui.startbegin->checkState() == Qt::Unchecked)||
				(ui.setoff->text() ==    "" && ui.theend->checkState() == Qt::Unchecked) ||
				(ui.Arrival->text() == "" && ui.startbegin->checkState() == Qt::Unchecked)) {
				QMessageBox::warning(this, tr("CTC"), tr("请确认数据数据填充完全！"), tr("确定"), "", 0);
				return;
			}

			

			//判断接发车时间
			if (ui.arrivaltime->dateTime() == ui.departuretime->dateTime()) {
				m_pTrafficLog->m_nPlanType = 4;
			}
			else {
				m_pTrafficLog->m_nPlanType = 1;
			}
			//到达车次
			m_pTrafficLog->m_strArrivalTrainNum = ui.Arrival->text();
			//出发车次
			m_pTrafficLog->m_strDepartTrainNum = ui.setoff->text();
			//办理客运
			m_pTrafficLog->m_bPassenger = ui.passenger->checkState();
			//是否允许股道与基本径路不一致
			m_pTrafficLog->m_bAllowTrackNotMatch = ui.railwaytrack->checkState();
			//是否允许出入口与基本径路不一致
			m_pTrafficLog->m_bAllowSignalNotMatch = ui.exit->checkState();

			m_pTrafficLog->m_nArrivalLimit = ui.Reach->currentIndex();
			m_pTrafficLog->m_strArrivalTrack = ui.pickupTrack->currentText();
			m_pTrafficLog->m_strArrivalSignal = ui.pickup->currentText();
			m_pTrafficLog->m_nDepartLimit = ui.Departure->currentIndex();
			m_pTrafficLog->m_strDepartTrack = ui.hairtrack->currentText();
			m_pTrafficLog->m_strDepartSignal = ui.hair->currentText();
			//始发
			if (ui.startbegin->checkState()) {
				m_pTrafficLog->m_nPlanType = 2;
				m_pTrafficLog->m_tProvArrivalTime = QDateTime::fromString("");
				m_pTrafficLog->m_nArrivalSignalCode = -1;
				m_pTrafficLog->m_nArrivalTrackCode = -1;
			}
			else {
				m_pTrafficLog->m_tProvArrivalTime = ui.arrivaltime->dateTime();
				m_pTrafficLog->m_nArrivalSignalCode = Station::MainStation()->getDeviceByName(ui.pickup->currentText(), SIGNALLAMP)->getCode();
				m_pTrafficLog->m_nArrivalTrackCode = Station::MainStation()->getDeviceByName(ui.pickupTrack->currentText(), TRACK)->getCode();
			}
			//终到
			if (ui.theend->checkState()) {
				m_pTrafficLog->m_nPlanType = 3;
				m_pTrafficLog->m_tProvDepartTime = QDateTime::fromString("");
				m_pTrafficLog->m_nDepartSignalCode = -1;
				m_pTrafficLog->m_nDepartTrackCode = -1;
			}
			else {
				m_pTrafficLog->m_tProvDepartTime = ui.departuretime->dateTime();
				m_pTrafficLog->m_nDepartSignalCode = Station::MainStation()->getDeviceByName(ui.hair->currentText(), SIGNALLAMP)->getCode();
				m_pTrafficLog->m_nDepartTrackCode = Station::MainStation()->getDeviceByName(ui.hairtrack->currentText(), TRACK)->getCode();
			}

			
			
			if (m_bAddNew) {
				//添加车次,获取车次id
				m_pTrafficLog->m_nTrainId = Station::MainStation()->AddNewTrain(pTrain);
				//添加行车日志信息，进路序列
				if (Station::MainStation()->AddNewTrafficLog(m_pTrafficLog)) {
					Station::MainStation()->CreatTrainRouteByTrafficLog(m_pTrafficLog);
				}
			}
			else {
				QByteArray btResult;
				QMap<QString, QByteArray> m_mapTrain = { 
					{ "trainNum", ui.startbegin->isChecked() ? ui.setoff->text().toLocal8Bit() : ui.Arrival->text().toLocal8Bit() },
					{ "trainTypeNumber", ui.traintype->currentText().toLocal8Bit() } ,
					{ "operationTypeNumber", ui.runtype->currentText().toLocal8Bit() } ,
					{ "keynote", QByteArray::number(ui.imp->isChecked()) } ,
					{ "electric", QByteArray::number(ui.electricity->isChecked()) },
					{ "army", QByteArray::number(ui.army->isChecked()) }
				};

				if (Http::HttpClient::UpdataStaTrainAttr(Station::MainStation()->getStationId(), m_mapTrain, btResult)) {
					Station::StaTrain* pTempTrain = new Station::StaTrain(*pTrain);
					if (ui.startbegin->isChecked()) {
						pTempTrain->m_strTrainNum = ui.setoff->text();
					}
					else {
						pTempTrain->m_strTrainNum = ui.Arrival->text();
					}
					pTempTrain->m_strTrainType = ui.traintype->currentText();
					pTempTrain->m_strOperationType = ui.runtype->currentText();
					pTempTrain->m_bImportant = ui.imp->isChecked();
					pTempTrain->m_bElectric = ui.electricity->isChecked();
					pTempTrain->m_bArmy = ui.army->isChecked();
					Station::MainStation()->AddTempTrain(pTempTrain);
					Station::MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x60, pTrain->m_nTrainId, 0x09);
				};


				QVector<Station::StaTrainRoute*> vecSubRoute;
				Station::StaTrainRoute* pArrivalRoute = Station::MainStation()->getStaTrainRouteById(m_pTrafficLog->m_nArrivalRouteId);
				
				if (pArrivalRoute) {
					vecSubRoute.append(pArrivalRoute->getSubTrainRouteList());
				}
				Station::StaTrainRoute* pDepartRoute = Station::MainStation()->getStaTrainRouteById(m_pTrafficLog->m_nDepartRouteId);
				if (pDepartRoute) {
					vecSubRoute.append(pDepartRoute->getSubTrainRouteList());
				}

				for (Station::StaTrainRoute* pRoute : vecSubRoute) {
					if (pRoute->m_nRouteState != 0) {
						QMessageBox::information(nullptr, MSGBOX_TITTLE, "进路状态不允许修改到达股道!", "确定");
						return;
					}
				}
				if (!Station::MainStation()->DeleteTrainRoute(vecSubRoute)) {
					return;
				}

				QMap<QString, QByteArray> m_mapLogAttr = { 
					{ "arrivalTrainNumber", m_pTrafficLog->m_strArrivalTrainNum.toLocal8Bit() },
					{ "departTrainNumber", m_pTrafficLog->m_strDepartTrainNum.toLocal8Bit() },
					{ "planType", QByteArray::number(m_pTrafficLog->m_nPlanType) },
					{ "passenger", QByteArray::number(m_pTrafficLog->m_bPassenger) },
					{ "allowTrackNotMatch", QByteArray::number(m_pTrafficLog->m_bAllowTrackNotMatch) },
					{ "allowSignalNotMatch", QByteArray::number(m_pTrafficLog->m_bAllowSignalNotMatch) },
					{ "arrivalLimit", QByteArray::number(m_pTrafficLog->m_nArrivalLimit) },
					{ "departLimit", QByteArray::number(m_pTrafficLog->m_nDepartLimit) },
					{ "homeSignalCode", QByteArray::number(m_pTrafficLog->m_nArrivalSignalCode) },
					{ "arrivalTrack", QByteArray::number(m_pTrafficLog->m_nArrivalTrackCode) },
					{ "provArrivalTime", m_pTrafficLog->m_tProvArrivalTime.toString(Qt::ISODate).toLocal8Bit() },
					{ "provDepartTime", m_pTrafficLog->m_tProvDepartTime.toString(Qt::ISODate).toLocal8Bit() },
					{ "departTrack", QByteArray::number(m_pTrafficLog->m_nDepartTrackCode) },
					{ "startingSignalCode", QByteArray::number(m_pTrafficLog->m_nDepartSignalCode) },
				};

				if (Http::HttpClient::UpdataStaTrafficLogAttr(m_pTrafficLog->m_nLogId, m_mapLogAttr, btResult)) {
					Station::MainStation()->CreatTrainRouteByTrafficLog(m_pTrafficLog);
				}
			}
			emit Station::MainStation()->TrafficLogTableUpData();
			this->close();
		}
	}
}


