#include "Global.h"
#include "../StationObject/Device/StaDevice.h"
#include <QJsonObject>
#pragma execution_character_set("utf-8")

namespace CTCWindows {
    CTCMainWindow* m_pMainWindow = nullptr;

    void SetMainWindow(CTCMainWindow* pMainWindow)
    {
        m_pMainWindow = pMainWindow;
    }

    CTCMainWindow* MainWindow()
    {
        return m_pMainWindow;
    }
}

namespace Station {
    MainStationObject* m_pMainStationObject = nullptr;
    
    void SetMainStation(MainStationObject* pStation)
    {
        m_pMainStationObject = pStation;
       
    }
  
    MainStationObject* MainStation()
    {
        return m_pMainStationObject;
    }

}

namespace Station {

    StaTrain::StaTrain(StaStagePlan* pStagePlan)
    {
        Device::DeviceBase* pSignal = nullptr;
        switch (pStagePlan->m_nPlanType)
        {
        case PLAN_TYPE_ARRIVED:
        case PLAN_TYPE_FINAL:
        case PLAN_TYPE_THROUGH: m_strTrainNum = pStagePlan->m_strArrivalTrainNum; break;
        case PLAN_TYPE_START: m_strTrainNum = pStagePlan->m_strDepartTrainNum; break;
        default:m_strTrainNum = pStagePlan->m_strArrivalTrainNum; break;
        }
        MainStation()->getDeviceByCode(pStagePlan->m_nEntrySignalCode, SIGNALLAMP)->getSXThroat();
        MainStation()->IsOverturn();
        switch (pStagePlan->m_nPlanType)
        {
        case PLAN_TYPE_ARRIVED:
        case PLAN_TYPE_FINAL:
        case PLAN_TYPE_THROUGH: {
            m_strTrainNum = pStagePlan->m_strArrivalTrainNum;
            pSignal = MainStation()->getDeviceByCode(pStagePlan->m_nEntrySignalCode, SIGNALLAMP);
            m_bRight = (pSignal->getSXThroat() && !MainStation()->IsOverturn());
        } break;
        case PLAN_TYPE_START: 
        {
            m_strTrainNum = pStagePlan->m_strDepartTrainNum;
            pSignal = MainStation()->getDeviceByCode(pStagePlan->m_nExitSignalCode, SIGNALLAMP);
            m_bRight = (!pSignal->getSXThroat() && !MainStation()->IsOverturn());
        } break; 
        }
        m_nPosCode = pStagePlan->m_nJJQDCode;
    }

    StaTrain::StaTrain(StaTrafficLog* TrafficLog)
    {
        switch (TrafficLog->m_nPlanType)
        {
        case PLAN_TYPE_ARRIVED: 
        case PLAN_TYPE_FINAL:
        case PLAN_TYPE_THROUGH: {
            m_strTrainNum = TrafficLog->m_strArrivalTrainNum;
            m_nTrainLengh = TrafficLog->m_nArrivalChange;
            m_strLocomotive = TrafficLog->m_strArrivalLocomotive;
            m_strDriver = TrafficLog->m_strArrivalDriver;
            m_nTrainMaster = TrafficLog->m_nArrivalTrainMaster;
            m_nTrainValue = TrafficLog->m_nArrivalTrainValue;
            m_nChangeLengh = TrafficLog->m_nArrivalChange;
            m_nWeight = TrafficLog->m_nArrivalWeight;
        } break;
        case PLAN_TYPE_START: {
            m_strTrainNum = TrafficLog->m_strDepartTrainNum;
            m_nTrainLengh = TrafficLog->m_nDepartChange;
            m_strLocomotive = TrafficLog->m_strDepartLocomotive;
            m_strDriver = TrafficLog->m_strDepartDriver;
            m_nTrainMaster = TrafficLog->m_nDepartTrainMaster;
            m_nTrainValue = TrafficLog->m_nDepartTrainValue;
            m_nChangeLengh = TrafficLog->m_nDepartChange;
            m_nWeight = TrafficLog->m_nDepartWeight;
        } break;
        }
        m_bRight = TrafficLog->m_bUpDown && !MainStation()->IsOverturn();
    }

