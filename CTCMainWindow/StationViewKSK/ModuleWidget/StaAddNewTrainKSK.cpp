#include "StaAddNewTrainKSK.h"
#include "Global.h"
#include "StationObject/GlobalStruct.h" 
#include <QMessageBox>
StaAddNewTrainKSK::StaAddNewTrainKSK(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitAddView();
	ConnectEvent();
}

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
			ui.comboBox->addItem(Station::MainStation()->getStationName());

			QMap<int, QString> mapPassengeTrain = Station::MainStation()->getPassengeTrain();
			QMap<int, QString> mapFreighTrain = Station::MainStation()->getFreighTrain();
			QMap<int, QString> mapTrainType = Station::MainStation()->getTrainType();
			for (int nIndex : mapPassengeTrain.keys()) {
				ui.comboBox_2->insertItem(nIndex, mapPassengeTrain[nIndex]);
			}
			for (int nIndex : mapFreighTrain.keys()) {
				ui.comboBox_2->insertItem(nIndex, mapFreighTrain[nIndex]);
			}
			for (int nIndex : mapTrainType.keys()) {
				ui.comboBox_7->insertItem(nIndex, mapTrainType[nIndex]);
			}

			ui.comboBox_6->addItem("");
			ui.comboBox_11->addItem("");
			Station::Device::StaTrack* pTrack = nullptr;
			Station::Device::StaSignal* pSignal = nullptr;
			for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(TRACK)) {
				pTrack = dynamic_cast<Station::Device::StaTrack*>(pDevice);
				if (pTrack->TrackType() == "GD_QD") {
					ui.comboBox_6->addItem(pDevice->getName());
					ui.comboBox_11->addItem(pDevice->getName());
				}
			}

			ui.comboBox_9->addItem("");
			ui.comboBox_4->addItem("");
			ui.comboBox_5->addItem("");
			ui.comboBox_10->addItem("");
			for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(SIGNALLAMP)) {
				pSignal = dynamic_cast<Station::Device::StaSignal*>(pDevice);
				if (pSignal->getXHDTYpe() == "JZ_XHJ") {
					ui.comboBox_9->addItem(pSignal->getName());
					ui.comboBox_4->addItem(pSignal->getName());
					ui.comboBox_5->addItem(pSignal->getDirection());
					ui.comboBox_10->addItem(pSignal->getDirection());
				}

			}
			// 设置日期和时间
			ui.dateTimeEdit->setDateTime(QDateTime::currentDateTime());
			ui.dateTimeEdit_2->setDateTime(QDateTime::currentDateTime());
		}

		void StaAddNewTrainKSK::ConnectEvent()
		{
			ui.comboBox_5->setDisabled(true);
			ui.comboBox_10->setDisabled(true);
			connect(ui.checkBox, &QCheckBox::stateChanged, [=](int nState) {
				if (nState) {
					ui.lineEdit->setDisabled(true);
					ui.comboBox_3->setDisabled(true);
					ui.comboBox_4->setDisabled(true);
					ui.comboBox_6->setDisabled(true);
					ui.lineEdit->clear();
					ui.comboBox_4->setCurrentIndex(0);
					ui.comboBox_6->setCurrentIndex(0);
				}
				else {
					ui.lineEdit->setDisabled(false);
					ui.comboBox_3->setDisabled(false);
					ui.comboBox_4->setDisabled(false);
					ui.comboBox_6->setDisabled(false);
				}
			});

			connect(ui.checkBox_2, &QCheckBox::stateChanged, [=](int nState) {
				if (nState) {
					ui.lineEdit_2->setDisabled(true);
					ui.comboBox_8->setDisabled(true);
					ui.comboBox_9->setDisabled(true);
					ui.comboBox_11->setDisabled(true);
					ui.lineEdit_2->clear();
					ui.comboBox_9->setCurrentIndex(0);
					ui.comboBox_11->setCurrentIndex(0);
				}
				else {
					ui.lineEdit_2->setDisabled(false);
					ui.comboBox_8->setDisabled(false);
					ui.comboBox_9->setDisabled(false);
					ui.comboBox_11->setDisabled(false);
				}
			});

			Station::Device::DeviceBase* pDevice = nullptr;
			connect(ui.comboBox_3, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
				ui.comboBox_8->setCurrentIndex(index);
			});
			connect(ui.comboBox_4, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
				ui.comboBox_5->setCurrentIndex(index);
			});
			connect(ui.comboBox_9, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
				ui.comboBox_10->setCurrentIndex(index);
			});
			connect(ui.comboBox_6, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index) {
				ui.comboBox_11->setCurrentIndex(index);
			});
			connect(ui.pushButton, &QPushButton::clicked, [=]() {
				ui.lineEdit_2->setText(ui.lineEdit->text());
			});

			connect(ui.pushButton_2, &QPushButton::clicked, this, &StaAddNewTrainKSK::AddNewTrafficLog);

			connect(ui.pushButton_3, &QPushButton::clicked, this, &StaAddNewTrainKSK::close);
		}

		void StaAddNewTrainKSK::AddNewTrafficLog()
		{
			Station::StaTrafficLog* pTrafficLog = new Station::StaTrafficLog;
			//判断不能同时终到始发
			if (ui.checkBox->checkState() && ui.checkBox_2->checkState()) {
				QMessageBox::warning(this, tr("CTC"), tr("不能同时选择始发终到！"), tr("确定"), "", 0);
				return;
			}

			if ((ui.comboBox_9->currentIndex() == 0 && ui.checkBox_2->checkState() == Qt::Unchecked) ||
				(ui.comboBox_4->currentIndex() == 0 && ui.checkBox->checkState() == Qt::Unchecked)) {
				QMessageBox::warning(this, tr("CTC"), tr("请确认数据数据填充完全！"), tr("确定"), "", 0);
				return;
			}
			//判断接发车时间
			if (ui.dateTimeEdit->dateTime() == ui.dateTimeEdit_2->dateTime()) {
				pTrafficLog->m_nPlanType = 4;
			}
			else {
				pTrafficLog->m_nPlanType = 1;
			}

			//到达车次
			pTrafficLog->m_strArrivalTrainNum = ui.lineEdit->text();
			//出发车次
			pTrafficLog->m_strDepartTrainNum = ui.lineEdit_2->text();

			//办理客运
			pTrafficLog->m_bPassenger = ui.checkBox_4->checkState();
			//是否允许股道与基本径路不一致
			pTrafficLog->m_bAllowTrackNotMatch = ui.checkBox_7->checkState();
			//是否允许出入口与基本径路不一致
			pTrafficLog->m_bAllowSignalNotMatch = ui.checkBox_8->checkState();

			pTrafficLog->m_strArrivalTrainNum = ui.lineEdit->text();
			pTrafficLog->m_nArrivalLimit = ui.comboBox_3->currentIndex();
			pTrafficLog->m_strArrivalTrack = ui.comboBox_6->currentText();
			pTrafficLog->m_strArrivaSignal = ui.comboBox_4->currentText();
			pTrafficLog->m_strDepartTrainNum = ui.lineEdit_2->text();
			pTrafficLog->m_nDepartLimit = ui.comboBox_8->currentIndex();
			pTrafficLog->m_strDepartTrack = ui.comboBox_11->currentText();
			pTrafficLog->m_strDepartSignal = ui.comboBox_9->currentText();
			//始发
			if (ui.checkBox->checkState()) {
				pTrafficLog->m_nPlanType = 2;
				pTrafficLog->m_tProvArrivalTime = QDateTime::fromString("");
				pTrafficLog->m_nArrivalSignalCode = -1;
				pTrafficLog->m_nArrivalTrackCode = -1;
			}
			else {
				pTrafficLog->m_tProvArrivalTime = ui.dateTimeEdit->dateTime();
				pTrafficLog->m_nArrivalSignalCode = Station::MainStation()->getDeviceByName(ui.comboBox_4->currentText(), SIGNALLAMP)->getCode();
				pTrafficLog->m_nArrivalTrackCode = Station::MainStation()->getDeviceByName(ui.comboBox_6->currentText(), TRACK)->getCode();
			}
			//终到
			if (ui.checkBox_2->checkState()) {
				pTrafficLog->m_nPlanType = 3;
				pTrafficLog->m_tProvDepartTime = QDateTime::fromString("");
				pTrafficLog->m_nDepartSignalCode = -1;
				pTrafficLog->m_nDepartTrackCode = -1;
			}
			else {
				pTrafficLog->m_tProvDepartTime = ui.dateTimeEdit_2->dateTime();
				pTrafficLog->m_nDepartSignalCode = Station::MainStation()->getDeviceByName(ui.comboBox_9->currentText(), SIGNALLAMP)->getCode();
				pTrafficLog->m_nDepartTrackCode = Station::MainStation()->getDeviceByName(ui.comboBox_11->currentText(), TRACK)->getCode();
			}

			Station::StaTrain* pTrain = new Station::StaTrain(pTrafficLog);
			if (ui.checkBox->checkState()) {
				pTrain->m_strTrainNum = ui.lineEdit_2->text();
			}
			else {
				pTrain->m_strTrainNum = ui.lineEdit->text();
			}

			//添加车次运行类型，列车类型
			pTrain->m_strTrainType = ui.comboBox_2->currentText();
			pTrain->m_strOperationType = ui.comboBox_7->currentText();
			//重点
			pTrain->m_bImportant = ui.checkBox_5->checkState();
			//电力
			pTrain->m_bElectric = ui.checkBox_3->checkState();
			//军运
			pTrain->m_bArmy = ui.checkBox_6->checkState();
			//获取车次id，添加车次信息
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
			this->close();
		}
	}
}

