#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //编发线
        class StaEndLine : public DeviceBase
        {
        public:
            explicit StaEndLine(QObject* pParent = nullptr);
            ~StaEndLine();

        private:
            //站场绘制
            void Draw(bool isMulti = false) override;

            //绘制信号灯
            void DrawEndLine();
            
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

        private:
            QPointF p1, p2, p3, p4, p5, p6, p7; //绘制坐标
        };
    }
}