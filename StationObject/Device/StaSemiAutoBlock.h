#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //半自动闭塞
        class StaSemiAutoBlock : public StaDistant, public DeviceArrow, public DeviceBtn
        {
        public:
            explicit StaSemiAutoBlock(QObject* pParent = nullptr);
            ~StaSemiAutoBlock();

        private:
            //站场绘制
            void Draw(const bool& isMulti = false);
            //绘制信号灯
            void DrawLight();
            //绘制文字
            void DrawText();
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos);
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos);
            //初始化设备点击事件
            void InitClickEvent();
            //设置按钮属性
            void SetBtnState();
            //获取箭头颜色
            void getArrowColor();
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
            uint m_nSelectBtnType = 0x0;  //选中类型(0-未选中; 0x01-闭塞; 0x02-复原; 0x04-事故)
        };
    }
}