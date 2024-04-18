#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //脱轨器
        class StaDerailer : public DeviceBase
        {
        public:
            explicit StaDerailer(QObject* parent = nullptr);
            ~StaDerailer();

        private:
            virtual bool eventFilter(QObject* obj, QEvent* event);

        private:
            //站场绘制
            void Draw(const bool& isMulti = false);
            //绘制脱轨器
            void DrawDerailer();

            //获取设备名称颜色
            QPen getDeviceNameColor();
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            QPoint p1, p2, p3;

            uint m_nRelayDCnode = 0;         
            bool m_bISDD = false;       
            bool m_bIsDCorTGQ = false;        
        };
    }
}