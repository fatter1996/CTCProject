#include "StaProtocol.h"
#include <QtConcurrent>
#include <QDebug>
#include "Global.h"
#include "../Device/StaDevice.h"


namespace Station {
    namespace Transmission {
        using namespace Device;

        StaProtocol::StaProtocol(QMap<QString, QVector<DeviceBase*>>& mapDeviceVector)
            : m_mapDeviceVector(mapDeviceVector)
        {
            m_mapUnPackOrder.insert(0x80, [=](const QByteArray& dataArray) { return UnpackLogin(dataArray); });
            m_mapUnPackOrder.insert(0x2f, [=](const QByteArray& dataArray) { return UnpackStationReset(dataArray); });
            m_mapUnPackOrder.insert(0x30, [=](const QByteArray& dataArray) { return UnpackStaViewState(dataArray); });
            m_mapUnPackOrder.insert(0x50, [=](const QByteArray& dataArray) { return UnpackStaStagePlan(dataArray); });
            m_mapUnPackOrder.insert(0x51, [=](const QByteArray& dataArray) { return UnpackStaDispatchOrder(dataArray); });
            m_mapUnPackOrder.insert(0x81, [=](const QByteArray& dataArray) { return UnpackCultivate(dataArray); });
            m_mapUnPackOrder.insert(0x82, [=](const QByteArray& dataArray) { return UnpackPractice(dataArray); });
            m_mapUnPackOrder.insert(0x60, [=](const QByteArray& dataArray) { return UnpackTrain(dataArray); });
            m_mapUnPackOrder.insert(0x61, [=](const QByteArray& dataArray) { return UnpackTrainRoute(dataArray); });
            m_mapUnPackOrder.insert(0x70, [=](const QByteArray& dataArray) { return UnpackLimits(dataArray); });
            m_mapUnPackOrder.insert(0x72, [=](const QByteArray& dataArray) { return UnpackAdjTimePoint(dataArray); });

            InitSubjectProtocol();
            CultivateObject::Subject::InitConversionFun();
        }

        StaProtocol::~StaProtocol()
        {
        
        }

        void StaProtocol::InitSubjectProtocol()
        {
            m_mapSubjectOrderType = {
                //信号机相关操作
                {"LCJL", CultivateObject::SubjectType::LCJL},
                {"DCJL", CultivateObject::SubjectType::DCJL},
                {"TGJL", CultivateObject::SubjectType::TGJL},
                {"XHFS", CultivateObject::SubjectType::XHFS},
                {"LAFS", CultivateObject::SubjectType::LAFS},
                {"DAFS", CultivateObject::SubjectType::DAFS},
                {"XHJF", CultivateObject::SubjectType::XHJF},
                {"LAJF", CultivateObject::SubjectType::LAJF},
                {"DAJF", CultivateObject::SubjectType::DAJF},
                {"YDJL", CultivateObject::SubjectType::YDJL},
                //道岔相关操作
                {"DCDC", CultivateObject::SubjectType::DCDC},
                {"DCDW", CultivateObject::SubjectType::DCDW},
                {"DCFW", CultivateObject::SubjectType::DCFW},
                {"DCDS", CultivateObject::SubjectType::DCDS},
                {"DCDJ", CultivateObject::SubjectType::DCDJ},
                {"DCCF", CultivateObject::SubjectType::DCCF},
                {"DCJF", CultivateObject::SubjectType::DCJF},
                {"DCFLBL", CultivateObject::SubjectType::DCFLBL},
                //其他
                {"FLBL", CultivateObject::SubjectType::FLBL},
                {"JLZQX", CultivateObject::SubjectType::JLZQX},
                {"JLZRJ", CultivateObject::SubjectType::JLZRJ},
                {"QDGZHF", CultivateObject::SubjectType::QGJ},
                {"YDZS", CultivateObject::SubjectType::YDZS}
            };
        }

        QByteArray StaProtocol::UnpackData(const QByteArray& dataArray)
        {
            if (m_mapUnPackOrder.contains(dataArray[10] & 0xFF)) {
                return m_mapUnPackOrder[dataArray[10] & 0xFF](dataArray);
            }
            else {
                return dataArray;
            }
        }

