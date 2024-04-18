#pragma once
#include "DeviceBase.h"

namespace Station {
    namespace Device {
        //场联
        class StaScattered : public StaDistant
        {
        public:
            explicit StaScattered(QObject* parent = nullptr);
            ~StaScattered();

        private:
            virtual bool eventFilter(QObject* obj, QEvent* event);

        private:
            //站场绘制
            void Draw(const bool& isMulti = false);
            //绘制信号灯
            void DrawLight();
            //绘制文字
            void DrawText();

            //站场翻转
            void setVollover(const QPoint& ptBase);
            //状态重置
            void ResetDevState();

        private:
            QRect getLempRectByTextRect(const QRect& rcText);

        private:
            QRect m_rcFuseAlarmText;    //熔丝报警
            QRect m_rcFuseAlarmLemp;
            QRect m_rcMainAuxPowerText; //主副电源
            QRect m_rcMainAuxPowerLemp;

            QRect m_rcTrackOutageSText; //上行轨道停电
            QRect m_rcTrackOutageSLemp;
            QRect m_rcTrackOutageXText; //下行轨道停电
            QRect m_rcTrackOutageXLemp;

            QRect m_rcTotalLockSText; //上行总锁闭
            QRect m_rcTotalLockSLemp;
            QRect m_rcTotalLockXText; //下行总锁闭
            QRect m_rcTotalLockXLemp;

            QRect m_rcFilamentBreakSText; //上行灯丝断丝
            QRect m_rcFilamentBreakSLemp;
            QRect m_rcFilamentBreakXText; //下行灯丝断丝
            QRect m_rcFilamentBreakXLemp;

            QRect m_rcCrowdAlarmText; //挤岔报警
            QRect m_rcCrowdAlarmLemp;
            QRect m_rcCodeAlarmText; //电码化报警
            QRect m_rcCodeAlarmLemp;

            QString m_strSGDTDJ;
            int m_nSGDTDJ = 0;
            QString m_strXGDTDJ;
            int m_nXGDTDJ = 0;

            QString m_strTrackType;
        };
    }
}