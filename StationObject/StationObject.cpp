#include "StationObject.h"
#include "Global.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QWidget>
#include <QtConcurrent>
#include <QDebug>
#include <QMessageBox>
#include "Device/StaDevice.h"
#include "CommonWidget/SealTechnique.h"
#include "CommonWidget/LeadSealDlg.h"

#pragma execution_character_set("utf-8")

#define StaViewMaxRatio			2.0
#define StaViewMinRatio			0.6

#define MutliViewMaxRatio		2.0
#define MutliViewMinRatio		0.2

namespace Station {
    using namespace Device;
    int StationObject::m_nUpState = 0;
    QXmlStreamReader* StationObject::m_pDeviceInfoReader = nullptr;
    QMap<QString, std::function<Device::DeviceBase* (StationObject*)>> StationObject::m_mapCreatDeviceVector;
    int StationObject::m_nTimerId_500 = -1;
    StationObject::StationObject(QObject* pParent)
    {
        InitDeviceMap();
        m_mapVisible = {
            { VisibleDev::button, true },
            { VisibleDev::trainButton, true },
            { VisibleDev::shuntButton, true },
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
        m_pShowWidget->setMouseTracking(true);
        for (DeviceBase* pDev : m_mapDeviceVector[ALLDEVICE]) {
            m_pShowWidget->installEventFilter(pDev);
        }
        if (m_nTimerId_500 < 0 && m_bMainStation) {
            m_nTimerId_500 = startTimer(500);
        }
    }

    int  StationObject::ReadStationInfoByXml(const QString& filePath)
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
                else {
                    file.close();
                    return -1;
                } 
            }
        }
        file.close();
        return 0;
    }

    int StationObject::ReadStationInfoByTxt(const QString& filePath)
    {
        // 打开txt文件
        QFile file("config/" + filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "无法打开txt文件";
            return -1;
        }
        QTextStream strInTxtStream(&file);
        QStringList strInfoList;
        QString strLine;
        QString strDeviceType;
        DeviceBase* pStaDevice = nullptr;
        bool bNextDevice = false;
        while (!strInTxtStream.atEnd()) {
            if (bNextDevice) {
                bNextDevice = false;
            }
            else {
                strLine = strInTxtStream.readLine();
            }
            
            if (strLine.left(2) == "##") {
                strDeviceType = strLine.mid(2);
                if (strDeviceType == SIGNALLAMP2) {
                    strDeviceType = SIGNALLAMP;
                }
                if (strDeviceType == TEXT2) {
                    strDeviceType = TEXT;
                }
                
                if (m_mapCreatDeviceVector.contains(strDeviceType)) {
                    pStaDevice = dynamic_cast<DeviceBase*>(m_mapCreatDeviceVector[strDeviceType](this));
                }
                else {
                    pStaDevice = nullptr;
                }

                if (pStaDevice) {
                    pStaDevice->setStrType(strDeviceType);
                    pStaDevice->InitAttributeMap();
                    strLine = strInTxtStream.readLine();
                    while (strLine.left(3) != "ADD" && strLine.left(2) != "##") {
                        strInfoList << strLine;
                        strLine = strInTxtStream.readLine();
                    }
                    if (strLine.left(2) == "##") {
                        bNextDevice = true;
                    }
                    pStaDevice->InitDeviceInfoFromTxt(strInfoList, strDeviceType);
                    m_mapDeviceVector[ALLDEVICE].append(pStaDevice);
                    m_mapDeviceVector[strDeviceType].append(pStaDevice);
                    strInfoList.clear();
                    pStaDevice = nullptr;
                }
            }
        }
        file.close();
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
                            pStaDevice->InitAttributeMap();
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
            if (pStaBtn->getType() == 81) {
                for (DeviceBase* pStaSignal : m_mapDeviceVector[SIGNALLAMP]) {
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
        m_mapDeviceVector.insert(MARK, QVector<Device::DeviceBase*>());             //标志
        m_mapDeviceVector.insert(TEXTSIGN, QVector<Device::DeviceBase*>());         //文字标注
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
        m_mapCreatDeviceVector.insert(MARK, [](StationObject* pStation) { return new StaMark(pStation); });                    //标志
        m_mapCreatDeviceVector.insert(TEXTSIGN, [](StationObject* pStation) { return new StaTextSign(pStation); });            //文字标注
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
        for (QString strDeviceType : rootObj.keys()) {

            if (strDeviceType == "Direction") {
                QJsonObject direction = rootObj.value(strDeviceType).toObject();
                StaSignal* pSignal = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SIGNALLAMP]) {
                    pSignal = dynamic_cast<StaSignal*>(pDevice);
                    pSignal->setDirection(direction.value(pSignal->getName()).toString());
                }
                continue;
            }

            if (strDeviceType == "TrackAdjSignal") {
                QJsonObject adjSignal = rootObj.value(strDeviceType).toObject();
                QJsonArray deviceArray;
                QVector<int> vecSignalCode;
                for (const QString& strKey : adjSignal.keys()) {
                    if (adjSignal[strKey].isArray()) {
                        deviceArray = adjSignal[strKey].toArray();
                        for (int i = 0; i < deviceArray.size(); i++) {
                            vecSignalCode.append(deviceArray[i].toInt());
                        }
                    }
                    m_mapTrackAdjSingalCode.insert(strKey, vecSignalCode);
                    QVector<int>().swap(vecSignalCode);
                }
                continue;
            }

            deviceArray = rootObj.value(strDeviceType).toArray();
            if (strDeviceType == "StateLamps" || strDeviceType == "LSLamp") {
                strDeviceType = LAMP;
            }
            if (strDeviceType == SIGNALLAMP2) {
                strDeviceType = SIGNALLAMP;
            }
            if (strDeviceType == TEXT2) {
                strDeviceType = TEXT;
            }

            for (const QJsonValue& value : deviceArray) {
                if (!value.isObject()) {
                    qDebug() << "Array item is not an object.";
                    continue;
                }

                if (strDeviceType == "AutoBlock" || strDeviceType == AUTOBLOCK) {
                    strDeviceType = AUTOBLOCK;
                    for (DeviceBase* pAutoBlock : m_mapDeviceVector[AUTOBLOCK]) {
                        if (pAutoBlock->getCode() == value.toObject().value("m_nCode").toString().toInt(nullptr, 16)) {
                            pStaDevice = pAutoBlock;
                            break;
                        }
                    }
                }

                if (strDeviceType == "SemiAutoBlock" || strDeviceType == SEMIAUTOBLOCK) {
                    strDeviceType = SEMIAUTOBLOCK;
                    for (DeviceBase* pAutoBlock : m_mapDeviceVector[SEMIAUTOBLOCK]) {
                        if (pAutoBlock->getCode() == value.toObject().value("m_nCode").toString().toInt(nullptr, 16)) {
                            pStaDevice = pAutoBlock;
                            break;
                        }
                    }
                }

                if (!pStaDevice && 
                        !value.toObject().contains("#m_nCode") &&
                        !value.toObject().contains("#m_nType")  && 
                        !value.toObject().contains("#m_strName")) {
                    pStaDevice = dynamic_cast<DeviceBase*>(m_mapCreatDeviceVector[strDeviceType](this));
                    m_mapDeviceVector[ALLDEVICE].append(pStaDevice);
                    m_mapDeviceVector[strDeviceType].append(pStaDevice);
                }

                if (pStaDevice) {
                    pStaDevice->setStrType(strDeviceType);
                    pStaDevice->InitAttributeMap();
                    pStaDevice->InitDeviceInfoFromJson(value.toObject());
                    pStaDevice = nullptr;
                }
            }
        }
        return 0;
    }

    void StationObject::AddNewTextSign(const QString& strText, const QPoint& ptPos, const QColor& colFont, const QColor& colBackground, int nSize)
    {
        StaTextSign* pTextSign = dynamic_cast<StaTextSign*>(m_mapCreatDeviceVector[TEXTSIGN](this));
        pTextSign->InitTextSign(strText, ptPos, colFont, colBackground, nSize);
        pTextSign->setStrType(TEXTSIGN);
        m_mapDeviceVector[TEXTSIGN].append(pTextSign);
        m_pShowWidget->installEventFilter(pTextSign);
    }

    void StationObject::DeleteTextSign(Device::StaTextSign* pTextSign)
    {
        m_mapDeviceVector[TEXTSIGN].removeOne(pTextSign);
        delete pTextSign;
        pTextSign = nullptr;
    }

    void StationObject::ClearAllTextSign()
    {
        for (DeviceBase* pTextSign : m_mapDeviceVector[TEXTSIGN]) {
            delete pTextSign;
            pTextSign = nullptr;
        }
        m_mapDeviceVector[TEXTSIGN].clear();
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

    DeviceBase* StationObject::getDeviceByCode(const uint& nCode, QString StrType)
    {
        for (DeviceBase* pDevice : m_mapDeviceVector[StrType]) {
            if (pDevice->getCode() == nCode) {
                return pDevice;
            }
        }
        return nullptr;
    }

    DeviceBase* StationObject::getDeviceByName(const QString& strName, QString StrType)
    {
        for (DeviceBase* pDevice : m_mapDeviceVector[StrType]) {
            if (pDevice->getName() == strName) {
                return pDevice;
            }
        }
        return nullptr;
    }

    DeviceBase* StationObject::getSwitchBySectionCode(int nCode)
    {
        for (DeviceBase* pDevice : m_mapDeviceVector[SWITCH]) {
            if (dynamic_cast<StaSwitch*>(pDevice)->getQDCode() == nCode) {
                return pDevice;
            }
        }
        return nullptr;
    }

    QSize StationObject::getStaFixedSize()
    {
        return m_szStation * MainStation()->getDiploid(m_bMainStation ? DiploidRatio::StaDiploid : DiploidRatio::MultiDiploid);
    }

    void StationObject::setStaFixedSize(int nWidth, int nHeight)
    {
        m_szStation.setWidth(nWidth);
        m_szStation.setHeight(nHeight);
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
            StaTrain* pTrain = static_cast<StaTrain*>(pOrder);
            StaTrain::Init(pTrain, subObj);
            m_vecStaTrain.append(pTrain);
            DeviceTrain* pDevice = dynamic_cast<DeviceTrain*>(getDeviceByCode(pTrain->m_nPosCode));
            if (pDevice) {
                pDevice->SetTrain(pTrain);
            }
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
            if (m_nUpState > 10) {
                for (DeviceBase* pSwitch : m_mapDeviceVector[SWITCH]) {
                    pSwitch->setState(SECTION_STATE_TAKEUP);
                    dynamic_cast<StaSwitch*>(pSwitch)->setSwitchState(SWITCH_STATE_SK);
                }
                for (DeviceBase* pSignal : m_mapDeviceVector[SIGNALLAMP]) {
                    pSignal->setState(static_cast<int>(Device::SignalState::DS));
                }
                for (DeviceBase* pTrack : m_mapDeviceVector[TRACK]) {
                    pTrack->setState(SECTION_STATE_TAKEUP);
                }
            }
            m_nUpState++;
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

    int MainStationObject::ReadChartConfig(const QString& filePath)
    {
        QFile file("config/" + filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "无法打开JSON文件：" << file.errorString();
            return -1;
        }

        // 读取JSON数据
        QByteArray jsonData = file.readAll();
        file.close();

        // 解析JSON
        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &error);
        if (jsonDoc.isNull()) {
            qDebug() << "JSON解析错误：" << error.errorString();
            qDebug() << "位置：" << error.offset;
            qDebug() << "描述：" << error.errorString();
            return -1;
        }

        // 获取LINE数组
        QJsonObject rootObj = jsonDoc.object();
        QJsonArray lineArray = rootObj["LINE"].toArray();
        if (lineArray.isEmpty()) {
            qDebug() << "LINE数组为空";
            return -1;
        }

        // 提取第一个线路配置
        QJsonObject config = lineArray[0].toObject();

        // 解析基础参数rainDiagram.miniteDistance = getString.toInt();
        m_TrainDiagram.miniteDistance = config["YXT_MINITE_DISTANCE"].toInt();
        m_TrainDiagram.stationDistance = config["YXT_STATION_DISTANCE"].toInt();
        m_TrainDiagram.lintDistance = config["YXT_LINE_DISTANCE"].toInt();
        m_TrainDiagram.linenum = config["YXT_LINE_NUM"].toInt();

        // 解析所有进路
        QStringList keys = config.keys();
        for (const QString& key : keys) {
            if (key.startsWith("Station")) {
                RailwayLine railway;
                QJsonArray stations = config[key].toArray();
                QStringList stationList;
                for (const QJsonValue& val : stations) {
                    stationList << val.toString();
                }
                railway.startStation = stationList[0];
                railway.middleStation = stationList[1];
                railway.endStation = stationList[2];
                m_TrainDiagram.vectRailwayLine.push_back(railway);
            }
        }
        return 0;
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
        emit TrafficLogTableUpData();
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
    }

    int MainStationObject::AddNewTrain(StaTrain* pStaTrain)
    {
        QByteArray btResult = "-1";
        int nTrainId = 0;
        while (btResult != "") {
            if (Http::HttpClient::SelectStaTrain(++nTrainId, btResult)) {
                if (nTrainId > 256) {
                    nTrainId = 0;
                }
            }
        }
        pStaTrain->m_nTrainId = nTrainId;
        if (pStaTrain->m_bRealTrain) {
            if (Http::HttpClient::AddStaTrain(pStaTrain, btResult)) {
                m_vecStaTempTrain.append(pStaTrain);
                emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x60, pStaTrain->m_nTrainId, 0x01));
                return pStaTrain->m_nTrainId;
            }
        }
        else {
            m_vecStaTrain.append(pStaTrain);
            DeviceTrain* pDevice = dynamic_cast<Device::DeviceTrain*>(getDeviceByCode(pStaTrain->m_nPosCode));
            if (pDevice) {
                pDevice->SetTrain(pStaTrain);
            }
            else {
                qDebug() << "未找到设备:" << pStaTrain->m_nPosCode;
            }
        }
        return 0;
    }

    int MainStationObject::DeleteTrain(StaTrain* pStaTrain)
    {
        QByteArray btResult;
        if (Http::HttpClient::DeleteStaTrain(pStaTrain->m_nTrainId, btResult)) {
            StaTrain* pTempTrain = new Station::StaTrain(*pStaTrain);
            m_vecStaTempTrain.append(pTempTrain);
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x60, pStaTrain->m_nTrainId, 0x02));
            return 1;
        }
        return 0;
    }

    int MainStationObject::SetTrainRunning(StaTrain* pStaTrain, bool bRunning)
    {
        QByteArray btResult;
        QMap<QString, QByteArray> mapTrainAttr = {
            { "isRun",  QByteArray::number(bRunning) },
        };
        if (Http::HttpClient::UpdataStaTrainAttr(pStaTrain->m_nTrainId, mapTrainAttr, btResult)) {
            StaTrain* pTempTrain = new Station::StaTrain(*pStaTrain);
            pTempTrain->m_bRunning = bRunning;
            m_vecStaTempTrain.append(pTempTrain);
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x60, pStaTrain->m_nTrainId, 0x04 + bRunning));
            return 1;
        }
        return 0;
    }

    int MainStationObject::ChangeTrainNum(StaTrain* pStaTrain, const QString& strTrainNum)
    {
        QByteArray btResult;
        QMap<QString, QByteArray> mapTrainAttr = {
            { "trainNum", strTrainNum.toLocal8Bit() }
        };
        if (Http::HttpClient::UpdataStaTrainAttr(pStaTrain->m_nTrainId, mapTrainAttr, btResult)) {
            StaTrain* pTempTrain = new Station::StaTrain(*pStaTrain);
            pTempTrain->m_strTrainNum = strTrainNum;
            m_vecStaTempTrain.append(pTempTrain);
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x60, pStaTrain->m_nTrainId, 0x03));
            return 1;
        }
        return 0;
    }

    int MainStationObject::ChangeTrainAttr(StaTrain* pStaTrain, int nSpeed, const QString& strLocomotive, bool bElectric)
    {
        QByteArray btResult;
        QMap<QString, QByteArray> mapTrainAttr = { 
            { "strLocomotive", strLocomotive.toLocal8Bit() },
            { "speed", QByteArray::number(nSpeed) },
            { "electric", QByteArray::number(bElectric) }
        };
        if (Http::HttpClient::UpdataStaTrainAttr(pStaTrain->m_nTrainId, mapTrainAttr, btResult)) {
            StaTrain* pTempTrain = new Station::StaTrain(*pStaTrain);
            pTempTrain->m_nSpeed = nSpeed;
            pTempTrain->m_strLocomotive = strLocomotive;
            pTempTrain->m_bElectric = bElectric;
            m_vecStaTempTrain.append(pTempTrain);
            emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x60, pStaTrain->m_nTrainId, 0x09));
            return 1;
        }
        return 0;
    }

    int MainStationObject::AddNewStagePlan(StaStagePlan* pStaStagePlan)
    {
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
        pTrafficLog->m_nArrivalRouteId = nArrivalRouteId;
        pTrafficLog->m_nDepartRouteId = nDepartRouteId;
        QMap<QString, QByteArray> m_mapLogAttr = { 
            { "arrivalRouteId", QByteArray::number(pTrafficLog->m_nArrivalRouteId) },
            { "departRouteId", QByteArray::number(pTrafficLog->m_nDepartRouteId) }
        };
        if (Http::HttpClient::UpdataStaTrafficLogAttr(pTrafficLog->m_nLogId, m_mapLogAttr, btResult)) {
            emit Station::MainStation()->TrafficLogTableUpData();
            emit Station::MainStation()->TrainRouteUpData();
        }
    }

    void MainStationObject::TrainRouteTrigger(StaTrainRoute* pCurRoute, const QVector<StaTrainRoute*>& vecRouteList, const QString& strType)
    {
        QString strTips = QString("确定要触发<%1>,股道:%2','%3'次列车,'%4','%5'进路?")
            .arg(MainStation()->getStationName())
            .arg(pCurRoute->m_strTrack)
            .arg(pCurRoute->getTrainNum())
            .arg(getTriggerString(vecRouteList))
            .arg(strType);
        if (QMessageBox::question(nullptr, MSGBOX_TITTLE, strTips, "是", "否")) {
            return;
        }
        QByteArray btResult;
        QMap<QString, QByteArray> m_mapRouteAttr;
        for (StaTrainRoute* pRoute : vecRouteList) {
            m_mapRouteAttr = {
                { "routeState", QByteArray::number(pRoute->m_nRouteState) }
            };
            if (Http::HttpClient::UpdataStaTrainRouteAttr(pRoute->m_nRouteId, m_mapRouteAttr, btResult)) {
                pRoute->m_nRouteState = 1;
                SendPacketMsg(TARGET_INTERLOCK, 0x61, pRoute->m_nRouteId, 0x02);
            }
        }
        emit TrainRouteUpData();
    }

    void MainStationObject::TrainRouteTriggerChange(const QVector<StaTrainRoute*>& vecRouteList, bool bAutoTouch)
    {
        QByteArray btResult;
        QMap<QString, QByteArray> m_mapRouteAttr;
        for (StaTrainRoute* pRoute : vecRouteList) {
            if (pRoute->m_nRouteState != 0) {
                QMessageBox::information(nullptr, MSGBOX_TITTLE, "进路状态不允许修改触发方式!", "确定");
                continue;
            }
            m_mapRouteAttr = {
                { "autoTouch", QByteArray::number(bAutoTouch) }
            };
            if (Http::HttpClient::UpdataStaTrainRouteAttr(pRoute->m_nRouteId, m_mapRouteAttr, btResult)) {
                pRoute->m_bAutoTouch = bAutoTouch;
            }
        }
        emit TrainRouteUpData();
    }

    bool MainStationObject::TrainRouteTrackChange(const QVector<StaTrainRoute*>& vecRouteList, Device::DeviceBase* pTrack)
    {
        if (!pTrack) {
            return false;
        }
        for (StaTrainRoute* pRoute : vecRouteList) {
            if (pRoute->m_nRouteState != 0) {
                QMessageBox::information(nullptr, MSGBOX_TITTLE, "进路状态不允许修改到达股道!", "确定");
                return false;
            }
        }

        QByteArray btResult;
        QMap<QString, QByteArray> m_mapRouteAttr;
        for (StaTrainRoute* pRoute : vecRouteList) {
            m_mapRouteAttr = {
                { "trackCode", QByteArray::number(pTrack->getCode()) }
            };
            if (Http::HttpClient::UpdataStaTrainRouteAttr(pRoute->m_nRouteId, m_mapRouteAttr, btResult)) {
                pRoute->ChangeTrack(pTrack->getCode(), pTrack->getName());
                MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x61, pRoute->m_nRouteId, 0x04);
            }
        }
        emit TrainRouteUpData();
        return true;
    }

    bool MainStationObject::TrainRouteSignalChange(const QVector<StaTrainRoute*>& vecRouteList, Device::DeviceBase* pSignal)
    {
        if (!pSignal) {
            return false;
        }
        for (StaTrainRoute* pRoute : vecRouteList) {
            if (pRoute->m_nRouteState != 0) {
                QMessageBox::information(nullptr, MSGBOX_TITTLE, "进路状态不允许修改信号机!", "确定");
                return false;
            }
        }
        QByteArray btResult;
        QMap<QString, QByteArray> m_mapRouteAttr;
        for (StaTrainRoute* pRoute : vecRouteList) {
            m_mapRouteAttr = {
                { "signalCode", QByteArray::number(pSignal->getCode()) }
            };
            if (Http::HttpClient::UpdataStaTrainRouteAttr(pRoute->m_nRouteId, m_mapRouteAttr, btResult)) {
                pRoute->ChangeSignal(pSignal->getCode(), pSignal->getName());
                MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x61, pRoute->m_nRouteId, 0x04);
            }
        }
        emit TrainRouteUpData();
        return true;
    }

    bool MainStationObject::DeleteTrainRoute(const QVector<StaTrainRoute*>& vecRouteList)
    {
        for (StaTrainRoute* pRoute : vecRouteList) {
            if (pRoute->m_nRouteState != 0) {
                QMessageBox::information(nullptr, MSGBOX_TITTLE, "进路状态不允许删除!", "确定");
                return false;
            }
        }
        QByteArray btResult;
        for (StaTrainRoute* pRoute : vecRouteList) {
            if (Http::HttpClient::DeleteStaTrainRoute(pRoute->m_nRouteId, btResult)) {
                emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x61, pRoute->m_nRouteId, 0x05));
            }
        }
        emit TrainRouteUpData();
        return true;
    }

    QString MainStationObject::getTriggerString(const QVector<StaTrainRoute*>& vecRouteList)
    {
        QString strDescrip;
        for (StaTrainRoute* pRoute : vecRouteList) {
            strDescrip.append(pRoute->m_strCurRouteDescrip);
            if (pRoute != vecRouteList.back()) {
                strDescrip.append(";");
            }
        }
        return strDescrip;
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
        return !m_StaLimits.bExStaControl && m_StaLimits.nControlMode;
    }

    bool MainStationObject::IsAllowShuntTrainOnly()
    {
        return !m_StaLimits.bExStaControl && m_StaLimits.nControlMode == 0x02;
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

    StaTrain* MainStationObject::getStaTempTrainById(int nTrainId)
    {
        for (StaTrain* pTrain : m_vecStaTempTrain) {
            if (pTrain->m_nTrainId == nTrainId) {
                return pTrain;
            }
        }
        return nullptr;
    }

    QVector<StaTrainRoute*> MainStationObject::getStaTrainRouteByTrain(int nTrainID)
    {
        QVector<StaTrainRoute*> vecTrainRoute;
        for (StaTrainRoute* pTrainRoute : m_vecStaTrainRoute) {
            if (pTrainRoute->m_nTrainId == nTrainID && !pTrainRoute->m_bSunTrainRoute) {
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

    StaTrafficLog* MainStationObject::getStaTrafficLogByTrain(int nTrainID)
    {
        for (StaTrafficLog* pTrafficLog : m_vecStaTrafficLog) {
            if (pTrafficLog->m_nTrainId == nTrainID) {
                return pTrafficLog;
            }
        }
        return nullptr;
    }

    void MainStationObject::RemoveTrain(StaTrain* pTrain)
    {
        if (pTrain) {
            DeviceTrain* pDevice = dynamic_cast<DeviceTrain*>(getDeviceByCode(pTrain->m_nPosCode));
            if (pDevice) {
                pDevice->SetTrain(nullptr);
                pTrain->m_bDelete = true;
            }
            m_vecStaTrain.removeOne(pTrain);
        }
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

    void MainStationObject::onReciveData(const QByteArray& dataArray)
    {
        //帧头帧尾校验
        if (dataArray.left(4) == QByteArray::fromHex("efefefef") && dataArray.right(4) == QByteArray::fromHex("fefefefe")) {
            if ((dataArray[4] & 0xFF) + ((dataArray[5] & 0xFF) << 8) != dataArray.length()) {
                qDebug() << "长度校验失败!";
            }
            m_nUpState = 0;
            QByteArrayList byteList = m_pProtocol->UnpackData(dataArray).split('#');
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

    void MainStationObject::onUserLogin(QString strUserName, QString strPassword)
    {
        m_infoCurrUser.m_strName = strUserName;
        m_infoCurrUser.m_strPassWord = strPassword;
        emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_CULTIVATE, 0x80));
    }

    void MainStationObject::onOrderClear(bool bClearTwinkle)
    {
        ClearDevice(bClearTwinkle);
        emit FunBtnStateReset();
    }

    void MainStationObject::onOrderIssued()
    {
        switch (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType())
        {
        case CTCWindows::FunType::RouteBuild: {
            bool bEnter = false;
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getOperObjType() == CTCWindows::OperObjType::Train) {
                bEnter = CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::InputTrain);
            }
            else if (CTCWindows::BaseWnd::StaFunBtnToolBar::getOperObjType() == CTCWindows::OperObjType::Shunt) {
                bEnter = CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::InputShuntingTime);
            }
            if (!bEnter) {
                emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x40,
                    static_cast<int>(CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType()),
                    static_cast<int>(CTCWindows::BaseWnd::StaFunBtnToolBar::getOperObjType())));
                onOrderClear();
                return;
            }
        } break;
        case CTCWindows::FunType::GuideBtn: {
            CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "引导按钮"); 
        } break;
        case CTCWindows::FunType::GuideClock: {
            switch (CTCWindows::BaseWnd::StaFunBtnToolBar::getOperObjType())
            {
            case CTCWindows::OperObjType::Ascend:
                CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "S引导总锁");
                break;
            case CTCWindows::OperObjType::Descend:
                CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "X引导总锁");
                break;
            default:
                break;
            }
        } break;
        case CTCWindows::FunType::TotalRelieve: {
            CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "总人解");
        } break;
        case CTCWindows::FunType::RegionRelieve: {
            CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "区故解");
        } break;
        case CTCWindows::FunType::Lighting: {
            CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "点灯");
        } break;
        case CTCWindows::FunType::UnLighting: {
            CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "灭灯");
        } break;
        case CTCWindows::FunType::RampUnlock: {
            CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "坡道解锁");
        } break;
        case CTCWindows::FunType::PoorRoute: {
            CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "分路不良");
            for (DeviceBase* pDevice : m_vecSelectDevice) {
                dynamic_cast<StaSection*>(pDevice)->SetShuntFault(static_cast<int>(CTCWindows::BaseWnd::StaFunBtnToolBar::getOperObjType()));
            }
        } break;
        case CTCWindows::FunType::IdleConfirm: {
            CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, "分路不良");
            for (DeviceBase* pDevice : m_vecSelectDevice) {
                dynamic_cast<StaSection*>(pDevice)->SetShuntFaultIdle();
            }
        } break;
        case CTCWindows::FunType::FunBtn: {
            QString strDeviceName = m_vecSelectDevice[0]->getName();
            switch (CTCWindows::BaseWnd::StaFunBtnToolBar::getOperObjType())
            {
            case CTCWindows::OperObjType::Blockage:
                CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, strDeviceName + "闭塞");
                break;
            case CTCWindows::OperObjType::Restoration:
                CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, strDeviceName + "事故");
                break;
            case CTCWindows::OperObjType::Accident:
                CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, strDeviceName + "复原");
                break;
            case CTCWindows::OperObjType::TotalAux:
                CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, strDeviceName + "总辅助");
                break;
            case CTCWindows::OperObjType::PickUpAux:
                CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, strDeviceName + "接车辅助");
                break;
            case CTCWindows::OperObjType::DepartureAux:
                CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, strDeviceName + "发车辅助");
                break;
            case CTCWindows::OperObjType::DirectionChange:
                CTCWindows::SealTechnique::InsertSealRecord(m_strStationName, strDeviceName + "允许改方");
                break;
            default:
                break;
            }
        } break;
        default:
            break;
        }
        
        emit SendDataToTCP(m_pStaPacket->PackOrder(TARGET_INTERLOCK, 0x40, 
            static_cast<int>(CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType()),
            static_cast<int>(CTCWindows::BaseWnd::StaFunBtnToolBar::getOperObjType())));
        onOrderClear(); 
        emit FunBtnStateReset();
    }

    void MainStationObject::CompareResult(const QByteArray& dataArray)
    {
        if (m_pCurSubject) {
            m_pCurSubject->CompareResult(dataArray);
        }
    }

    void MainStationObject::StationReset()
    {
        for (StaStagePlan* pStagePlan : m_vecStaStagePlan) {
            delete pStagePlan;
            pStagePlan = nullptr;
        }
        QVector<StaStagePlan*>().swap(m_vecStaStagePlan);
        qDebug() << "已清空阶段计划";

        for (StaTrain* pTrain : m_vecStaTrain) {
            pTrain->m_bDelete = true;
        }
        QVector<StaTrain*>().swap(m_vecStaTrain);
        qDebug() << "已清空车次信息";

        for (StaTrainRoute* pTrainRoute : m_vecStaTrainRoute) {
            delete pTrainRoute;
            pTrainRoute = nullptr;
        }
        QVector<StaTrainRoute*>().swap(m_vecStaTrainRoute);
        qDebug() << "已清空进路序列";
        emit TrainRouteUpData();

        for (StaDispatchOrder* pDispatchOrder : m_vecStaDispatchOrder) {
            QVector<StaTrainDispatch*> m_vecTrainDispatch;
            for (StaTrainDispatch* pTrainDispatch : pDispatchOrder->m_vecTrainDispatch) {
                delete pTrainDispatch;
                pTrainDispatch = nullptr;
            }
            QVector<StaTrainDispatch*>().swap(pDispatchOrder->m_vecTrainDispatch);
            delete pDispatchOrder;
            pDispatchOrder = nullptr;
        }
        QVector<StaDispatchOrder*>().swap(m_vecStaDispatchOrder);
        qDebug() << "已清空调度命令";

        for (StaTrafficLog* pTrafficLog : m_vecStaTrafficLog) {
            delete pTrafficLog;
            pTrafficLog = nullptr;
        }
        QVector<StaTrafficLog*>().swap(m_vecStaTrafficLog);
        qDebug() << "已清空行车日志";
        emit TrafficLogTableUpData();

        SelectStaLimits();
    }

    void MainStationObject::ClearDevice(bool bClearTwinkle)
    {
        for (DeviceBase* pDevice : m_vecSelectDevice) {
            pDevice->OrderClear(bClearTwinkle);
        }
        for (DeviceBase* pDevice : getDeviceVectorByType(SIGNALLAMP)) {
            dynamic_cast<StaSignal*>(pDevice)->setFlash(false);
        }
        QVector<Device::DeviceBase*>().swap(m_vecSelectDevice);
        Device::StaSignal::ClearFlashSignal();
    }

    int MainStationObject::ReadInterLock(const QString& filePath)
    {  // 打开txt文件
        QFile file("config/" + filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "无法打开txt文件";
            return -1;
        }

        QTextStream strInTxtStream(&file);
        while (!strInTxtStream.atEnd()) {
            InterLockfileAnalysis(strInTxtStream.readLine());
        }
        file.close();
        return 0;
    }

    void MainStationObject::InterLockfileAnalysis(QString strLine)
    {
        strLine.replace(' ', "");
        if (strLine.left(2) == "//") {
            return;
        }
        
        QStringList list = strLine.split("|", Qt::SkipEmptyParts, Qt::CaseSensitive);
        if (list.size() > 2) {
            Device::SignalBtn* pSignalBtn = new Device::SignalBtn;
            QStringList strSingalList = list[2].split(",");
            QString strSingal;
            for (int i = 0; i < strSingalList.size(); i++) {
                strSingal = strSingalList[i];
                if (i == 0) {
                    if (strSingal.right(2) == "LA") {
                        pSignalBtn->nType = SIGNAL_TYPE_LC;
                    }
                    else if (strSingal.left(1) == "D" || strSingal.right(2) == "DA") {
                        pSignalBtn->nType = SIGNAL_TYPE_DC;
                    }
                    else if (strSingal.right(2) == "TA") {
                        pSignalBtn->nType = SIGNAL_TYPE_TG;
                    }
                }

                if (strSingal.right(2) == "TA") {
                    strSingal = strSingal.left(strSingal.size() - 2);
                }
                else if (strSingal.right(2) == "LA") {
                    strSingal = strSingal.left(strSingal.size() - 2);
                }
                else if (strSingal.left(1) == "D") {
                    strSingal = strSingal.left(strSingal.size() - 1);
                }
                else if (strSingal.right(2) == "DA") {
                    strSingal = strSingal.left(strSingal.size() - 2);
                }
                pSignalBtn->strBtnNameList.append(strSingal);
            }
            m_vecSignalBtn.append(pSignalBtn);
        }
    }

    void MainStationObject::PutThrough(int nTime)
    {
        for (DeviceBase* pSwitch : m_mapDeviceVector[SWITCH]) {
            dynamic_cast<StaSwitch*>(pSwitch)->setPutThrough(true);
        }
        for (DeviceBase* pTrack : m_mapDeviceVector[TRACK]) {
            dynamic_cast<StaTrack*>(pTrack)->setPutThrough(true);
        }
        QTimer::singleShot(nTime * 1000, [&]() {
            for (DeviceBase* pSwitch : m_mapDeviceVector[SWITCH]) {
                dynamic_cast<StaSwitch*>(pSwitch)->setPutThrough(false);
            }
            for (DeviceBase* pTrack : m_mapDeviceVector[TRACK]) {
                dynamic_cast<StaTrack*>(pTrack)->setPutThrough(false);
            }
        });
    }

    int MainStationObject::getStaLimits(Limits type)
    {
        switch (type)
        {
        case Station::Limits::RouteLimits:              return m_StaLimits.nRouteLimits;
        case Station::Limits::AutoSignStage:            return m_StaLimits.bAutoSignStage;
        case Station::Limits::ExStaControl:             return m_StaLimits.bExStaControl;
        case Station::Limits::ControlMode:              return m_StaLimits.nControlMode;
        case Station::Limits::PlanMode:                 return m_StaLimits.nPlanMode;
        case Station::Limits::PlanControl:              return m_StaLimits.nPlanControl;
        case Station::Limits::ApplyControlMode:         return m_StaLimits.nApplyControlMode;
        case Station::Limits::ActiveApplyControlMode:   return m_StaLimits.nActiveApplyControlMode;
        default: return 0;
        }
    }

    void MainStationObject::setStaLimits(Limits type, int nValue)
    {
        switch (type)
        {
        case Station::Limits::RouteLimits:              m_StaLimits.nRouteLimits = nValue;              break;
        case Station::Limits::AutoSignStage:            m_StaLimits.bAutoSignStage = nValue;            break;
        case Station::Limits::ExStaControl:             m_StaLimits.bExStaControl = nValue;             break;
        case Station::Limits::ControlMode:              m_StaLimits.nControlMode = nValue;              break;
        case Station::Limits::PlanMode:                 m_StaLimits.nPlanMode = nValue;                 break;
        case Station::Limits::PlanControl:              m_StaLimits.nPlanControl = nValue;              break;
        case Station::Limits::ApplyControlMode:         m_StaLimits.nApplyControlMode = nValue;         break;
        case Station::Limits::ActiveApplyControlMode:   m_StaLimits.nActiveApplyControlMode = nValue;   break;
        default:                                                                                        break;
        }
        emit FunBtnStateReset();
    }
}