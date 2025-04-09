#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //脱轨器
        class StaDerailer : public DeviceBase
        {
        public:
            explicit StaDerailer(QObject* pParent = nullptr);
            ~StaDerailer();

        private:
            void InitAttributeMap() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制脱轨器
            void DrawDerailer();

            //获取设备名称颜色
            QPen getDeviceNameColor() override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

        private:
            QPointF p1, p2, p3;

            uint m_nRelayDCnode = 0;         
            bool m_bISDD = false;       
            bool m_bIsDCorTGQ = false;        
        };
    }
}