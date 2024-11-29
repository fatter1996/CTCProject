#include "StaSemiAutoBlock.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaSemiAutoBlock::StaSemiAutoBlock(QObject* pParent) 
            : DeviceArrow(m_mapAttribute), StaDistant(pParent)
        {
            m_mapAttribute.insert("BS_Text", [&](const QString& strElement) { m_ptBSText = QStringToQPointF(strElement); });
            m_mapAttribute.insert("FY_Text", [&](const QString& strElement) { m_ptFYText = QStringToQPointF(strElement); });
            m_mapAttribute.insert("SG_Text", [&](const QString& strElement) { m_ptSGText = QStringToQPointF(strElement); });
            
            m_mapAttribute.insert("BS1_rect", [&](const QString& strElement) { 
                m_rcBSBtn = QStringToQRectF(strElement); 
                m_rcBSBtn.setWidth(17);
                m_rcBSBtn.setHeight(17);
            });

            m_mapAttribute.insert("FY1_rect", [&](const QString& strElement) {
                m_rcFYBtn = QStringToQRectF(strElement);
                m_rcFYBtn.setWidth(17);
                m_rcFYBtn.setHeight(17);
            });

            m_mapAttribute.insert("SG1_rect", [&](const QString& strElement) {
                m_rcSGBtn = QStringToQRectF(strElement);
                m_rcSGBtn.setWidth(17);
                m_rcSGBtn.setHeight(17);
            });

            m_mapAttribute.insert("BSType", [&](const QString& strElement) { m_nBlockType = strElement.toUInt(); });
        }

        StaSemiAutoBlock::~StaSemiAutoBlock()
        {

        }

        void StaSemiAutoBlock::Draw(bool isMulti)
        {
            DrawArrow(m_pPainter);
            if (m_bMainStation) {
                DrawButton(m_pPainter, Scale(m_rcBSBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x01);
                DrawButton(m_pPainter, Scale(m_rcFYBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x02);
                DrawButton(m_pPainter, Scale(m_rcSGBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x04);
            }
            return StaDistant::Draw(isMulti);
        }

        void StaSemiAutoBlock::DrawLight()
        {
        
        }

        void StaSemiAutoBlock::DrawText()
        {
            if (!MainStation()->IsVisible(VisibleDev::funButtonName)) {
                return;
            }
            QFont font;
            font.setFamily("Î¢ÈíÑÅºÚ");
            font.setPixelSize(Scale(m_nFontSize));//×ÖºÅ

            m_pPainter.setFont(font);//ÉèÖÃ×ÖÌå
            m_pPainter.setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            //±ÕÈû
            m_pPainter.drawText(Scale(QRectF(m_ptBSText, fontMetrics.size(Qt::TextSingleLine, "±ÕÈû"))), "±ÕÈû", QTextOption(Qt::AlignCenter));
            //¸´Ô­
            m_pPainter.drawText(Scale(QRectF(m_ptFYText, fontMetrics.size(Qt::TextSingleLine, "¸´Ô­"))), "¸´Ô­", QTextOption(Qt::AlignCenter));
            //ÊÂ¹Ê
            m_pPainter.drawText(Scale(QRectF(m_ptSGText, fontMetrics.size(Qt::TextSingleLine, "ÊÂ¹Ê"))), "ÊÂ¹Ê", QTextOption(Qt::AlignCenter));
        }

        bool StaSemiAutoBlock::Contains(const QPoint& ptPos)
        {
            return m_rcBSBtn.contains(ptPos) || m_rcFYBtn.contains(ptPos) || m_rcSGBtn.contains(ptPos);
        }

        bool StaSemiAutoBlock::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                if (m_rcBSBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x01;
                    return true;
                }
                else if (m_rcFYBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x02;
                    return true;
                }
                else if (m_rcSGBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x04;
                    return true;
                }
            }
            return false;
        }

        void StaSemiAutoBlock::InitClickEvent()
        {
            m_mapClickEvent.insert(CTCWindows::FunType::FunBtn, [&]() {
                OnButtonClick(this);
            });
        }

        void StaSemiAutoBlock::SetBtnState()
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                m_nBtnState = m_nSelectBtnType;
                m_nFirstBtnType = 5;
                switch (m_nBtnState)
                {
                case 0x01: CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Blockage);    break;
                case 0x02: CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Restoration); break;
                case 0x04: CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Accident);    break;
                default: break;
                }
            }
        }

        void StaSemiAutoBlock::getArrowColor()
        {
            QColor* cColor1 = &m_cColor1;
            QColor* cColor2 = &m_cColor2;
            if (m_bUpDown) {
                cColor2 = &m_cColor1;
                cColor1 = &m_cColor2;
            }
            
            switch (m_nArrowState & 0x0f) {
            case 0x01: *cColor1 = COLOR_LIGHT_YELLOW; break;
            case 0x02: *cColor1 = COLOR_LIGHT_GREEN;  break;
            case 0x04: *cColor1 = COLOR_LIGHT_RED;    break;
            case 0x08: *cColor1 = COLOR_LIGHT_RED;    break;
            default:   *cColor1 = COLOR_LIGHT_BLACK;  break;
            }
            switch (m_nArrowState & 0xf0) {
            case 0x10: *cColor2 = COLOR_LIGHT_YELLOW; break; 
            case 0x20: *cColor2 = COLOR_LIGHT_GREEN;  break;
            case 0x40: *cColor2 = COLOR_LIGHT_RED;    break;
            case 0x80: *cColor2 = COLOR_LIGHT_RED;    break;
            default:   *cColor2 = COLOR_LIGHT_BLACK;  break;
            }
        }

        void StaSemiAutoBlock::OrderClear()
        {
            BtnStateReset();
        }

        void StaSemiAutoBlock::setVollover(const QPointF& ptBase)
        {

        }

        void StaSemiAutoBlock::ResetDevState()
        {

        }
    }
}