    void StaTrain::SetMap()
    {
        m_mapTrainValue["trainNum"] = m_strTrainNum;
        m_mapTrainValue["direction"] = QString::number(m_bRight);
        m_mapTrainValue["trainLength"] = QString::number(m_nTrainLengh);
        m_mapTrainValue["speed"] = QString::number(m_nSpeed);
        m_mapTrainValue["electric"] = QString::number(m_bElectric);
        m_mapTrainValue["overLimitLevel"] = QString::number(m_nOverLimitLevel);
        m_mapTrainValue["freightTrain"] = QString::number(m_bFreightTrain);
        m_mapTrainValue["posCode"] = QString::number(m_nPosCode);
        m_mapTrainValue["trainTypeNumber"] = QString::number(m_nTrainType);
        m_mapTrainValue["operationTypeNumber"] = QString::number(m_nOperationType);
        m_mapTrainValue["isRun"] = QString::number(m_bRunning);
        m_mapTrainValue["army"] = QString::number(m_bArmy);
        m_mapTrainValue["keynote"] = QString::number(m_bImportant);
        m_mapTrainValue["trainId"] = QString::number(m_nTrainId);
    }

    QString StaTrain::OverLimitLevel()
    {
        switch (m_nOverLimitLevel)
        {
        case 0: return "";
        case 1: return "一级超限";
        case 2: return "二级超限";
        case 3: return "三级超限";
        case 4: return "超级超限";
        default: return "";
        }
    }

    void StaTrain::Init(StaTrain* pTrain, const QJsonObject& subObj)
    {
        pTrain->m_nTrainId = subObj.value("trainId").toInt();
        pTrain->m_strTrainNum = subObj.value("trainNum").toString();
        pTrain->m_bRight = subObj.value("direction").toInt();
        pTrain->m_nTrainLengh = subObj.value("trainLength").toInt();
        pTrain->m_nSpeed = subObj.value("speed").toInt();
        pTrain->m_bElectric = subObj.value("electric").toInt();
        pTrain->m_nOverLimitLevel = subObj.value("overLimitLevel").toInt();
        pTrain->m_bFreightTrain = subObj.value("freightTrain").toInt();
        pTrain->m_nPosCode = subObj.value("posCode").toInt();
        pTrain->m_nTrainType = subObj.value("trainTypeNumber").toInt();
        pTrain->m_strTrainType;	//列车类型
        pTrain->m_nOperationType = subObj.value("operationTypeNumber").toInt();
        pTrain->m_strOperationType;	//运行类型
        pTrain->m_bRunning = subObj.value("isRun").toInt();
        pTrain->m_bArmy = subObj.value("army").toInt();
        pTrain->m_bImportant = subObj.value("keynote").toInt();
    }

    void StaStagePlan::Init(StaStagePlan* pStagePlan, const QJsonObject& subObj)
    {
        pStagePlan->m_nPlanId = subObj.value("planId").toInt();
        pStagePlan->m_strPlanNum = subObj.value("planNum").toString();
        pStagePlan->m_nPlanType = subObj.value("planType").toInt();

        pStagePlan->m_strArrivalTrainNum = subObj.value("arriveTrainNum").toString();
        pStagePlan->m_nArrivalTrackCode = subObj.value("arriveTrackCode").toInt();
        pStagePlan->m_strArrivalTrack = MainStation()->getDeviceByCode(pStagePlan->m_nArrivalTrackCode, TRACK)->getName();
        pStagePlan->m_nEntrySignalCode = subObj.value("entrySignalCode").toInt();
        pStagePlan->m_strEntrySignal = MainStation()->getDeviceByCode(pStagePlan->m_nEntrySignalCode, SIGNALLAMP)->getName();
        pStagePlan->m_tArrivalTime = QDateTime::fromString(subObj.value("arriveTime").toString(), Qt::ISODate);

        pStagePlan->m_strDepartTrainNum = subObj.value("departTrainNum").toString();
        pStagePlan->m_nDepartTrackCode = subObj.value("arriveTrackCode").toInt();
        pStagePlan->m_strDepartTrack = MainStation()->getDeviceByCode(pStagePlan->m_nDepartTrackCode, TRACK)->getName();
        pStagePlan->m_nExitSignalCode = subObj.value("entrySignalCode").toInt();
        pStagePlan->m_strExitSignal = MainStation()->getDeviceByCode(pStagePlan->m_nExitSignalCode, SIGNALLAMP)->getName();
        pStagePlan->m_tDepartTime = QDateTime::fromString(subObj.value("departTime").toString(), Qt::ISODate);
    }