void StaAddNewTrainKSK::SetTrainValue(Station::StaTrafficLog* m_pCurTrafficLog)
{
	ui.lineEdit->setText(m_pCurTrafficLog->m_strArrivalTrainNum);//到达车次号
	ui.lineEdit_2->setText(m_pCurTrafficLog->m_strDepartTrainNum);//发车车次号
	Station::StaTrain* pTrain = Station::MainStation()->getStaTrainById(m_pCurTrafficLog->m_nTrainId);//车次信息修改车次号
	int index  = ui.comboBox_2->findText(pTrain->m_strTrainType);//列车类型
	if (index != -1) {
		ui.comboBox_2->setCurrentIndex(index);
	}
	index = ui.comboBox_7->findText(pTrain->m_strOperationType);//运行类型
	if (index != -1) {
		ui.comboBox_7->setCurrentIndex(index);
	}
	ui.comboBox_3->setCurrentIndex(m_pCurTrafficLog->m_nArrivalLimit - 1);//到达超限
	ui.comboBox_8->setCurrentIndex(m_pCurTrafficLog->m_nDepartLimit - 1);//出发超限

	index = ui.comboBox_4->findText(m_pCurTrafficLog->m_strArrivaSignal);//接车口
	if (index != -1) {
		ui.comboBox_4->setCurrentIndex(index);	
		ui.comboBox_5->setCurrentIndex(index);//来向车站
	}

	index = ui.comboBox_9->findText(m_pCurTrafficLog->m_strDepartSignal);
	if (index != -1) {
		ui.comboBox_9->setCurrentIndex(index);//发车口
		ui.comboBox_10->setCurrentIndex(index);//去向车站
	}

	index = ui.comboBox_6->findText(m_pCurTrafficLog->m_strArrivalTrack); 	
	if (index != -1) {
		ui.comboBox_6->setCurrentIndex(index);//接车股道
	}
	index = ui.comboBox_11->findText(m_pCurTrafficLog->m_strDepartTrack);
	if (index != -1) {
		ui.comboBox_11->setCurrentIndex(index);//发车股道
	}
	
	
	if (m_pCurTrafficLog->m_tRealArrivalTime.isNull()) {//到达时间
		ui.dateTimeEdit->setDateTime(m_pCurTrafficLog->m_tProvArrivalTime);
	}
	else {
		ui.dateTimeEdit->setDateTime(m_pCurTrafficLog->m_tRealArrivalTime);
	}
	if (m_pCurTrafficLog->m_tRealDepartTime.isNull()) {//发车时间
		ui.dateTimeEdit_2->setDateTime(m_pCurTrafficLog->m_tProvDepartTime);
	}
	else {
		ui.dateTimeEdit_2->setDateTime(m_pCurTrafficLog->m_tRealDepartTime);
	}

}


