#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //�����
        class StaLocomotive : public StaDistant
        {
        public:
            explicit StaLocomotive(QObject* pParent = nullptr);
            ~StaLocomotive();

        private:
            //վ������
            void Draw(bool isMulti = false) override;
            //�����źŵ�
            void DrawLight() override;
            //��������
            void DrawText() override;

            //վ����ת
            void setVollover(const QPointF& ptBase) override;
            //״̬����
            void ResetDevState() override;

        private:
            QRectF m_rcLight;
            
        };
    }
}