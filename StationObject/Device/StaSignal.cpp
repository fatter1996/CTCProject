#include "StaSignal.h"
#include "Global.h"
#include <QDebug>
namespace Station {
    namespace Device {

        StaSignal::StaSignal(QObject* parent)
        {
            m_mapAttribute.insert("p12", [&](QString strElement) { p12 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p7", [&](QString strElement) { p7 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p8", [&](QString strElement) { p8 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p9", [&](QString strElement) { p9 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p10", [&](QString strElement) { p10 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p13", [&](QString strElement) { p13 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p14", [&](QString strElement) { p14 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("m_nXHDType", [&](const QString& strElement) { m_nXHDType = strElement.toUInt(); });
            m_mapAttribute.insert("radius", [&](const QString& strElement) { m_nRadius = strElement.toUInt(); });
            m_mapAttribute.insert("SignalType", [&](const QString& strElement) { m_nSignalType = strElement.toUInt(); });
            m_mapAttribute.insert("D_B_C_Signa", [&](const QString& strElement) { m_nD_B_C_Signal = strElement.toUInt(); });
            m_mapAttribute.insert("DC_LC_Signa", [&](const QString& strElement) { m_nDC_LC_Signal = strElement.toUInt(); });
            m_mapAttribute.insert("safeLamp", [&](const QString& strElement) { m_strSafeLamp = strElement; });
            m_mapAttribute.insert("isHigh", [&](const QString& strElement) { m_bHigh = strElement.toInt(); });
            m_mapAttribute.insert("IsHaveBSQ", [&](const QString& strElement) { m_bIsIsHaveBSQ = strElement.toInt(); });
            m_mapAttribute.insert("bsqNum", [&](const QString& strElement) { m_nBSQNum = strElement.toInt(); });
            m_mapAttribute.insert("bsqInterlockBus", [&](const QString& strElement) { m_nBSQInterlockBus = strElement.toInt(); });
            m_mapAttribute.insert("m_nBSQModuAddr", [&](const QString& strElement) { m_nBSQModuAddr = strElement.toInt(); });
            m_mapAttribute.insert("isMD", [&](const QString& strElement) { m_bSingleDeng = strElement.toInt(); });
            m_mapAttribute.insert("DC_LC_Signa", [&](const QString& strElement) { m_bMD = strElement.toInt(); });
        }

        StaSignal::~StaSignal()
        {

        }

        bool StaSignal::eventFilter(QObject* obj, QEvent* event)
        {
            return DeviceBase::eventFilter(obj, event);
        }

        void StaSignal::InitDeviceAttribute()
        {
            //列车按钮
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {
                n_rcTrainBtn = QRect(p7, p8);
            }
            //调车按钮
            if (m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) {
                if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {
                    n_rcShuntBtn = QRect(p9, p10);
                }
                else {
                    n_rcShuntBtn = QRect(p7, p8);
                }
            }
            //引导按钮,仅可作为始端
            if ((m_nAttr & SIGNAL_LCSD) && (m_nAttr & SIGNAL_JCXH || m_nAttr & SIGNAL_FCJLXH) && p13 != QPoint() && p14 != QPoint()) {
                if ((m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) && (QRect(p13, p14) == QRect(p9, p10))) {
                    n_rcGuideBtn = QRect(
                        QPoint((m_nAttr & SIGNAL_SYH) ? (p13.x() - 20) : (p13.x() + 20), p13.y()),
                        QPoint((m_nAttr & SIGNAL_SYH) ? (p14.x() - 20) : (p14.x() + 20), p14.y()));
                }
                else {
                    n_rcGuideBtn = QRect(p13, p14);
                }
            }

            int nDiameter = m_nRadius * 2;
            //第一灯位
            if (p12.x() > p8.x()) {    //向右
                n_rcLight1 = QRect(p12.x() + m_nRadius, p12.y() - m_nRadius, Scale(nDiameter), Scale(nDiameter));
            }
            else {
                n_rcLight1 = QRect(p12.x() - 3 * m_nRadius, p12.y() - m_nRadius, Scale(nDiameter), Scale(nDiameter));
            }
            //第二灯位
            if ((m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) && ((m_nAttr & (SIGNAL_LCZH | SIGNAL_AQH)) != SIGNAL_LCZH)) {
                if (p12.x() > p8.x()) {    //向右
                    n_rcLight2 = QRect(p12.x() + 3 * m_nRadius, p12.y() - m_nRadius, Scale(nDiameter), Scale(nDiameter));
                }
                else {
                    n_rcLight2 = QRect(p12.x() - 5 * m_nRadius, p12.y() - m_nRadius, Scale(nDiameter), Scale(nDiameter));
                }
            }
        }

        void StaSignal::Draw(const bool& bElapsed, const bool& isMulti)
        {
            if (!m_pPainter) {
                return;
            }

            if (!isMulti && bShowBtn) {
                DrawSignalButton(bElapsed);
            }
            DrawSignalLight(bElapsed);
            return DeviceBase::Draw(bElapsed, isMulti);
        }

        void StaSignal::DrawSignalButton(const bool& bElapsed)
        {
            //列车按钮
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {   
                DrawButton(m_pPainter, bElapsed, Scale(n_rcTrainBtn), COLOR_BTN_GREEN);
            }
            //调车按钮
            if (m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) {
                DrawButton(m_pPainter, bElapsed, Scale(n_rcShuntBtn), COLOR_BTN_DEEPGRAY, 2);
            }
            //引导按钮,仅可作为始端
            if ((m_nAttr & SIGNAL_LCSD) && (m_nAttr & SIGNAL_JCXH || m_nAttr & SIGNAL_FCJLXH) && n_rcGuideBtn != QRect()) {
                DrawButton(m_pPainter, bElapsed, Scale(n_rcGuideBtn), COLOR_BTN_BLUE_YD);
            }
        }

        void StaSignal::DrawSignalLight(const bool& bElapsed)
        {
            if (m_nAttr & SIGNAL_ISXXH) {  //虚信号机,不绘制灯位
                return;
            }

            m_pPainter->setPen(QPen(COLOR_TRACK_BLUE, 2));
            //绘制灯柱
            m_pPainter->drawLine(Scale(QPoint(p12.x(), p12.y() + m_nRadius)), Scale(QPoint(p12.x(), p12.y() - m_nRadius)));
            if (p12.x() > p8.x()) {    //向右
                m_pPainter->drawLine(Scale(QPoint(p12.x(), p12.y())), Scale(QPoint(p12.x() + m_nRadius, p12.y())));
            }
            else {
                m_pPainter->drawLine(Scale(QPoint(p12.x(), p12.y())), Scale(QPoint(p12.x() - m_nRadius, p12.y())));
            }
            
            QColor cColor1;
            QColor cColor2;
            GetSignalLightColor(cColor1, cColor2, bElapsed);
            m_pPainter->setRenderHint(QPainter::Antialiasing, true);
            //第一灯位
            m_pPainter->setBrush(cColor1);
            m_pPainter->drawEllipse(Scale(n_rcLight1));
            //第二灯位
            if ((m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) && ((m_nAttr & (SIGNAL_LCZH | SIGNAL_AQH)) != SIGNAL_LCZH)) {  
                m_pPainter->setBrush(cColor2);
                m_pPainter->drawEllipse(Scale(n_rcLight2));
            }   
        }

        void StaSignal::DrawSelectRange()
        {
            DrawLightRange();
            //选中类型(0-未选中; 0x01-选中信号机名称; 0x02-选中列车按钮; 0x04-选中调车按钮; 0x08-选中通过按钮; 0x1f-选中信号机灯位)
            if (m_nSelectType & 0x01) {  //信号机名称
                DeviceBase::DrawSelectRange();
            }
            DrawButtonRange();
        }

        void StaSignal::DrawLightRange()
        {
            if (m_nAttr & SIGNAL_ISXXH) {  //虚信号机,不绘制灯位
                return;
            }

            m_pPainter->setBrush(Qt::NoBrush);
            m_pPainter->setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            //第一灯位
            m_pPainter->drawRect(Scale(n_rcLight1));
            //第二灯位
            if ((m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) && ((m_nAttr & (SIGNAL_LCZH | SIGNAL_AQH)) != SIGNAL_LCZH)) {
                m_pPainter->drawRect(Scale(n_rcLight2));
            }
        }

        void StaSignal::DrawDeviceNameRange()
        {
            m_pPainter->setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            m_pPainter->setBrush(COLOR_TRACK_SELECT_BLUE);
            m_pPainter->drawRect(Scale(OutSideRect(m_rcTextRect, 2, 0)));
        }

        void StaSignal::DrawButtonRange()
        {
            m_pPainter->setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            m_pPainter->setBrush(Qt::NoBrush);

            if (m_nSelectType & 0x02) {   //列车信号机按钮
                if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {
                    m_pPainter->drawRect(Scale(OutSideRect(n_rcTrainBtn, 1, 1)));
                    
                }
            }
            if (m_nSelectType & 0x04) {   //调车信号机按钮
                if (m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) {
                    m_pPainter->drawRect(Scale(OutSideRect(n_rcShuntBtn, 1, 1)));
                }
            }
            if (m_nSelectType & 0x08) {   //通过信号机按钮
                if (m_pRelatedBtn) {
                    m_pRelatedBtn->setRangeVisible(true);
                }
            }
            if (m_nSelectType & 0x10) {   //引导信号机按钮
                if ((m_nAttr & SIGNAL_LCSD) && (m_nAttr & SIGNAL_JCXH || m_nAttr & SIGNAL_FCJLXH)) {
                    m_pPainter->drawRect(Scale(OutSideRect(n_rcGuideBtn, 1, 1)));
                }
            }
        }

        void StaSignal::GetSignalLightColor(QColor& cColor1, QColor& cColor2, const bool& bElapsed)
        {
            switch (m_nState & 0x0f) {
                case SIGNAL_STATE_B   : cColor1 = COLOR_LIGHT_WHITE;   cColor2 = COLOR_LIGHT_BLACK;     break;
                case SIGNAL_STATE_A   : cColor1 = COLOR_LIGHT_BLUE;    cColor2 = COLOR_LIGHT_BLACK;     break;
                case SIGNAL_STATE_H   : cColor1 = COLOR_LIGHT_RED;     cColor2 = COLOR_LIGHT_BLACK;     break;
                case SIGNAL_STATE_L   : cColor1 = COLOR_LIGHT_BLACK;   cColor2 = COLOR_LIGHT_GREEN;     break;
                case SIGNAL_STATE_LL  : cColor1 = COLOR_LIGHT_GREEN;   cColor2 = COLOR_LIGHT_GREEN;     break;
                case SIGNAL_STATE_U   : cColor1 = COLOR_LIGHT_BLACK;   cColor2 = COLOR_LIGHT_YELLOW;    break;
                case SIGNAL_STATE_UU  : cColor1 = COLOR_LIGHT_YELLOW;  cColor2 = COLOR_LIGHT_YELLOW;    break;
                case SIGNAL_STATE_2U  : cColor1 = COLOR_LIGHT_BLACK;   cColor2 = COLOR_LIGHT_YELLOW;    break;
                case SIGNAL_STATE_HB  : cColor1 = COLOR_LIGHT_WHITE;   cColor2 = COLOR_LIGHT_RED;       break;
                case SIGNAL_STATE_LU  : cColor1 = COLOR_LIGHT_GREEN;   cColor2 = COLOR_LIGHT_YELLOW;    break;
                case SIGNAL_STATE_BS  : cColor1 = bElapsed ? COLOR_LIGHT_WHITE : COLOR_LIGHT_BLACK;     cColor2 = COLOR_LIGHT_BLACK;     break;
                case SIGNAL_STATE_HS  : cColor1 = bElapsed ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK;       cColor2 = COLOR_LIGHT_BLACK;     break;
                case SIGNAL_STATE_LS  : cColor1 = COLOR_LIGHT_BLACK;   cColor2 = bElapsed ? COLOR_LIGHT_GREEN : COLOR_LIGHT_BLACK;       break;
                case SIGNAL_STATE_USU : cColor1 = bElapsed ? COLOR_LIGHT_YELLOW : COLOR_LIGHT_BLACK;    cColor2 = COLOR_LIGHT_YELLOW;    break;
                case SIGNAL_STATE_US  : cColor1 = bElapsed ? COLOR_LIGHT_YELLOW : COLOR_LIGHT_BLACK;    cColor2 = COLOR_LIGHT_BLACK;     break;
                case SIGNAL_STATE_DS  : cColor1 = bElapsed ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK;       cColor2 = COLOR_LIGHT_BLACK;     break;
                default               : cColor1 = COLOR_LIGHT_BLACK;   cColor2 = COLOR_LIGHT_BLACK;     break;
            }
        }

        QPen StaSignal::getDeviceNameColor(const bool& bElapsed)
        {
            return QPen((m_bRangeVisible && (m_nSelectType & 0x01)) ? Qt::black : Qt::white);
        }

        void StaSignal::OnButtonClick()
        {
        }

        void StaSignal::setVollover(const QPoint& ptBase)
        {

        }

        void StaSignal::ResetDevState()
        {

        }
    }
}