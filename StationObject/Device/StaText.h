#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //�ѹ���
        class StaText : public DeviceBase
        {
        public:
            explicit StaText(QObject* pParent = nullptr);
            ~StaText();

        public:
            //վ������
            void Draw(bool isMulti = false) override;

        private:
            bool m_bIsTitle = false;
        };
    }
}