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
        switch (pStagePlan->m_nPlanType)
        {
        case PLAN_TYPE_ARRIVED:
        case PLAN_TYPE_FINAL:
        case PLAN_TYPE_THROUGH: m_strTrainNum = pStagePlan->m_strArrivalTrainNum; break;
        case PLAN_TYPE_START: m_strTrainNum = pStagePlan->m_strDepartTrainNum; break;
        default:m_strTrainNum = pStagePlan->m_strArrivalTrainNum; break;
        }
        MainStation()->getDeviceByCode(pStagePlan->m_nEntrySignalCode)->getSXThroat();
        MainStation()->IsOverturn();
        switch (pStagePlan->m_nPlanType)
        {
        case PLAN_TYPE_ARRIVED:
        case PLAN_TYPE_FINAL:
        case PLAN_TYPE_THROUGH: {
            m_strTrainNum = pStagePlan->m_strArrivalTrainNum;
            QString strSignalName = MainStation()->getDeviceByCode(pStagePlan->m_nEntrySignalCode)->getName();
            m_bRight = (strSignalName.contains("X") && !MainStation()->IsOverturn());
        } break;
        case PLAN_TYPE_START: 
        {
            m_strTrainNum = pStagePlan->m_strDepartTrainNum;
            QString strSignalName = MainStation()->getDeviceByCode(pStagePlan->m_nExitSignalCode)->getName();
            m_bRight = (strSignalName.contains("S") && !MainStation()->IsOverturn());
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
        m_bRight = TrafficLog->m_bUpDown;
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
    }

    void StaStagePlan::Init(StaStagePlan* pStagePlan, const QJsonObject& subObj)
    {
        pStagePlan->m_nPlanId = subObj.value("planId").toInt();
        pStagePlan->m_strPlanNum = subObj.value("planNum").toString();
        pStagePlan->m_nPlanType = subObj.value("planType").toInt();

        pStagePlan->m_strArrivalTrainNum = subObj.value("arriveTrainNum").toString();
        pStagePlan->m_nArrivalTrackCode = subObj.value("arriveTrackCode").toInt();
        pStagePlan->m_strArrivalTrack = MainStation()->getDeviceByCode(pStagePlan->m_nArrivalTrackCode)->getName();
        pStagePlan->m_nEntrySignalCode = subObj.value("entrySignalCode").toInt();
        pStagePlan->m_strEntrySignal = MainStation()->getDeviceByCode(pStagePlan->m_nEntrySignalCode)->getName();
        pStagePlan->m_tArrivalTime = QDateTime::fromString(subObj.value("arriveTime").toString(), Qt::ISODate);

        pStagePlan->m_strDepartTrainNum = subObj.value("departTrainNum").toString();
        pStagePlan->m_nDepartTrackCode = subObj.value("arriveTrackCode").toInt();
        pStagePlan->m_strDepartTrack = MainStation()->getDeviceByCode(pStagePlan->m_nDepartTrackCode)->getName();
        pStagePlan->m_nExitSignalCode = subObj.value("entrySignalCode").toInt();
        pStagePlan->m_strExitSignal = MainStation()->getDeviceByCode(pStagePlan->m_nExitSignalCode)->getName();
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
        Device::StaSignal* pSignal = dynamic_cast<Device::StaSignal*>(MainStation()->getDeviceByCode(m_nSignalCode));
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
        

        //进路描述
        if (m_strTrack == "") {
            m_strRouteDescrip.append(pTrafficLog->m_strArrivaSignal);
            m_strRouteDescrip.append(",");
            m_strRouteDescrip.append(pTrafficLog->m_strDepartSignal);
        }
        else {
            QString strSignalName;
            Device::DeviceBase* pArrivaSignal = MainStation()->getDeviceByCode(m_nSignalCode);
            for (Device::DeviceBase* pSignal : MainStation()->getDeviceVectorByType(SIGNALLAMP)) {
                if (pSignal->getSXThroat() == pArrivaSignal->getSXThroat() && (pSignal->getAttr() & SIGNAL_FCXH) &&
                    pSignal->getName().mid(1) == m_strTrack.left(m_strTrack.indexOf("G"))) {
                    strSignalName = pSignal->getName();
                    break;
                }
            }
            if (m_bArrivaRoute) {   //接车
                m_strRouteDescrip.append(m_strSignal);
                m_strRouteDescrip.append(",");
                m_strRouteDescrip.append(strSignalName);
            }
            else {  //发车
                m_strRouteDescrip.append(strSignalName);
                m_strRouteDescrip.append(",");
                m_strRouteDescrip.append(m_strSignal);
            }
        }
    }

    void StaTrainRoute::ChangeTrack(int nCode, const QString& strName)
    {
        m_nTrackCode = nCode;
        m_strTrack = strName;

        QString strSignalName;
        Device::DeviceBase* pArrivaSignal = MainStation()->getDeviceByCode(m_nSignalCode);
        for (Device::DeviceBase* pSignal : MainStation()->getDeviceVectorByType(SIGNALLAMP)) {
            if (pSignal->getSXThroat() == pArrivaSignal->getSXThroat() && (pSignal->getAttr() & SIGNAL_FCXH) &&
                pSignal->getName().mid(1) == m_strTrack.left(m_strTrack.indexOf("G"))) {
                strSignalName = pSignal->getName();
                break;
            }
        }
        if (m_bArrivaRoute) {   //接车
            m_strRouteDescrip.append(m_strSignal);
            m_strRouteDescrip.append(",");
            m_strRouteDescrip.append(strSignalName);
        }
        else {  //发车
            m_strRouteDescrip.append(strSignalName);
            m_strRouteDescrip.append(",");
            m_strRouteDescrip.append(m_strSignal);
        }
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
        pTrainRoute->m_strTrack = MainStation()->getDeviceByCode(pTrainRoute->m_nTrackCode)->getName();
        pTrainRoute->m_nSignalCode = subObj.value("signalCode").toInt();
        pTrainRoute->m_strSignal = MainStation()->getDeviceByCode(pTrainRoute->m_nSignalCode)->getName();

        pTrainRoute->m_strRouteDescrip = subObj.value("routeDepict").toString();
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

    bool StaTrafficLog::IsReportedPoints()
    {
        return m_tRealArrivalTime.isNull() && m_tAgrAdjDepartTime.isNull()
            && m_tAdjDepartTime.isNull() && m_tRealDepartTime.isNull()
            && m_tAdjAgrDepartTime.isNull() && m_tAdjArrivalTime.isNull();
    }

    void StaTrafficLog::Init(StaTrafficLog* pTrain, const QJsonObject& subObj)
    {
    }
}

namespace Station {
    QMap<int, QString> m_mapPassengeTrain;
    QMap<int, QString> m_mapFreighTrain;
    QMap<int, QString> m_mapTrainType;

    void InsterTrainType(int nType, int nIndex, QString strtype)
    {
        if (nType == PASSENGE_TYPE) {
            m_mapPassengeTrain.insert(nIndex, strtype);
        }
        if (nType == FREIGH_TYPE) {
            m_mapFreighTrain.insert(nIndex, strtype);
        }
        if (nType == TRAIN_TYPE) {
            m_mapTrainType.insert(nIndex, strtype);
        }
    }
}