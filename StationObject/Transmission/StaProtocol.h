#pragma once
#include "../Device/DeviceBase.h"

namespace Station {
    namespace Transmission {
        class StaProtocol {

        public:
            StaProtocol(QMap<QString, QVector<Device::DeviceBase*>>& mapDeviceVector);
            ~StaProtocol();

        public:
            void UnpackData(const QByteArray& dataAyyay);

        private:
            void UnpackLogin(const QByteArray& dataAyyay);   //解析登录信息
            void UnpackStaViewState(const QByteArray& dataAyyay);   //解析站场信息
            void UnpackCultivate(const QByteArray& dataAyyay);      //解析培训信息
            Device::DeviceBase* getDeviceByCode(uint nCode);

        private:
            QMap<QString, QVector<Device::DeviceBase*>>& m_mapDeviceVector;
            QMap<int, std::function<void(const QByteArray&)>> m_mapUnPackOrder;
        };
    }
}