    StaTrainRoute::StaTrainRoute(StaTrafficLog* pTrafficLog, bool bArrivaRoute)
    {
        m_bArrivaRoute = bArrivaRoute;
        m_nTrainId = pTrafficLog->m_nTrainId;
        if (m_bArrivaRoute) {   //接车
            m_tPlanTime = pTrafficLog->m_tProvArrivalTime;
            m_tTriggerTime = pTrafficLog->m_tProvArrivalTime;
            m_nTrackCode = pTrafficLog->m_nArrivalTrackCode;
            m_strTrack = pTrafficLog->m_strArrivalTrack;
            m_nSignalCode = pTrafficLog->m_nArrivalSignalCode;
            m_strSignal = pTrafficLog->m_strArrivaSignal;
        }
        else {  //发车
            m_tPlanTime = pTrafficLog->m_tProvDepartTime;
            m_tTriggerTime = pTrafficLog->m_tProvDepartTime;
            m_nTrackCode = pTrafficLog->m_nDepartTrackCode;
            m_strTrack = pTrafficLog->m_strDepartTrack;
            m_nSignalCode = pTrafficLog->m_nDepartSignalCode;
            m_strSignal = pTrafficLog->m_strDepartSignal;
        }

        //进路方向
        Device::StaSignal* pSignal = dynamic_cast<Device::StaSignal*>(MainStation()->getDeviceByCode(m_nSignalCode, SIGNALLAMP));
        m_strDirection = pSignal->getDirection();
        if (pSignal->getSXThroat()) {
            if (m_bArrivaRoute) {   //接车
                m_strDirection = pSignal->getDirection() + "<--";
            }
            else {  //发车
                m_strDirection = pSignal->getDirection() + "-->";
            }
        }
        else {
            if (m_bArrivaRoute) {   //接车
                m_strDirection = "-->" + pSignal->getDirection();
            }
            else {  //发车
                m_strDirection = "<--" + pSignal->getDirection();
            }
        }
        getRouteDescrip();
    }

    void StaTrainRoute::ChangeTrack(int nCode, const QString& strName)
    {
        m_nTrackCode = nCode;
        m_strTrack = strName;
        getRouteDescrip();
    }

    void StaTrainRoute::SetMap()
    {
        m_mapRouteMember["trainId"] = QString::number(m_nTrainId);
        m_mapRouteMember["arriveRoute"] = QString::number(m_bArrivaRoute);
        m_mapRouteMember["autoTouch"] = QString::number(m_bAutoTouch);
        m_mapRouteMember["planTime"] = m_tPlanTime.toString(Qt::ISODate);
        m_mapRouteMember["triggerTime"] = m_tTriggerTime.toString(Qt::ISODate);
        m_mapRouteMember["cleanTime"] = m_tCleanTime.toString(Qt::ISODate);
        m_mapRouteMember["trackCode"] = QString::number(m_nTrackCode);
        m_mapRouteMember["signalCode"] = QString::number(m_nSignalCode);
        m_mapRouteMember["routeDepict"] = m_strCurRouteDescrip;
        m_mapRouteMember["direction"] = m_strDirection;
        m_mapRouteMember["routeState"] = QString::number(m_nRouteState);
        m_mapRouteMember["subRouteId"] = QString::number(m_vecSubRouteId.size() - 1);
        m_mapRouteMember["logId"] = QString::number(m_nLogId);
    }

    QString StaTrainRoute::getStateStr()
    {
        switch (m_nRouteState)
        {
        case 0: return "等待触发";
        case 1: return "正在触发";
        case 2: return "触发完成";
        case 3: return "占用";
        case 4: return "出清";
        case 5: return "人工取消";
        default: return "等待触发";
        }
    }

