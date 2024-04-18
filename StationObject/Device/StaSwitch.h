#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //道岔
        class StaSwitch : public StaSection
        {
        public:
            explicit StaSwitch(QObject* parent = nullptr);
            ~StaSwitch();

        private:
            virtual bool eventFilter(QObject* obj, QEvent* event);
        
        private:
            //初始化设备属性
            void InitDeviceAttribute();
            //站场绘制
            void Draw(const bool& isMulti = false);
            //绘制股道
            void DrawTrack(const QPen& pen, const uint nPosition = 7, const bool bOutSide = false, const int nOffset = 0);
            //绘制岔心
            void DrawSwitchCenterTrack();
            void DrawSwitchCenter(const QPen& pen, const uint nPosition, const bool bOutSide = false, const int nOffset = 0);
            //绘制外边缘
            void DrawDeviceOutSide();
            //绘制设备选中虚线框
            void DrawSelectRange();
            //绘制绝缘节
            void DrawInsulateNode();
            //绘制道岔状态
            void DrawSwitchState();
            //判断鼠标是否在高亮显示范围内
            bool Contains(const QPoint& ptPos);
            //初始化设备点击事件
            void InitClickEvent();
             //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();
            //获取岔心颜色
            QColor getSwitchCenterColor();
            //获取设备名称颜色
            QPen getDeviceNameColor();

        public:
            void setSwitchState(const uint& nSwitchState);
            uint getSwitchState() { return m_nSwitchState; }
            uint getCQSwitchCode() { return m_nQ; }
            uint getDWSwitchCode() { return m_nD; }
            uint getFWSwitchCode() { return m_nF; }

        private:
            bool isSwitchSK();

        private:
            uint m_nQDCode = 0xFFFF;
            uint m_nJyj = 0; //绝缘节
            QPoint p5, p6, p56, pz56; //绘制坐标
            QPoint p34C, p56C;   //岔心关键坐标
            QRect m_rcDevRect;

            uint m_nCxjy = 0;  //超限绝缘节
            int m_nDSCode = 0;
            int m_nQDMKInterlockBus = 0;
            int m_nOneToMore = 0;
            int m_nMainDCQD = 0;
            bool m_bSafetySwitch = false;
            int m_nQ = -1;
            int m_nD = -1;
            int m_nF = -1;

            uint m_nSwitchState = 0;  //道岔状态(1-定位,2-反位,0,3-岔前)
            uint m_nShuntFault = 0;  //分路不良(1-定位,2-反位,4-岔前)(可组合)
            bool m_bShuntFaultIdle = false;  //分路不良空闲
            uint m_nPowerCut = 0;  //是否停电(1-定位,2-反位,4-岔前)(可组合)
            uint m_nSpeedLimit = 0;  //是否限速（临时限速）(1-定位,2-反位,4-岔前)(可组合)
        };
    }
}

