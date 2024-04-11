#include "StaTrack.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaTrack::StaTrack(QObject* parent)
        {
            m_mapAttribute.insert("GD_Type", [&](const QString& strElement) { m_strTrackType = strElement; });
        }

        StaTrack::~StaTrack()
        {

        }

        bool StaTrack::eventFilter(QObject* obj, QEvent* event)
        {
            return StaSection::eventFilter(obj, event);
        }

        void StaTrack::Draw(const bool& bElapsed, const bool& isMulti)
        {
            //绘制股道底部的灰色区域
            DrawTrackBack();
            //绘制股道
            DrawTrack(QPen(getTrackColor(bElapsed), Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

            return StaSection::Draw(bElapsed, isMulti);
        }

        void StaTrack::DrawTrackBack()
        {
            if (m_strTrackType == "ZX_GD" || m_strTrackType == "GD_QD") {
                QRect bkRect(
                    Scale(QPoint(p12.x() + (p34.x() - p12.x()) / 4, p12.y() - 12)),
                    QSize(Scale((p34.x() - p12.x()) / 2), Scale(p34.y() - p12.y() + 24)));
                m_pPainter.setPen(QPen(COLOR_TRACK_DEEPBLUE));
                m_pPainter.setBrush(QBrush(COLOR_TRACK_DEEPBLUE, Qt::SolidPattern));
                m_pPainter.drawRect(bkRect);
            }
        }

        void StaTrack::DrawTrack(const QPen& pen, const bool bOutSide, const int nOffset)
        {
            if (m_nZ == 0) {
                DrawTrackLine(pen, p12, p34, bOutSide, nOffset);
            }
            else if (m_nZ == 1) {
                DrawTrackLine(pen, p12, m_ptCenter, bOutSide, nOffset);
                DrawTrackLine(pen, m_ptCenter, p34, bOutSide, nOffset);
            }
            else {
                DrawTrackLine(pen, p12, pz12, bOutSide, nOffset);
                DrawTrackLine(pen, pz12, pz34, bOutSide, nOffset);
                DrawTrackLine(pen, pz34, p34, bOutSide, nOffset);
            }
        }

        void StaTrack::DrawDeviceOutSide(const bool& bElapsed)
        {
            //绘制股道外边缘-分路不良
            if (m_bShuntFault) {
                if (m_bSpeedLimit || m_bPowerCut) {
                    if (m_bShuntFaultIdle || bElapsed) {
                        DrawTrack(QPen(COLOR_TRACK_OUTSIDE_RED, 2), true, 4);
                    }
                }
                else {
                    if (m_bShuntFaultIdle || bElapsed) {
                        DrawTrack(QPen(COLOR_TRACK_OUTSIDE_RED, 2), true, 2);
                    }
                }
            }
            //绘制股道外边缘-临时限速,股道无电
            if (m_bSpeedLimit) { //临时限速
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_YELLOW, 2), true, 2);
            }
            else if (m_bPowerCut) { //股道无电
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_GREEN, 2), true, 2);
            }
        }
        
        void StaTrack::DrawSelectRange()
        {
            //股道虚线框
            DrawTrack(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine), true, 5);
            return DeviceBase::DrawSelectRange();
        }

        void StaTrack::DrawInsulateNode()
        {
            if (!m_bInsulateNodeChange) {
                return;
            }

            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.drawLine(Scale(QPoint(p1.x(), p1.y())), Scale(QPoint(p2.x(), p2.y()))); //绘制轨道区段左侧绝缘节
            m_pPainter.drawLine(Scale(QPoint(p3.x(), p3.y())), Scale(QPoint(p4.x(), p4.y()))); //绘制轨道区段右侧绝缘节
        }

        QPen StaTrack::getDeviceNameColor(const bool& bElapsed)
        {
            return QPen(m_bRangeVisible ? Qt::black : Qt::white);
        }

        void StaTrack::setVollover(const QPoint& ptBase)
        {

        }

        void StaTrack::ResetDevState()
        {

        }
    }
}