    void StaTrainRoute::getRouteDescrip()
    {
        m_strRouteDescripList.clear();
        QMap<QString,QVector<int>> mapTrackAdjSingal = Station::MainStation()->getTrackAdjSingal();
        Device::DeviceBase* pDevice = nullptr;
        QString strEntrySingal;
        QString strExitSingal;
        for (Device::SignalBtn* pSingalBtn : MainStation()->getSignalBtn()) {
            
            if (m_bArrivaRoute) {
                strEntrySingal = pSingalBtn->strBtnNameList[0];
                if (strEntrySingal == "SJ") {
                    strExitSingal = pSingalBtn->strBtnNameList[pSingalBtn->strBtnNameList.size() - 2];
                }
                else {
                    strExitSingal = pSingalBtn->strBtnNameList[pSingalBtn->strBtnNameList.size() - 1];
                }
            }
            else {
                strExitSingal = pSingalBtn->strBtnNameList[0];
                if (strExitSingal == "SJ") {
                    strEntrySingal = pSingalBtn->strBtnNameList[pSingalBtn->strBtnNameList.size() - 2];
                }
                else {
                    strEntrySingal = pSingalBtn->strBtnNameList[pSingalBtn->strBtnNameList.size() - 1];
                }
            }
        
            if (strEntrySingal != m_strSignal) {
                continue;
            }
            pDevice = MainStation()->getDeviceByName(strExitSingal, SIGNALLAMP);
            if (!pDevice) {
                continue;
            }
            for (int nCode : mapTrackAdjSingal[m_strTrack]) {
                if (nCode == pDevice->getCode()) {
                    QString strDescrip;
                    if (m_bArrivaRoute) {
                        strDescrip.append(pSingalBtn->strBtnNameList[0]);
                        strDescrip.append(",");
                        strDescrip.append(pDevice->getName());
                    }
                    else {
                        strDescrip.append(pDevice->getName());
                        strDescrip.append(",");
                        strDescrip.append(pSingalBtn->strBtnNameList[pSingalBtn->strBtnNameList.size()-1]);
                    }
                    qDebug() << strDescrip;
                    m_strRouteDescripList.append(strDescrip);
                    break;
                }
            }
        }
        if (m_strRouteDescripList.size() > 0) {
            m_strCurRouteDescrip = m_strRouteDescripList[0];
        }
    }

    void StaTrainRoute::Init(StaTrainRoute* pTrainRoute, const QJsonObject& subObj)
    {
        pTrainRoute->m_nRouteId = subObj.value("routeId").toInt();
        pTrainRoute->m_nTrainId = subObj.value("trainId").toInt();
        pTrainRoute->m_bArrivaRoute = subObj.value("arriveRoute").toInt();
        pTrainRoute->m_bAutoTouch = subObj.value("autoTouch").toInt();

        pTrainRoute->m_tPlanTime = QDateTime::fromString(subObj.value("planTime").toString(), Qt::ISODate);
        pTrainRoute->m_tTriggerTime = QDateTime::fromString(subObj.value("triggerTime").toString(), Qt::ISODate);
        pTrainRoute->m_tCleanTime = QDateTime::fromString(subObj.value("cleanTime").toString(), Qt::ISODate);

        pTrainRoute->m_nTrackCode = subObj.value("trackCode").toInt();
        pTrainRoute->m_strTrack = MainStation()->getDeviceByCode(pTrainRoute->m_nTrackCode, TRACK)->getName();
        pTrainRoute->m_nSignalCode = subObj.value("signalCode").toInt();
        pTrainRoute->m_strSignal = MainStation()->getDeviceByCode(pTrainRoute->m_nSignalCode, SIGNALLAMP)->getName();

        pTrainRoute->m_strCurRouteDescrip = subObj.value("routeDepict").toString();
        pTrainRoute->m_strDirection = subObj.value("direction").toString();
        pTrainRoute->m_nRouteState = subObj.value("routeState").toInt();

        QStringList strSubRoute = subObj.value("subRouteId").toString().split(',');
        for (QString strRouteId : strSubRoute) {
            if (strRouteId.toInt() != 0) {
                pTrainRoute->m_vecSubRouteId.append(strRouteId.toInt());
            }
        }
    }