        QByteArray StaProtocol::UnpackLogin(const QByteArray& dataArray)
        {
            MainStation()->setUserId(dataArray[12] & 0xFF);
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackStationReset(const QByteArray& dataArray)
        {
            MainStation()->StationReset();
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackStaViewState(const QByteArray& dataArray)
        {
            int nFlag = 11;
            //道岔
            {
                StaSwitch* pSwitch = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SWITCH]) {
                    pSwitch = dynamic_cast<StaSwitch*>(pDevice);
                    pSwitch->setSwitchState(dataArray[nFlag] & 0x0F);
                    pSwitch->setState((dataArray[nFlag++] >> 4) & 0x0F);
                    pSwitch = nullptr;
                }
            }
            //区段
            {  
                bool bAddByte = false;
                StaSwitchSection* pSection = nullptr;
                DeviceBase* pSubDevice = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SECTION]) {
                    pSection = dynamic_cast<StaSwitchSection*>(pDevice);
                    
                    for (int nSubDeviceCode : pSection->m_vecSectionsCode) {
                        pSubDevice = MainStation()->getDeviceByCode(nSubDeviceCode);
                    }
                    if (!bAddByte) {
                        if (pSubDevice->getStrType() != "DC") {
                            pSubDevice->setState(dataArray[nFlag] & 0x0F);
                        }
                    
                        if (pSection == m_mapDeviceVector[SECTION].at(m_mapDeviceVector[SECTION].size() - 1)) {
                            nFlag++;
                        }
                    }
                    else {
                        if (pSubDevice->getStrType() != "DC") {
                            pSubDevice->setState((dataArray[nFlag] >> 4) & 0x0F);
                        }
                        nFlag++;
                    }
                    bAddByte = !bAddByte;
                    pSection = nullptr;
                    pSubDevice = nullptr;
                }
            }
            //信号机
            {
                for (DeviceBase* pDevice : m_mapDeviceVector[SIGNALLAMP]) {
                    dynamic_cast<StaSignal*>(pDevice)->setState(dataArray[nFlag++] & 0xFF);
                }
            }
            //半自动闭塞
            {
                for (DeviceBase* pDevice : m_mapDeviceVector[SEMIAUTOBLOCK]) {
                    dynamic_cast<StaSemiAutoBlock*>(pDevice)->setArrowState(dataArray[nFlag++] & 0xFF);
                    //dynamic_cast<StaSemiAutoBlock*>(pDevice)->setState(dataArray[nFlag++] & 0xff);
                }
            }
            //自动闭塞
            {
                StaAutoBlock* pAutoBlock = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[AUTOBLOCK]) {
                    pAutoBlock = dynamic_cast<StaAutoBlock*>(pDevice);
                    pAutoBlock->setArrowState(dataArray[nFlag] & 0x0F);
                    pAutoBlock->setState(dataArray[nFlag] & 0xF0);
                    nFlag += 2;
                }
            }
            //场联
            {
                StaConnection* pConnection = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[CONNECTION]) {
                    pConnection = dynamic_cast<StaConnection*>(pDevice);
                    pConnection->setArrowState(dataArray[nFlag] & 0x33);
                    pConnection->setState(dataArray[nFlag++] & 0xCC);
                }
            }
            //机务段
            {
                bool bAddByte = false;
                StaLocomotive* pLocomotive = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[LOCOMOTIVE]) {
                    pLocomotive = dynamic_cast<StaLocomotive*>(pDevice);
                    if (!bAddByte) {
                        pLocomotive->setState(dataArray[nFlag] & 0x0F);
                        if (pLocomotive == m_mapDeviceVector[LOCOMOTIVE].at(m_mapDeviceVector[LOCOMOTIVE].size() - 1)) {
                            nFlag++;
                        }
                    }
                    else {
                        pLocomotive->setState((dataArray[nFlag++] >> 4) & 0x0F);
                    }
                    bAddByte = !bAddByte;
                }
            }
            
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackStaStagePlan(const QByteArray& dataArray)
        {
            if (dataArray[16] == 0x01) {   //下发
                StaStagePlan* pStaStagePlan = new StaStagePlan;
                int nFlag = 11;
                pStaStagePlan->m_nPlanId = dataArray[nFlag++] & 0xFF;
                pStaStagePlan->m_strPlanNum = dataArray.mid(nFlag, 4);
                nFlag += 4;
                nFlag++;
                pStaStagePlan->m_nPlanType = dataArray[nFlag++];
                int len = dataArray[nFlag++] & 0xFF;
                pStaStagePlan->m_strArrivalTrainNum = dataArray.mid(nFlag, len);
                nFlag += len;
                pStaStagePlan->m_nArrivalTrackCode = (dataArray[nFlag] & 0xFF) + (dataArray[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;
                pStaStagePlan->m_strArrivalTrack = MainStation()->getDeviceByCode(pStaStagePlan->m_nArrivalTrackCode, TRACK)->getName();
                pStaStagePlan->m_nEntrySignalCode = (dataArray[nFlag] & 0xFF) + (dataArray[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;
                pStaStagePlan->m_strEntrySignal = MainStation()->getDeviceByCode(pStaStagePlan->m_nEntrySignalCode, SIGNALLAMP)->getName();
                pStaStagePlan->m_tArrivalTime = ByteArrayToDateTime(dataArray.mid(nFlag, 7));
                nFlag += 7;
                len = dataArray[nFlag++];
                pStaStagePlan->m_strDepartTrainNum = dataArray.mid(nFlag, len);
                nFlag += len;
                pStaStagePlan->m_nDepartTrackCode = (dataArray[nFlag] & 0xFF) + (dataArray[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;
                pStaStagePlan->m_strDepartTrack = MainStation()->getDeviceByCode(pStaStagePlan->m_nArrivalTrackCode, TRACK)->getName();
                pStaStagePlan->m_nExitSignalCode = (dataArray[nFlag] & 0xFF) + (dataArray[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;
                pStaStagePlan->m_strExitSignal = MainStation()->getDeviceByCode(pStaStagePlan->m_nExitSignalCode, SIGNALLAMP)->getName();
                pStaStagePlan->m_tDepartTime = ByteArrayToDateTime(dataArray.mid(nFlag, 7));
                nFlag += 7;
                pStaStagePlan->m_bElectric = dataArray[nFlag++];
                pStaStagePlan->m_nOverLimitLevel = dataArray[nFlag++];
                pStaStagePlan->m_bFreightTrain = dataArray[nFlag++];
                pStaStagePlan->m_nJJQDCode = (dataArray[nFlag] & 0xFF) + (dataArray[nFlag + 1] & 0xFF) * 256;
                if (pStaStagePlan->m_nArrivalTrackCode == 0 || pStaStagePlan->m_nDepartTrackCode == 0) {
                    pStaStagePlan->m_nPlanType = 5;
                    pStaStagePlan->m_strArrivalTrack = "";
                    pStaStagePlan->m_strDepartTrack = "";
                }
                QtConcurrent::run(MainStation(), &MainStationObject::AddNewStagePlan, pStaStagePlan);
                //MainStation()->AddNewStagePlan(pStaStagePlan);
            }
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackStaDispatchOrder(const QByteArray& dataArray)
        {
            StaDispatchOrder* pDispatch = new StaDispatchOrder;
            int nFlag = 11;
            pDispatch->m_nOrderId = dataArray[nFlag++] & 0xFF;
            pDispatch->m_strOrderNum = QString::fromLocal8Bit(dataArray.mid(nFlag, 8));
            nFlag += 8;
            pDispatch->m_tSendTime = ByteArrayToDateTime(dataArray.mid(nFlag, 7));;
            nFlag += 7;
            int len = dataArray[nFlag++] & 0xFF;
            pDispatch->m_strSendName = QString::fromLocal8Bit(dataArray.mid(nFlag, len));
            nFlag += len;
            len = dataArray[nFlag++] & 0xFF;
            pDispatch->m_strSendAgency = QString::fromLocal8Bit(dataArray.mid(nFlag, len));
            nFlag += len;
            len = dataArray[nFlag++] & 0xFF;
            pDispatch->m_strOrderTip = QString::fromLocal8Bit(dataArray.mid(nFlag, len));
            nFlag += len;
            len = (dataArray[nFlag] & 0xFF) + (dataArray[nFlag + 1] & 0xFF) * 256;
            nFlag += 2;
            pDispatch->m_strContent = QString::fromLocal8Bit(dataArray.mid(nFlag, len));
            nFlag += len;
            len = dataArray[nFlag++] & 0xFF;
            nFlag += len;
            MainStation()->AddNewDispatchOrder(pDispatch);
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackCultivate(const QByteArray& dataArray)
        {
            if (MainStation()->getUserId() != (dataArray[11] & 0xFF)) {
                return QByteArray();
            }

            CultivateObject::Subject::ClearTips();
            QByteArray byteToInterLock;
            QStringList subList;
            for (QString strOrder : QString(dataArray.mid(13, dataArray[12] & 0xFF)).split("@")) {
                subList = strOrder.split(":");
                if (m_mapSubjectOrderType.contains(subList[0]) && subList.size() == 2) {
                    CultivateObject::Subject::SetDevSubject(subList[1].split(","), m_mapSubjectOrderType[subList[0]]);
                }
                else {
                    byteToInterLock.append(subList.join(":"));
                }
            }
            return byteToInterLock;
        }

        QByteArray StaProtocol::UnpackPractice(const QByteArray& dataArray)
        {
            if (MainStation()->getUserId() != (dataArray[11] & 0xFF)) {
                return QByteArray();
            }

            if (dataArray[13] == 0x02) {   //提交本题
                MainStation()->SubmitCurSubject();
                return QByteArray();
            }
            else if (dataArray[13] == 0x03) { //上传结果
                CultivateObject::Subject* pSubject = MainStation()->getCurSubject();
                if (pSubject) {
                    pSubject->setResult(dataArray[22]);
                }
            }
            QString strMsg;
            if (dataArray[12] == 0x01 && dataArray[13] == 0x01) {    //基础实训
                MainStation()->setCurSubject(new CultivateObject::Subject(dataArray[14] & 0xFF, dataArray.mid(15, 7)));
                strMsg = QString(dataArray.mid(23, dataArray[22] & 0xFF));
            }
            else if (dataArray[12] == 0x02 && dataArray[13] == 0x01) {   //流程实训 
                MainStation()->setCurSubject(new CultivateObject::Subject(
                    dataArray[14] & 0xFF, dataArray.mid(15, 7), 
                    QString(dataArray.mid(23, dataArray[22] & 0xFF))));
                strMsg = QString(dataArray.mid(25 + (dataArray[22] & 0xFF), dataArray[24 + (dataArray[22] & 0xFF)] & 0xFF));
            }
            
            QString strScene;   //场景
            if (strMsg.indexOf("#") >= 0) {
                strScene = strMsg.left(strMsg.indexOf("#"));
            }

            QByteArray byteToInterLock;
            if (!strScene.isEmpty()) {
                byteToInterLock.append(strScene);
            }
            byteToInterLock.append("#");

            QStringList subList;
            for (QString strOrder : strMsg.mid(strMsg.indexOf("#") + 1).split("@")) {
                subList = strOrder.split(":");
                if (m_mapSubjectOrderType.contains(subList[0])) {
                    MainStation()->getCurSubject()->SetSubjectArray((subList.size() == 2) ? subList[1].split(",") : QStringList(), m_mapSubjectOrderType[subList[0]]);
                }
                else {
                    byteToInterLock.append(subList.join(":"));
                }
            }
            return byteToInterLock;
        }

        QByteArray StaProtocol::UnpackTrain(const QByteArray& dataArray)
        {
            if (dataArray[11] == 0x01) {   //添加车次
                StaTrain* pTrain = new StaTrain;
                int nFlag = 12;
                pTrain->m_nTrainId = dataArray[nFlag++] & 0xFF;
                int nLen = dataArray[nFlag++] & 0xFF;
                pTrain->m_strTrainNum = dataArray.mid(nFlag, nLen);
                nFlag += nLen;
                pTrain->m_nPosCode = (dataArray[nFlag] & 0xFF) + (dataArray[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;

                pTrain->m_bRight = dataArray[nFlag++];
                pTrain->m_bElectric = dataArray[nFlag++];
                pTrain->m_bFreightTrain = dataArray[nFlag++];
                pTrain->m_nOverLimitLevel = dataArray[nFlag++];
                pTrain->m_nSpeed = dataArray[nFlag++];
                nLen = dataArray[nFlag++] & 0xFF;
                pTrain->m_strTrainType = dataArray.mid(nFlag, nLen);
                nFlag += nLen;
                
                MainStation()->AddTrain(pTrain);
                emit  MainStation()->TrafficLogTableUpData();
                emit  MainStation()->TrainRouteUpData();
                DeviceTrain* pDevice = dynamic_cast<Device::DeviceTrain*>(MainStation()->getDeviceByCode(pTrain->m_nPosCode));
                if (pDevice) {
                    pDevice->SetTrain(pTrain);
                }
                else {
                    qDebug() << "未找到设备:" << pTrain->m_nPosCode;
                }
            }
            else {
                StaTrain* pTrain = MainStation()->getStaTrainById(dataArray[12]);
                if (!pTrain) {
                    return QByteArray();
                }

                if (dataArray[11] == 0x02) {   //删除车次
                    MainStation()->RemoveTrain(pTrain);
                }
                else if (dataArray[11] == 0x03) {   //变更车次
                    pTrain->m_strTrainNum = QString(dataArray.mid(14, dataArray[13] & 0xFF));;
                }
                else if (dataArray[11] == 0x04) {   //车次停稳
                    pTrain->m_bRunning = false;
                }
                else if (dataArray[11] == 0x05) {   //车次启动
                    pTrain->m_bRunning = true;
                }
                else if (dataArray[11] == 0x06) {   //更新位置
                    qDebug() << "MoveTo";
                    DeviceBase* pDeviceTrain = MainStation()->getDeviceByCode(pTrain->m_nPosCode);
                    if (!pDeviceTrain) {
                        qDebug() << "Not found TrainPos";
                        return QByteArray();
                    }
                    pTrain->m_nPosCode = (dataArray[13] & 0xFF) + (dataArray[14] & 0xFF) * 256;
                    DeviceBase* pNextDevice = MainStation()->getDeviceByCode(pTrain->m_nPosCode);
                    if (!pNextDevice) {
                        pNextDevice = MainStation()->getSwitchBySectionCode(pTrain->m_nPosCode);
                    }
                    if (pNextDevice) {
                        if (pNextDevice != pDeviceTrain) {
                            dynamic_cast<DeviceTrain*>(pDeviceTrain)->MoveTo(dynamic_cast<DeviceTrain*>(pNextDevice));
                            pTrain->m_nPosCode = pNextDevice->getCode();
                            qDebug() << "MoveTo" << pNextDevice->getName();
                        }
                    }
                    else {
                        qDebug() << "Not found NextDevice" << pTrain->m_nPosCode;
                    }
                }
            }
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackTrainRoute(const QByteArray& dataArray)
        {
            if (dataArray[11] == 0x03) {   //进路状态
                StaTrainRoute* pTrainRoute = MainStation()->getStaTrainRouteById(dataArray[12]);
                if (pTrainRoute) {
                    QByteArray btResult;
                    if (Http::HttpClient::UpdataRouteState(dataArray[12], dataArray[13], btResult)) {
                        pTrainRoute->m_nRouteState = dataArray[13];
                        emit Station::MainStation()->TrainRouteUpData();
                    }
                }
            }
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackLimits(const QByteArray& dataArray)
        {
            if (dataArray[11] == 0x01) {
                MainStation()->setStaLimits(Limits::RouteLimits, dataArray[12]);
                CTCWindows::MainWindow()->RoutePlanWnd()->UpDataRouteLimits();
            }
            else if (dataArray[11] == 0x02) {
                MainStation()->setStaLimits(Limits::ExStaControl, dataArray[12]);
            }
            else if (dataArray[11] == 0x03) {
                MainStation()->setStaLimits(Limits::ApplyControlMode, dataArray[12]);
            }
            else if (dataArray[11] == 0x04) {
                MainStation()->setStaLimits(Limits::PlanControl, dataArray[12]);
            }
            else if (dataArray[11] == 0x05) {
                if (dataArray[12]) {
                    MainStation()->setStaLimits(Limits::ControlMode, 
                        MainStation()->getStaLimits(Limits::ActiveApplyControlMode));
                    MainStation()->setStaLimits(Limits::ActiveApplyControlMode, -1);
                }
            }
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackAdjTimePoint(const QByteArray& dataArray)
        {
            StaTrafficLog* pTrafficLog = MainStation()->getStaTrafficLogByTrain(dataArray[12] & 0xFF);
            if (pTrafficLog) {
                QDateTime date = ByteArrayToDateTime(dataArray.mid(13, 7));
                switch (dataArray[11])
                {
                case 0x01: pTrafficLog->m_tAdjDepartTime = date;    break;
                case 0x02: pTrafficLog->m_tAdjArrivalTime = date;   break;
                case 0x03: pTrafficLog->m_tAdjAgrDepartTime = date; break;
                case 0x04: pTrafficLog->m_tAdjDepartTime = date;    break;
                default:
                    break;
                }
                emit Station::MainStation()->TrafficLogTableUpData();
            }
            return QByteArray();
        }

        QDateTime StaProtocol::ByteArrayToDateTime(const QByteArray& dataArray)
        {
            QString strDepartTime = QString("%1-%2-%3T%4:%5:%6")
                .arg((dataArray[0] & 0xFF) + (dataArray[1] & 0xFF) * 256)
                .arg(dataArray[2], 2, 10, QLatin1Char('0'))
                .arg(dataArray[3], 2, 10, QLatin1Char('0'))
                .arg(dataArray[4], 2, 10, QLatin1Char('0'))
                .arg(dataArray[5], 2, 10, QLatin1Char('0'))
                .arg(dataArray[6], 2, 10, QLatin1Char('0'));

            return QDateTime::fromString(strDepartTime, Qt::ISODate);
        }
    }
}