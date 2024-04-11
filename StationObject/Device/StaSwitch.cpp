#include "StaSwitch.h"
#include "Global.h"

#define MAX(a, b)  (a > b ? a : b)
#define MIN(a, b)  (a < b ? a : b)

namespace Station {
    namespace Device {

        StaSwitch::StaSwitch(QObject* parent)
        {
            m_mapAttribute.insert("m_nQDCode", [&](const QString& strElement) { m_nQDCode = strElement.toUInt(nullptr, 16); });
            m_mapAttribute.insert("m_nCxjy", [&](const QString& strElement) { m_nCxjy = strElement.toUInt(); });
            m_mapAttribute.insert("m_nDSCode", [&](const QString& strElement) { m_nDSCode = strElement.toInt(); });
            m_mapAttribute.insert("QDMKInterlockBus", [&](const QString& strElement) { m_nQDMKInterlockBus = strElement.toInt(); });
            m_mapAttribute.insert("m_nJyj", [&](const QString& strElement) { m_nJyj = strElement.toInt(); });
            m_mapAttribute.insert("oneToMore", [&](const QString& strElement) { m_nOneToMore = strElement.toInt(); });
            m_mapAttribute.insert("p5", [&](const QString& strElement) { p5 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p6", [&](const QString& strElement) { p6 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p56", [&](const QString& strElement) { p56 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("pZ12", [&](const QString& strElement) { pz12 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("pZ34", [&](const QString& strElement) { pz34 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("pZ56", [&](const QString& strElement) { pz56 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("m_MainDCQD", [&](const QString& strElement) { m_nMainDCQD = strElement.toInt(); });
            m_mapAttribute.insert("isSafetySwitch", [&](const QString& strElement) { m_bSafetySwitch = strElement.toInt(); });
            m_mapAttribute.insert("m_nCQ", [&](const QString& strElement) { m_nQ = strElement.toInt(); });
            m_mapAttribute.insert("m_nDW", [&](const QString& strElement) { m_nD = strElement.toInt(); });
            m_mapAttribute.insert("m_nFW", [&](const QString& strElement) { m_nF = strElement.toInt(); });
        }

        StaSwitch::~StaSwitch()
        {
            
        }

        bool StaSwitch::eventFilter(QObject* obj, QEvent* event)
        {
            return StaSection::eventFilter(obj, event);
        }

        void StaSwitch::InitDeviceAttribute()
        {
            //三角函数求出岔心坐标
            double a, b, c;
            a = (double)m_ptCenter.x() - (double)pz34.x();
            b = (double)m_ptCenter.y() - (double)pz34.y();
            c = sqrt(a * a + b * b);
            p34C.setX(m_ptCenter.x() - 16 * a / c);
            p34C.setY(m_ptCenter.y() - 16 * b / c);

            a = (double)m_ptCenter.x() - (double)pz56.x();
            b = (double)m_ptCenter.y() - (double)pz56.y();
            c = sqrt(a * a + b * b);
            p56C.setX(m_ptCenter.x() - 16 * a / c);
            p56C.setY(m_ptCenter.y() - 16 * b / c);
        }
    
        void StaSwitch::Draw(const bool& bElapsed, const bool& isMulti)
        {
            //绘制股道
            DrawTrack(QPen(getTrackColor(bElapsed), Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nSwitchState | SWITCH_DRAW_CQ);
            DrawTrack(QPen(COLOR_TRACK_BLUE, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), 0x07 ^ (m_nSwitchState | SWITCH_DRAW_CQ));
            //绘制岔心
            DrawSwitchCenterTrack(bElapsed);
            //绘制股道状态
            DrawSwitchState();

            return StaSection::Draw(bElapsed, isMulti);
        }

        void StaSwitch::DrawTrack(const QPen& pen, const uint nPosition, const bool bOutSide, const int nOffset)
        {
            //岔前
            if (nPosition & SWITCH_DRAW_CQ) {
                if (m_nZ & SWITCH_KINK_CQ) {
                    DrawTrackLine(pen, p12, pz12, bOutSide, nOffset);
                    DrawTrackLine(pen, pz12, m_ptCenter, bOutSide, nOffset);
                }
                else {
                    DrawTrackLine(pen, p12, m_ptCenter, bOutSide, nOffset);
                }
            }
            //定位
            if (nPosition & SWITCH_DRAW_DW) {
                if (m_nZ & SWITCH_KINK_DW) {
                    DrawTrackLine(pen, p34C, pz34, bOutSide, nOffset);
                    DrawTrackLine(pen, pz34, p34, bOutSide, nOffset);
                }
                else {
                    DrawTrackLine(pen, p34C, p34, bOutSide, nOffset);
                }
            }
            //反位
            if (nPosition & SWITCH_DRAW_FW) {
                if (m_nZ & SWITCH_KINK_FW) {
                    DrawTrackLine(pen, p56C, pz56, bOutSide, nOffset);
                    DrawTrackLine(pen, pz56, p56, bOutSide, nOffset);
                }
                else {
                    DrawTrackLine(pen, p56C, p56, bOutSide, nOffset);
                }
            }
        }

        void StaSwitch::DrawSwitchCenterTrack(const bool& bElapsed)
        {
            if (m_bRangeVisible) { //选中状态
                DrawSwitchCenter(QPen(COLOR_TRACK_SELECT_BLUE, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), SWITCH_DRAW_DW | SWITCH_DRAW_FW);
            }
            else if (m_nShuntFault) { //分路不良
                if (bElapsed && !m_bShuntFaultIdle) {
                    DrawSwitchCenter(QPen(COLOR_TRACK_YELLOW, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nShuntFault & m_nSwitchState);
                }
            }
            else {
                QColor cColor = getSwitchCenterColor(bElapsed);
                DrawSwitchCenter(QPen(cColor == Qt::black ? Qt::NoPen : cColor, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nSwitchState & 0x03);
            }

            if (m_nSpeedLimit) { //临时限速
                DrawSwitchCenter(QPen(COLOR_TRACK_OUTSIDE_YELLOW, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nSpeedLimit & m_nSwitchState, true, 2);
            }
        }

        void StaSwitch::DrawSwitchCenter(const QPen& pen, const uint nPosition, const bool bOutSide, const int nOffset)
        {
            if (nPosition & SWITCH_STATE_DW) {  //定位
                DrawTrackLine(pen, m_ptCenter, p34C, bOutSide, nOffset);
            }
            else if (nPosition & SWITCH_STATE_FW) { //反位
                DrawTrackLine(pen, m_ptCenter, p56C, bOutSide, nOffset);
            }
            else {  //四开
                DrawTrackLine(pen, m_ptCenter, p34C, bOutSide, nOffset);
                DrawTrackLine(pen, m_ptCenter, p56C, bOutSide, nOffset);
            }
        }

        void StaSwitch::DrawDeviceOutSide(const bool& bElapsed)
        {
            //绘制股道外边缘-分路不良
            if (m_nShuntFault && (m_bShuntFaultIdle || bElapsed)) {
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_RED, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nShuntFault & (m_nSpeedLimit | m_nPowerCut), true, 4);
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_RED, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nShuntFault & (~(m_nSpeedLimit | m_nPowerCut)), true, 2);
            }
            //股道无电
            if (m_nPowerCut) { 
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_GREEN, 2), m_nPowerCut & (~m_nSpeedLimit), true, 2);
            }
            //临时限速
            if (m_nSpeedLimit) { 
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_YELLOW, 2), m_nSpeedLimit & m_nPowerCut, true, 2);
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_YELLOW, 2), m_nSpeedLimit & (~m_nPowerCut), true, 2);
            }
        }

        void StaSwitch::DrawSelectRange()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            //道岔虚线框
            int minX = MIN(MIN(p12.x(), p34.x()), p56.x());
            int minY = MIN(MIN(p12.y(), p34.y()), p56.y());
            int maxX = MAX(MAX(p12.x(), p34.x()), p56.x());
            int maxY = MAX(MAX(p12.y(), p34.y()), p56.y());
            m_rcDevRect = QRect(QPoint(minX - 10, minY - 10), QPoint(maxX + 10, maxY + 10));
            m_pPainter.drawRect(Scale(m_rcDevRect));

            return DeviceBase::DrawSelectRange();
        }

        void StaSwitch::DrawInsulateNode()
        {
            m_pPainter.setRenderHint(QPainter::Antialiasing, true);
            
            //岔前
            if (m_nJyj & SWITCH_KINK_CQ) {
                if (m_nCxjy & SWITCH_KINK_CQ) {
                    m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 1));
                    m_pPainter.drawEllipse(p12, Scale(5), Scale(5));
                }
                m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
                m_pPainter.drawLine(p1, p2);
            }
            //定位
            if (m_nJyj & SWITCH_KINK_DW) {
                if (m_nCxjy & SWITCH_KINK_DW) {
                    m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 1));
                    m_pPainter.drawEllipse(p34, Scale(5), Scale(5));
                }
                m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
                m_pPainter.drawLine(p3, p4);
            }
            //反位
            if (m_nJyj & SWITCH_KINK_FW) {
                if (m_nCxjy & SWITCH_KINK_FW) {
                    m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 1));
                    m_pPainter.drawEllipse(p56, Scale(5), Scale(5));
                }
                m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
                m_pPainter.drawLine(p5, p6);
            }

