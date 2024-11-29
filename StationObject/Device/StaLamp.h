#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device { //独立按钮
        //独立按钮
        class StaLamp : public DeviceBase
        {
        public:
            explicit StaLamp(QObject* pParent = nullptr);
            ~StaLamp();

        private:
            void InitDeviceAttribute() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //站场信号灯
            void DrawLight();

        private:
            QRectF m_rcLamp;  //指示灯范围
        };
    }
}
