#pragma once
#include "../Device/DeviceBase.h"

namespace Station {
    namespace Transmission {

        class StaProtocol {

        public:
            StaProtocol(QMap<QString, QVector<Device::DeviceBase*>>& mapDeviceVector);
            ~StaProtocol();

        public:
            QByteArray UnpackData(const QByteArray& dataAyyay);

        private:
            QByteArray UnpackLogin(const QByteArray& dataAyyay);   //解析登录信息
            QByteArray UnpackStaViewState(const QByteArray& dataAyyay);   //解析站场信息
            QByteArray UnpackCultivate(const QByteArray& dataAyyay);      //解析培训信息
            QByteArray UnpackPractice(const QByteArray& dataAyyay);      //解析实训信息
            Device::DeviceBase* getDeviceByCode(uint nCode);
            Device::DeviceBase* getDeviceByName(QString strName);
        

        private:
            QMap<QString, QVector<Device::DeviceBase*>>& m_mapDeviceVector;
            QMap<int, std::function<QByteArray(const QByteArray&)>> m_mapUnPackOrder;
        };
    }
}