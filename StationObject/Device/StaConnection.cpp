#include "StaConnection.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        StaConnection::StaConnection(QObject* pParent) : DeviceArrow(m_mapAttribute)
        {
            m_mapAttribute.insert("flag", [&](const QString& strElement) { m_nFlag = strElement.toInt(); });
            m_mapAttribute.insert("Pxh", [&](const QString& strElement) { m_ptSignal = QStringToQPoint(strElement); });
            m_mapAttribute.insert("Pjj", [&](const QString& strElement) { m_ptNear = QStringToQPoint(strElement); });
            m_mapAttribute.insert("Pzc", [&](const QString& strElement) { m_ptTakeLook = QStringToQPoint(strElement); });
            m_mapAttribute.insert("Pyx", [&](const QString& strElement) { m_ptAllow = QStringToQPoint(strElement); });
            m_mapAttribute.insert("Pbc", [&](const QString& strElement) { m_ptFileld = QStringToQPoint(strElement); });
            m_mapAttribute.insert("Plc", [&](const QString& strElement) { m_ptAdjFileld = QStringToQPoint(strElement); });
            m_mapAttribute.insert("JJbsdRect", [&](const QString& strElement) { m_rcNearLight = QStringToQRect(strElement); });
            m_mapAttribute.insert("BCbsdRect", [&](const QString& strElement) { m_rcFileldLight = QStringToQRect(strElement); });
            m_mapAttribute.insert("LCbsdRect", [&](const QString& strElement) { m_rcAdjFileldLight = QStringToQRect(strElement); });
            m_mapAttribute.insert("YXANRect1", [&](const QString& strElement) { 
                m_rcAllowBtn = QStringToQRect(strElement);
                m_rcAllowBtn.setWidth(17);
                m_rcAllowBtn.setHeight(17);
            });
            m_mapAttribute.insert("CL_Type", [&](const QString& strElement) { m_strCLType = strElement; });
        }

        StaConnection::~StaConnection()
        {

        }

        void StaConnection::Draw(const bool& isMulti)
        {
            DrawArrow(m_pPainter);
            if (m_strCLType.contains("_DC")) {
                DrawButton(m_pPainter, Scale(m_rcAllowBtn), COLOR_BTN_DEEPGRAY, m_nBtnState, 2);
            }
            return StaDistant::Draw(isMulti);
        }

        void StaConnection::DrawLight()
        {
            //接近
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));
            m_pPainter.setBrush((m_nState & 0x04) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcNearLight));
            if (m_strCLType.contains("_DC")) {
                //本场
                m_pPainter.setBrush((m_nState & 0x40) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
                m_pPainter.drawEllipse(Scale(m_rcFileldLight));
                //邻场
                m_pPainter.setBrush((m_nState & 0x80) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
                m_pPainter.drawEllipse(Scale(m_rcAdjFileldLight));
            }
        }

        void StaConnection::DrawText()
        {
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号

            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            
            //接近
            m_pPainter.drawText(Scale(QRect(m_ptNear, fontMetrics.size(Qt::TextSingleLine, "接近"))), "接近", QTextOption(Qt::AlignCenter));
            
            if (m_bUpDown) {
                //信号
                m_pPainter.drawText(Scale(QRect(m_ptSignal, fontMetrics.size(Qt::TextSingleLine, "信号"))), "信号", QTextOption(Qt::AlignCenter));
                //照查  
                m_pPainter.drawText(Scale(QRect(m_ptTakeLook, fontMetrics.size(Qt::TextSingleLine, "照查"))), "照查", QTextOption(Qt::AlignCenter));
            }
            else {
                //信号
                m_pPainter.drawText(Scale(QRect(m_ptTakeLook, fontMetrics.size(Qt::TextSingleLine, "信号"))), "信号", QTextOption(Qt::AlignCenter));
                //照查  
                m_pPainter.drawText(Scale(QRect(m_ptSignal, fontMetrics.size(Qt::TextSingleLine, "照查"))), "照查", QTextOption(Qt::AlignCenter));
            }
            
            if (m_strCLType.contains("_DC")) {
                //允许
                m_pPainter.drawText(Scale(QRect(m_ptAllow, fontMetrics.size(Qt::TextSingleLine, "允许操动道岔"))), "允许操动道岔", QTextOption(Qt::AlignCenter));
                //本场
                m_pPainter.drawText(Scale(QRect(m_ptFileld, fontMetrics.size(Qt::TextSingleLine, "本场"))), "本场", QTextOption(Qt::AlignCenter));
                //邻场
                m_pPainter.drawText(Scale(QRect(m_ptAdjFileld, fontMetrics.size(Qt::TextSingleLine, "邻场"))), "邻场", QTextOption(Qt::AlignCenter));
            }
        }

        void StaConnection::getArrowColor()
        {
            QColor cColor1;
            QColor cColor2;

            if ((m_nArrowState & 0x03) == 0x01) {
                cColor1 = COLOR_LIGHT_GREEN;
            }
            else if ((m_nArrowState & 0x03) == 0x02) {
                cColor1 = COLOR_LIGHT_WHITE;
            }
            else {
                cColor1 = COLOR_LIGHT_BLACK;
            }

            if ((m_nArrowState & 0x30) == 0x10) {
                cColor2 = COLOR_LIGHT_GREEN;
            }
            else if ((m_nArrowState & 0x30) == 0x20) {
                cColor2 = COLOR_LIGHT_WHITE;
            }
            else {
                cColor2 = COLOR_LIGHT_BLACK;
            }

            if (m_bUpDown) {
                m_cColor1 = cColor1;
                m_cColor2 = cColor2;
            }
            else {
                m_cColor2 = cColor1;
                m_cColor1 = cColor2;
            }
        }

        bool StaConnection::Contains(const QPoint& ptPos)
        {
            return m_rcAllowBtn.contains(ptPos);
        }

        bool StaConnection::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                return m_rcAllowBtn.contains(ptPos);
            }
            return false;
        }

        void StaConnection::InitClickEvent()
        {
            m_mapClickEvent.insert(CTCWindows::FunType::FunBtn, [&]() {
                OnButtonClick(this);
            });
        }

        void StaConnection::SetBtnState()
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                m_nBtnState = 1;
                m_nFirstBtnType = 5;
                CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Allow);
            }
        }

        void StaConnection::setVollover(const QPoint& ptBase)
        {

        }

        void StaConnection::ResetDevState()
        {

        }
    }
}