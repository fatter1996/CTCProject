#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //道岔
        class StaSwitch : public StaSection, public DeviceTrain
        {
        public:
            explicit StaSwitch(QObject* pParent = nullptr);
            ~StaSwitch();

        private:
            void InitAttributeMap() override;
            //初始化设备属性
            void InitDeviceAttribute() override;
            //站场绘制
            void Draw(bool isMulti = false) override;
            //绘制股道
            void DrawTrack(const QPen& pen, const uint nPosition = 7, const bool bOutSide = false, const int nOffset = 0);
            //绘制岔心
            void DrawSwitchCenterTrack();
            void DrawSwitchCenter(const QPen& pen, const uint nPosition, const bool bOutSide = false, const int nOffset = 0);
            //绘制外边缘
            void DrawDeviceOutSide() override;
            //绘制设备选中虚线框
            void DrawSelectRange() override;
            //绘制绝缘节
            void DrawInsulateNode() override;
            //绘制道岔状态
            void DrawSwitchState();
            //绘制培训提示信息
            void DrawCultivateTips() override;
            //判断鼠标是否在高亮显示范围内
            bool Contains(const QPoint& ptPos) override;
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos) override;
            //初始化设备点击事件
            void InitClickEvent() override;
            //右键菜单
            void ShowDeviceMenu(const QPoint& ptPos) override;
            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;
            //获取岔心颜色
            QColor getSwitchCenterColor();
            //获取设备名称颜色
            QPen getDeviceNameColor() override;

        public:
            void setSwitchState(const uint& nSwitchState) { m_nSwitchState = nSwitchState; }
            uint getSwitchState() const { return m_nSwitchState; }
            uint getCQSwitchCode() const { return m_nQ; }
            uint getDWSwitchCode() const { return m_nD; }
            uint getFWSwitchCode() const { return m_nF; }
            uint getQDCode() const { return m_nQDCode; }

        private:
            bool isSwitchSK();

        private:
            uint m_nQDCode = 0xFFFF;
            uint m_nJyj = 0; //绝缘节
            QPointF p5, p6, p56, pz56; //绘制坐标
            QPointF p34C, p56C;   //岔心关键坐标
            QRectF m_rcDevRect;

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
        };
    }
}