    void StaTrainDispatch::Init(StaTrainDispatch* pTrainDispatch, const QJsonObject& subObj)
    {
        pTrainDispatch->m_nTrainId = subObj.value("trainId").toInt();
        pTrainDispatch->m_nOrderId = subObj.value("orderId").toInt();
        pTrainDispatch->m_strTrainNum = subObj.value("strTrainNum").toString();
        pTrainDispatch->m_strLocomotive = subObj.value("strLocomotiveNumber").toString();
        pTrainDispatch->m_tRecv = QDateTime::fromString(subObj.value("receiveTime").toString(), Qt::ISODate);
        pTrainDispatch->m_nState = subObj.value("state").toInt();
    }

    void StaDispatchOrder::Init(StaDispatchOrder* pDispatch, const QJsonObject& subObj)
    {
        pDispatch->m_nOrderId = subObj.value("orderId").toInt();
        pDispatch->m_nOrderType = subObj.value("orderType").toInt();
        pDispatch->m_nSendState = subObj.value("sendState").toInt();
        pDispatch->m_strOrderNum = subObj.value("strOrderNum").toString();

        pDispatch->m_strType = subObj.value("strType").toString();
        pDispatch->m_strOrderTip = subObj.value("strOrderTip").toString();
        pDispatch->m_strContent = subObj.value("strContent").toString();

        pDispatch->m_strSendAgency = subObj.value("strSendAgency").toString();
        pDispatch->m_strSendName = subObj.value("strSendStarter").toString();
        pDispatch->m_tSendTime = QDateTime::fromString(subObj.value("strSendTime").toString(), Qt::ISODate);

        pDispatch->m_strOrderedUnit = subObj.value("strOrderUnit").toString();
        pDispatch->m_strSignName = subObj.value("strSignName").toString();
        pDispatch->m_tSignTime = QDateTime::fromString(subObj.value("strSignTime").toString(), Qt::ISODate);
        pDispatch->m_bImportant = subObj.value("stress").toInt();
        pDispatch->m_nStateDisOrder = subObj.value("stateDisOrder").toInt();
    }

    StaDispatchOrder* StaTrainDispatch::getParentOrder()
    {
        for (StaDispatchOrder* pOrder : MainStation()->DispatchOrderList()) {
            if (m_nOrderId == pOrder->m_nOrderId) {
                return pOrder;
            }
        }
        return nullptr;
    }

    QString StaTrainDispatch::getStateStr()
    {
        switch (m_nState)
        {
        case 0: return "已发送";
        case 1: return "机车自动确认";
        case 2: return "发送成功";
        default: return "已发送";
        }
    }

    StaTrafficLog::StaTrafficLog(StaStagePlan* pStaStagePlan, int nTrainId)
    {
        m_nTrainId = nTrainId;
        m_nPlanType = pStaStagePlan->m_nPlanType;

        m_strArrivalTrainNum = pStaStagePlan->m_strArrivalTrainNum;
        m_nArrivalTrackCode = pStaStagePlan->m_nArrivalTrackCode;
        m_strArrivalTrack = pStaStagePlan->m_strArrivalTrack;
        m_nArrivalSignalCode = pStaStagePlan->m_nEntrySignalCode;
        m_strArrivaSignal = pStaStagePlan->m_strEntrySignal;
        m_tProvArrivalTime = pStaStagePlan->m_tArrivalTime;

        m_strDepartTrainNum = pStaStagePlan->m_strDepartTrainNum;
        m_nDepartTrackCode = pStaStagePlan->m_nDepartTrackCode;
        m_strDepartTrack = pStaStagePlan->m_strDepartTrack;
        m_nDepartSignalCode = pStaStagePlan->m_nExitSignalCode;
        m_strDepartSignal = pStaStagePlan->m_strExitSignal;
        m_tProvDepartTime = pStaStagePlan->m_tDepartTime;
        if (m_strArrivalTrack == "" && m_strDepartTrack == "") {
            m_nPlanType = 5;
        }
    }

