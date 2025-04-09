#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //�ѹ���
        class StaTextSign : public DeviceBase
        {
        public:
            explicit StaTextSign(QObject* pParent = nullptr);
            ~StaTextSign();

        public:
            void InitAttributeMap() override;
            //վ������
            void Draw(bool isMulti = false) override;

        private:
            bool m_bIsTitle = false;
        };
    }
}
