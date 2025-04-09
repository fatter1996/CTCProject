#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device { //�����ƹ�
        //�����ƹ�
        class StaLamp : public DeviceBase
        {
        public:
            explicit StaLamp(QObject* pParent = nullptr);
            ~StaLamp();

        private:
            void InitAttributeMap() override;
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
