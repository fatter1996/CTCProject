#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //区段
        class StaSwitchSection : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaSwitchSection(QObject* parent = nullptr);
            ~StaSwitchSection();

        private:
            //清除某一道岔区段内不连续道岔状态
            void ClearStateInSection(const DeviceBase* pMainDevice, DeviceBase* pDevice);
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            uint m_nChildNum = 0;

        public:
            QVector<DeviceBase*> m_vecSections;
        };


        //千分之六标识
        class StaPermillSix : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaPermillSix(QObject* parent = nullptr);
            ~StaPermillSix();

        private:
            //站场绘制
            void Draw(const bool& bElapsed, const bool& isMulti = false);
            //绘制千分之六标识
            void DrawPermillSix();

            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            QPoint p1, p2;
        };
    }
}