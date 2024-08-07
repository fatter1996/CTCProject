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
            void Draw(const bool& isMulti = false);
            //绘制驼峰
            void DrawHump();
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
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            QPoint p1, p2, p3, p4, p5, p6, p7, p8, p9; //绘制坐标
            QPoint m_ptCutOff; //切断推送
            QPoint m_ptAllow; //允许推送
            QPoint m_ptTakeLook; //照查
            QRect m_rcButton;
            QRect m_rcCutOff; //切断推送
            QRect m_rcAllow; //允许推送
            QRect m_rcTakeLook; //照查
            QRect m_rcSignal;
        };
    }
}