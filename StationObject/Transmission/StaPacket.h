#pragma once
#include "../Device/DeviceBase.h"

namespace Station {
    namespace Transmission {
        class StaPacket {

        public:
            StaPacket();
            ~StaPacket();

        public:
            QByteArray PackOrder(int nStationId, int nTargetCode, int nOrderType = 0);
            QByteArray PackUserLogin();
            QByteArray PackStaOperation();
            QByteArray PackCultivate();

        private:
            QMap<int, std::function<QByteArray()>> m_mapPackOrder;
        };
    }
}