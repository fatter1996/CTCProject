#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //零散
        class StaScattered : public StaDistant
        {
        public:
            explicit StaScattered(QObject* pParent = nullptr);
            ~StaScattered();

        private:
            //绘制信号灯
            void DrawLight();
            //绘制文字
            void DrawText();

            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            QRect getLampRectByTextRect(const QRect& rcText);

        private:
            QRect m_rcFuseAlarmText;    //熔丝报警
            QRect m_rcFuseAlarmLamp;
            QRect m_rcMainAuxPowerText; //主副电源
            QRect m_rcMainAuxPowerLamp;

            QRect m_rcTrackOutageSText; //上行轨道停电
            QRect m_rcTrackOutageSLamp;
            QRect m_rcTrackOutageXText; //下行轨道停电
            QRect m_rcTrackOutageXLamp;

            QRect m_rcTotalLockSText; //上行总锁闭
            QRect m_rcTotalLockSLamp;
            QRect m_rcTotalLockXText; //下行总锁闭
            QRect m_rcTotalLockXLamp;

            QRect m_rcFilamentBreakSText; //上行灯丝断丝
            QRect m_rcFilamentBreakSLamp;
            QRect m_rcFilamentBreakXText; //下行灯丝断丝
            QRect m_rcFilamentBreakXLamp;

            QRect m_rcCrowdAlarmText; //挤岔报警
            QRect m_rcCrowdAlarmLamp;
            QRect m_rcCodeAlarmText; //电码化报警
            QRect m_rcCodeAlarmLamp;

            QString m_strSGDTDJ;
            int m_nSGDTDJ = 0;
            QString m_strXGDTDJ;
            int m_nXGDTDJ = 0;

            QString m_strTrackType;
        };
    }
}