    void StaTrafficLog::SetMap()
    {
        m_mapLogMember["trainId"] = QString::number(m_nTrainId);
        m_mapLogMember["planType"] = QString::number(m_nPlanType);
        m_mapLogMember["arrivalTrainNumber"] = m_strArrivalTrainNum;
        m_mapLogMember["arrivalTrack"] = QString::number(m_nArrivalTrackCode);
        m_mapLogMember["provArrivalTime"] = m_tProvArrivalTime.toString(Qt::ISODate);
        m_mapLogMember["realArrivalTime"] = m_tRealArrivalTime.toString(Qt::ISODate);
        m_mapLogMember["homeSignalCode"] = QString::number(m_nArrivalSignalCode);
        m_mapLogMember["agrAdjDepartTime"] = m_tAgrAdjDepartTime.toString(Qt::ISODate);
        m_mapLogMember["adjDepartTime"] = m_tAdjDepartTime.toString(Qt::ISODate);
        m_mapLogMember["arrivalLimit"] = QString::number(m_nArrivalLimit);
        m_mapLogMember["arrivalRouteId"] = QString::number(m_nArrivalRouteId);
        m_mapLogMember["arrivalLocomotive"] = m_strArrivalLocomotive;
        m_mapLogMember["arrivalDriver"] = m_strArrivalDriver;
        m_mapLogMember["arrivalLength"] = m_nArrivalTrainMaster;
        m_mapLogMember["arrivalTrainNum"] = QString::number(m_nArrivalTrainValue); 
        m_mapLogMember["arrivalChange"] = QString::number(m_nArrivalChange);
        m_mapLogMember["arrivalWeight"] = QString::number(m_nArrivalWeight);

        m_mapLogMember["departTrainNumber"] = m_strDepartTrainNum;
        m_mapLogMember["departTrack"] = QString::number(m_nDepartTrackCode);
        m_mapLogMember["provDepartTime"] = m_tProvDepartTime.toString(Qt::ISODate);
        m_mapLogMember["realDepartTime"] = m_tRealDepartTime.toString(Qt::ISODate);
        m_mapLogMember["startingSignalCode"] = QString::number(m_nDepartSignalCode);
        m_mapLogMember["adjAgrDepartTime"] = m_tAdjAgrDepartTime.toString(Qt::ISODate);
        m_mapLogMember["adjArrivalTime"] = m_tAdjArrivalTime.toString(Qt::ISODate);
        m_mapLogMember["delayReason"] = m_strDelayReason;
        m_mapLogMember["departLimit"] = QString::number(m_nDepartLimit);
        m_mapLogMember["departRouteId"] = QString::number(m_nDepartRouteId);
        m_mapLogMember["departLocomotive"] = m_strDepartLocomotive;
        m_mapLogMember["departDriver"] = m_strDepartDriver;
        m_mapLogMember["departLength"] = m_nDepartTrainMaster;
        m_mapLogMember["departTrainNum"] = QString::number(m_nDepartTrainValue);
        m_mapLogMember["departChange"] = QString::number(m_nDepartChange);
        m_mapLogMember["departWeight"] = QString::number(m_nDepartWeight);

        m_mapLogMember["upDown"] = QString::number(m_bUpDown);
        m_mapLogMember["deleteFlag"] = QString::number(m_bDeleteFlag);
        m_mapLogMember["notes"] = m_strNotes;
        m_mapLogMember["passenger"] = QString::number(m_bPassenger);

        m_mapLogMember["allowTrackNotMatch"] = QString::number(m_bAllowTrackNotMatch);
        m_mapLogMember["allowSignalNotMatch"] = QString::number(m_bAllowSignalNotMatch);
        m_mapLogMember["ljStatus"] = QString::number(m_bLJStatus);
        m_mapLogMember["jpStatus"] = QString::number(m_bJPStatus);
        m_mapLogMember["lwStatus"] = QString::number(m_bLWStatus);
        m_mapLogMember["jcStatus"] = QString::number(m_bJCStatus);
        m_mapLogMember["hjStatus"] = QString::number(m_bHJStatus);
        m_mapLogMember["cjStatus"] = QString::number(m_bCJStatus);
        m_mapLogMember["ssStatus"] = QString::number(m_bSSStatus);
        m_mapLogMember["zgStatus"] = QString::number(m_bZGStatus);
        m_mapLogMember["hcStatus"] = QString::number(m_bHCStatus);
        m_mapLogMember["zxStatus"] = QString::number(m_bZXStatus);
        m_mapLogMember["xwStatus"] = QString::number(m_bXWStatus);
        m_mapLogMember["dkStatus"] = QString::number(m_bDKStatus);
        m_mapLogMember["chStatus"] = QString::number(m_bCHStatus);
        m_mapLogMember["zwStatus"] = QString::number(m_bZWStatus);
        m_mapLogMember["zkStatus"] = QString::number(m_bZKStatus);
        m_mapLogMember["trainPosStatus"] = m_strTrainPosStatus;
        m_mapLogMember["proc"] = m_strProc;
        m_mapLogMember["strTrainArr"] = m_strTrainArr;
    }
    bool StaTrafficLog::IsReportedPoints()
    {
        return m_tRealArrivalTime.isNull() && m_tAgrAdjDepartTime.isNull()
            && m_tAdjDepartTime.isNull() && m_tRealDepartTime.isNull()
            && m_tAdjAgrDepartTime.isNull() && m_tAdjArrivalTime.isNull();
    }

