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

			Station::Device::DeviceBase* pDevice = nullptr;
			connect(ui.Reach, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
				ui.Departure->setCurrentIndex(index);
			});
			connect(ui.pickup, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
				ui.Come->setCurrentIndex(index);
			});
			connect(ui.hair, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
				ui.Head->setCurrentIndex(index);
			});
			connect(ui.pickupTrack, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
				if (m_pTrafficLog) {
					QVector<Station::StaTrainRoute*> pStaRoute = Station::MainStation()->getStaTrainRouteByTrain(m_pTrafficLog->m_nTrainId);
					for (int i = 0; i < pStaRoute.size(); i++) {
						if (pStaRoute[i]->m_nRouteState !=0 ) {
							QMessageBox::warning(this, tr("CTC"), tr("进路状态不允许修改！"), tr("确定"), "", 0);
						}
					}
				}
				ui.hairtrack->setCurrentIndex(index);
			});
			connect(ui.hairtrack, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
				if (m_pTrafficLog) {
					QVector<Station::StaTrainRoute*> pStaRoute = Station::MainStation()->getStaTrainRouteByTrain(m_pTrafficLog->m_nTrainId);
					for (int i = 0; i < pStaRoute.size(); i++) {
						if (pStaRoute[i]->m_nRouteState != 0) {
							QMessageBox::warning(this, tr("CTC"), tr("进路状态不允许修改！"), tr("确定"), "", 0);
						}
					}
				}
				});
			connect(ui.Match, &QPushButton::clicked, [=]() {
				ui.setoff->setText(ui.Arrival->text());
			});

			connect(ui.sure, &QPushButton::clicked, [=]() {
				AddNewTrafficLog(m_pTrafficLog);
				});

			connect(ui.cancel, &QPushButton::clicked, this, &StaAddNewTrainKSK::close);
		}

		void StaAddNewTrainKSK::AddNewTrafficLog(Station::StaTrafficLog* pCurTrafficLog)
		{
			bool Change = false;
			Station::StaTrafficLog* pTrafficLog = nullptr;
			if (pCurTrafficLog == nullptr) {
				pTrafficLog = new Station::StaTrafficLog;
			}
			else {
				pTrafficLog = pCurTrafficLog;
				Change = true;
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
				pTrafficLog->m_nPlanType = 4;
			}
			else {
				pTrafficLog->m_nPlanType = 1;
			}

			//到达车次
			pTrafficLog->m_strArrivalTrainNum = ui.Arrival->text();
			//出发车次
			pTrafficLog->m_strDepartTrainNum = ui.setoff->text();

			//办理客运
			pTrafficLog->m_bPassenger = ui.passenger->checkState();
			//是否允许股道与基本径路不一致
			pTrafficLog->m_bAllowTrackNotMatch = ui.railwaytrack->checkState();
			//是否允许出入口与基本径路不一致
			pTrafficLog->m_bAllowSignalNotMatch = ui.exit->checkState();

			pTrafficLog->m_nArrivalLimit = ui.Reach->currentIndex();
			pTrafficLog->m_strArrivalTrack = ui.pickupTrack->currentText();
			pTrafficLog->m_strArrivaSignal = ui.pickup->currentText();
			pTrafficLog->m_nDepartLimit = ui.Departure->currentIndex();
			pTrafficLog->m_strDepartTrack = ui.hairtrack->currentText();
			pTrafficLog->m_strDepartSignal = ui.hair->currentText();
			//始发
			if (ui.startbegin->checkState()) {
				pTrafficLog->m_nPlanType = 2;
				pTrafficLog->m_tProvArrivalTime = QDateTime::fromString("");
				pTrafficLog->m_nArrivalSignalCode = -1;
				pTrafficLog->m_nArrivalTrackCode = -1;
			}
			else {
				pTrafficLog->m_tProvArrivalTime = ui.arrivaltime->dateTime();
				pTrafficLog->m_nArrivalSignalCode = Station::MainStation()->getDeviceByName(ui.pickup->currentText(), SIGNALLAMP)->getCode();
				pTrafficLog->m_nArrivalTrackCode = Station::MainStation()->getDeviceByName(ui.pickupTrack->currentText(), TRACK)->getCode();
			}
			//终到
			if (ui.theend->checkState()) {
				pTrafficLog->m_nPlanType = 3;
				pTrafficLog->m_tProvDepartTime = QDateTime::fromString("");
				pTrafficLog->m_nDepartSignalCode = -1;
				pTrafficLog->m_nDepartTrackCode = -1;
			}
			else {
				pTrafficLog->m_tProvDepartTime = ui.departuretime->dateTime();
				pTrafficLog->m_nDepartSignalCode = Station::MainStation()->getDeviceByName(ui.hair->currentText(), SIGNALLAMP)->getCode();
				pTrafficLog->m_nDepartTrackCode = Station::MainStation()->getDeviceByName(ui.hairtrack->currentText(), TRACK)->getCode();
			}

			Station::StaTrain* pTrain = new Station::StaTrain(pTrafficLog);
			if (ui.startbegin->checkState()) {
				pTrain->m_strTrainNum = ui.setoff->text();
			}
			else {
				pTrain->m_strTrainNum = ui.Arrival->text();
			}

			//添加车次运行类型，列车类型
			pTrain->m_strTrainType = ui.traintype->currentText();
			pTrain->m_strOperationType = ui.runtype->currentText();
			//重点
			pTrain->m_bImportant = ui.imp->checkState();
			//电力
			pTrain->m_bElectric = ui.electricity->checkState();
			//军运
			pTrain->m_bArmy = ui.army->checkState();
			//获取车次id，添加车次信息
			if (Change) {
				Station::StaTrain* pStaTrain = Station::MainStation()->getStaTrainById(m_pTrafficLog->m_nTrainId);
				pTrain->m_nTrainId = pStaTrain->m_nTrainId;
				pStaTrain = pTrain;
				pTrafficLog->m_nTrainId = pTrain->m_nTrainId;
				m_pTrafficLog = pTrafficLog;
				QByteArray btResult;
				QMap<QString, QByteArray>m_mapTrain = { {"trainNum",pTrain->m_strTrainNum.toLocal8Bit()},
				{"trainTypeNumber",pTrain->m_strTrainType.toLocal8Bit()} ,
				{"operationTypeNumber",pTrain->m_strOperationType.toLocal8Bit()} ,
				{"keynote",QByteArray::number(pTrain->m_bImportant)} ,
				{"electric",QByteArray::number(pTrain->m_bElectric)},
				{"army",QByteArray::number(pTrain->m_bArmy)}};

				if (Http::HttpClient::ChangeStaTrain(Station::MainStation()->getStationId(), m_mapTrain, btResult)) {
				};
				QMap<QString, QByteArray>m_mapLogValue = { {"arrivalTrainNumber",pTrafficLog->m_strArrivalTrainNum.toLocal8Bit()},
					{"planType",QByteArray::number(pTrafficLog->m_nPlanType)},
					{"passenger",QByteArray::number(pTrafficLog->m_bPassenger)},
					{"allowTrackNotMatch",QByteArray::number(pTrafficLog->m_bAllowTrackNotMatch)},
					{"allowSignalNotMatch",QByteArray::number(pTrafficLog->m_bAllowSignalNotMatch)},
					{"departLimit",QByteArray::number(pTrafficLog->m_nDepartLimit)},
					{"homeSignalCode",QByteArray::number(pTrafficLog->m_nArrivalSignalCode)},
					{"arrivalTrack",QByteArray::number(pTrafficLog->m_nArrivalTrackCode)},
					{"provArrivalTime",pTrafficLog->m_tProvArrivalTime.toString(Qt::ISODate).toLocal8Bit() },
					{"provDepartTime",pTrafficLog->m_tProvDepartTime.toString(Qt::ISODate).toLocal8Bit()},
					{"departTrack",QByteArray::number(pTrafficLog->m_nDepartTrackCode)},
					{"startingSignalCode",QByteArray::number(pTrafficLog->m_nDepartSignalCode)},
				};
				if (Http::HttpClient::ChangeStaTrafficLogData(pTrafficLog->m_nLogId, m_mapLogValue, btResult)) {
					emit Station::MainStation()->TrafficLogTableUpData();
				}
				//Station::MainStation()->ChangeStaTrafficLogData(pTrafficLog);
				Change = false;
			}
			else {
				pTrain->m_nTrainId = Station::MainStation()->AddNewTrain(pTrain);//进站列车
				//获取行车日志列车id
				pTrafficLog->m_nTrainId = pTrain->m_nTrainId;

				//添加行车日志信息，行车计划
				if (Station::MainStation()->AddNewTrafficLog(pTrafficLog) == 0) {
					qDebug() << "添加行车日志失败";
				}
				if (Station::MainStation()->getAutoSendPlan()) {
					Station::MainStation()->CreatTrainRouteByTrafficLog(pTrafficLog);
				}
			}
			this->close();

		}

		void StaAddNewTrainKSK::InitAddView(Station::StaTrafficLog* m_pCurTrafficLog)
		{
			QVector<Station::StaTrain*> pt = Station::MainStation()->TrainList();
			ui.stationname->addItem(Station::MainStation()->getStationName());
			ui.Arrival->setText(m_pCurTrafficLog->m_strArrivalTrainNum);//到达车次号
			ui.setoff->setText(m_pCurTrafficLog->m_strDepartTrainNum);//发车车次号
			Station::StaTrain* pTrain = Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId);//车次信息修改车次号
			int index = ui.traintype->findText(pTrain->m_strTrainType);//列车类型
			if (index != -1) {
				ui.traintype->setCurrentIndex(index);
			}
			index = ui.runtype->findText(pTrain->m_strOperationType);//运行类型
			if (index != -1) {
				ui.runtype->setCurrentIndex(index);
			}
			ui.Reach->setCurrentIndex(m_pCurTrafficLog->m_nArrivalLimit);//到达超限
			ui.Departure->setCurrentIndex(m_pCurTrafficLog->m_nDepartLimit);//出发超限

			index = ui.pickup->findText(m_pCurTrafficLog->m_strArrivaSignal);//接车口
			if (index != -1) {
				ui.pickup->setCurrentIndex(index);
				ui.Come->setCurrentIndex(index);//来向车站
			}

			index = ui.hair->findText(m_pCurTrafficLog->m_strDepartSignal);
			if (index != -1) {
				ui.hair->setCurrentIndex(index);//发车口
				ui.Head->setCurrentIndex(index);//去向车站
			}
			if(m_pCurTrafficLog->m_nPlanType == 0x02 ){
				ui.startbegin->setCheckState(Qt::Checked);
			}
			else if(m_pCurTrafficLog->m_nPlanType == 0x03){
				ui.theend->setCheckState(Qt::Checked);
			}
			index = ui.pickupTrack->findText(m_pCurTrafficLog->m_strArrivalTrack);
			if (index != -1) {
				ui.pickupTrack->setCurrentIndex(index);//接车股道
			}
			index = ui.hairtrack->findText(m_pCurTrafficLog->m_strDepartTrack);
			if (index != -1) {
				ui.hairtrack->setCurrentIndex(index);//发车股道
			}

			//重点
			if (pTrain->m_bImportant) {
				ui.imp->setCheckState(Qt::Checked);
			}
			else {
				ui.imp->setCheckState(Qt::Unchecked);
			}
			//电力
			if (pTrain->m_bElectric) {
				ui.electricity->setCheckState(Qt::Checked);
			}
			else {
				ui.electricity->setCheckState(Qt::Unchecked);
			}
			//军运
			if (pTrain->m_bArmy) {
				ui.army->setCheckState(Qt::Checked);
			}
			else {
				ui.army->setCheckState(Qt::Unchecked);
			}
			//客运
			if (m_pCurTrafficLog->m_bPassenger) {
				ui.passenger->setCheckState(Qt::Checked);
			}
			else {
				ui.passenger->setCheckState(Qt::Unchecked);
			}
			//股道不一致
			if (m_pCurTrafficLog->m_bAllowTrackNotMatch) {
				ui.railwaytrack->setCheckState(Qt::Checked);
			}
			else {
				ui.railwaytrack->setCheckState(Qt::Unchecked);
			}
			//出入口不一致
			if (m_pCurTrafficLog->m_bAllowSignalNotMatch) {
				ui.exit->setCheckState(Qt::Checked);
			}
			else {
				ui.exit->setCheckState(Qt::Unchecked);
			}

			if (m_pCurTrafficLog->m_tRealArrivalTime.isNull()) {//到达时间
				ui.arrivaltime->setDateTime(m_pCurTrafficLog->m_tProvArrivalTime);
			}
			else {
				ui.arrivaltime->setDateTime(m_pCurTrafficLog->m_tRealArrivalTime);
			}
			if (m_pCurTrafficLog->m_tRealDepartTime.isNull()) {//发车时间
				ui.departuretime->setDateTime(m_pCurTrafficLog->m_tProvDepartTime);
			}
			else {
				ui.departuretime->setDateTime(m_pCurTrafficLog->m_tRealDepartTime);
			}
			m_pTrafficLog = m_pCurTrafficLog;
		}
	}
}


