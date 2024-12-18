#include "StationObject.h"
#include "Global.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QWidget>
#include <QtConcurrent>
#include <QDebug>
#include"Device/StaDevice.h"
#pragma execution_character_set("utf-8")

#define StaViewMaxRatio			2.0
#define StaViewMinRatio			0.6

#define MutliViewMaxRatio		2.0
#define MutliViewMinRatio		0.2

namespace Station {
    using namespace Device;

    QXmlStreamReader* StationObject::m_pDeviceInfoReader = nullptr;
    QMap<QString, std::function<Device::DeviceBase* (StationObject*)>> StationObject::m_mapCreatDeviceVector;
    int StationObject::m_nTimerId_500 = -1;

    StationObject::StationObject(QObject* pParent)
    {
        InitDeviceMap();
        
        m_mapVisible = {
            { VisibleDev::button, true },
            { VisibleDev::trackName, true },
            { VisibleDev::unswitchSectionName, true },
            { VisibleDev::sectionName, true },
            { VisibleDev::auxiliaryLineName, true },
            { VisibleDev::switchName, true },
            { VisibleDev::direction, true },
            { VisibleDev::phaseSeparaName, true },
            { VisibleDev::switchSectionName, true },
            { VisibleDev::stationName, true },
            { VisibleDev::text, true },
            { VisibleDev::entrySignalName, true },
            { VisibleDev::exitSignalName, true },
            { VisibleDev::shuntSignalName, true },
            { VisibleDev::sectionSignalName, true },
            { VisibleDev::stateLempName, true },
            { VisibleDev::occludeLempName, true },
            { VisibleDev::RBCChangeName, true },
            { VisibleDev::stateLemp, true },
            { VisibleDev::phaseSepara, true },
            { VisibleDev::notes, true },
            { VisibleDev::shuntSignal, true },
            { VisibleDev::sectionSignal, true },
            { VisibleDev::occludeLemp, true },
            { VisibleDev::RBCChange, true },
            { VisibleDev::auxiliaryLine, true },
            { VisibleDev::insulationNode, true },
            { VisibleDev::platform, true },
            { VisibleDev::endMarker, true },
            { VisibleDev::trainNumWnd, false },
            { VisibleDev::preview, true },
            { VisibleDev::permillSix, true },
            { VisibleDev::funButtonName, true },
            { VisibleDev::squeezeLamp, true },
            { VisibleDev::counter, true },
            { VisibleDev::previewFrame, true }
        };
    }

    StationObject::~StationObject()
    {

    }

    void StationObject::InitDeviceEventFilter(QWidget* pWidget)
    {
        m_pShowWidget = pWidget;
        for (DeviceBase* pDev : m_mapDeviceVector[ALLDEVICE]) {
            pWidget->installEventFilter(pDev);
            pWidget->setMouseTracking(true);
        }
        if (m_nTimerId_500 < 0 && m_bMainStation) {
            m_nTimerId_500 = startTimer(500);
        }
    }

    int  StationObject::ReadStationInfo(const QString& filePath)
    {
        // 打开xml文件
        QFile file("config/" + filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "无法打开xml文件";
            return -1;
        }

        if (!m_pDeviceInfoReader) {
            m_pDeviceInfoReader = new QXmlStreamReader;
        }
        m_pDeviceInfoReader->setDevice(&file);
        
        while (!m_pDeviceInfoReader->atEnd()) {
            m_pDeviceInfoReader->readNext();
            if (m_pDeviceInfoReader->isStartElement()) {
                if (m_pDeviceInfoReader->name() == "Station") {
                    ReadDeviceInfoHead();
                    ReadDeviceInfo();
                }
                else return -1;
            }
        }
        return 0;
    }

    void StationObject::ReadDeviceInfoHead()
    {
        while (!m_pDeviceInfoReader->atEnd()) {
            m_pDeviceInfoReader->readNext();
            if (m_pDeviceInfoReader->isStartElement() && m_pDeviceInfoReader->name() == "HEAD") {
                while (!m_pDeviceInfoReader->atEnd()) {
                    m_pDeviceInfoReader->readNext();
                    if (m_pDeviceInfoReader->isStartElement()) {
                        if (m_pDeviceInfoReader->name() == "m_picWidth") {
                            m_szStation.setWidth(m_pDeviceInfoReader->readElementText().toInt());
                        }
                        else if (m_pDeviceInfoReader->name() == "m_picHeight") {
                            m_szStation.setHeight(m_pDeviceInfoReader->readElementText().toInt());
                        }
                        else if (m_pDeviceInfoReader->name() == "STATION_ID") {
                            m_nStationId = m_pDeviceInfoReader->readElementText().toInt();
                        }
                    }
                    else if (m_pDeviceInfoReader->isEndElement() && m_pDeviceInfoReader->name() == "HEAD") {
                        return;
                    }
                }
            }
        }
    }

