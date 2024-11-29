#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device { //������ť
        //������ť
        class StaLamp : public DeviceBase
        {
        public:
            explicit StaLamp(QObject* pParent = nullptr);
            ~StaLamp();

        private:
            void InitDeviceAttribute() override;
            //վ������
            void Draw(bool isMulti = false) override;
            //վ���źŵ�
            void DrawLight();

        private:
            QRectF m_rcLamp;  //ָʾ�Ʒ�Χ
        };
    }
}