#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QtConcurrent>

#include "CTCObject.h"
#include "StationViewKSK/StationViewKSK.h"
#include "StationViewTKY/StationViewTKY.h"
#include "StationViewTH/StationViewTH.h"
#include "CommonWidget/UserLoginDlg.h" 
#include "Global.h"

namespace CTCDoc{
	using namespace CTCWindows;
	using namespace Station;

	CTCObject::CTCObject()
	{
		m_pSocketTCP = new Socket::SocketTCP;
		m_pSocketUDP = new Socket::SocketUDP;

		StationObject::InitCreatDeviceMap();
		
		m_pMainStation = new MainStationObject;
		SetMainStation(m_pMainStation);
		if (ConfigFileInit() < 0) {
			qDebug() << "配置文件解析失败。";
		}
		if (TtainTypeFileInit() < 0) {
			qDebug() << "车辆类型解析失败。";
		}

		//初始化站场设备
		m_pMainStation->InitStaDevice();
		//网络通信初始化
		SocketComInit();
	}

	CTCObject::~CTCObject()
	{
		delete m_pMainStation;
		m_pMainStation = nullptr;

		delete m_pSocketUDP;
		m_pSocketUDP = nullptr;
	}

	QMainWindow* CTCObject::CreatCTCMainWnd()
	{
		switch (CTCObject::m_nStationViewType) {
		case 1: m_pCTCMainWindow = CASCO::StationViewKSK::CreatStationView(); break;
		case 2: m_pCTCMainWindow = CARS::StationViewTKY::CreatStationView(); break;
		case 3: m_pCTCMainWindow = TH::StationViewTH::CreatStationView(); break;
		case 4: break;
		default: break;
		}
		
		UserLogin();
		if (m_pCTCMainWindow) {
			//初始化主界面
			CTCWindows::SetMainWindow(m_pCTCMainWindow);
			m_pCTCMainWindow->InitStattionView();
			m_pCTCMainWindow->SetShowToolbar(m_bShowToolbarBtn, m_bShowToolbarLabel);
			m_pCTCMainWindow->setFixedSize(m_pMainStation->getStaFixedSize());
			m_pMainStation->InitDeviceEventFilter(m_pCTCMainWindow->StaPaintView());
			StaOperationConnect();
			//车站命令信息
			//m_pMainStation->SelectStationOrder();
			m_pCTCMainWindow->MultiDisp()->InitMultiStation(m_vecMultiStation);
			QtConcurrent::run(m_pMainStation, &MainStationObject::SelectStationOrder);
			m_pMainStation->OnLine();
		}
		
		return m_pCTCMainWindow;
	}

	int CTCObject::ConfigFileInit()
	{
		// 打开JSON文件
		QFile file("config/station.json");
		if (!file.open(QIODevice::ReadOnly)) {
			qDebug() << "无法打开JSON文件";
			return -1;
		}
		// 读取JSON数据
		QByteArray jsonData = file.readAll();
		file.close();
		// 将JSON数据转换为QJsonDocument对象
		QJsonParseError error;
		QJsonDocument josnDoc = QJsonDocument::fromJson(jsonData, &error);
		if (josnDoc.isNull()) {
			qDebug() << "无效的JSON格式:" << error.errorString();
			return -1;
		}
		// 提取根节点
		QJsonObject rootObj = josnDoc.object();
		//站名
		m_pMainStation->setStationName(rootObj.value("staName").toString());
		m_pMainStation->setStationId(rootObj.value("stationID").toInt());
		m_pMainStation->setStaFixedSize(rootObj.value("stationWidth").toInt(), rootObj.value("stationHeight").toInt());
		//站场界面类型
		m_nStationViewType = rootObj.value("stationType").toInt();
		m_bShowToolbarBtn = rootObj.value("isShowToolBarBtn").toInt();
		m_bShowToolbarLabel = rootObj.value("isShowToolBarLabel").toInt();
		//通信地址
		QJsonObject addressObj = rootObj.value("comAddress").toObject();
		m_pSocketUDP->setLocalAddress(QHostAddress(addressObj.value("localIp").toString()), addressObj.value("localPortUDP").toInt());
		m_pSocketUDP->setServerAddress(QHostAddress(addressObj.value("serverIp").toString()), addressObj.value("serverPortUDP").toInt());
		m_pSocketTCP->setLocalAddress(QHostAddress(addressObj.value("localIp").toString()), addressObj.value("localPortTCP").toInt());
		m_pSocketTCP->setServerAddress(QHostAddress(addressObj.value("serverIp").toString()), addressObj.value("serverPortTCP").toInt());
		Http::HttpClient::setServerAddress(QHostAddress(addressObj.value("HttpServerIp").toString()), addressObj.value("HttpServerPort").toInt());
		
		//if (strList[1] == "STATIONWIDTH") {
		//	m_szStation.setWidth(strList[2].toInt());
		//}
		//if (strList[1] == "STATIONHEIGHT") {
		//	m_szStation.setHeight(strList[2].toInt());
		//解析站场设备
		QString strFileName = rootObj.value("stationInfo").toString();
		if (strFileName.right(3) == "xml") {
			if (m_pMainStation->ReadStationInfoByXml(strFileName) < 0) {
				qDebug() << "无效的xml文件.";
				return -1;
			}
		}
		else if (strFileName.right(3) == "txt") {
			if (m_pMainStation->ReadStationInfoByTxt(strFileName) < 0) {
				qDebug() << "无效的txt文件.";
				return -1;
			}
		}
		
		if (m_pMainStation->ReadDeviceConfig(rootObj.value("deviceInfo").toString()) < 0) {
			qDebug() << "无效的json文件.";
			return -1;
		}

		if (m_pMainStation->ReadInterLock(rootObj.value("interlockTable").toString()) < 0) {
			qDebug() << "无效的txt文件.";
			return -1;
		}
		if (m_pMainStation->ReadChartConfig(rootObj.value("chartconversion").toString()) < 0) {
			qDebug() << "无效的json文件.";
			return -1;
		}
		//解析站间透明信息
		QJsonArray multiArray = rootObj.value("multiStation").toArray();
		QJsonObject subObj;
		for (const QJsonValue& value : multiArray) {
			subObj = value.toObject();
			StationObject* pStation = new StationObject;
			pStation->setStationName(subObj.value("staName").toString());
			pStation->setStationId(subObj.value("stationID").toInt());
			pStation->setStaFixedSize(subObj.value("stationWidth").toInt(), subObj.value("stationHeight").toInt());
			//解析站场设备
			strFileName = subObj.value("stationInfo").toString();
			if (strFileName.right(3) == "xml") {
				if (pStation->ReadStationInfoByXml(strFileName) < 0) {
					qDebug() << "无效的xml文件.";
					continue;
				}
			}
			else if (strFileName.right(3) == "txt") {
				if (pStation->ReadStationInfoByTxt(strFileName) < 0) {
					qDebug() << "无效的txt文件.";
					continue;
				}
			}

			if (pStation->ReadDeviceConfig(subObj.value("deviceInfo").toString()) < 0) {
				qDebug() << "无效的json文件.";
				continue;
			}

			//初始化站场设备
			pStation->InitStaDevice();
			m_vecMultiStation.append(pStation);
		}
		return 0;
	}

