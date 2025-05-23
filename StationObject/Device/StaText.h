#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //����
        class StaText : public DeviceBase
        {
        public:
            explicit StaText(QObject* pParent = nullptr);
            ~StaText();

        public:
            void InitAttributeMap() override;
            //վ������
            void Draw(bool isMulti = false) override;

        private:
            bool m_bIsTitle = false;
        };
    }
}