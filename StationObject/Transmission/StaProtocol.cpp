﻿#include "StaProtocol.h"
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
            m_mapUnPackOrder.insert(0x80, [=](const QByteArray& dataAyyay) { return UnpackLogin(dataAyyay); });
            m_mapUnPackOrder.insert(0x2f, [=](const QByteArray& dataAyyay) { return UnpackStationReset(dataAyyay); });
            m_mapUnPackOrder.insert(0x30, [=](const QByteArray& dataAyyay) { return UnpackStaViewState(dataAyyay); });
            m_mapUnPackOrder.insert(0x50, [=](const QByteArray& dataAyyay) { return UnpackStaStagePlan(dataAyyay); });
            m_mapUnPackOrder.insert(0x51, [=](const QByteArray& dataAyyay) { return UnpackStaDispatchOrder(dataAyyay); });
            m_mapUnPackOrder.insert(0x81, [=](const QByteArray& dataAyyay) { return UnpackCultivate(dataAyyay); });
            m_mapUnPackOrder.insert(0x82, [=](const QByteArray& dataAyyay) { return UnpackPractice(dataAyyay); });
            m_mapUnPackOrder.insert(0x60, [=](const QByteArray& dataAyyay) { return UnpackTrain(dataAyyay); });
            m_mapUnPackOrder.insert(0x61, [=](const QByteArray& dataAyyay) { return UnpackTrainRoute(dataAyyay); });
            m_mapUnPackOrder.insert(0x70, [=](const QByteArray& dataAyyay) { return UnpackLimits(dataAyyay); });
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

        QByteArray StaProtocol::UnpackData(const QByteArray& dataAyyay)
        {
            if (m_mapUnPackOrder.contains(dataAyyay[10] & 0xFF)) {
                return m_mapUnPackOrder[dataAyyay[10] & 0xFF](dataAyyay);
            }
            else {
                return dataAyyay;
            }
        }

        QByteArray StaProtocol::UnpackLogin(const QByteArray& dataAyyay)
        {
            MainStation()->setUserId(dataAyyay[12] & 0xFF);
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackStationReset(const QByteArray& dataAyyay)
        {
            MainStation()->StationReset();
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackStaViewState(const QByteArray& dataAyyay)
        {
            int nFlag = 11;
            //道岔
            {
                StaSwitch* pSwitch = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SWITCH]) {
                    pSwitch = dynamic_cast<StaSwitch*>(pDevice);
                    pSwitch->setSwitchState(dataAyyay[nFlag] & 0x0f);
                    pSwitch->setState((dataAyyay[nFlag++] >> 4) & 0x0f);
                    pSwitch = nullptr;
                }
            }
            //区段
            {  
                bool bAddByte = false;
                StaSwitchSection* pSection = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SECTION]) {
                    pSection = dynamic_cast<StaSwitchSection*>(pDevice);

                    DeviceBase* pSubDevice = nullptr;
                    for (int nSubDeviceCode : pSection->m_vecSectionsCode) {
                        pSubDevice = MainStation()->getDeviceByCode(nSubDeviceCode);
                        if (pSubDevice->getType() == 1) {
                        }
                    }

                    if (!bAddByte) {
                        pSubDevice->setState(dataAyyay[nFlag] & 0x0f);

                        if (pSection == m_mapDeviceVector[SECTION].at(m_mapDeviceVector[SECTION].size() - 1)) {
                            nFlag++;
                        }
                    }
                    else {
                        pSubDevice->setState((dataAyyay[nFlag] >> 4) & 0x0f);
                        nFlag++;
                    }
                    bAddByte = !bAddByte;
                }
            }
            //信号机
            {
                StaSignal* pSignal = nullptr;
                int nFlagTemp = nFlag + m_mapDeviceVector[SIGNALLAMP].size(); //信号机按钮
                int nFlagTemp2 = nFlagTemp + m_mapDeviceVector[SIGNALLAMP].size(); //信号表示器
                bool bAddByte = false;
                bool bAddByte2 = false;
                for (DeviceBase* pDevice : m_mapDeviceVector[SIGNALLAMP]) {
                    pSignal = dynamic_cast<StaSignal*>(pDevice);
                    pSignal->setState(dataAyyay[nFlag++] & 0xff);
                    pSignal = nullptr;
                }
            }
            //半自动闭塞
            {
                StaSemiAutoBlock* pSemiAutoBlock = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[SEMIAUTOBLOCK]) {
                    pSemiAutoBlock = dynamic_cast<StaSemiAutoBlock*>(pDevice);

                    pSemiAutoBlock->setArrowState(dataAyyay[nFlag++] & 0xff);
                    //pSemiAutoBlock->setState(dataAyyay[nFlag++] & 0xff);
                }
            }
            //场联
            {
                StaConnection* pConnection = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[CONNECTION]) {
                    pConnection = dynamic_cast<StaConnection*>(pDevice);
                    //qDebug() << pConnection->getName() << (dataAyyay[nFlag] & 0xff);
                    pConnection->setArrowState(dataAyyay[nFlag] & 0x33);
                    pConnection->setState(dataAyyay[nFlag++] & 0xcc);
                }
            }
            //机务段
            {
                bool bAddByte = false;
                StaLocomotive* pLocomotive = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[LOCOMOTIVE]) {
                    pLocomotive = dynamic_cast<StaLocomotive*>(pDevice);
                    if (!bAddByte) {
                        pLocomotive->setState(dataAyyay[nFlag] & 0x0f);
                        if (pLocomotive == m_mapDeviceVector[LOCOMOTIVE].at(m_mapDeviceVector[LOCOMOTIVE].size() - 1)) {
                            nFlag++;
                        }
                    }
                    else {
                        pLocomotive->setState((dataAyyay[nFlag++] >> 4) & 0x0f);
                    }
                    bAddByte = !bAddByte;
                }
            }
            //自动闭塞
            {
                StaAutoBlock* pAutoBlock = nullptr;
                for (DeviceBase* pDevice : m_mapDeviceVector[AUTOBLOCK]) {
                    pAutoBlock = dynamic_cast<StaAutoBlock*>(pDevice);
                    pAutoBlock->setArrowState(dataAyyay[nFlag] & 0x0f);
                    pAutoBlock->setState(dataAyyay[nFlag] & 0xf0);
                    pAutoBlock->setLeaveTrackState(dataAyyay[nFlag + 1]);
                    nFlag += 2;
                }
            }
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackStaStagePlan(const QByteArray& dataAyyay)
        {
            if (dataAyyay[16] == 0x01) {   //下发
                StaStagePlan* pStaStagePlan = new StaStagePlan;
                pStaStagePlan->m_strPlanNum = dataAyyay.mid(12, 4);
                int nFlag = 17;
                pStaStagePlan->m_nPlanType = dataAyyay[nFlag++];
                int len = dataAyyay[nFlag++];
                pStaStagePlan->m_strArrivalTrainNum = dataAyyay.mid(nFlag, len);
                nFlag += len;
                pStaStagePlan->m_nArrivalTrackCode = (dataAyyay[nFlag] & 0xFF) + (dataAyyay[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;
                pStaStagePlan->m_strArrivalTrack = MainStation()->getDeviceByCode(pStaStagePlan->m_nArrivalTrackCode)->getName();
                pStaStagePlan->m_nEntrySignalCode = (dataAyyay[nFlag] & 0xFF) + (dataAyyay[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;
                pStaStagePlan->m_strEntrySignal = MainStation()->getDeviceByCode(pStaStagePlan->m_nEntrySignalCode)->getName();
                QString strArrivaTime = QString("%1-%2-%3T%4:%5:%6")
                    .arg((dataAyyay[nFlag] & 0xFF) + (dataAyyay[nFlag + 1] & 0xFF) * 256)
                    .arg(dataAyyay[nFlag + 2] & 0xFF, 2, 10, QLatin1Char('0'))
                    .arg(dataAyyay[nFlag + 3] & 0xFF, 2, 10, QLatin1Char('0'))
                    .arg(dataAyyay[nFlag + 4] & 0xFF, 2, 10, QLatin1Char('0'))
                    .arg(dataAyyay[nFlag + 5] & 0xFF, 2, 10, QLatin1Char('0'))
                    .arg(dataAyyay[nFlag + 6] & 0xFF, 2, 10, QLatin1Char('0'));
                nFlag += 7;
                pStaStagePlan->m_tArrivalTime = QDateTime::fromString(strArrivaTime, Qt::ISODate);
                int len2 = dataAyyay[nFlag++];
                pStaStagePlan->m_strDepartTrainNum = dataAyyay.mid(nFlag, len2);
                nFlag += len2;
                pStaStagePlan->m_nDepartTrackCode = (dataAyyay[nFlag] & 0xFF) + (dataAyyay[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;
                pStaStagePlan->m_strDepartTrack = MainStation()->getDeviceByCode(pStaStagePlan->m_nArrivalTrackCode)->getName();
                pStaStagePlan->m_nExitSignalCode = (dataAyyay[nFlag] & 0xFF) + (dataAyyay[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;
                pStaStagePlan->m_strExitSignal = MainStation()->getDeviceByCode(pStaStagePlan->m_nExitSignalCode)->getName();
                QString strDepartTime = QString("%1-%2-%3T%4:%5:%6")
                    .arg((dataAyyay[nFlag] & 0xFF) + (dataAyyay[nFlag + 1] & 0xFF) * 256)
                    .arg(dataAyyay[nFlag + 2] & 0xFF, 2, 10, QLatin1Char('0'))
                    .arg(dataAyyay[nFlag + 3] & 0xFF, 2, 10, QLatin1Char('0'))
                    .arg(dataAyyay[nFlag + 4] & 0xFF, 2, 10, QLatin1Char('0'))
                    .arg(dataAyyay[nFlag + 5] & 0xFF, 2, 10, QLatin1Char('0'))
                    .arg(dataAyyay[nFlag + 6] & 0xFF, 2, 10, QLatin1Char('0'));
                nFlag += 7;
                pStaStagePlan->m_tDepartTime = QDateTime::fromString(strDepartTime, Qt::ISODate);
                pStaStagePlan->m_nJJQDCode = (dataAyyay[nFlag] & 0xFF) + (dataAyyay[nFlag + 1] & 0xFF) * 256;
                if (pStaStagePlan->m_nArrivalTrackCode == 0 || pStaStagePlan->m_nDepartTrackCode == 0) {
                    pStaStagePlan->m_nPlanType = 5;
                    pStaStagePlan->m_strArrivalTrack = "";
                    pStaStagePlan->m_strDepartTrack = "";
                }
                //QtConcurrent::run(MainStation(), &MainStationObject::AddNewStagePlan, pStaStagePlan);
                MainStation()->AddNewStagePlan(pStaStagePlan);
            }
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackStaDispatchOrder(const QByteArray& dataAyyay)
        {
            StaDispatchOrder* pDispatch = new StaDispatchOrder;
            int flag = 12;
            pDispatch->m_strOrderNum = QString::fromLocal8Bit(dataAyyay.mid(flag, 8));
            flag += 8;
            QString strDepartTime = QString("%1-%2-%3T%4:%5:%6")
                .arg((dataAyyay[flag] & 0xFF) + (dataAyyay[flag + 1] & 0xFF) * 256)
                .arg(dataAyyay[flag + 2] & 0xFF, 2, 10, QLatin1Char('0'))
                .arg(dataAyyay[flag + 3] & 0xFF, 2, 10, QLatin1Char('0'))
                .arg(dataAyyay[flag + 4] & 0xFF, 2, 10, QLatin1Char('0'))
                .arg(dataAyyay[flag + 5] & 0xFF, 2, 10, QLatin1Char('0'))
                .arg(dataAyyay[flag + 6] & 0xFF, 2, 10, QLatin1Char('0'));
            pDispatch->m_tSendTime = QDateTime::fromString(strDepartTime, Qt::ISODate);
            flag += 7;
            int len = dataAyyay[flag++] & 0xFF;
            pDispatch->m_strSendName = QString::fromLocal8Bit(dataAyyay.mid(flag, len));
            flag += len;
            len = dataAyyay[flag++] & 0xFF;
            pDispatch->m_strSendAgency = QString::fromLocal8Bit(dataAyyay.mid(flag, len));
            flag += len;
            len = dataAyyay[flag++] & 0xFF;
            pDispatch->m_strOrderTip = QString::fromLocal8Bit(dataAyyay.mid(flag, len));
            flag += len;
            len = (dataAyyay[flag] & 0xFF) + (dataAyyay[flag + 1] & 0xFF) * 256;
            flag += 2;
            pDispatch->m_strContent = QString::fromLocal8Bit(dataAyyay.mid(flag, len));
            flag += len;
            len = dataAyyay[flag++] & 0xFF;
            flag += len;
            QtConcurrent::run(MainStation(), &MainStationObject::AddNewDispatchOrder, pDispatch);
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackCultivate(const QByteArray& dataAyyay)
        {
            if (MainStation()->getUserId() != (dataAyyay[11] & 0xFF)) {
                return QByteArray();
            }

            CultivateObject::Subject::ClearTips();
            QByteArray byteToInterLock;
            QStringList subList;
            for (QString strOrder : QString(dataAyyay.mid(13, dataAyyay[12] & 0xFF)).split("@")) {
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

        QByteArray StaProtocol::UnpackPractice(const QByteArray& dataAyyay)
        {
            if (MainStation()->getUserId() != (dataAyyay[11] & 0xFF)) {
                return QByteArray();
            }

            if (dataAyyay[13] == 0x02) {   //提交本题
                MainStation()->SubmitCurSubject();
                return QByteArray();
            }
            else if (dataAyyay[13] == 0x03) { //上传结果
                CultivateObject::Subject* pSubject = MainStation()->getCurSubject();
                if (pSubject) {
                    int Result = dataAyyay[22] & 0xFF;
                    pSubject->setResult(Result);
                }
            }
            QString strMsg;
            if (dataAyyay[12] == 0x01 && dataAyyay[13] == 0x01) {    //基础实训
                MainStation()->setCurSubject(new CultivateObject::Subject(dataAyyay[14] & 0xFF, dataAyyay.mid(15, 7)));
                strMsg = QString(dataAyyay.mid(23, dataAyyay[22] & 0xFF));
            }
            else if (dataAyyay[12] == 0x02 && dataAyyay[13] == 0x01) {   //流程实训 
                MainStation()->setCurSubject(new CultivateObject::Subject(
                    dataAyyay[14] & 0xFF, dataAyyay.mid(15, 7), 
                    QString(dataAyyay.mid(23, dataAyyay[22] & 0xFF))));
                strMsg = QString(dataAyyay.mid(25 + (dataAyyay[22] & 0xFF), dataAyyay[24 + (dataAyyay[22] & 0xFF)] & 0xFF));
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

        QByteArray StaProtocol::UnpackTrain(const QByteArray& dataAyyay)
        {
            int nFlag = 13;
            int nTrainNumLen = dataAyyay[nFlag++] & 0xFF;
            QString strTrainNum = QString(dataAyyay.mid(14, nTrainNumLen));
            nFlag += nTrainNumLen;
            if (dataAyyay[11] == 0x01) {   //添加车次
                StaTrain* pTrain = new StaTrain;
                pTrain->m_nTrainId = dataAyyay[12];
                pTrain->m_strTrainNum = strTrainNum;
                pTrain->m_nPosCode = (dataAyyay[nFlag] & 0xFF) + (dataAyyay[nFlag + 1] & 0xFF) * 256;
                nFlag += 2;
                pTrain->m_bRight = dataAyyay[nFlag++];
                pTrain->m_bElectric = dataAyyay[nFlag++];
                pTrain->m_bFreightTrain = dataAyyay[nFlag++];
                pTrain->m_nOverLimitLevel = dataAyyay[nFlag++];
                pTrain->m_nSpeed = dataAyyay[nFlag++];
                int nTrainTypeLen = dataAyyay[nFlag++] & 0xFF;
                pTrain->m_strTrainType = QString(dataAyyay.mid(nFlag, nTrainNumLen));
                nFlag += nTrainTypeLen;

                QByteArray btResult;
                int nTrainId = 1;
                while (btResult != "") {
                    if (Http::HttpClient::SelectStaTrain(nTrainId, btResult)) {
                        nTrainId++;
                    }
                }
                pTrain->m_nTrainId = nTrainId;
                if (Http::HttpClient::AddStaTrain(pTrain, btResult)) {
                    MainStation()->TrainList().append(pTrain);
                    MainStation()->SendPacketMsg(TARGET_TEACHER, 0x60, pTrain->m_nTrainId, 0x07, pTrain->m_strTrainNum.toLocal8Bit());
                    DeviceBase* pDevice = MainStation()->getDeviceByCode(pTrain->m_nPosCode);
                    if (pDevice && (pDevice->getStrType() == TRACK || pDevice->getStrType() == AUTOBLOCK)) {
                        dynamic_cast<Device::StaTrack*>(pDevice)->SetTrain(pTrain);
                    }
                }
            }
            else if (dataAyyay[11] == 0x02) {   //删除车次
                QByteArray btResult;
                if (Http::HttpClient::DeleteStaTrain(dataAyyay[12], btResult)) {
                    StaTrain* pTrain = MainStation()->getStaTrainById(dataAyyay[12]);
                    if (pTrain) {
                        DeviceTrain* pDevice = dynamic_cast<DeviceTrain*>(MainStation()->getDeviceByCode(pTrain->m_nPosCode));
                        if (pDevice) {
                            pDevice->SetTrain(nullptr);
                        }
                        MainStation()->TrainList().removeOne(pTrain);
                    }
                }
            }
            else if (dataAyyay[11] == 0x03) {   //变更车次
                int nNewTrainNumLen = dataAyyay[nFlag++] & 0xFF;
                QString strNewTrainNum = QString(dataAyyay.mid(nFlag, nNewTrainNumLen));
                nFlag += nNewTrainNumLen;
                StaTrain* pTrain = MainStation()->getStaTrainById(dataAyyay[12]);
                if (pTrain) {
                    QByteArray btResult;
                    if (Http::HttpClient::ChangeTrainNum(dataAyyay[12], strTrainNum, btResult)) {
                        pTrain->m_strTrainNum = strNewTrainNum;
                    }
                }
            }
            else if (dataAyyay[11] == 0x04) {   //车次停稳
                StaTrain* pTrain = MainStation()->getStaTrainById(dataAyyay[12]);
                if (pTrain) {
                    QByteArray btResult;
                    if (Http::HttpClient::SetTrainRunState(dataAyyay[12], false, btResult)) {
                        pTrain->m_bRunning = false;
                    }
                }
            }
            else if (dataAyyay[11] == 0x05) {   //车次启动
                StaTrain* pTrain = MainStation()->getStaTrainById(dataAyyay[12]);
                if (pTrain) {
                    QByteArray btResult;
                    if (Http::HttpClient::SetTrainRunState(dataAyyay[12], true, btResult)) {
                        pTrain->m_bRunning = true;
                    }
                }
            }
            else if (dataAyyay[11] == 0x06) {   //更新位置
                qDebug() << "UpdataPos";
                StaTrain* pTrain = MainStation()->getStaTrainById(dataAyyay[12]);
                if (pTrain) {
                    DeviceBase* pDevice = MainStation()->getDeviceByCode(pTrain->m_nPosCode);
                    DeviceTrain* pDeviceTrain = dynamic_cast<DeviceTrain*>(MainStation()->getDeviceByCode(pTrain->m_nPosCode));
                    if (pDeviceTrain) {
                        pTrain->m_nPosCode = (dataAyyay[nFlag] & 0xFF) + (dataAyyay[nFlag + 1] & 0xFF) * 256;
                        DeviceBase* pNextDevice = MainStation()->getDeviceByCode(pTrain->m_nPosCode);
                        qDebug() << "getDeviceByCode";
                        if (pNextDevice) {
                            QByteArray btResult;
                            qDebug() << "UpdataTrainPos";
                            if (Http::HttpClient::UpdataTrainPos(dataAyyay[12], pNextDevice->getCode(), btResult)) {
                                pDeviceTrain->MoveTo(dynamic_cast<DeviceTrain*>(pNextDevice));
                            }
                        }
                    }
                }
            }
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackTrainRoute(const QByteArray& dataAyyay)
        {
            if (dataAyyay[11] == 0x03) {   //进路状态
                StaTrainRoute* pTrainRoute = MainStation()->getStaTrainRouteById(dataAyyay[12]);
                if (pTrainRoute) {
                    QByteArray btResult;
                    if (Http::HttpClient::ChangeTriggerType(dataAyyay[12], dataAyyay[13], btResult)) {
                        pTrainRoute->m_nRouteState = dataAyyay[13];
                    }
                }
            }
            return QByteArray();
        }

        QByteArray StaProtocol::UnpackLimits(const QByteArray& dataAyyay)
        {
            if (dataAyyay[11] == 0x01) {
                Station::MainStation()->setStaLimits(Station::Limits::RouteLimits, dataAyyay[12]);
            }
            else if (dataAyyay[11] == 0x02) {
                Station::MainStation()->setStaLimits(Station::Limits::ExStaControl, dataAyyay[12]);
            }
            if (dataAyyay[11] == 0x03) {
                Station::MainStation()->setStaLimits(Station::Limits::ApplyControlMode, dataAyyay[12]);
            }
            if (dataAyyay[11] == 0x04) {
                Station::MainStation()->setStaLimits(Station::Limits::PlanControl, dataAyyay[12]);
            }
            if (dataAyyay[11] == 0x04) {
                if (dataAyyay[12]) {
                    Station::MainStation()->setStaLimits(Station::Limits::ControlMode, 
                        Station::MainStation()->getStaLimits(Station::Limits::ApplyControlMode));
                    Station::MainStation()->setStaLimits(Station::Limits::ApplyControlMode, -1);
                }
            }
            return QByteArray();
        }
    }
}