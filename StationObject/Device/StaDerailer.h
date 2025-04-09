#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //�ѹ���
        class StaDerailer : public DeviceBase
        {
        public:
            explicit StaDerailer(QObject* pParent = nullptr);
            ~StaDerailer();

        private:
            void InitAttributeMap() override;
            //վ������
            void Draw(bool isMulti = false) override;
            //�����ѹ���
            void DrawDerailer();

            //��ȡ�豸������ɫ
            QPen getDeviceNameColor() override;
            //վ����ת
            void setVollover(const QPointF& ptBase) override;
            //״̬����
            void ResetDevState() override;

        private:
            QPointF p1, p2, p3;

            uint m_nRelayDCnode = 0;         
            bool m_bISDD = false;       
            bool m_bIsDCorTGQ = false;        
        };
    }
}