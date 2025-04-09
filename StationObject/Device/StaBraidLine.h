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
            void InitAttributeMap() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            
            //绘制信号灯
            void DrawLight() override;
            //绘制文字
            void DrawText() override;

            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

        private:
            QRectF m_rcAllowText; //同意发车
            QRectF m_rcAllowLamp;
        };
    }
}