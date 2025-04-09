#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //ÍÑ¹ìÆ÷
        class StaTextSign : public DeviceBase
        {
        public:
            explicit StaTextSign(QObject* pParent = nullptr);
            ~StaTextSign();

        public:
            void InitAttributeMap() override;
            //Õ¾³¡»æÖÆ
            void Draw(bool isMulti = false) override;

        private:
            bool m_bIsTitle = false;
        };
    }
}
