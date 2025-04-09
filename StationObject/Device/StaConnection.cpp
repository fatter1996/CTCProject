#include "StaConnection.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        StaConnection::StaConnection(QObject* pParent) 
            : StaDistant(pParent)
        {
            
        }

        StaConnection::~StaConnection()
        {

        }

        void StaConnection::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute.insert(m_strType, mapAttrFun);
            m_mapAttribute[m_strType].insert("flag", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_nFlag = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("Pxh", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_ptSignal = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("Pjj", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_ptNear = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("Pzc", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_ptTakeLook = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("Pyx", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_ptAllow = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("Pbc", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_ptFileld = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("Plc", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_ptAdjFileld = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("JJbsdRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_rcNearLight = QStringToQRectF(strElement); });
            m_mapAttribute[m_strType].insert("BCbsdRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_rcFileldLight = QStringToQRectF(strElement); });
            m_mapAttribute[m_strType].insert("LCbsdRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_rcAdjFileldLight = QStringToQRectF(strElement); });
            m_mapAttribute[m_strType].insert("YXANRect1", [](DeviceBase* pDevice, const QString& strElement) {
                dynamic_cast<StaConnection*>(pDevice)->m_rcAllowBtn = QStringToQRectF(strElement);
                dynamic_cast<StaConnection*>(pDevice)->m_rcAllowBtn.setWidth(17);
                dynamic_cast<StaConnection*>(pDevice)->m_rcAllowBtn.setHeight(17);
            });
            m_mapAttribute[m_strType].insert("CL_Type", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaConnection*>(pDevice)->m_strCLType = strElement; });
            InitArrowAttributeMap(m_strType, m_mapAttribute);
            return StaDistant::InitAttributeMap();
        }

        void StaConnection::Draw(bool isMulti)
        {
            DrawArrow(m_pPainter);
            if (m_strCLType.contains("_DC") && m_bMainStation) {
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
            m_pPainter.drawText(Scale(QRectF(m_ptNear, fontMetrics.size(Qt::TextSingleLine, "接近"))), "接近", QTextOption(Qt::AlignCenter));
            
            if (m_bUpDown) {
                //信号
                m_pPainter.drawText(Scale(QRectF(m_ptSignal, fontMetrics.size(Qt::TextSingleLine, "信号"))), "信号", QTextOption(Qt::AlignCenter));
                //照查  
                m_pPainter.drawText(Scale(QRectF(m_ptTakeLook, fontMetrics.size(Qt::TextSingleLine, "照查"))), "照查", QTextOption(Qt::AlignCenter));
            }
            else {
                //信号
                m_pPainter.drawText(Scale(QRectF(m_ptTakeLook, fontMetrics.size(Qt::TextSingleLine, "信号"))), "信号", QTextOption(Qt::AlignCenter));
                //照查  
                m_pPainter.drawText(Scale(QRectF(m_ptSignal, fontMetrics.size(Qt::TextSingleLine, "照查"))), "照查", QTextOption(Qt::AlignCenter));
            }
            
            if (m_strCLType.contains("_DC")) {
                //允许
                m_pPainter.drawText(Scale(QRectF(m_ptAllow, fontMetrics.size(Qt::TextSingleLine, "允许操动道岔"))), "允许操动道岔", QTextOption(Qt::AlignCenter));
                //本场
                m_pPainter.drawText(Scale(QRectF(m_ptFileld, fontMetrics.size(Qt::TextSingleLine, "本场"))), "本场", QTextOption(Qt::AlignCenter));
                //邻场
                m_pPainter.drawText(Scale(QRectF(m_ptAdjFileld, fontMetrics.size(Qt::TextSingleLine, "邻场"))), "邻场", QTextOption(Qt::AlignCenter));
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
            return Scale(m_rcAllowBtn).contains(ptPos);
        }

        bool StaConnection::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                return Scale(m_rcAllowBtn).contains(ptPos);
            }
            return false;
        }

        void StaConnection::InitClickEvent()
        {
            m_mapClickEvent[m_strType].insert(CTCWindows::FunType::FunBtn, [](DeviceBase* pDevice) {
                dynamic_cast<StaConnection*>(pDevice)->OnButtonClick();
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

        void StaConnection::setVollover(const QPointF& ptBase)
        {

        }

        void StaConnection::ResetDevState()
        {

        }
    }
}