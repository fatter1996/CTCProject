#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //编发线
        class StaBraidLine : public StaDistant
        {
        public:
            explicit StaBraidLine(QObject* pParent = nullptr);
            ~StaBraidLine();

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
            QRect m_rcAllowText; //同意发车
            QRect m_rcAllowLamp;
        };
    }
}