    void StationObject::ReadDeviceInfo()
    {
        DeviceBase* pStaDevice = nullptr;
        QString strDeviceType;
        while (!m_pDeviceInfoReader->atEnd()) {
            m_pDeviceInfoReader->readNext();
            if (m_pDeviceInfoReader->isStartElement() && m_pDeviceInfoReader->name() == "DEVICE") {
                while (!m_pDeviceInfoReader->atEnd()) {
                    m_pDeviceInfoReader->readNext();
                    if (m_pDeviceInfoReader->isStartElement()) {
                        strDeviceType = m_pDeviceInfoReader->name().toString();
                        if (m_mapCreatDeviceVector.contains(strDeviceType)) {
                            pStaDevice = dynamic_cast<DeviceBase*>(m_mapCreatDeviceVector[strDeviceType](this));
                        }
                        else {
                            pStaDevice = nullptr;
                        }
                        
                        if (pStaDevice) {
                            pStaDevice->setStrType(strDeviceType);
                            pStaDevice->InitDeviceInfoFromXml(m_pDeviceInfoReader, strDeviceType);
                            m_mapDeviceVector[ALLDEVICE].append(pStaDevice);
                            m_mapDeviceVector[strDeviceType].append(pStaDevice);
                            pStaDevice = nullptr;
                        }
                    }
                    else if (m_pDeviceInfoReader->isEndElement() && m_pDeviceInfoReader->name() == "DEVICE") {
                        return;
                    }
                }
            }
        }
    }

    void StationObject::InitStaDevice()
    {
        //关联通过按钮和信号机
        QString strName;
        for (DeviceBase* pStaBtn : m_mapDeviceVector[BUTTON]) {
            for (DeviceBase* pStaSignal : m_mapDeviceVector[SIGNALLAMP]) {
                if (pStaBtn->getType() == 81) {
                    strName = pStaBtn->getName().left(pStaBtn->getName().length() - 2);
                    if (strName == pStaSignal->getName()) {
                        dynamic_cast<StaSignal*>(pStaSignal)->setRelatedBtn(pStaBtn);
                        dynamic_cast<StaButton*>(pStaBtn)->setRelatedSignal(pStaSignal);
                        break;
                    }
                }
            }
        }
    }

    void StationObject::InitDeviceMap()
    {
        m_mapDeviceVector.insert(ALLDEVICE, QVector<Device::DeviceBase*>());
        m_mapDeviceVector.insert(SWITCH, QVector<Device::DeviceBase*>());           //道岔
        m_mapDeviceVector.insert(DERAILER, QVector<Device::DeviceBase*>());         //脱轨器
        m_mapDeviceVector.insert(SIGNALLAMP, QVector<Device::DeviceBase*>());       //信号机
        m_mapDeviceVector.insert(TRACK, QVector<Device::DeviceBase*>());            //股道
        m_mapDeviceVector.insert(SEMIAUTOBLOCK, QVector<Device::DeviceBase*>());    //半自动闭塞
        m_mapDeviceVector.insert(LOCOMOTIVE, QVector<Device::DeviceBase*>());       //机务段
        m_mapDeviceVector.insert(CONNECTION, QVector<Device::DeviceBase*>());       //场联
        m_mapDeviceVector.insert(BUTTON, QVector<Device::DeviceBase*>());           //按钮
        m_mapDeviceVector.insert(AUTOBLOCK, QVector<Device::DeviceBase*>());        //自动闭塞 
        m_mapDeviceVector.insert(SCATTERED, QVector<Device::DeviceBase*>());        //零散表示灯
        m_mapDeviceVector.insert(HUMP, QVector<Device::DeviceBase*>());             //驼峰   
        m_mapDeviceVector.insert(BRAIDLINE, QVector<Device::DeviceBase*>());        //编发线
        m_mapDeviceVector.insert(ENDLINE, QVector<Device::DeviceBase*>());          //尽头线
        m_mapDeviceVector.insert(TEXT, QVector<Device::DeviceBase*>());             //文字
        m_mapDeviceVector.insert(PERMILLSIX, QVector<Device::DeviceBase*>());       //千分之六标识
        m_mapDeviceVector.insert(SECTION, QVector<Device::DeviceBase*>());          //道岔区段
        m_mapDeviceVector.insert(LAMP, QVector<Device::DeviceBase*>());             //指示灯
        m_mapDeviceVector.insert(NEUTRAL, QVector<Device::DeviceBase*>());          //分相区
    }

    void StationObject::InitCreatDeviceMap()
    {
        m_mapCreatDeviceVector.insert(SWITCH, [](StationObject* pStation) { return new StaSwitch(pStation); });                //道岔
        m_mapCreatDeviceVector.insert(DERAILER, [](StationObject* pStation) { return new StaDerailer(pStation); });            //脱轨器
        m_mapCreatDeviceVector.insert(SIGNALLAMP, [](StationObject* pStation) { return new StaSignal(pStation); });            //信号机
        m_mapCreatDeviceVector.insert(TRACK, [](StationObject* pStation) { return new StaTrack(pStation); });                  //股道
        m_mapCreatDeviceVector.insert(SEMIAUTOBLOCK, [](StationObject* pStation) { return new StaSemiAutoBlock(pStation); });  //半自动闭塞
        m_mapCreatDeviceVector.insert(LOCOMOTIVE, [](StationObject* pStation) { return new StaLocomotive(pStation); });        //机务段
        m_mapCreatDeviceVector.insert(CONNECTION, [](StationObject* pStation) { return new StaConnection(pStation); });        //场联
        m_mapCreatDeviceVector.insert(BUTTON, [](StationObject* pStation) { return new StaButton(pStation); });                //按钮
        m_mapCreatDeviceVector.insert(AUTOBLOCK, [](StationObject* pStation) { return new StaAutoBlock(pStation); });          //自动闭塞 
        m_mapCreatDeviceVector.insert(SCATTERED, [](StationObject* pStation) { return new StaScattered(pStation); });          //零散表示灯
        m_mapCreatDeviceVector.insert(HUMP, [](StationObject* pStation) { return new StaHump(pStation); });                    //驼峰 
        m_mapCreatDeviceVector.insert(BRAIDLINE, [](StationObject* pStation) { return new StaBraidLine(pStation); });          //编发线
        m_mapCreatDeviceVector.insert(ENDLINE, [](StationObject* pStation) { return new StaEndLine(pStation); });              //尽头线
        m_mapCreatDeviceVector.insert(TEXT, [](StationObject* pStation) { return new StaText(pStation); });                    //文字
        m_mapCreatDeviceVector.insert(PERMILLSIX, [](StationObject* pStation) { return new StaPermillSix(pStation); });        //千分之六标识
        m_mapCreatDeviceVector.insert(SECTION, [](StationObject* pStation) { return new StaSwitchSection(pStation); });        //道岔区段
        m_mapCreatDeviceVector.insert(LAMP, [](StationObject* pStation) { return new StaLamp(pStation); });                    //指示灯
        m_mapCreatDeviceVector.insert(NEUTRAL, [](StationObject* pStation) { return new StaNeutral(pStation); });              //分相区
    }

