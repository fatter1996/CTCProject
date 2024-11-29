#include "StaPacket.h"
#include "Global.h"
#include "../Device/StaSwitch.h"
namespace Station {
    namespace Transmission {

        StaPacket::StaPacket()
        {
            m_mapPackOrder = {
                { 0x40, [&](int nAttr1, int nAttr2, QByteArray) { return PackStaOperation(nAttr1, nAttr2); }},
                { 0x80, [&](int, int, QByteArray) { return PackUserLogin(); }},
                { 0x81, [&](int, int, QByteArray) { return PackCultivate(); }},
                { 0x82, [&](int nAttr1, int, QByteArray btAttr3) {
                    if (nAttr1 == -1) {
                        return PackSubject();
                    }
                    else {
                        return PackSubject(nAttr1, btAttr3);
                    }
                }},
                { 0x60, [&](int nTrainId, int nType, QByteArray btTrainNum) { return PackTrain(nTrainId, nType, btTrainNum); }},
                { 0x61, [&](int nRouteId, int nType, QByteArray) { return PackTrainRoute(nRouteId, nType); }},
                { 0x70, [&](int nType, int nValue, QByteArray) { return PackLimits(nType, nValue); }},
            };
        }

        StaPacket::~StaPacket()
        {

        }

        QByteArray StaPacket::PackOrder(int nTargetCode, int nOrderType, int nAttr1, int nAttr2, QByteArray btAttr3)
        {
            QByteArray byteOrder;
            byteOrder.append(QByteArray::fromHex("efefefef"));
            byteOrder.append(QByteArray::fromHex("0000"));
            byteOrder.append(MainStation()->getStationId() & 0xff);
            byteOrder.append(MainStation()->getStationId() >> 8);
            byteOrder.append(TARGET_CTC);
            byteOrder.append(nTargetCode);
            byteOrder.append(nOrderType);
            byteOrder.append(m_mapPackOrder[nOrderType](nAttr1, nAttr2, btAttr3));
            byteOrder.append(QByteArray::fromHex("fefefefe"));
            byteOrder[4] = byteOrder.length() & 0xff;
            byteOrder[5] = byteOrder.length() >> 8;
            return byteOrder;
        }

        QByteArray StaPacket::PackUserLogin()
        {
            UserInfo info = MainStation()->getCurrUser();
            QByteArray byteOperation;
            byteOperation.append(0x01);
            byteOperation.append(info.m_strName.length());
            byteOperation.append(info.m_strName);
            byteOperation.append(info.m_strPassWord.length());
            byteOperation.append(info.m_strPassWord);
            return byteOperation;
        }

        QByteArray StaPacket::PackStaOperation(int nCurrFunType, int nOperObjType)
        { 
            QByteArray byteOperation;
            byteOperation.append((nCurrFunType == -1) ? static_cast<int>(CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType()) : nCurrFunType);
            byteOperation.append((nOperObjType == -1) ? static_cast<int>(CTCWindows::BaseWnd::StaFunBtnToolBar::getOperObjType()) : nOperObjType);
            byteOperation.append(MainStation()->getSelectDevice().size());
            for (Device::DeviceBase* pDevice : MainStation()->getSelectDevice()) {
                if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RegionRelieve && pDevice->getStrType() == SWITCH) {
                    Device::StaSwitch* pSwitch = dynamic_cast<Device::StaSwitch*>(pDevice);
                    byteOperation.append(pSwitch->getQDCode() & 0xff);
                    byteOperation.append(pSwitch->getQDCode() >> 8);
                }
                else {
                    byteOperation.append(pDevice->getCode() & 0xff);
                    byteOperation.append(pDevice->getCode() >> 8);
                }
            }
            return byteOperation;
        }

        QByteArray StaPacket::PackCultivate()
        {
            QByteArray byteOperation;
            byteOperation.append(MainStation()->getUserId());
            byteOperation.append(MainStation()->getCultivateOrder().length());
            byteOperation.append(MainStation()->getCultivateOrder());
            return byteOperation;
        }

        QByteArray StaPacket::PackSubject()
        {
            QByteArray byteOperation;
            CultivateObject::Subject* pSubject = MainStation()->getCurSubject();
            if (pSubject) {
                byteOperation.append(MainStation()->getUserId());
                byteOperation.append(pSubject->getCultivateType());
                byteOperation.append(0x03);
                if (pSubject->getCultivateType() == 0x01) { //基础实训
                    byteOperation.append(pSubject->getSubjectId());
                    byteOperation.append(pSubject->getTestPaperNum());
                    byteOperation.append(pSubject->getResult());
                }
                else if (pSubject->getCultivateType() == 0x02) { //流程实训
                    byteOperation.append(pSubject->getSubjectId());
                    byteOperation.append(pSubject->getTestPaperNum());
                    byteOperation.append(pSubject->getFlowId().length());
                    byteOperation.append(pSubject->getFlowId());
                    byteOperation.append(pSubject->getResult());
                }
            }
            return byteOperation;
        }

