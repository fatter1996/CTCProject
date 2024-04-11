#pragma once
#include "../Device/DeviceBase.h"

namespace Station {
    namespace Protocol {
        class StaStateProtocol {

        public:
            StaStateProtocol(QMap<QString, QVector<Device::DeviceBase*>>& mapDeviceVector);
            ~StaStateProtocol();

        public:
            void UnpackStaViewState(QByteArray& dataAyyay);
            Device::DeviceBase* getDeviceByCode(uint nCode);

        private:
            QMap<QString, QVector<Device::DeviceBase*>>& m_mapDeviceVector;
        };
    }
}