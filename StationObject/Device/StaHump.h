#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //驼峰
        class StaHump : public StaDistant, public DeviceBtn
        {
        public:
            explicit StaHump(QObject* pParent = nullptr);
            ~StaHump();

        private:
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制驼峰
            void DrawHump();
            //绘制信号灯
            void DrawLight() override;
            //绘制文字
            void DrawText() override;
            //判断鼠标是否在事件范围内
            bool Contains(const QPoint& ptPos) override;
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos) override;
            //初始化设备点击事件
            void InitClickEvent() override;
            //设置按钮属性
            void SetBtnState() override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

        private:
            QPointF p1, p2, p3, p4, p5, p6, p7, p8, p9; //绘制坐标
            QPointF m_ptCutOff; //切断推送
            QPointF m_ptAllow; //允许推送
            QPointF m_ptTakeLook; //照查
            QRectF m_rcButton;
            QRectF m_rcCutOff; //切断推送
            QRectF m_rcAllow; //允许推送
            QRectF m_rcTakeLook; //照查
            QRectF m_rcSignal;
        };
    }
}