    void StaTrafficLog::Init(StaTrafficLog* pTrafficLog, const QJsonObject& subObj)
    {
        pTrafficLog->m_nLogId = subObj.value("id").toInt();
        pTrafficLog->m_nTrainId = subObj.value("trainId").toInt();
        pTrafficLog->m_nPlanType = subObj.value("planType").toInt();

        //到达信息
        if (pTrafficLog->m_nPlanType != 0x02) {
            pTrafficLog->m_strArrivalTrainNum = subObj.value("arrivalTrainNumber").toString();
            pTrafficLog->m_nArrivalTrackCode = subObj.value("arrivalTrack").toInt();
            pTrafficLog->m_strArrivalTrack = MainStation()->getDeviceByCode(pTrafficLog->m_nArrivalTrackCode, TRACK)->getName();
            pTrafficLog->m_nArrivalSignalCode = subObj.value("homeSignalCode").toInt();
            pTrafficLog->m_strArrivaSignal = MainStation()->getDeviceByCode(pTrafficLog->m_nArrivalSignalCode, SIGNALLAMP)->getName();
            pTrafficLog->m_tProvArrivalTime = QDateTime::fromString(subObj.value("provArrivalTime").toString(), Qt::ISODate);
            pTrafficLog->m_tRealArrivalTime = QDateTime::fromString(subObj.value("realArrivalTime").toString(), Qt::ISODate);
            pTrafficLog->m_tAgrAdjDepartTime = QDateTime::fromString(subObj.value("agrAdjDepartTime").toString(), Qt::ISODate);
            pTrafficLog->m_tAdjDepartTime = QDateTime::fromString(subObj.value("adjDepartTime").toString(), Qt::ISODate);
            pTrafficLog->m_nArrivalLimit = subObj.value("arrivalLimit").toInt();
            pTrafficLog->m_nArrivalRouteId = subObj.value("arrivalRouteId").toInt();
            pTrafficLog->m_strArrivalLocomotive = subObj.value("arrivalLocomotive").toString();
            pTrafficLog->m_strArrivalDriver = subObj.value("arrivalDriver").toString();
            pTrafficLog->m_nArrivalTrainMaster = subObj.value("arrivalLength").toString();
            pTrafficLog->m_nArrivalTrainValue = subObj.value("arrivalTrainNum").toInt();
            pTrafficLog->m_nArrivalChange = subObj.value("arrivalChange").toInt();
            pTrafficLog->m_nArrivalWeight = subObj.value("arrivalWeight").toInt();
        }
        //出发信息
        if (pTrafficLog->m_nPlanType != 0x03) {
            pTrafficLog->m_strDepartTrainNum = subObj.value("departTrainNumber").toString();
            pTrafficLog->m_nDepartTrackCode = subObj.value("departTrack").toInt();
            pTrafficLog->m_strDepartTrack = MainStation()->getDeviceByCode(pTrafficLog->m_nDepartTrackCode, TRACK)->getName();
            pTrafficLog->m_nDepartSignalCode = subObj.value("startingSignalCode").toInt();
            pTrafficLog->m_strDepartSignal = MainStation()->getDeviceByCode(pTrafficLog->m_nDepartSignalCode, SIGNALLAMP)->getName();
            pTrafficLog->m_tProvDepartTime = QDateTime::fromString(subObj.value("provDepartTime").toString(), Qt::ISODate);
            pTrafficLog->m_tRealDepartTime = QDateTime::fromString(subObj.value("realDepartTime").toString(), Qt::ISODate);
            pTrafficLog->m_tAdjAgrDepartTime = QDateTime::fromString(subObj.value("adjAgrDepartTime").toString(), Qt::ISODate);
            pTrafficLog->m_tAdjArrivalTime = QDateTime::fromString(subObj.value("adjArrivalTime").toString(), Qt::ISODate);
            pTrafficLog->m_nDepartLimit = subObj.value("departLimit").toInt();
            pTrafficLog->m_nDepartRouteId = subObj.value("departRouteId").toInt();
            pTrafficLog->m_strDepartLocomotive = subObj.value("departLocomotive").toString();
            pTrafficLog->m_strDepartDriver = subObj.value("departDriver").toString();
            pTrafficLog->m_nDepartTrainMaster = subObj.value("departLength").toString();
            pTrafficLog->m_nDepartTrainValue = subObj.value("departTrainNum").toInt();
            pTrafficLog->m_nDepartChange = subObj.value("departChange").toInt();
            pTrafficLog->m_nDepartWeight = subObj.value("departWeight").toInt();
        }

        pTrafficLog->m_strDelayReason = subObj.value("delayReason").toString();
        pTrafficLog->m_bUpDown = subObj.value("upDown").toBool();
        pTrafficLog->m_bDeleteFlag = subObj.value("deleteFlag").toInt();
        pTrafficLog->m_strTrainArr = subObj.value("strTrainArr").toString();
        pTrafficLog->m_strNotes = subObj.value("notes").toString();
        pTrafficLog->m_bPassenger = subObj.value("passenger").toBool();
        pTrafficLog->m_bAllowTrackNotMatch = subObj.value("allowTrackNotMatch").toInt();
        pTrafficLog->m_bAllowSignalNotMatch = subObj.value("allowSignalNotMatch").toInt();

        pTrafficLog->m_bLJStatus = subObj.value("ljStatus").toBool();
        pTrafficLog->m_bJLStatus = subObj.value("jlStatus").toBool();
        pTrafficLog->m_bJPStatus = subObj.value("jpStatus").toBool();
        pTrafficLog->m_bLWStatus = subObj.value("lwStatus").toBool();
        pTrafficLog->m_bJCStatus = subObj.value("jcStatus").toBool();
        pTrafficLog->m_bHJStatus = subObj.value("hjStatus").toBool();
        pTrafficLog->m_bCJStatus = subObj.value("cjStatus").toBool();
        pTrafficLog->m_bSSStatus = subObj.value("ssStatus").toBool();
        pTrafficLog->m_bZGStatus = subObj.value("zgStatus").toBool();
        pTrafficLog->m_bHCStatus = subObj.value("hcStatus").toBool();
        pTrafficLog->m_bZXStatus = subObj.value("zxStatus").toBool();
        pTrafficLog->m_bXWStatus = subObj.value("xwStatus").toBool();
        pTrafficLog->m_bDKStatus = subObj.value("dkStatus").toBool();
        pTrafficLog->m_bCHStatus = subObj.value("chStatus").toBool();
        pTrafficLog->m_bZWStatus = subObj.value("zwStatus").toBool();
        pTrafficLog->m_bZKStatus = subObj.value("zkStatus").toBool();
        pTrafficLog->m_strTrainPosStatus = subObj.value("trainPosStatus").toString();
        pTrafficLog->m_strProc = subObj.value("proc").toString();
    }
}

namespace Station {

    void InsterTrainType(int nType, int nIndex, QString strtype)
    {
        if (nType == PASSENGE_TYPE) {
            MainStation()->AddPassengeTrain(nIndex, strtype);
        }
        if (nType == FREIGH_TYPE) {
            MainStation()->AddFreighTrain(nIndex, strtype);
        }
        if (nType == TRAIN_TYPE) {
            MainStation()->AddTrainType(nIndex, strtype);
        }
    }
}