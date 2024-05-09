#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //ÍÑ¹ìÆ÷
        class StaText : public DeviceBase
        {
        public:
            explicit StaText(QObject* parent = nullptr);
            ~StaText();

        private:
            bool m_bIsTitle = false;
        };
    }
}