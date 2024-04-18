#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //机务段
        class StaLocomotive : public StaDistant
        {
        public:
            explicit StaLocomotive(QObject* parent = nullptr);
            ~StaLocomotive();

        private:
            virtual bool eventFilter(QObject* obj, QEvent* event);

        private:
            //站场绘制
            void Draw(const bool& isMulti = false);
            //绘制信号灯
            void DrawLight();
            //绘制文字
            void DrawText();

            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            QRect m_rcLight;
            
        };
    }
}