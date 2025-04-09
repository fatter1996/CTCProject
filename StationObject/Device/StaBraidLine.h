#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //�෢��
        class StaBraidLine : public StaDistant
        {
        public:
            explicit StaBraidLine(QObject* pParent = nullptr);
            ~StaBraidLine();

        private:
            void InitAttributeMap() override;
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
            QRectF m_rcAllowText; //ͬ�ⷢ��
            QRectF m_rcAllowLamp;
        };
    }
}