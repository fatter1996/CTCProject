#pragma once
#include "../Device/DeviceBase.h"

namespace Station {
    namespace Transmission {
        class StaPacket {

        public:
            StaPacket();
            ~StaPacket();

        public:
            QByteArray PackOrder(int nTargetCode, int nOrderType = 0, int nAttr1 = -1, int nAttr2 = -1, QByteArray btAttr3 = QByteArray());

        private:
            QByteArray PackDeviceOnline(int nType);
            QByteArray PackUserLogin();
            QByteArray PackStaOperation(int nCurrFunType, int nOperObjType);
            QByteArray PackCultivate();
            QByteArray PackSubject();
            QByteArray PackSubject(int nType, QByteArray btToInterLock);
            QByteArray PackTrain(int nTrainId, int nType);
            QByteArray PackTrainRoute(int nRouteId, int nType);
            QByteArray PackLimits(int nType, int nValue);

        private:
            QMap<int, std::function<QByteArray(int, int, QByteArray)>> m_mapPackOrder;
        };
    }
}