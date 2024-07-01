#include "Global.h"
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

    void StaTrain::Init(StaTrain* pTrain, const QJsonObject& subObj)
    {
        pTrain->m_nTrainId = subObj.value("trainId").toInt();
        pTrain->m_strArrivalTrainNum = subObj.value("arriveTrainNum").toString();
        pTrain->m_strDepartTrainNum = subObj.value("departTrainNum").toString();
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

    StaTrainRoute::StaTrainRoute(StaStagePlan* pStaStagePlan, bool bArrivaRoute, int nTrainId)
    {
        m_bArrivaRoute = bArrivaRoute;
        m_nTrainId = nTrainId;
        if (m_bArrivaRoute) {   //接车
            m_tPlanTime = pStaStagePlan->m_tArrivalTime;
            m_tTriggerTime = pStaStagePlan->m_tArrivalTime;
            m_nTrackCode = pStaStagePlan->m_nArrivalTrackCode;
            m_strTrack = pStaStagePlan->m_strArrivalTrack;
            m_nSignalCode = pStaStagePlan->m_nEntrySignalCode;
            m_strSignal = pStaStagePlan->m_strEntrySignal;
        }
        else {  //发车
            m_tPlanTime = pStaStagePlan->m_tDepartTime;
            m_tTriggerTime = pStaStagePlan->m_tDepartTime;
            m_nTrackCode = pStaStagePlan->m_nDepartTrackCode;
            m_strTrack = pStaStagePlan->m_strDepartTrack;
            m_nSignalCode = pStaStagePlan->m_nExitSignalCode;
            m_strSignal = pStaStagePlan->m_strExitSignal;
        }

        //进路描述
        QString strSignalName;
        Device::DeviceBase* pArrivaSignal = MainStation()->getDeviceByCode(m_nSignalCode);
        for (Device::DeviceBase* pSignal : MainStation()->getDeviceVectorByType(SIGNALLAMP)) {
            QString str1 = pSignal->getName().mid(1);
            QString str2 = m_strTrack.left(m_strTrack.indexOf("G"));
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
        pTrainRoute->m_nAutoTouch = subObj.value("autoTouch").toInt();

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
            pTrainRoute->m_vecSubRouteId.append(strRouteId.toInt());
        }
    }

    void StaTrainDispatch::Init(StaTrainDispatch* pTrainDispatch, const QJsonObject& subObj)
    {
        pTrainDispatch->m_nTrainId = subObj.value("trainId").toInt();
        pTrainDispatch->m_nOrderId = subObj.value("orderId").toInt();
        pTrainDispatch->m_strTrainNum = subObj.value("strTrainNum").toString();
        pTrainDispatch->m_strLocomotive = subObj.value("strLocomotiveNumber").toString();
        pTrainDispatch->m_timRecv = QDateTime::fromString(subObj.value("receiveTime").toString(), Qt::ISODate);
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