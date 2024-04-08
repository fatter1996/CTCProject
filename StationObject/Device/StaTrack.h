#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //股道
        class StaTrack : public StaSection
        {
        public:
            explicit StaTrack(QObject* parent = nullptr);
            ~StaTrack();

        private:
            virtual bool eventFilter(QObject* obj, QEvent* event);

        private:
            //站场绘制
            void Draw(const bool& bElapsed, const bool& isMulti = false);
            //绘制股道背景
            void DrawTrackBack();
            //绘制股道
            void DrawTrack(const QPen& pen, const bool bOutSide = false, const int nOffset = 0);
            //绘制外边缘.
            void DrawDeviceOutSide(const bool& bElapsed);
            //绘制设备选中虚线框
            void DrawSelectRange();
            //绘制绝缘节
            void DrawInsulateNode();
            //获取设备名称颜色
            QPen getDeviceNameColor(const bool& bElapsed);
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