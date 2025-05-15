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
			setWindowTitle("�����г���Ϣ");
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
			// �������ں�ʱ��
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
							QMessageBox::warning(this, tr("CTC"), tr("��·״̬�������޸ģ�"), tr("ȷ��"), "", 0);
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
							QMessageBox::warning(this, tr("CTC"), tr("��·״̬�������޸ģ�"), tr("ȷ��"), "", 0);
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
			//�жϲ���ͬʱ�յ�ʼ��
			if (ui.startbegin->checkState() && ui.theend->checkState()) {
				QMessageBox::warning(this, tr("CTC"), tr("����ͬʱѡ��ʼ���յ���"), tr("ȷ��"), "", 0);
				return;
			}

			if ((ui.hair->currentIndex() == 0 && ui.theend->checkState() == Qt::Unchecked) ||
				(ui.pickup->currentIndex() == 0 && ui.startbegin->checkState() == Qt::Unchecked)||
				(ui.setoff->text() ==    "" && ui.theend->checkState() == Qt::Unchecked) ||
				(ui.Arrival->text() == "" && ui.startbegin->checkState() == Qt::Unchecked)) {
				QMessageBox::warning(this, tr("CTC"), tr("��ȷ���������������ȫ��"), tr("ȷ��"), "", 0);
				return;
			}
			//�жϽӷ���ʱ��
			if (ui.arrivaltime->dateTime() == ui.departuretime->dateTime()) {
				pTrafficLog->m_nPlanType = 4;
			}
			else {
				pTrafficLog->m_nPlanType = 1;
			}

			//���ﳵ��
			pTrafficLog->m_strArrivalTrainNum = ui.Arrival->text();
			//��������
			pTrafficLog->m_strDepartTrainNum = ui.setoff->text();

			//�������
			pTrafficLog->m_bPassenger = ui.passenger->checkState();
			//�Ƿ�����ɵ��������·��һ��
			pTrafficLog->m_bAllowTrackNotMatch = ui.railwaytrack->checkState();
			//�Ƿ����������������·��һ��
			pTrafficLog->m_bAllowSignalNotMatch = ui.exit->checkState();

			pTrafficLog->m_nArrivalLimit = ui.Reach->currentIndex();
			pTrafficLog->m_strArrivalTrack = ui.pickupTrack->currentText();
			pTrafficLog->m_strArrivaSignal = ui.pickup->currentText();
			pTrafficLog->m_nDepartLimit = ui.Departure->currentIndex();
			pTrafficLog->m_strDepartTrack = ui.hairtrack->currentText();
			pTrafficLog->m_strDepartSignal = ui.hair->currentText();
			//ʼ��
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
			//�յ�
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

			//��ӳ����������ͣ��г�����
			pTrain->m_strTrainType = ui.traintype->currentText();
			pTrain->m_strOperationType = ui.runtype->currentText();
			//�ص�
			pTrain->m_bImportant = ui.imp->checkState();
			//����
			pTrain->m_bElectric = ui.electricity->checkState();
			//����
			pTrain->m_bArmy = ui.army->checkState();
			//��ȡ����id����ӳ�����Ϣ
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
				pTrain->m_nTrainId = Station::MainStation()->AddNewTrain(pTrain);//��վ�г�
				//��ȡ�г���־�г�id
				pTrafficLog->m_nTrainId = pTrain->m_nTrainId;

				//����г���־��Ϣ���г��ƻ�
				if (Station::MainStation()->AddNewTrafficLog(pTrafficLog) == 0) {
					qDebug() << "����г���־ʧ��";
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
			ui.Arrival->setText(m_pCurTrafficLog->m_strArrivalTrainNum);//���ﳵ�κ�
			ui.setoff->setText(m_pCurTrafficLog->m_strDepartTrainNum);//�������κ�
			Station::StaTrain* pTrain = Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId);//������Ϣ�޸ĳ��κ�
			int index = ui.traintype->findText(pTrain->m_strTrainType);//�г�����
			if (index != -1) {
				ui.traintype->setCurrentIndex(index);
			}
			index = ui.runtype->findText(pTrain->m_strOperationType);//��������
			if (index != -1) {
				ui.runtype->setCurrentIndex(index);
			}
			ui.Reach->setCurrentIndex(m_pCurTrafficLog->m_nArrivalLimit);//���ﳬ��
			ui.Departure->setCurrentIndex(m_pCurTrafficLog->m_nDepartLimit);//��������

			index = ui.pickup->findText(m_pCurTrafficLog->m_strArrivaSignal);//�ӳ���
			if (index != -1) {
				ui.pickup->setCurrentIndex(index);
				ui.Come->setCurrentIndex(index);//����վ
			}

			index = ui.hair->findText(m_pCurTrafficLog->m_strDepartSignal);
			if (index != -1) {
				ui.hair->setCurrentIndex(index);//������
				ui.Head->setCurrentIndex(index);//ȥ��վ
			}
			if(m_pCurTrafficLog->m_nPlanType == 0x02 ){
				ui.startbegin->setCheckState(Qt::Checked);
			}
			else if(m_pCurTrafficLog->m_nPlanType == 0x03){
				ui.theend->setCheckState(Qt::Checked);
			}
			index = ui.pickupTrack->findText(m_pCurTrafficLog->m_strArrivalTrack);
			if (index != -1) {
				ui.pickupTrack->setCurrentIndex(index);//�ӳ��ɵ�
			}
			index = ui.hairtrack->findText(m_pCurTrafficLog->m_strDepartTrack);
			if (index != -1) {
				ui.hairtrack->setCurrentIndex(index);//�����ɵ�
			}

			//�ص�
			if (pTrain->m_bImportant) {
				ui.imp->setCheckState(Qt::Checked);
			}
			else {
				ui.imp->setCheckState(Qt::Unchecked);
			}
			//����
			if (pTrain->m_bElectric) {
				ui.electricity->setCheckState(Qt::Checked);
			}
			else {
				ui.electricity->setCheckState(Qt::Unchecked);
			}
			//����
			if (pTrain->m_bArmy) {
				ui.army->setCheckState(Qt::Checked);
			}
			else {
				ui.army->setCheckState(Qt::Unchecked);
			}
			//����
			if (m_pCurTrafficLog->m_bPassenger) {
				ui.passenger->setCheckState(Qt::Checked);
			}
			else {
				ui.passenger->setCheckState(Qt::Unchecked);
			}
			//�ɵ���һ��
			if (m_pCurTrafficLog->m_bAllowTrackNotMatch) {
				ui.railwaytrack->setCheckState(Qt::Checked);
			}
			else {
				ui.railwaytrack->setCheckState(Qt::Unchecked);
			}
			//����ڲ�һ��
			if (m_pCurTrafficLog->m_bAllowSignalNotMatch) {
				ui.exit->setCheckState(Qt::Checked);
			}
			else {
				ui.exit->setCheckState(Qt::Unchecked);
			}

			if (m_pCurTrafficLog->m_tRealArrivalTime.isNull()) {//����ʱ��
				ui.arrivaltime->setDateTime(m_pCurTrafficLog->m_tProvArrivalTime);
			}
			else {
				ui.arrivaltime->setDateTime(m_pCurTrafficLog->m_tRealArrivalTime);
			}
			if (m_pCurTrafficLog->m_tRealDepartTime.isNull()) {//����ʱ��
				ui.departuretime->setDateTime(m_pCurTrafficLog->m_tProvDepartTime);
			}
			else {
				ui.departuretime->setDateTime(m_pCurTrafficLog->m_tRealDepartTime);
			}
			m_pTrafficLog = m_pCurTrafficLog;
		}
	}
}


