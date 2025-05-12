#include "StaAddNewTrainKSK.h"
#include "Global.h"
#include "StationObject/GlobalStruct.h" 
#pragma execution_character_set("utf-8")
#include <QMessageBox>
StaAddNewTrainKSK::StaAddNewTrainKSK(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitAddView();
	ConnectEvent();
}

StaAddNewTrainKSK::~StaAddNewTrainKSK()
{
}

void StaAddNewTrainKSK::InitAddView()
{
	int i = 0;
	this->setWindowTitle("新增列车信息");
	ui.comboBox->addItem(Station::MainStation()->getStationName());
	m_mapPassengeTrain = Station::MainStation()->getPassengeTrain();
	for (auto it = m_mapPassengeTrain.constBegin(); it != m_mapPassengeTrain.constEnd(); ++it) {
		ui.comboBox_2->addItem(it.value());
		if (i > 3) { break; }
		i++;
	}
	m_mapFreighTrain = Station::MainStation()->getFreighTrain();
	for (auto it = m_mapFreighTrain.constBegin(); it != m_mapFreighTrain.constEnd(); ++it) {
		ui.comboBox_2->addItem(it.value());
	}
	m_mapTrainType = Station::MainStation()->getTrainType();
	for (auto it = m_mapTrainType.constBegin(); it != m_mapTrainType.constEnd(); ++it) {
		ui.comboBox_7->addItem(it.value());
	}
	ui.comboBox_6->addItem("");
	ui.comboBox_11->addItem("");
	ui.comboBox_9->addItem("");
	ui.comboBox_4->addItem("");
	ui.comboBox_5->addItem("");
	ui.comboBox_10->addItem("");
	Station::Device::StaTrack* pTrack = nullptr;
	Station::Device::StaSignal* pSignal = nullptr;
	for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(TRACK)) {
		pTrack = dynamic_cast<Station::Device::StaTrack*>(pDevice);
		if (pTrack->TrackType() == "GD_QD") {
			ui.comboBox_6->addItem(pDevice->getName());
			ui.comboBox_11->addItem(pDevice->getName());
		}
	}
	for (Station::Device::DeviceBase* pDevice : Station::MainStation()->getDeviceVectorByType(SIGNALLAMP)) {
		pSignal = dynamic_cast<Station::Device::StaSignal*>(pDevice);
		if (pSignal->getXHDTYpe() == "JZ_XHJ") {
			ui.comboBox_9->addItem(pDevice->getName());
			ui.comboBox_4->addItem(pDevice->getName());
			ui.comboBox_5->addItem(pSignal->getDirection());
			ui.comboBox_10->addItem(pSignal->getDirection());
		}

	}
	begin = true;
	end = true;
}