        QByteArray StaPacket::PackSubject(int nType, QByteArray btToInterLock)
        {
            CultivateObject::Subject* pSubject = MainStation()->getCurSubject();
            QByteArray byteOrder;
            byteOrder.append(MainStation()->getUserId());
            byteOrder.append(pSubject->getCultivateType());
            byteOrder.append(nType);
            if (pSubject->getCultivateType() == 0x01) {
                byteOrder.append(pSubject->getSubjectId());
                byteOrder.append(pSubject->getTestPaperNum());
                byteOrder.append(btToInterLock.length());
                byteOrder.append(btToInterLock);
            }
            else if (pSubject->getCultivateType() == 0x02) {
                byteOrder.append(pSubject->getSubjectId());
                byteOrder.append(pSubject->getTestPaperNum());
                byteOrder.append(pSubject->getFlowId().length());
                byteOrder.append(pSubject->getFlowId());
                byteOrder.append(btToInterLock.length());
                byteOrder.append(btToInterLock);
            }
            return byteOrder;
        }

        QByteArray StaPacket::PackTrain(int nTrainId, int nType, QByteArray btTrainNum)
        {
            QByteArray byteOperation;
            StaTrain* pTrain = MainStation()->getStaTrainById(nTrainId);
            if (!pTrain) {
                return QByteArray();
            }
            byteOperation.append(nType);
            byteOperation.append(pTrain->m_nTrainId);
            byteOperation.append(btTrainNum.length());
            byteOperation.append(btTrainNum);
            if (nType == 0x01) {    //添加车次
                byteOperation.append(pTrain->m_nPosCode & 0xff);
                byteOperation.append(pTrain->m_nPosCode >> 8);
                byteOperation.append(pTrain->m_bRight);
                byteOperation.append(pTrain->m_bElectric);
                byteOperation.append(pTrain->m_bFreightTrain);
                byteOperation.append(pTrain->m_nOverLimitLevel);
                byteOperation.append(pTrain->m_nSpeed);
                byteOperation.append(pTrain->m_strTrainType.length());
                byteOperation.append(pTrain->m_strTrainType);
            }
            else if (nType == 0x03) {    //变更车次
                byteOperation.append(pTrain->m_strTrainNum.length());
                byteOperation.append(pTrain->m_strTrainNum);
            }
            else if (nType == 0x06) {    //添加车次
                byteOperation.append(pTrain->m_nPosCode & 0xff);
                byteOperation.append(pTrain->m_nPosCode >> 8);
            }
            else if (nType == 0x08) {    //修改车次信息
                byteOperation.append(pTrain->m_nSpeed);
                byteOperation.append(pTrain->m_bElectric);
            }
            return byteOperation;
        }

        QByteArray StaPacket::PackTrainRoute(int nRouteId, int nType)
        {
            QByteArray byteOperation;
            StaTrainRoute* pTrainRoute = MainStation()->getStaTrainRouteById(nRouteId);
            if (!pTrainRoute) {
                return QByteArray();
            }
            byteOperation.append(nType);
            byteOperation.append(nRouteId);
            if (nType == 0x01) {    //添加进路
                StaTrain* pTrain = MainStation()->getStaTrainById(pTrainRoute->m_nTrainId);
                if (!pTrain) {
                    return QByteArray();
                }
                byteOperation.append(pTrain->m_strTrainNum.length());
                byteOperation.append(pTrain->m_strTrainNum);
                byteOperation.append(pTrainRoute->m_strRouteDescrip.length());
                byteOperation.append(pTrainRoute->m_strRouteDescrip);
            }
            if (nType == 0x04) {    //修改进路
                byteOperation.append(pTrainRoute->m_strRouteDescrip.length());
                byteOperation.append(pTrainRoute->m_strRouteDescrip);
            }
            return byteOperation;
        }

        QByteArray StaPacket::PackLimits(int nType, int nValue)
        {
            QByteArray byteOperation;
            byteOperation.append(nType);
            byteOperation.append(nValue);
            return byteOperation;
        }
    }
}