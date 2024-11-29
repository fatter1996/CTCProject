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
            void DrawLight() override;
            //绘制文字
            void DrawText() override;

            //站场翻转
            void setVollover(const QPointF& ptBase) override;
            //状态重置
            void ResetDevState() override;

        private:
            QRectF getLampRectByTextRect(const QRectF& rcText);

        private:
            QRectF m_rcFuseAlarmText;    //熔丝报警
            QRectF m_rcFuseAlarmLamp;
            QRectF m_rcMainAuxPowerText; //主副电源
            QRectF m_rcMainAuxPowerLamp;

            QRectF m_rcTrackOutageSText; //上行轨道停电
            QRectF m_rcTrackOutageSLamp;
            QRectF m_rcTrackOutageXText; //下行轨道停电
            QRectF m_rcTrackOutageXLamp;

            QRectF m_rcTotalLockSText; //上行总锁闭
            QRectF m_rcTotalLockSLamp;
            QRectF m_rcTotalLockXText; //下行总锁闭
            QRectF m_rcTotalLockXLamp;

            QRectF m_rcFilamentBreakSText; //上行灯丝断丝
            QRectF m_rcFilamentBreakSLamp;
            QRectF m_rcFilamentBreakXText; //下行灯丝断丝
            QRectF m_rcFilamentBreakXLamp;

            QRectF m_rcCrowdAlarmText; //挤岔报警
            QRectF m_rcCrowdAlarmLamp;
            QRectF m_rcCodeAlarmText; //电码化报警
            QRectF m_rcCodeAlarmLamp;

            QString m_strSGDTDJ;
            int m_nSGDTDJ = 0;
            QString m_strXGDTDJ;
            int m_nXGDTDJ = 0;

            QString m_strTrackType;
        };
    }
}