void StaAddNewTrainKSK::ConnectEvent()
{
	ui.comboBox_5->setDisabled(true);
	ui.comboBox_10->setDisabled(true);
	connect(ui.checkBox, &QCheckBox::stateChanged, [=]() {
		if (begin) {
			ui.lineEdit->setDisabled(true);
		
			ui.comboBox_3->setDisabled(true);
			ui.comboBox_4->setDisabled(true);
			ui.comboBox_6->setDisabled(true);
			ui.lineEdit->clear();
			ui.comboBox_4->setCurrentIndex(0);
			ui.comboBox_6->setCurrentIndex(0);
			begin = false;
		}
		else {
			ui.lineEdit->setDisabled(false);
			ui.comboBox_3->setDisabled(false);
			ui.comboBox_4->setDisabled(false);
			ui.comboBox_6->setDisabled(false);
			begin = true;
		}
		});
	connect(ui.checkBox_2, &QCheckBox::stateChanged, [=]() {
		if (end) {
			ui.lineEdit_2->setDisabled(true);
			ui.comboBox_8->setDisabled(true);
			ui.comboBox_9->setDisabled(true);
			ui.comboBox_11->setDisabled(true);
			ui.lineEdit_2->clear();
			ui.comboBox_9->setCurrentIndex(0);
			ui.comboBox_11->setCurrentIndex(0);
			end = false;
		}
		else {
			ui.lineEdit_2->setDisabled(false);
			ui.comboBox_8->setDisabled(false);
			ui.comboBox_9->setDisabled(false);
			ui.comboBox_11->setDisabled(false);
			end = true;
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

	connect(ui.pushButton_2, &QPushButton::clicked, [=]() {
		Station::Device::DeviceBase* pTrackDevice = nullptr;
		Station::Device::DeviceBase* pSignalDevice = nullptr;
		
		Station::StaTrafficLog* pTrafficLog = new Station::StaTrafficLog();
		Station::StaTrain* pTrain = new Station::StaTrain(pTrafficLog);
		//判断不能同时终到始发
		if (ui.checkBox->checkState() == Qt::Checked && ui.checkBox_2->checkState() == Qt::Checked)
		{
			QMessageBox::warning(this, tr("CTC"), tr("不能同时选择始发终到！"), tr("确定"), "", 0);
			return;
		}
		if (ui.comboBox_9->currentIndex() == 0 && ui.checkBox_2->checkState() == Qt::Unchecked ||
			ui.comboBox_4->currentIndex() == 0 && ui.checkBox->checkState() == Qt::Unchecked||
			ui.lineEdit_2->text()==""&& ui.checkBox_2->checkState() == Qt::Unchecked||
			ui.lineEdit->text() == "" && ui.checkBox->checkState() == Qt::Unchecked
			) {
			QMessageBox::warning(this, tr("CTC"), tr("请确认数据数据填充完全！"), tr("确定"), "", 0);
			return;
		}
		//判断接发车时间
		if (ui.dateTimeEdit->text() == ui.dateTimeEdit_2->text()) {
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
		if (ui.checkBox_4->checkState() == Qt::Checked) { 
			pTrafficLog->m_bPassenger = true;
		}
		else {
			pTrafficLog->m_bPassenger = false;
		}
		//是否允许股道与基本径路不一致
		if (ui.checkBox_7->checkState() == Qt::Checked) {
			pTrafficLog->m_bAllowTrackNotMatch = true;
		}
		else {
			pTrafficLog->m_bAllowTrackNotMatch = false;
		}
		//是否允许出入口与基本径路不一致
		if (ui.checkBox_8->checkState() == Qt::Checked) {
			pTrafficLog->m_bAllowSignalNotMatch = true;
		}
		else {
			pTrafficLog->m_bAllowSignalNotMatch = false;
		}
		pTrain->m_strTrainNum = ui.lineEdit->text();
		//始发选中
		if (ui.checkBox->checkState() == Qt::Checked) {
			pTrain->m_strTrainNum = ui.lineEdit_2->text();
			pTrafficLog->m_nPlanType = 2;		
			pTrafficLog->m_strArrivalTrainNum = "";
			pTrafficLog->m_nArrivalLimit = 0;
			pTrafficLog->m_strArrivalTrack = "";
			pTrafficLog->m_tProvDepartTime = QDateTime::fromString("");
			pTrafficLog->m_strArrivaSignal ="";
			pTrafficLog->m_nArrivalSignalCode = -1;
			pTrafficLog->m_nArrivalTrackCode =-1;
		}
		else {
			pTrackDevice = Station::MainStation()->getDeviceByName(ui.comboBox_6->currentText());
			pSignalDevice = Station::MainStation()->getDeviceByName(ui.comboBox_4->currentText());
			pTrafficLog->m_strArrivalTrainNum = ui.lineEdit->text();
			pTrafficLog->m_nArrivalLimit = ui.comboBox_3->currentIndex() + 1;
			pTrafficLog->m_strArrivalTrack = ui.comboBox_6->currentText();
			pTrafficLog->m_tProvArrivalTime = QDateTime::fromString(ui.dateTimeEdit->text(), "yyyy/M/d h:m");
			pTrafficLog->m_strArrivaSignal = ui.comboBox_4->currentText();
			pTrafficLog->m_nArrivalSignalCode = pSignalDevice->getCode();
			pTrafficLog->m_nArrivalTrackCode = pTrackDevice->getCode();
		}
		//终到选中
		if (ui.checkBox_2->checkState() == Qt::Checked) {
			pTrafficLog->m_nPlanType = 3;
			pTrafficLog->m_strDepartTrainNum ="";
			pTrafficLog->m_nDepartLimit = 0;
			pTrafficLog->m_strDepartTrack = "";
			pTrafficLog->m_strDepartSignal = "";
			pTrafficLog->m_nDepartSignalCode = -1;
			pTrafficLog->m_nDepartTrackCode = -1;
		}
		else {
			pSignalDevice = Station::MainStation()->getDeviceByName(ui.comboBox_9->currentText());
			pTrackDevice = Station::MainStation()->getDeviceByName(ui.comboBox_11->currentText());
			pTrafficLog->m_strDepartTrainNum = ui.lineEdit_2->text();
			pTrafficLog->m_strDepartTrack = ui.comboBox_11->currentText();
			pTrafficLog->m_nDepartLimit = ui.comboBox_8->currentIndex() + 1;
			pTrafficLog->m_tProvDepartTime = QDateTime::fromString(ui.dateTimeEdit_2->text(), "yyyy/M/d h:m");
			pTrafficLog->m_strDepartSignal = ui.comboBox_9->currentText();
			pTrafficLog->m_nDepartSignalCode = pSignalDevice->getCode();
			pTrafficLog->m_nDepartTrackCode = pTrackDevice->getCode();
		}


		//添加车次运行类型，列车类型
		pTrain->m_strTrainType = ui.comboBox_2->currentText();
		pTrain->m_strOperationType = ui.comboBox_7->currentText();
		
		//重点
		if (ui.checkBox_5->checkState() == Qt::Checked) {
			pTrain->m_bImportant = true;
		}
		else {
			pTrain->m_bImportant = false;
		}
		//电力
		if (ui.checkBox_3->checkState() == Qt::Checked) {
			pTrain->m_bElectric = true;
		}
		else {
			pTrain->m_bElectric = false;
		}
		//军运
		if (ui.checkBox_6->checkState() == Qt::Checked) {
			pTrain->m_bArmy = true;
		}
		else {
			pTrain->m_bArmy = false;
		}	
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
		this->hide();
	});
	connect(ui.pushButton_3, &QPushButton::clicked, [=]() {
		this->hide();
	});
}
void StaAddNewTrainKSK::closeEvent(QCloseEvent* event)  {
	this->hide();
	event->ignore(); 
}

void StaAddNewTrainKSK::GetNowTime()
{
	QDate date = QDate::currentDate();
	QTime time = QTime::currentTime();

	// 设置日期和时间
	ui.dateTimeEdit->setDate(date);
	ui.dateTimeEdit->setTime(time);
	ui.dateTimeEdit_2->setDate(date);
	ui.dateTimeEdit_2->setTime(time);
}

void StaAddNewTrainKSK::ClearWidget()
{
	ui.dateTimeEdit->clear();
	ui.dateTimeEdit_2->clear();
	ui.comboBox->setCurrentIndex(0);
	ui.comboBox_2->setCurrentIndex(0);
	ui.comboBox_3->setCurrentIndex(0);
	ui.comboBox_4->setCurrentIndex(0);
	ui.comboBox_5->setCurrentIndex(0);
	ui.comboBox_6->setCurrentIndex(0);
	ui.comboBox_7->setCurrentIndex(0);
	ui.comboBox_8->setCurrentIndex(0);
	ui.comboBox_9->setCurrentIndex(0);
	ui.comboBox_10->setCurrentIndex(0);
	ui.comboBox_11->setCurrentIndex(0);
	ui.checkBox->setChecked(false);
	ui.checkBox_2->setChecked(false);
	ui.checkBox_3->setChecked(false);
	ui.checkBox_4->setChecked(false);
	ui.checkBox_5->setChecked(false);
	ui.checkBox_6->setChecked(false);
	ui.checkBox_7->setChecked(false);
	ui.checkBox_8->setChecked(false);
	ui.lineEdit->clear();
	ui.lineEdit_2->clear();
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


