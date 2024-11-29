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
            void InitDeviceAttribute() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制按钮
            void DrawStaButton();
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos) override;
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos) override;
            //初始化设备点击事件
            void InitClickEvent() override;
            //按钮点击事件
            void SetBtnState() override;
            //命令清除
            void OrderClear() override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

        public:
            void setRelatedSignal(const DeviceBase* pDevice) { m_pRelatedSignal = const_cast<DeviceBase*>(pDevice); }
            QRectF getButtonRect() const { return n_rcButton; }

        private:
            QPointF p1, p2; //绘制坐标
            uint m_nTZB = 0;
            uint m_nButLong = 0;    //按钮大小
            QRectF n_rcButton;  //按钮范围

            DeviceBase* m_pRelatedSignal = nullptr; //关联信号机
        };
    }
}