	int CTCObject::TtainTypeFileInit()
	{
		// 打开JSON文件
		QFile file("config/TrainType.json");
		if (!file.open(QIODevice::ReadOnly)) {
			qDebug() << "无法打开JSON文件";
			return -1;
		}
		// 读取JSON数据
		QByteArray jsonData = file.readAll();
		file.close();
		// 将JSON数据转换为QJsonDocument对象
		QJsonParseError error;
		QJsonDocument josnDoc = QJsonDocument::fromJson(jsonData, &error);
		if (josnDoc.isNull()) {
			qDebug() << "无效的JSON格式:" << error.errorString();
			return -1;
		}
		// 提取根节点
		QJsonObject rootObj = josnDoc.object();
		QJsonObject trainNumTypeObj = rootObj.value("trainNumType").toObject();

		QJsonArray passengeTrainArray = trainNumTypeObj.value("passengeTrain").toArray();
		for (int i = 0; i < passengeTrainArray.size(); i++) {
			InsterTrainType(PASSENGE_TYPE, i, passengeTrainArray.at(i).toString());
		}

		QJsonArray freighTrainArray = trainNumTypeObj.value("freightTrain").toArray();
		for (int i = 0; i < freighTrainArray.size(); i++) {
			InsterTrainType(FREIGH_TYPE, i, freighTrainArray.at(i).toString());
		}

		QJsonArray trainTypeArray = rootObj.value("trainType").toArray();
		for (int i = 0; i < trainTypeArray.size(); i++) {
			InsterTrainType(TRAIN_TYPE, i, trainTypeArray.at(i).toString());
		}
		return 0;
	}

	void CTCObject::SocketComInit()
	{
		m_pSocketUDP->InitSocket();
		m_pSocketTCP->InitClient();
		QObject::connect(m_pSocketUDP, &Socket::SocketUDP::recvData, m_pMainStation, &MainStationObject::onReciveData);
		QObject::connect(m_pMainStation, &MainStationObject::SendDataToUDP, m_pSocketUDP, &Socket::SocketUDP::onSendData);
		QObject::connect(m_pSocketTCP, &Socket::SocketTCP::recvData, m_pMainStation, &MainStationObject::onReciveData);
		QObject::connect(m_pMainStation, &MainStationObject::SendDataToTCP, m_pSocketTCP, &Socket::SocketTCP::onSendData);
	}


	void CTCObject::StaOperationConnect()
	{
		QObject::connect(m_pMainStation, &MainStationObject::TrainRouteUpData, m_pCTCMainWindow->RoutePlanWnd(), &BaseWnd::StaRoutePlan::OnTrainRouteUpData);
		QObject::connect(m_pMainStation, &MainStationObject::TrafficLogTableUpData, m_pCTCMainWindow->StationLogWnd(), &BaseWnd::StationLogDisp::OnTrafficLogTableUpData);
		//功能按钮
		BaseWnd::StaFunBtnToolBar* pStaFunBtnToolBar = m_pCTCMainWindow->StaFunBtnToolBar();
		//命令清除
		QObject::connect(pStaFunBtnToolBar, &BaseWnd::StaFunBtnToolBar::OrderClear, m_pMainStation, &MainStationObject::onOrderClear);
		//命令下达
		QObject::connect(pStaFunBtnToolBar, &BaseWnd::StaFunBtnToolBar::OrderIssued, m_pMainStation, &MainStationObject::onOrderIssued);
		//功能按钮复位
		QObject::connect(m_pMainStation, &MainStationObject::FunBtnStateReset, pStaFunBtnToolBar, &BaseWnd::StaFunBtnToolBar::onFunBtnStateReset);
	}

	bool CTCObject::UserLogin()
	{
		UserLoginDlg dlgUserLogin;
		QObject::connect(&dlgUserLogin, &UserLoginDlg::UserLogin, m_pMainStation, &MainStationObject::onUserLogin);
		return dlgUserLogin.exec() == QDialog::Accepted;
	}
}


