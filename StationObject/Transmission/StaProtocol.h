#pragma once
#include "../Device/DeviceBase.h"
#include "../../CultivateObject/Subject.h"

namespace Station {
    namespace Transmission {

        class StaProtocol {

        public:
            StaProtocol(QMap<QString, QVector<Device::DeviceBase*>>& mapDeviceVector);
            ~StaProtocol();
            void InitSubjectProtocol();

        public:
            QByteArray UnpackData(const QByteArray& dataAyyay);

        private:
            QByteArray UnpackLogin(const QByteArray& dataAyyay);   //������¼��Ϣ
            QByteArray UnpackStationReset(const QByteArray& dataAyyay);   //����վ������
            QByteArray UnpackStaViewState(const QByteArray& dataAyyay);   //����վ����Ϣ
            QByteArray UnpackStaStagePlan(const QByteArray& dataAyyay);   //�����׶μƻ�
            QByteArray UnpackStaDispatchOrder(const QByteArray& dataAyyay);   //������������
            QByteArray UnpackCultivate(const QByteArray& dataAyyay);      //������ѵ��Ϣ
            QByteArray UnpackPractice(const QByteArray& dataAyyay);      //����ʵѵ��Ϣ
            QByteArray UnpackTrain(const QByteArray& dataAyyay);      //����������Ϣ
            QByteArray UnpackTrainRoute(const QByteArray& dataAyyay);      //�����г���·��Ϣ
            QByteArray UnpackLimits(const QByteArray& dataAyyay);      //����Ȩ����Ϣ
            QByteArray UnpackAdjTimePoint(const QByteArray& dataAyyay);      //������վ����

        private:
            QMap<QString, QVector<Device::DeviceBase*>>& m_mapDeviceVector;
            QMap<int, std::function<QByteArray(const QByteArray&)>> m_mapUnPackOrder;
            QMap<QString, CultivateObject::SubjectType> m_mapSubjectOrderType;
        };
    }
}