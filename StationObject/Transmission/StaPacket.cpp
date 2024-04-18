#include "StaPacket.h"
#include "Global.h"

namespace Station {
    namespace Transmission {

        StaPacket::StaPacket()
        {
            m_mapPackOrder.insert(0x40, [=]() { return PackStaOperation(); });
            m_mapPackOrder.insert(0x80, [=]() { return PackUserLogin(); });
        }

        StaPacket::~StaPacket()
        {

        }

        QByteArray StaPacket::PackOrder(int nStationId, int nTargetCode, int nOrderType)
        {
            QByteArray byteOrder;
            byteOrder.append(QByteArray::fromHex("efefefef"));
            byteOrder.append(QByteArray::fromHex("0000"));
            byteOrder.append(nStationId & 0xff);
            byteOrder.append(nStationId >> 8);
            byteOrder.append(TARGET_CTC);
            byteOrder.append(nTargetCode);
            byteOrder.append(nOrderType);
            byteOrder.append(m_mapPackOrder[nOrderType]());
            byteOrder.append(QByteArray::fromHex("fefefefe"));
            byteOrder[4] = byteOrder.length() & 0xff;
            byteOrder[6] = byteOrder.length() >> 8;
            qDebug() << byteOrder.toHex();
            return byteOrder;
        }

        QByteArray StaPacket::PackUserLogin()
        {
            UserInfo info = StationObject::getCurrUser();
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
            byteOperation.append(CTCWindows::getFunBtnOrderCode());
            byteOperation.append(StationObject::getSelectDevice().size());
            for (Device::DeviceBase* pDevice : StationObject::getSelectDevice()) {
                byteOperation.append(pDevice->getCode() & 0xff);
                byteOperation.append(pDevice->getCode() >> 8);
            }
            return byteOperation;
        }

        QByteArray StaPacket::PackCultivate()
        {
        
        }
    }
}