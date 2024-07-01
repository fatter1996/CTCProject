#include "StaPacket.h"
#include "Global.h"
#include "../Device/StaSwitch.h"
namespace Station {
    namespace Transmission {

        StaPacket::StaPacket()
        {
            m_mapPackOrder = {
                {0x40, [&]() { return PackStaOperation(); }},
                {0x80, [&]() { return PackUserLogin(); }},
                {0x81, [&]() { return PackCultivate(); }},
                {0x82, [&]() { return PackSubject(); }}
            };
        }

        StaPacket::~StaPacket()
        {

        }

        QByteArray StaPacket::PackOrder(int nTargetCode, int nOrderType)
        {
            QByteArray byteOrder;
            byteOrder.append(QByteArray::fromHex("efefefef"));
            byteOrder.append(QByteArray::fromHex("0000"));
            byteOrder.append(MainStation()->getStationId() & 0xff);
            byteOrder.append(MainStation()->getStationId() >> 8);
            byteOrder.append(TARGET_CTC);
            byteOrder.append(nTargetCode);
            byteOrder.append(nOrderType);
            byteOrder.append(m_mapPackOrder[nOrderType]());
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

        QByteArray StaPacket::PackStaOperation()
        {
            QByteArray byteOperation;
            byteOperation.append(static_cast<int>(CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType()));
            byteOperation.append(static_cast<int>(CTCWindows::BaseWnd::StaFunBtnToolBar::getOperObjType()));
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

        QByteArray StaPacket::PackSubjectToInterLock(int nType, QByteArray btToInterLock)
        {
            CultivateObject::Subject* pSubject = MainStation()->getCurSubject();
            QByteArray byteOrder;
            byteOrder.append(QByteArray::fromHex("efefefef"));
            byteOrder.append(QByteArray::fromHex("0000"));
            byteOrder.append(MainStation()->getStationId() & 0xff);
            byteOrder.append(MainStation()->getStationId() >> 8);
            byteOrder.append(TARGET_CTC);
            byteOrder.append(TARGET_INTERLOCK);
            byteOrder.append(0x82);
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
            
            byteOrder.append(QByteArray::fromHex("fefefefe"));
            byteOrder[4] = byteOrder.length() & 0xff;
            byteOrder[5] = byteOrder.length() >> 8;
            return byteOrder;
        }
    }
}