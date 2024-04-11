#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "Global.h"
#include "CTCObject.h"
#include "CTCMainWindow/StationViewKSK/StationViewKSK.h"
#include "CTCMainWindow/StationViewTKY/StationViewTKY.h"

namespace CTCDoc{

	CTCObject::CTCObject()
	{
		socketUDP = new Socket::SocketUDP;
		m_staMainStation = new Station::StationObject;

		if (ConfigInit() < 0) {
			qDebug() << "配置文件解析失败。";
		}
		//初始化站场设备
		m_staMainStation->InitStaDevice();

		//网络通信初始化
		socketUDP->InitSocket();
		QObject::connect(socketUDP, &Socket::SocketUDP::recvDataSignal, [&](QByteArray dataAyyay) {
			m_staMainStation->UnpackData(dataAyyay);
		});
		
	}

	CTCObject::~CTCObject()
	{
		delete m_staMainStation;
		m_staMainStation = nullptr;

		delete socketUDP;
		socketUDP = nullptr;
	}

	QMainWindow* CTCObject::CreatCTCMainWnd()
	{
		switch (CTCObject::m_nStationViewType) {
		case 1: m_pCTCMainWindow = CTCWindows::StationViewKSK::CreatStationView(); break;
		case 2: m_pCTCMainWindow = CTCWindows::StationViewTKY::CreatStationView(); break;
		case 3: break;
		case 4: break;
		default: break;
		}

		if (m_pCTCMainWindow) {
			//初始化主界面
			m_pCTCMainWindow->InitStattionView();
			m_staMainStation->InitDeviceEventFilter(m_pCTCMainWindow->GetStationCtrlDisp()->GetStationPaintView());
		}
		return m_pCTCMainWindow;
	}

	int CTCObject::ConfigInit()
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
		m_staMainStation->setStationName(rootObj.value("staName").toString());
		//站场界面类型
		m_nStationViewType = rootObj.value("StationType").toInt();
		//联锁通信地址
		QJsonObject addressObj = rootObj.value("comAddress").toObject();
		socketUDP->setLocalAddress(QHostAddress(addressObj.value("localIp").toString()), addressObj.value("localPort").toInt());
		socketUDP->setInterlockAddress(QHostAddress(addressObj.value("interlockIp").toString()), addressObj.value("interlockPort").toInt());
		//解析站场设备
		QString path = rootObj.value("deviceInfo").toString();
		QString path2 = rootObj.value("lampInfo").toString();
		if (m_staMainStation->ReadStationInfo(rootObj.value("deviceInfo").toString()) < 0) {
			qDebug() << "无效的xml文件.";
			return -1;
		}
		return 0;
	}
}


