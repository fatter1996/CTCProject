#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //半自动闭塞
        class StaSemiAutoBlock : public StaDistant, public DeviceArrow, public DeviceBtn
        {
        public:
            explicit StaSemiAutoBlock(QObject* parent = nullptr);
            ~StaSemiAutoBlock();

        private:
            virtual bool eventFilter(QObject* obj, QEvent* event);

        private:
            //站场绘制
            void Draw(const bool& bElapsed, const bool& isMulti = false);
            //绘制信号灯
            void DrawLight();
            //绘制文字
            void DrawText();

            //获取箭头颜色
            void getArrowColor();
            //按钮点击事件
            void OnButtonClick();
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            QPoint m_ptBSText;
            QPoint m_ptFYText;
            QPoint m_ptSGText;

            QRect m_rcBSBtn;
            QRect m_rcFYBtn;
            QRect m_rcSGBtn;

            uint m_nBlockType = 0;
        };
    }
}