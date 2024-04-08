#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //场联
        class StaConnection : public StaDistant, public DeviceArrow, public DeviceBtn
        {
        public:
            explicit StaConnection(QObject* parent = nullptr);
            ~StaConnection();

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
            int m_nFlag = 0;
            
            QPoint m_ptSignal;      //信号
            QPoint m_ptNear;        //接近
            QPoint m_ptTakeLook;    //照查    
            QPoint m_ptAllow;       //允许
            QPoint m_ptFileld;      //本场
            QPoint m_ptAdjFileld;   //邻场

            QRect m_rcNearLight;
            QRect m_rcFileldLight;
            QRect m_rcAdjFileldLight;
            QRect m_rcAllowBtn;

            QString m_strCLType;
        };
    }
}