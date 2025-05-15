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
            QByteArray UnpackLogin(const QByteArray& dataArray);   //������¼��Ϣ
            QByteArray UnpackStationReset(const QByteArray& dataArray);   //����վ������
            QByteArray UnpackStaViewState(const QByteArray& dataArray);   //����վ����Ϣ
            QByteArray UnpackStaStagePlan(const QByteArray& dataArray);   //�����׶μƻ�
            QByteArray UnpackStaDispatchOrder(const QByteArray& dataArray);   //������������
            QByteArray UnpackCultivate(const QByteArray& dataArray);      //������ѵ��Ϣ
            QByteArray UnpackPractice(const QByteArray& dataArray);      //����ʵѵ��Ϣ
            QByteArray UnpackTrain(const QByteArray& dataArray);      //����������Ϣ
            QByteArray UnpackTrainRoute(const QByteArray& dataArray);      //�����г���·��Ϣ
            QByteArray UnpackLimits(const QByteArray& dataArray);      //����Ȩ����Ϣ
            QByteArray UnpackAdjTimePoint(const QByteArray& dataArray);      //������վ����

        private:
            QDateTime ByteArrayToDateTime(const QByteArray& dataArray);

        private:
            QMap<QString, QVector<Device::DeviceBase*>>& m_mapDeviceVector;
            QMap<int, std::function<QByteArray(const QByteArray&)>> m_mapUnPackOrder;
            QMap<QString, CultivateObject::SubjectType> m_mapSubjectOrderType;
        };
    }
}