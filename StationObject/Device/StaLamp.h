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
            void InitDeviceAttribute();

        private:
            //站场绘制
            void Draw(const bool& isMulti = false);
            //站场信号灯
            void DrawLight();

        private:
            QRect n_rcLamp;  //指示灯范围
        };
    }
}
