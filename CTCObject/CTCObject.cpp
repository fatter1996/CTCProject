#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include "CTCObject.h"
#include "CTCMainWindow/StationViewKSK/StationViewKSK.h"
#include "CTCMainWindow/StationViewTKY/StationViewTKY.h"
#include "CTCMainWindow/CurrentWnd/UserLoginDlg.h" 

namespace CTCDoc{
	using namespace CTCWindows;
	using namespace Station;
	using namespace Socket;

	CTCObject::CTCObject()
	{
		socketTCP = new SocketTCP;
		socketUDP = new SocketUDP;
		m_pMainStation = new StationObject;

		if (ConfigFileInit() < 0) {
			qDebug() << "配置文件解析失败。";
		}
		//初始化站场设备
		m_pMainStation->InitStaDevice();

		//网络通信初始化
		socketUDP->InitSocket();
		socketTCP->InitServer();
	}

	CTCObject::~CTCObject()
	{
		delete m_pMainStation;
		m_pMainStation = nullptr;

		delete socketUDP;
		socketUDP = nullptr;
	}

	QMainWindow* CTCObject::CreatCTCMainWnd()
	{
		switch (CTCObject::m_nStationViewType) {
		case 1: m_pCTCMainWindow = StationViewKSK::CreatStationView(); break;
		case 2: m_pCTCMainWindow = StationViewTKY::CreatStationView(); break;
		case 3: break;
		case 4: break;
		default: break;
		}

		if (m_pCTCMainWindow) {
			//初始化主界面
			m_pCTCMainWindow->InitStattionView();
			m_pCTCMainWindow->setFixedSize(m_pMainStation->getStaFixedSize());
			m_pMainStation->InitDeviceEventFilter(m_pCTCMainWindow->StaPaintView());
			StaOperationConnect();
		}
		UserLogin();
		return m_pCTCMainWindow;
		//if (UserLogin()) {
		//	return m_pCTCMainWindow;
		//}
		//else {
		//	return nullptr;
		//}
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
		//站场界面类型
		m_nStationViewType = rootObj.value("StationType").toInt();
		//通信地址
		QJsonObject addressObj = rootObj.value("comAddress").toObject();
		socketUDP->setLocalAddress(QHostAddress(addressObj.value("localIp").toString()), addressObj.value("localPortUDP").toInt());
		socketUDP->setInterlockAddress(QHostAddress(addressObj.value("interlockIp").toString()), addressObj.value("interlockPort").toInt());
		socketTCP->setLocalAddress(QHostAddress(addressObj.value("localIp").toString()), addressObj.value("localPortTCP").toInt());
		socketTCP->setCultivateAddress(QHostAddress(addressObj.value("cultivateIp").toString()), addressObj.value("cultivatePort").toInt());
		
		//解析站场设备
		QString path = rootObj.value("deviceInfo").toString();
		QString path2 = rootObj.value("lampInfo").toString();
		if (m_pMainStation->ReadStationInfo(rootObj.value("deviceInfo").toString()) < 0) {
			qDebug() << "无效的xml文件.";
			return -1;
		}
		return 0;
	}

	void CTCObject::StaOperationConnect()
	{
		QObject::connect(socketUDP, &SocketUDP::recvData, m_pMainStation, &StationObject::onReciveData);
		QObject::connect( m_pMainStation, &StationObject::SendDataToUDP,socketUDP, &SocketUDP::onSendData);
		QObject::connect(socketTCP, &SocketTCP::recvData, m_pMainStation, &StationObject::onReciveData);
		QObject::connect(m_pMainStation, &StationObject::SendDataToTCP, socketTCP, &SocketTCP::onSendData);

		//命令清除
		const StaFunBtnToolBar* pStaFunBtnToolBar = dynamic_cast<const StaFunBtnToolBar*>(m_pCTCMainWindow->StaFunBtnToolBar());
		QObject::connect(pStaFunBtnToolBar, &StaFunBtnToolBar::OrderClear, m_pMainStation, &StationObject::onOrderClear);
		//命令下达
		QObject::connect(pStaFunBtnToolBar, &StaFunBtnToolBar::OrderIssued, m_pMainStation, &StationObject::onOrderIssued);
	}

	bool CTCObject::UserLogin()
	{
		UserLoginDlg dlgUserLogin;
		QObject::connect(&dlgUserLogin, &UserLoginDlg::UserLogin, m_pMainStation, &StationObject::UserLogin);
		
		return dlgUserLogin.exec() == QDialog::Accepted;
	}
}


