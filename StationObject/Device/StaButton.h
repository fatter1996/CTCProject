#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device { //独立按钮
        //独立按钮
        class StaButton : public DeviceBase, public DeviceBtn
        {
        public:
            explicit StaButton(QObject* parent = nullptr);
            ~StaButton();

        private:
            virtual bool eventFilter(QObject* obj, QEvent* event);

        private:
            //初始化设备属性
            void InitDeviceAttribute();
            //站场绘制
            void Draw(const bool& bElapsed, const bool& isMulti = false);
            //绘制按钮
            void DrawStaButton(const bool& bElapsed);
            //获取设备名称颜色
            QPen getDeviceNameColor(const bool& bElapsed);
            //按钮点击事件
            void OnButtonClick();
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        public:
            void setRelatedSignal(const DeviceBase* pDevice) { m_pRelatedSignal = const_cast<DeviceBase*>(pDevice); }

        private:
            QPoint p1, p2; //绘制坐标
            uint m_nTZB = 0;
            uint m_nButLong = 0;    //按钮大小
            QRect n_rcButton;  //按钮范围

            DeviceBase* m_pRelatedSignal = nullptr; //关联信号机
        };
    }
}
