#pragma once
#include "../Device/DeviceBase.h"
#include "../../CultivateObject/Subject.h"
#include <QDateTime>

namespace Station {
    namespace Transmission {

        class StaProtocol {

        public:
            StaProtocol(QMap<QString, QVector<Device::DeviceBase*>>& mapDeviceVector);
            ~StaProtocol();
            void InitSubjectProtocol();

        public:
            QByteArray UnpackData(const QByteArray& dataArray);

        private:
            QByteArray UnpackLogin(const QByteArray& dataArray);   //解析登录信息
            QByteArray UnpackStationReset(const QByteArray& dataArray);   //解析站场重置
            QByteArray UnpackStaViewState(const QByteArray& dataArray);   //解析站场信息
            QByteArray UnpackStaStagePlan(const QByteArray& dataArray);   //解析阶段计划
            QByteArray UnpackStaDispatchOrder(const QByteArray& dataArray);   //解析调度命令
            QByteArray UnpackCultivate(const QByteArray& dataArray);      //解析培训信息
            QByteArray UnpackPractice(const QByteArray& dataArray);      //解析实训信息
            QByteArray UnpackTrain(const QByteArray& dataArray);      //解析车次信息
            QByteArray UnpackTrainRoute(const QByteArray& dataArray);      //解析列车进路信息
            QByteArray UnpackLimits(const QByteArray& dataArray);      //解析权限信息
            QByteArray UnpackAdjTimePoint(const QByteArray& dataArray);      //解析邻站报点

        private:
            QDateTime ByteArrayToDateTime(const QByteArray& dataArray);

        private:
            QMap<QString, QVector<Device::DeviceBase*>>& m_mapDeviceVector;
            QMap<int, std::function<QByteArray(const QByteArray&)>> m_mapUnPackOrder;
            QMap<QString, CultivateObject::SubjectType> m_mapSubjectOrderType;
        };
    }
}