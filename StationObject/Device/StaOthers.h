#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //区段
        class StaSwitchSection : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaSwitchSection(QObject* pParent = nullptr);
            ~StaSwitchSection();

        private:
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            uint m_nChildNum = 0;

        public:
            QVector<int> m_vecSectionsCode;
        };


        //千分之六标识
        class StaPermillSix : public DeviceBase {
            Q_OBJECT
        public:
            explicit StaPermillSix(QObject* pParent = nullptr);
            ~StaPermillSix();

        private:
            //站场绘制
            void Draw(const bool& isMulti = false);
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