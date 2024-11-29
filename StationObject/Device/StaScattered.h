#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //��ɢ
        class StaScattered : public StaDistant
        {
        public:
            explicit StaScattered(QObject* pParent = nullptr);
            ~StaScattered();

        private:
            //�����źŵ�
            void DrawLight() override;
            //��������
            void DrawText() override;

            //վ����ת
            void setVollover(const QPointF& ptBase) override;
            //״̬����
            void ResetDevState() override;

        private:
            QRectF getLampRectByTextRect(const QRectF& rcText);

        private:
            QRectF m_rcFuseAlarmText;    //��˿����
            QRectF m_rcFuseAlarmLamp;
            QRectF m_rcMainAuxPowerText; //������Դ
            QRectF m_rcMainAuxPowerLamp;

            QRectF m_rcTrackOutageSText; //���й��ͣ��
            QRectF m_rcTrackOutageSLamp;
            QRectF m_rcTrackOutageXText; //���й��ͣ��
            QRectF m_rcTrackOutageXLamp;

            QRectF m_rcTotalLockSText; //����������
            QRectF m_rcTotalLockSLamp;
            QRectF m_rcTotalLockXText; //����������
            QRectF m_rcTotalLockXLamp;

            QRectF m_rcFilamentBreakSText; //���е�˿��˿
            QRectF m_rcFilamentBreakSLamp;
            QRectF m_rcFilamentBreakXText; //���е�˿��˿
            QRectF m_rcFilamentBreakXLamp;

            QRectF m_rcCrowdAlarmText; //������
            QRectF m_rcCrowdAlarmLamp;
            QRectF m_rcCodeAlarmText; //���뻯����
            QRectF m_rcCodeAlarmLamp;

            QString m_strSGDTDJ;
            int m_nSGDTDJ = 0;
            QString m_strXGDTDJ;
            int m_nXGDTDJ = 0;

            QString m_strTrackType;
        };
    }
}