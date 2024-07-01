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
            void Draw(const bool& isMulti = false);

            //绘制信号灯
            void DrawEndLine();
            
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            QPoint p1, p2, p3, p4, p5, p6, p7; //绘制坐标
        };
    }
}