    int StationObject::ReadDeviceConfig(const QString& filePath)
    {
        // 打开JSON文件
        QFile file("config/" + filePath);
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
        DeviceBase* pStaDevice = nullptr;
        QJsonArray deviceArray;
        QJsonObject direction;
        for (QString strDeviceType : rootObj.keys()) {
            if (strDeviceType == "Direction") {
                direction = rootObj.value(strDeviceType).toObject();
                StaSignal* pSignal = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SIGNALLAMP]) {
                    pSignal = dynamic_cast<StaSignal*>(pDevice);
                    pSignal->setDirection(direction.value(pSignal->getName()).toString());
                }
                continue;
            }

            
            deviceArray = rootObj.value(strDeviceType).toArray();
            if (strDeviceType == "StateLamps" || strDeviceType == "LSLamp") {
                strDeviceType = LAMP;
            }
            for (const QJsonValue& value : deviceArray) {
                if (!value.isObject()) {
                    qDebug() << "Array item is not an object.";
                    continue;
                }
                if (m_mapCreatDeviceVector.contains(strDeviceType)) {
                    pStaDevice = dynamic_cast<DeviceBase*>(m_mapCreatDeviceVector[strDeviceType](this));
                }
                else if (strDeviceType == "AutoBlock") {
                    for (DeviceBase* pAutoBlock : m_mapDeviceVector[AUTOBLOCK]) {
                        if (pAutoBlock->getCode() == value.toObject().value("m_nCode").toString().toInt(nullptr, 16)) {
                            pStaDevice = pAutoBlock;
                            break;
                        }
                    }
                } 
                else {
                    pStaDevice = nullptr;
                }

                if (pStaDevice) {
                    pStaDevice->setStrType(strDeviceType);
                    pStaDevice->InitInitDeviceInfoFromJson(value.toObject(), strDeviceType);

                    m_mapDeviceVector[ALLDEVICE].append(pStaDevice);
                    m_mapDeviceVector[strDeviceType].append(pStaDevice);
                    pStaDevice = nullptr;
                }
            }
        }
        return 0;
    }

    void StationObject::SetVisible(VisibleDev devType, bool bVisible)
    {
        m_mapVisible[devType] = bVisible;
        if (devType == VisibleDev::insulationNode) {
            CTCWindows::MainWindow()->MenuStateSync(2);
        }
    }

    bool StationObject::IsVisible(VisibleDev devType)
    {
        return m_mapVisible[devType];
    }

    DeviceBase* StationObject::getDeviceByCode(const uint& nCode)
    {
        for (DeviceBase* pDevice : m_mapDeviceVector[ALLDEVICE]) {
            if (pDevice->getCode() == nCode) {
                return pDevice;
            }
        }
        return nullptr;
    }

    DeviceBase* StationObject::getDeviceByName(const QString& strName)
    {
        for (DeviceBase* pDevice : m_mapDeviceVector[ALLDEVICE]) {
            if (pDevice->getName() == strName) {
                return pDevice;
            }
        }
        return nullptr;
    }

    QSize StationObject::getStaFixedSize()
    {
        return m_szStation * MainStation()->getDiploid(m_bMainStation ? DiploidRatio::StaDiploid : DiploidRatio::MultiDiploid);
    }

    MainStationObject::MainStationObject(QObject* pParent)
    {
        m_bMainStation = true;
        m_pProtocol = new Transmission::StaProtocol(m_mapDeviceVector);
        m_pStaPacket = new Transmission::StaPacket;

        m_mapCreatStationOrder = {
            { Order::train, [&]() { return new StaTrain; } },
            { Order::stagePlan, [&]() { return new StaStagePlan; } },
            { Order::trainRoute, [&]() { return new StaTrainRoute; } },
            { Order::trainDispatch, [&]() { return new StaTrainDispatch; } },
            { Order::dispatch, [&]() { return new StaDispatchOrder; } },
            { Order::trafficLog, [&]() { return new StaTrafficLog; } },
        };

        m_mapStationOrder.insert(Order::train, [&](void* pOrder, const QJsonObject& subObj) {
            StaTrain::Init(static_cast<StaTrain*>(pOrder), subObj);
            m_vecStaTrain.append(static_cast<StaTrain*>(pOrder));
        });

        m_mapStationOrder.insert(Order::stagePlan, [&](void* pOrder, const QJsonObject& subObj) {
            StaStagePlan::Init(static_cast<StaStagePlan*>(pOrder), subObj);
            m_vecStaStagePlan.append(static_cast<StaStagePlan*>(pOrder));
        });

        m_mapStationOrder.insert(Order::trainRoute, [&](void* pOrder, const QJsonObject& subObj) {
            StaTrainRoute::Init(static_cast<StaTrainRoute*>(pOrder), subObj);
            m_vecStaTrainRoute.append(static_cast<StaTrainRoute*>(pOrder));
        });

        m_mapStationOrder.insert(Order::trainDispatch, [&](void* pOrder, const QJsonObject& subObj) {
            StaTrainDispatch::Init(static_cast<StaTrainDispatch*>(pOrder), subObj);
            StaDispatchOrder* pDispatchOrder = static_cast<StaTrainDispatch*>(pOrder)->getParentOrder();
            if (pDispatchOrder) {
                pDispatchOrder->m_vecTrainDispatch.append(static_cast<StaTrainDispatch*>(pOrder));
            }
        });

        m_mapStationOrder.insert(Order::dispatch, [&](void* pOrder, const QJsonObject& subObj) {
            StaDispatchOrder::Init(static_cast<StaDispatchOrder*>(pOrder), subObj);
            m_vecStaDispatchOrder.append(static_cast<StaDispatchOrder*>(pOrder));
            SelectStaTrainDispatch(static_cast<StaDispatchOrder*>(pOrder));
        });

        m_mapStationOrder.insert(Order::trafficLog, [&](void* pOrder, const QJsonObject& subObj) {
            StaTrafficLog::Init(static_cast<StaTrafficLog*>(pOrder), subObj);
            m_vecStaTrafficLog.append(static_cast<StaTrafficLog*>(pOrder));
        });

        m_mapDiploidRatio = {
            { DiploidRatio::StaDiploid, 1.0},
            { DiploidRatio::StaTrainNumDiploid, 1.0},
            { DiploidRatio::MultiDiploid, 0.8},
            { DiploidRatio::MultiTrainNumDiploid, 0.8}
        };
    } 

    MainStationObject::~MainStationObject()
    {
        killTimer(m_nTimerId_500);
        emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_SERVER, 0x2A, 0x00));
        QByteArray btResult;
        for (StaStagePlan* pStagePlan : m_vecStaStagePlan) {
            if (pStagePlan->m_nOrderState) {
                if (Http::HttpClient::DeleteStaStagePlan(pStagePlan->m_nPlanId, btResult)) {
                    qDebug() << "已删除阶段计划-->" << pStagePlan->m_nPlanId;
                }
            }
        }

        for (StaDispatchOrder* pDispatchOrder : m_vecStaDispatchOrder) {
            if (pDispatchOrder->m_nStateDisOrder) {
                if (Http::HttpClient::DeleteDispatchOrder(pDispatchOrder->m_nOrderId, btResult)) {
                    qDebug() << "已删除调度命令-->" << pDispatchOrder->m_nOrderId;
                }
            }
        }

        QVector<StaTrainRoute*> vecTrainRoute;
        bool bAllClear = true;
        for (StaTrafficLog* pTrafficLog : m_vecStaTrafficLog) {
            vecTrainRoute = getStaTrainRouteByTrain(pTrafficLog->m_nTrainId);
            for (StaTrainRoute* pTrainRoute : vecTrainRoute) {
                bAllClear &= (pTrainRoute->m_nRouteState == 4);
            }
            if (bAllClear) {
                if (Http::HttpClient::DeleteStaTrafficLog(pTrafficLog->m_nLogId, btResult)) {
                    qDebug() << "已删除行车日志-->" << pTrafficLog->m_nLogId;
                }
                for (StaTrainRoute* pTrainRoute : vecTrainRoute) {
                    if (Http::HttpClient::DeleteStaTrainRoute(pTrainRoute->m_nRouteId, btResult)) {
                        qDebug() << "已删除进路序列-->" << pTrainRoute->m_nRouteId;
                    }
                }
                if (Http::HttpClient::DeleteStaTrain(pTrafficLog->m_nTrainId, btResult)) {
                    qDebug() << "已删除车次信息-->" << pTrafficLog->m_nTrainId;
                }
            }
        }

        QVector<StaTrafficLog*> m_vecStaTrafficLog;

        delete m_pProtocol;
        m_pProtocol = nullptr;

        delete m_pStaPacket;
        m_pStaPacket = nullptr;
    }

    void MainStationObject::timerEvent(QTimerEvent* event)
    {
        if (event->timerId() == m_nTimerId_500) {
            DeviceBase::setElapsed();
        }
        if (event->timerId() == m_nTimerId_1000) {
            m_nCountdown--;
            if (m_nCountdown <= 0) {
                killTimer(m_nTimerId_1000);
                onOrderClear();
            }
        }
        return QObject::timerEvent(event);
    }

    void MainStationObject::OnLine()
    {
        emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_SERVER, 0x2A, 0x01));
    }

    void MainStationObject::SelectStationOrder()
    {
        SelectStaLimits();
        SelectStaTrain();
        SelectStaStagePlan();
        SelectStaTrainRoute();
        SelectStaDispatch();
        SelectStaTrafficLog();
    }

    void MainStationObject::SelectStaLimits()
    {
        QByteArray btResult;
        if (!Http::HttpClient::SelectStaLimits(btResult)) {
            qDebug() << "权限信息获取失败";
            return;
        }
        QJsonParseError error;
        QJsonDocument josnDoc = QJsonDocument::fromJson(btResult, &error);
        if (josnDoc.isNull()) {
            qDebug() << "无效的JSON格式:" << error.errorString();
            return;
        }
        QJsonObject rootObj = josnDoc.object();
        m_StaLimits.nRouteLimits = rootObj.value("routePermit").toInt();
        m_StaLimits.bAutoSignStage = rootObj.value("autoSignStage").toInt();
        m_StaLimits.bExStaControl = rootObj.value("fczkMode").toInt();
        m_StaLimits.nControlMode = rootObj.value("fszlMode").toInt();
        m_StaLimits.nPlanMode = rootObj.value("planMode").toInt();
        m_StaLimits.nPlanControl = rootObj.value("planCtrl").toInt();
        emit FunBtnStateReset();
    }

    void MainStationObject::SelectStaTrain()
    {
        QByteArray btResult;
        if (!Http::HttpClient::SelectStaTrainList(btResult)) {
            qDebug() << "车辆信息获取失败";
            return;
        }
        GetStationOrderByJosn(btResult, Order::train);
        qDebug() << QString("查询到%1条车辆信息").arg(m_vecStaTrain.size());
    };

    void MainStationObject::SelectStaStagePlan()
    {
        QByteArray btResult;
        if (!Http::HttpClient::SelectStaStagePlan(btResult)) {
            qDebug() << "阶段计划获取失败";
            return;
        }
        GetStationOrderByJosn(btResult, Order::stagePlan);
        qDebug() << QString("查询到%1条阶段计划").arg(m_vecStaStagePlan.size());
    }

    void MainStationObject::SelectStaTrainRoute()
    {
        QByteArray btResult;
        if (!Http::HttpClient::SelectStaTrainRoute(btResult)) {
            qDebug() << "进路序列获取失败";
            return;
        }
        GetStationOrderByJosn(btResult, Order::trainRoute);
        qDebug() << QString("查询到%1条进路序列").arg(m_vecStaTrainRoute.size());
        InitCombinativeRoute();
        emit TrainRouteUpData();
    }

    void MainStationObject::SelectStaDispatch()
    {
        QByteArray btResult;
        if (!Http::HttpClient::SelectStaDispatchOrder(btResult)) {
            qDebug() << "调度命令获取失败";
            return;
        }
        GetStationOrderByJosn(btResult, Order::dispatch);
        qDebug() << QString("查询到%1条调度命令").arg(m_vecStaDispatchOrder.size());
    }

    void MainStationObject::SelectStaTrainDispatch(StaDispatchOrder* pOrder)
    {
        QByteArray btResult;
        if (!Http::HttpClient::SelectStaTrainDispatch(pOrder->m_nOrderId, btResult)) {
            qDebug() << "机车调度命令获取失败";
            return;
        }
        GetStationOrderByJosn(btResult, Order::trainDispatch);
        qDebug() << QString("查询到%1条机车调度命令").arg(pOrder->m_vecTrainDispatch.size());
    }

    void MainStationObject::SelectStaTrafficLog()
    {
        QByteArray btResult;
        if (!Http::HttpClient::SelectStaTrafficLog(btResult)) {
            qDebug() << "调度命令获取失败";
            return;
        }
        GetStationOrderByJosn(btResult, Order::trafficLog);
        qDebug() << QString("查询到%1条行车日志").arg(m_vecStaTrafficLog.size());
        //CTCWindows::MainWindow()->StationLogWnd()->TrafficLogTableUpData();
    }

    void MainStationObject::GetStationOrderByJosn(const QByteArray& btDataArray, Order type)
    {
        QJsonParseError error;
        QJsonDocument josnDoc = QJsonDocument::fromJson(btDataArray, &error);
        if (josnDoc.isNull()) {
            qDebug() << "无效的JSON格式:" << error.errorString();
            return;
        }

        StaOrder* pOrder = nullptr;
        for (const QJsonValue& value : josnDoc.array()) {
            if (!value.isObject()) {
                qDebug() << "Array item is not an object.";
                pOrder = nullptr;
                continue;
            }
            pOrder = m_mapCreatStationOrder[type]();
            m_mapStationOrder[type](pOrder, value.toObject());
        }
    }

    void MainStationObject::AddSelectDevice(Device::DeviceBase* pDevice)
    {
        if (m_vecSelectDevice.indexOf(pDevice) < 0) {
            m_vecSelectDevice.append(pDevice);
        }
        //emit FunBtnStateReset();
        m_nTimerId_1000 = startTimer(1000);
        m_nCountdown = 15;
    }

    int MainStationObject::AddNewTrain(StaTrain* pStaTrain)
    {
        QByteArray btResult;
        int nTrainId = 1;
        while (btResult != "") {
            if (Http::HttpClient::SelectStaTrain(nTrainId, btResult)) {
                nTrainId++;
                if (nTrainId > 16) {
                    nTrainId = 1;
                }
            }
        }
        pStaTrain->m_nTrainId = nTrainId;
        if (Http::HttpClient::AddStaTrain(pStaTrain, btResult)) {
            m_vecStaTrain.append(pStaTrain);
            DeviceTrain* pDevice = dynamic_cast<DeviceTrain*>(getDeviceByCode(pStaTrain->m_nPosCode));
            if (pDevice) {
                pDevice->SetTrain(pStaTrain);
            }
            
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_TEACHER, 0x60, pStaTrain->m_nTrainId, 0x01, pStaTrain->m_strTrainNum.toLocal8Bit()));
            return pStaTrain->m_nTrainId;
        }
        return 0;
    }

    int MainStationObject::DeleteTrain(StaTrain* pStaTrain)
    {
        QByteArray btResult;
        if (Http::HttpClient::DeleteStaTrain(pStaTrain->m_nTrainId, btResult)) {
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_TEACHER, 0x60, pStaTrain->m_nTrainId, 0x02, pStaTrain->m_strTrainNum.toLocal8Bit()));
            m_vecStaTrain.removeOne(pStaTrain);
            return 1;
        }
        return 0;
    }

    int MainStationObject::SetTrainRunning(StaTrain* pStaTrain, bool bRunning)
    {
        QByteArray btResult;
        if (Http::HttpClient::SetTrainRunState(pStaTrain->m_nTrainId, bRunning, btResult)) {
            pStaTrain->m_bRunning = bRunning;
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_TEACHER, 0x60, pStaTrain->m_nTrainId, 0x04 + bRunning, pStaTrain->m_strTrainNum.toLocal8Bit()));
            return 1;
        }
        return 0;
    }

    int MainStationObject::ChangeTrainNum(StaTrain* pStaTrain, QString strTrainNum)
    {
        QByteArray btResult;
        if (Http::HttpClient::ChangeTrainNum(pStaTrain->m_nTrainId, strTrainNum, btResult)) {
            QString strOldTrainNum = pStaTrain->m_strTrainNum;
            pStaTrain->m_strTrainNum = strTrainNum;
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_TEACHER, 0x60, pStaTrain->m_nTrainId, 0x03, strOldTrainNum.toLocal8Bit()));
            return 1;
        }

        return 0;
    }

    int MainStationObject::ChangeTrainAttr(StaTrain* pStaTrain, int nSpeed, QString strLocomotive, bool bElectric)
    {
        QByteArray btResult;
        if (Http::HttpClient::ChangeTrainAttr(pStaTrain->m_nTrainId, nSpeed, strLocomotive, bElectric, btResult)) {
            pStaTrain->m_nSpeed = nSpeed;
            pStaTrain->m_strLocomotive = strLocomotive;
            pStaTrain->m_bElectric = bElectric;
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_TEACHER, 0x60, pStaTrain->m_nTrainId, 0x08, pStaTrain->m_strTrainNum.toLocal8Bit()));
            return 1;
        }
        return 0;
    }

    int MainStationObject::AddNewStagePlan(StaStagePlan* pStaStagePlan)
    {
        QByteArray btResult;
        if (Http::HttpClient::AddStaStagePlan(pStaStagePlan, btResult) == 0) {
            qDebug() << "添加阶段计划失败";
            return 0;
        }
        pStaStagePlan->m_nPlanId = btResult.toInt();
        m_vecStaStagePlan.append(pStaStagePlan);
        m_pNewStagePlan = pStaStagePlan;
        //添加新的车次
        int nTrainId = AddNewTrain(new StaTrain(pStaStagePlan));
        if (nTrainId == 0) {
            qDebug() << "添加车次失败";
            return 0;
        }
        //生成行车日志
        StaTrafficLog* pTrafficLog = new StaTrafficLog(pStaStagePlan, nTrainId);
        if (AddNewTrafficLog(pTrafficLog) == 0) {
            qDebug() << "添加行车日志失败";
            return 0;
        }
        if (m_bAutoSendPlan) {
            CreatTrainRouteByTrafficLog(pTrafficLog);
        }
        return pStaStagePlan->m_nPlanId;
    }

    int MainStationObject::AddNewTrainRoute(StaTrainRoute* pTrainRoute)
    {
        QByteArray btResult;
        if (Http::HttpClient::AddStaTrainRoute(pTrainRoute, btResult)) {
            pTrainRoute->m_nRouteId = btResult.toInt();
            m_vecStaTrainRoute.append(pTrainRoute);
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x61, pTrainRoute->m_nRouteId, 0x01));
            emit TrainRouteUpData();
            return pTrainRoute->m_nRouteId;
        }
        return 0;
    }

    void MainStationObject::InitCombinativeRoute()
    {
        StaTrainRoute* pSubTrainRoute = nullptr;
        for (StaTrainRoute* pTrainRoute : m_vecStaTrainRoute) {
            for (int nRouteId : pTrainRoute->m_vecSubRouteId) {
                pSubTrainRoute = getStaTrainRouteById(nRouteId);
                if (pSubTrainRoute) {
                    pSubTrainRoute->m_bSunTrainRoute = true;
                }
            }
        }
    }

    int MainStationObject::AddNewDispatchOrder(StaDispatchOrder* pDispatchOrder)
    {
        QByteArray btResult;
        if (Http::HttpClient::AddStaDispatchOrder(pDispatchOrder, btResult)) {
            pDispatchOrder->m_nOrderId = btResult.toInt();
            m_vecStaDispatchOrder.append(pDispatchOrder);
            m_pNewDispatchOrder = pDispatchOrder;
            return pDispatchOrder->m_nOrderId;
        }
        return 0;
    }

    int MainStationObject::AddNewTrafficLog(StaTrafficLog* pTrafficLog)
    {
        QByteArray btResult;
        if (Http::HttpClient::AddStaTrafficLog(pTrafficLog, btResult)) {
            pTrafficLog->m_nLogId = btResult.toInt();
            m_vecStaTrafficLog.append(pTrafficLog);
            emit TrafficLogTableUpData();
            //CTCWindows::MainWindow()->StationLogWnd()->TrafficLogTableUpData();
            return pTrafficLog->m_nLogId;
        }
        return 0;
    }

    void MainStationObject::CreatTrainRouteByTrafficLog(StaTrafficLog* pTrafficLog)
    {
        QByteArray btResult;
        int nArrivalRouteId = 0;
        int nDepartRouteId = 0;
        //生成进路序列
        switch (pTrafficLog->m_nPlanType) {
        case 0x01:      //接发
        case 0x04: {    //通过
            nArrivalRouteId = AddNewTrainRoute(new StaTrainRoute(pTrafficLog, ROUTE_TYPE_ARRIVAL));
            nDepartRouteId = AddNewTrainRoute(new StaTrainRoute(pTrafficLog, ROUTE_TYPE_DEPART));
        } break;
        case 0x02: {    //始发
            nDepartRouteId = AddNewTrainRoute(new StaTrainRoute(pTrafficLog, ROUTE_TYPE_DEPART));
        } break;
        case 0x03:      //终到
        case 0x05: {    //线路所通过 
            nArrivalRouteId = AddNewTrainRoute(new StaTrainRoute(pTrafficLog, ROUTE_TYPE_ARRIVAL));
        } break;
        default: {
            nArrivalRouteId = AddNewTrainRoute(new StaTrainRoute(pTrafficLog, ROUTE_TYPE_ARRIVAL));
        } break;
        }

        if (Http::HttpClient::UpdataRouteId(pTrafficLog->m_nLogId, nArrivalRouteId, nDepartRouteId, btResult)) {
            pTrafficLog->m_nArrivalRouteId = nArrivalRouteId;
            pTrafficLog->m_nDepartRouteId = nDepartRouteId;
        }
    }

    void MainStationObject::SendPacketMsg(int nTargetCode, int nOrderType, int nAttr1, int nAttr2, QByteArray btAttr3)
    {
        emit SendDataToTCP(m_pStaPacket->PackOrder(nTargetCode, nOrderType, nAttr1, nAttr2, btAttr3));
        onOrderClear();
    }

    void MainStationObject::SubmitCurSubject()
    {
        emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_CULTIVATE, 0x82));
        if (m_pCurSubject) {
            delete m_pCurSubject;
            m_pCurSubject = nullptr;
        }
    }

    bool MainStationObject::IsAllowStaOperation()
    {
        return !m_StaLimits.bExStaControl;
    }

    void MainStationObject::setCurSubject(CultivateObject::Subject* pSubject)
    {
        if (m_pCurSubject) {
            delete m_pCurSubject;
            m_pCurSubject = nullptr;
        }
        m_pCurSubject = pSubject;
    }

    StaTrain* MainStationObject::getStaTrainById(int nTrainId)
    {
        for (StaTrain* pTrain : m_vecStaTrain) {
            if (pTrain->m_nTrainId == nTrainId) {
                return pTrain;
            }
        }
        return nullptr;
    }

    QVector<StaTrainRoute*>  MainStationObject::getStaTrainRouteByTrain(int nTrainID)
    {
        QVector<StaTrainRoute*> vecTrainRoute;
        for (StaTrainRoute* pTrainRoute : m_vecStaTrainRoute) {
            if (pTrainRoute->m_nTrainId == nTrainID) {
                vecTrainRoute.append(pTrainRoute);
            }
        }
        return vecTrainRoute;
    }

    StaTrainRoute* MainStationObject::getStaTrainRouteById(int nRouteId)
    {
        for (StaTrainRoute* pTrainRoute : m_vecStaTrainRoute) {
            if (pTrainRoute->m_nRouteId == nRouteId) {
                return pTrainRoute;
            }
        }
        return nullptr;
    }

    StaTrainRoute* MainStationObject::getStaTrainRouteByRowIndex(int nRow)
    {
        for (StaTrainRoute* pTrainRoute : m_vecStaTrainRoute) {
            if (pTrainRoute->m_nRowIndex == nRow) {
                return pTrainRoute;
            }
        }
        return nullptr;
    }

    StaTrafficLog* MainStationObject::getStaTrafficLogById(int nLogId)
    {
        for (StaTrafficLog* pTrafficLog : m_vecStaTrafficLog) {
            if (pTrafficLog->m_nLogId == nLogId) {
                return pTrafficLog;
            }
        }
        return nullptr;
    }

    void MainStationObject::setDiploid(DiploidOperate operate, int nType)
    {
        DiploidRatio ratio;
        if (nType == STAVIEW) {
            ratio = CTCWindows::MainWindow()->IsMultiStaView() ? DiploidRatio::MultiDiploid : DiploidRatio::StaDiploid;
        }
        else if (nType == TRAINNUM) {
            ratio = CTCWindows::MainWindow()->IsMultiStaView() ? DiploidRatio::MultiTrainNumDiploid : DiploidRatio::StaTrainNumDiploid;
        }

        if (operate == DiploidOperate::amplify) {
            if (m_mapDiploidRatio[ratio] < (m_bMainStation ? StaViewMaxRatio : MutliViewMaxRatio)) {
                m_mapDiploidRatio[ratio] += 0.1;
            }
        }
        else if (operate == DiploidOperate::reduce) {
            if (m_mapDiploidRatio[ratio] > (m_bMainStation ? StaViewMinRatio : MutliViewMinRatio)) {
                m_mapDiploidRatio[ratio] -= 0.1;
            }
        }
        else {
            m_mapDiploidRatio[ratio] = 1;
            CTCWindows::MainWindow()->setMouseState(CTCWindows::MouseState::Default);
        }

        CTCWindows::MainWindow()->MenuStateSync(1, nType);
    }

    void MainStationObject::onReciveData(const QByteArray& dataAyyay)
    {
        //帧头帧尾校验
        if (dataAyyay.left(4) == QByteArray::fromHex("efefefef") && dataAyyay.right(4) == QByteArray::fromHex("fefefefe")) {
            int lenght = (dataAyyay[4] & 0xFF) + ((dataAyyay[5] & 0xFF) << 8);
            if (lenght == dataAyyay.length()) {
                if ((dataAyyay[10] & 0xFF) != 0x30) {
                    qDebug() << "接收数据：" << dataAyyay.toHex();
                }
                QByteArray byteToInterLock = m_pProtocol->UnpackData(dataAyyay);
                if (!byteToInterLock.isEmpty()) {
                    QByteArrayList byteList = byteToInterLock.split('#');
                    if (byteList.size() == 2) {
                        if (!byteList[0].isEmpty()) {
                            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x82, 0x04, -1, byteList[0]));
                        }

                        if (!byteList[1].isEmpty()) {
                            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x82, 0x05, -1, byteList[1]));
                        }
                    }
                }
            }
            else {
                qDebug() << "长度校验失败！";
                if (dataAyyay[10] == 0x50) {
                    m_pProtocol->UnpackData(dataAyyay);
                }
            }
        }
    }

    void MainStationObject::onUserLogin(QString strUserName, QString strPassword)
    {
        m_infoCurrUser.m_strName = strUserName;
        m_infoCurrUser.m_strPassWord = strPassword;
        emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_CULTIVATE, 0x80));
    }

    void MainStationObject::onOrderClear()
    {
        ClearDevice();
        emit FunBtnStateReset();
    }

    void MainStationObject::onOrderIssued()
    {
        emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x40));
        onOrderClear(); 
        emit FunBtnStateReset();
    }

    void MainStationObject::CompareResult(const QByteArray& dataAyyay)
    {
        if (m_pCurSubject) {
            m_pCurSubject->CompareResult(dataAyyay);
        }
    }

    void MainStationObject::StationReset()
    {
        QByteArray btResult;
        if (Http::HttpClient::ClearStaStagePlan(btResult)) {
            for (StaStagePlan* pStagePlan : m_vecStaStagePlan) {
                delete pStagePlan;
                pStagePlan = nullptr;
            }
            QVector<StaStagePlan*>().swap(m_vecStaStagePlan);
            qDebug() << "已清空阶段计划";
        }
        if (Http::HttpClient::ClearStaTrain(btResult)) {
            for (StaTrain* pTrain : m_vecStaTrain) {
                delete pTrain;
                pTrain = nullptr;
            }
            QVector<StaTrain*>().swap(m_vecStaTrain);
            qDebug() << "已清车次信息";
        }
        if (Http::HttpClient::ClearStaTrainRoute(btResult)) {
            for (StaTrainRoute* pTrainRoute : m_vecStaTrainRoute) {
                delete pTrainRoute;
                pTrainRoute = nullptr;
            }
            QVector<StaTrainRoute*>().swap(m_vecStaTrainRoute);
            qDebug() << "已清空进路序列";
        }
        if (Http::HttpClient::ClearStaDispatchOrder(btResult)) {
            for (StaDispatchOrder* pDispatchOrder : m_vecStaDispatchOrder) {
                delete pDispatchOrder;
                pDispatchOrder = nullptr;
            }
            QVector<StaDispatchOrder*>().swap(m_vecStaDispatchOrder);
            qDebug() << "已清空调度命令";
        }
        if (Http::HttpClient::ClearStaTrainDispatch(btResult)) {
            qDebug() << "已清空机车调度命令";
        }
        if (Http::HttpClient::ClearStaTrafficLog(btResult)) {
            for (StaTrafficLog* pTrafficLog : m_vecStaTrafficLog) {
                delete pTrafficLog;
                pTrafficLog = nullptr;
            }
            QVector<StaTrafficLog*>().swap(m_vecStaTrafficLog);
            qDebug() << "已清空行车日志";
        }
    }

    void MainStationObject::ClearDevice()
    {
        for (DeviceBase* pDevice : m_vecSelectDevice) {
            pDevice->OrderClear();
        }
        QVector<Device::DeviceBase*>().swap(m_vecSelectDevice);
    }

    int MainStationObject::getStaLimits(Limits type)
    {
        switch (type)
        {
        case Station::Limits::RouteLimits:      return m_StaLimits.nRouteLimits;
        case Station::Limits::AutoSignStage:    return m_StaLimits.bAutoSignStage;
        case Station::Limits::ExStaControl:     return m_StaLimits.bExStaControl;
        case Station::Limits::ControlMode:      return m_StaLimits.nControlMode;
        case Station::Limits::PlanMode:         return m_StaLimits.nPlanMode;
        case Station::Limits::PlanControl:      return m_StaLimits.nPlanControl;
        case Station::Limits::ApplyControlMode: return m_StaLimits.nApplyControlMode;
        default: return 0;
        }
    }

    void MainStationObject::setStaLimits(Limits type, int nValue)
    {
        switch (type)
        {
        case Station::Limits::RouteLimits:      m_StaLimits.nRouteLimits = nValue;  break;
        case Station::Limits::AutoSignStage:    m_StaLimits.bAutoSignStage = nValue;  break;
        case Station::Limits::ExStaControl:     m_StaLimits.bExStaControl = nValue;  break;
        case Station::Limits::ControlMode:      m_StaLimits.nControlMode = nValue;  break;
        case Station::Limits::PlanMode:         m_StaLimits.nPlanMode = nValue;  break;
        case Station::Limits::PlanControl:      m_StaLimits.nPlanControl = nValue;  break;
        case Station::Limits::ApplyControlMode: m_StaLimits.nApplyControlMode = nValue;  break;
        default:
            break;
        }
        emit FunBtnStateReset();
    }
}