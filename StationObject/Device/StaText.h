#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //ÍÑ¹ìÆ÷
        class StaText : public DeviceBase
        {
        public:
            explicit StaText(QObject* pParent = nullptr);
            ~StaText();

        public:
            //Õ¾³¡»æÖÆ
            void Draw(const bool& isMulti = false);

        private:
            bool m_bIsTitle = false;
        };
    }
}