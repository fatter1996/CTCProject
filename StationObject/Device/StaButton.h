#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device { //独立按钮
        //独立按钮
        class StaButton : public DeviceBase, public DeviceBtn
        {
        public:
            explicit StaButton(QObject* pParent = nullptr);
            ~StaButton();

        private:
            //初始化设备属性
            void InitDeviceAttribute();
            //站场绘制
            void Draw(const bool& isMulti = false);
            //绘制按钮
            void DrawStaButton();
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos);
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos);
            //初始化设备点击事件
            void InitClickEvent();
            //按钮点击事件
            void SetBtnState();
            //命令清除
            void OrderClear();
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        public:
            void setRelatedSignal(const DeviceBase* pDevice) { m_pRelatedSignal = const_cast<DeviceBase*>(pDevice); }
            QRect getButtonRect() { return n_rcButton; }

        private:
            QPoint p1, p2; //绘制坐标
            uint m_nTZB = 0;
            uint m_nButLong = 0;    //按钮大小
            QRect n_rcButton;  //按钮范围

            DeviceBase* m_pRelatedSignal = nullptr; //关联信号机
        };
    }
}
