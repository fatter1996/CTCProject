#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //股道
        class StaTrack : public StaSection
        {
        public:
            explicit StaTrack(QObject* pParent = nullptr);
            ~StaTrack();

        private:
            //初始化设备属性
            void InitDeviceAttribute();
            //站场绘制
            void Draw(const bool& isMulti = false);
            //绘制股道背景
            void DrawTrackBack();
            //绘制股道
            void DrawTrack(const QPen& pen, const bool bOutSide = false, const int nOffset = 0);
            //绘制外边缘.
            void DrawDeviceOutSide();
            //绘制设备选中虚线框
            void DrawSelectRange();
            //绘制绝缘节
            void DrawInsulateNode();
            //判断鼠标是否在高亮显示范围内
            bool Contains(const QPoint& ptPos);
            //鼠标是否在设备上
            bool IsMouseWheel(const QPoint& ptPos);
            //初始化设备点击事件
            void InitClickEvent();
            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            QString m_strTrackType; //股道类型

            bool m_bPowerCut = false;         //是否停电
            bool m_bSpeedLimit = false;       //是否限速（临时限速）
            bool m_bShuntFault = false;        //是否分路不良
            bool m_bShuntFaultIdle = false;   //是否分路不良空闲
        };
    }
}