            m_pPainter.setRenderHint(QPainter::Antialiasing, false);
        }

        void StaSwitch::DrawSwitchState()
        {
            if (m_nSwitchState & SWITCH_STATE_LOCK) { //单锁
                //反走样,防止出现锯齿状线条
                m_pPainter.setRenderHint(QPainter::Antialiasing, true);

                m_pPainter.setPen(QPen(isSwitchSK() ? COLOR_TRACK_RED : COLOR_TRACK_GREEN, Scale(1)));
                m_pPainter.drawEllipse(m_ptCenter, Scale(8), Scale(8));

                m_pPainter.setRenderHint(QPainter::Antialiasing, false);
            }
            if (m_nSwitchState & SWITCH_STATE_BLOCK) { //封锁
                m_pPainter.setPen(QPen(Qt::red, Scale(1)));
                m_pPainter.drawRect(Scale(m_rcTextRect));
            }
        }

        void StaSwitch::setVollover(const QPoint& ptBase)
        {

        }

        void StaSwitch::ResetDevState()
        {

        }

        QColor StaSwitch::getSwitchCenterColor(const bool& bElapsed)
        {
            QColor cTrackColor = Qt::black;

            if (m_nSwitchState & SWITCH_DRAW_DW) {
                cTrackColor = COLOR_TRACK_GREEN;
            }
            else if (m_nSwitchState & SWITCH_DRAW_FW) {
                cTrackColor = COLOR_TRACK_YELLOW;
            }
            else {
                cTrackColor = bElapsed ? COLOR_TRACK_RED : cTrackColor;
            }

            if (m_nState & SECTION_STATE_TAKEUP) {
                cTrackColor = COLOR_TRACK_RED;
            }

            if (m_nState & SECTION_STATE_PRELOCK) {
                cTrackColor = COLOR_TRACK_PRELOCK_BLUE;
            }
            else if (m_nState & SECTION_STATE_LOCK) {
                cTrackColor = COLOR_TRACK_WHITE;
            }
            else if (m_nState & SECTION_STATE_FAULTLOCK) {
                cTrackColor = COLOR_TRACK_RED;
            }

            if (m_nState & SECTION_STATE_BLOCK) {
                cTrackColor = bElapsed ? COLOR_TRACK_RED : cTrackColor;
            }

            if (m_bRangeVisible) {
                cTrackColor = COLOR_TRACK_SELECT_BLUE;
            }
            return cTrackColor;
        }

        QPen StaSwitch::getDeviceNameColor(const bool& bElapsed)
        {
            QPen pen;
            if (isSwitchSK()) {
                pen = QPen(!bElapsed ? COLOR_TRACK_RED : Qt::NoPen);
            }
            else if (m_nSwitchState & SWITCH_STATE_DW) {
                pen = QPen(COLOR_TRACK_GREEN);
            }
            else if (m_nSwitchState & SWITCH_STATE_FW) {
                pen = QPen(COLOR_TRACK_YELLOW);
            }

            if (m_bRangeVisible) {
                pen = QPen(Qt::black);
            }

            return pen;
        }

        void StaSwitch::setSwitchState(const uint& nSwitchState)
        { 
            m_nSwitchState = nSwitchState;
        }

        bool StaSwitch::isSwitchSK()
        { 
            return ((m_nSwitchState & SWITCH_STATE_SK) == SWITCH_STATE_SK) || ((m_nSwitchState & SWITCH_STATE_SK) == 0);
        }
    }
}
