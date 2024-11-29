#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //机务段
        class StaLocomotive : public StaDistant
        {
        public:
            explicit StaLocomotive(QObject* pParent = nullptr);
            ~StaLocomotive();

        private:
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
            QRectF m_rcLight;
            
        };
    }
}