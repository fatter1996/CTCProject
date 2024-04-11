#include "StaSemiAutoBlock.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaSemiAutoBlock::StaSemiAutoBlock(QObject* parent) : DeviceArrow(m_mapAttribute)
        {
            m_mapAttribute.insert("BS_Text", [&](const QString& strElement) { m_ptBSText = QStringToQPoint(strElement); });
            m_mapAttribute.insert("FY_Text", [&](const QString& strElement) { m_ptFYText = QStringToQPoint(strElement); });
            m_mapAttribute.insert("SG_Text", [&](const QString& strElement) { m_ptSGText = QStringToQPoint(strElement); });
            
            m_mapAttribute.insert("BS1_rect", [&](const QString& strElement) { 
                m_rcBSBtn = QStringToQRect(strElement); 
                m_rcBSBtn.setWidth(17);
                m_rcBSBtn.setHeight(17);
            });

            m_mapAttribute.insert("FY1_rect", [&](const QString& strElement) {
                m_rcFYBtn = QStringToQRect(strElement);
                m_rcFYBtn.setWidth(17);
                m_rcFYBtn.setHeight(17);
            });

            m_mapAttribute.insert("SG1_rect", [&](const QString& strElement) {
                m_rcSGBtn = QStringToQRect(strElement);
                m_rcSGBtn.setWidth(17);
                m_rcSGBtn.setHeight(17);
                });

            m_mapAttribute.insert("BSType", [&](const QString& strElement) { m_nBlockType = strElement.toUInt(); });
        }

        StaSemiAutoBlock::~StaSemiAutoBlock()
        {

        }

        bool StaSemiAutoBlock::eventFilter(QObject* obj, QEvent* event)
        {
            return DeviceBase::eventFilter(obj, event);
        }

        void StaSemiAutoBlock::Draw(const bool& bElapsed, const bool& isMulti)
        {
            DrawArrow(m_pPainter);
            DrawButton(m_pPainter, bElapsed, Scale(m_rcBSBtn), COLOR_BTN_DEEPGRAY);
            DrawButton(m_pPainter, bElapsed, Scale(m_rcFYBtn), COLOR_BTN_DEEPGRAY);
            DrawButton(m_pPainter, bElapsed, Scale(m_rcSGBtn), COLOR_BTN_DEEPGRAY);

            return StaDistant::Draw(bElapsed, isMulti);
        }

        void StaSemiAutoBlock::DrawLight()
        {
        
        }

        void StaSemiAutoBlock::DrawText()
        {
            QFont font;
            font.setFamily("Î¢ÈíÑÅºÚ");
            font.setPixelSize(Scale(m_nFontSize));//×ÖºÅ

            m_pPainter.setFont(font);//ÉèÖÃ×ÖÌå
            m_pPainter.setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            //±ÕÈû
            m_pPainter.drawText(Scale(QRect(m_ptBSText, fontMetrics.size(Qt::TextSingleLine, "±ÕÈû"))), "±ÕÈû", QTextOption(Qt::AlignCenter));
            //¸´Ô­
            m_pPainter.drawText(Scale(QRect(m_ptFYText, fontMetrics.size(Qt::TextSingleLine, "¸´Ô­"))), "¸´Ô­", QTextOption(Qt::AlignCenter));
            //ÊÂ¹Ê
            m_pPainter.drawText(Scale(QRect(m_ptSGText, fontMetrics.size(Qt::TextSingleLine, "ÊÂ¹Ê"))), "ÊÂ¹Ê", QTextOption(Qt::AlignCenter));
        }

        void StaSemiAutoBlock::getArrowColor()
        {
            QColor& cColor1 = m_cColor1;
            QColor& cColor2 = m_cColor2;
            if (m_nSX) {
                cColor2 = m_cColor1;
                cColor1 = m_cColor2;
            }
            
            switch (m_nArrowState & 0x0f) {
            case 0x01: cColor1 = COLOR_LIGHT_YELLOW; break;
            case 0x02: cColor1 = COLOR_LIGHT_GREEN;  break;
            case 0x04: cColor1 = COLOR_LIGHT_RED;    break;
            case 0x08: cColor1 = COLOR_LIGHT_RED;    break;
            default:   cColor1 = COLOR_LIGHT_BLACK;  break;
            }
            switch (m_nArrowState & 0xf0) {
            case 0x10: cColor2 = COLOR_LIGHT_YELLOW; break;
            case 0x20: cColor2 = COLOR_LIGHT_GREEN;  break;
            case 0x40: cColor2 = COLOR_LIGHT_RED;    break;
            case 0x80: cColor2 = COLOR_LIGHT_RED;    break;
            default:   cColor2 = COLOR_LIGHT_BLACK;  break;
            }
        }

        void StaSemiAutoBlock::OnButtonClick()
        {
        
        }

        void StaSemiAutoBlock::setVollover(const QPoint& ptBase)
        {

        }

        void StaSemiAutoBlock::ResetDevState()
        {

        }
    }
}