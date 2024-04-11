#include "StaAutoBlock.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaAutoBlock::StaAutoBlock(QObject* parent) : DeviceArrow(m_mapAttribute)
        {
            m_mapAttribute.insert("ZFZ1_rect", [&](const QString& strElement) {
                m_rcZFZBtn = QStringToQRect(strElement);
                m_rcZFZBtn.setWidth(17);
                m_rcZFZBtn.setHeight(17);
            });

            m_mapAttribute.insert("FFZ1_rect", [&](const QString& strElement) {
                m_rcFCFZBtn = QStringToQRect(strElement);
                m_rcFCFZBtn.setWidth(17);
                m_rcFCFZBtn.setHeight(17);
            });

            m_mapAttribute.insert("JFZ1_rect", [&](const QString& strElement) {
                m_rcJCFZBtn = QStringToQRect(strElement);
                m_rcJCFZBtn.setWidth(17);
                m_rcJCFZBtn.setHeight(17);
            });

            m_mapAttribute.insert("FZBSD_rect", [&](const QString& strElement) {
                m_rcFZLight = QStringToQRect(strElement);
                m_rcFZLight.setWidth(17);
                m_rcFZLight.setHeight(17);
            });

            m_mapAttribute.insert("ZFZ_Text", [&](const QString& strElement) { m_ptZFZText = QStringToQPoint(strElement); });
            m_mapAttribute.insert("FFZ_Text", [&](const QString& strElement) { m_ptJCFZText = QStringToQPoint(strElement); });
            m_mapAttribute.insert("JFZ_Text", [&](const QString& strElement) { m_ptFCFZText = QStringToQPoint(strElement); });
            m_mapAttribute.insert("FZ_Text", [&](const QString& strElement) { m_ptFZText = QStringToQPoint(strElement); });

            m_mapAttribute.insert("LampNum", [&](const QString& strElement) {
                m_nLempNum = strElement.toUInt();
                for (int i = 0; i < m_nLempNum; i++) {
                    m_vecStaLeaveTrack.append(StaLeaveTrack());
                    m_vecStaLeaveTrack[i].m_nIndex = i;
                }
            });

            m_mapAttribute.insert("isLeave", [&](const QString& strElement) { m_bLeave = strElement.toUInt(); });
            m_mapAttribute.insert("LampCenter0", [&](const QString& strElement) { m_ptLempCenter = QStringToQPoint(strElement); });
            m_mapAttribute.insert("ZDBS_Type", [&](const QString& strElement) { m_strAutoBlockType = strElement; });
            
            m_mapAttribute.insert("trackName", [&](const QString& strElement) {
                if (m_vecStaLeaveTrack.size() > nIndex) {
                    m_vecStaLeaveTrack[nIndex].m_strName = strElement;
                }
            });

            m_mapAttribute.insert("trackNamePoint", [&](const QString& strElement) {
                if (m_vecStaLeaveTrack.size() > nIndex) {
                    m_vecStaLeaveTrack[nIndex].m_ptName = QStringToQPoint(strElement);
                }
            });

            m_mapAttribute.insert("CommLeaveTrack", [&](const QString& strElement) {
                if (m_vecStaLeaveTrack.size() > nIndex) {
                    m_vecStaLeaveTrack[nIndex].m_rcTrack = QStringToQRect(strElement);
                }
            });

            m_mapAttribute.insert("ZFZ_JS", [&](const QString& strElement) { m_ptZFZ_JS = QStringToQPoint(strElement); });
            m_mapAttribute.insert("FFZ_JS", [&](const QString& strElement) { m_ptJCFZ_JS = QStringToQPoint(strElement); });
            m_mapAttribute.insert("JFZ_JS", [&](const QString& strElement) { m_ptFCFZ_JS = QStringToQPoint(strElement); });
        }

        StaAutoBlock::~StaAutoBlock()
        {

        }

        bool StaAutoBlock::eventFilter(QObject* obj, QEvent* event)
        {
            return DeviceBase::eventFilter(obj, event);
        }

        void StaAutoBlock::Draw(const bool& bElapsed, const bool& isMulti)
        {
            DrawArrow(m_pPainter);
            DrawButton(m_pPainter, bElapsed, Scale(m_rcZFZBtn), COLOR_BTN_DEEPGRAY, 1);
            DrawButton(m_pPainter, bElapsed, Scale(m_rcJCFZBtn), COLOR_BTN_DEEPGRAY, 1);
            DrawButton(m_pPainter, bElapsed, Scale(m_rcFCFZBtn), COLOR_BTN_DEEPGRAY, 1);
            
            for (StaLeaveTrack& track : m_vecStaLeaveTrack) {
                DrawLeaveTrack(bElapsed, track);
            }

            return StaDistant::Draw(bElapsed, isMulti);
        }

        void StaAutoBlock::DrawLeaveTrack(const bool& bElapsed, const StaLeaveTrack& track)
        {
            QFont font;
            font.setFamily("Î¢ÈíÑÅºÚ");
            font.setPixelSize(Scale(m_nFontSize));//×ÖºÅ
            QFontMetrics  fontMetrics(font);
            m_pPainter.setFont(font);//ÉèÖÃ×ÖÌå

            //»æÖÆ¹ÉµÀÃû³Æ
            m_pPainter.setPen(Qt::white);
            m_pPainter.drawText(Scale(QRect(track.m_ptName, fontMetrics.size(Qt::TextSingleLine, track.m_strName))), track.m_strName, QTextOption(Qt::AlignCenter));
            //»æÖÆ¹ÉµÀ
            m_pPainter.setPen(QPen(getTrackColor(bElapsed, track.m_nIndex), Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            m_pPainter.drawLine(Scale(QPoint(track.m_rcTrack.left(), m_ptLempCenter.y())),
                Scale(QPoint(track.m_rcTrack.right(), m_ptLempCenter.y())));
            //»æÖÆ¾øÔµ½Ú
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.drawLine(Scale(track.m_rcTrack.topLeft()), Scale(track.m_rcTrack.bottomLeft())); //»æÖÆ¹ìµÀÇø¶Î×ó²à¾øÔµ½Ú
            m_pPainter.drawLine(Scale(track.m_rcTrack.topRight()), Scale(track.m_rcTrack.bottomRight())); //»æÖÆ¹ìµÀÇø¶ÎÓÒ²à¾øÔµ½Ú
        }

        void StaAutoBlock::DrawLight()
        {
            m_pPainter.setRenderHint(QPainter::Antialiasing, true);
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));

            //¸¨Öú±ÕÈûµÆ
            m_pPainter.setBrush((m_nState & 0x10) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcFZLight));

            m_pPainter.setRenderHint(QPainter::Antialiasing, false);
        }

        void StaAutoBlock::DrawText()
        {
            QFont font;
            font.setFamily("Î¢ÈíÑÅºÚ");
            font.setPixelSize(Scale(m_nFontSize));//×ÖºÅ
            m_pPainter.setFont(font);//ÉèÖÃ×ÖÌå
            m_pPainter.setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            //×Ü¸¨Öú
            m_pPainter.drawText(Scale(QRect(m_ptZFZText, fontMetrics.size(Qt::TextSingleLine, "×Ü¸¨Öú"))), "×Ü¸¨Öú", QTextOption(Qt::AlignCenter));
            //½Ó³µ¸¨Öú
            m_pPainter.drawText(Scale(QRect(m_ptJCFZText, fontMetrics.size(Qt::TextSingleLine, "½Ó³µ¸¨Öú"))), "½Ó³µ¸¨Öú", QTextOption(Qt::AlignCenter));
            //·¢³µ¸¨Öú
            m_pPainter.drawText(Scale(QRect(m_ptFCFZText, fontMetrics.size(Qt::TextSingleLine, "·¢³µ¸¨Öú"))), "·¢³µ¸¨Öú", QTextOption(Qt::AlignCenter));
            //·¢³µ¸¨Öú
            m_pPainter.drawText(Scale(QRect(m_ptFZText, fontMetrics.size(Qt::TextSingleLine, "¸¨Öú°ìÀí"))), "¸¨Öú°ìÀí", QTextOption(Qt::AlignCenter));
        }

        void StaAutoBlock::setLeaveTrackState(int nState)
        {
            for (StaLeaveTrack& pTrack : m_vecStaLeaveTrack) {
                pTrack.m_nState = nState & 0x03;
                nState = nState >> 2;
            }
        }

        QColor StaAutoBlock::getTrackColor(const bool& bElapsed, int nIndex)
        {
            if (bElapsed){
                if (m_vecStaLeaveTrack.at(nIndex).m_nState == 0x02) {
                    return COLOR_TRACK_RED;
                }
                else {
                    return COLOR_TRACK_BLUE;
                }
            }
            else {
                return COLOR_TRACK_BLUE;
            }
        }

        void StaAutoBlock::getArrowColor()
        {
            switch (m_nArrowState & 0x0f) {
            case 0x01: m_cColor1 = COLOR_LIGHT_RED;    break;
            case 0x02: m_cColor1 = COLOR_LIGHT_GREEN;  break;
            case 0x04: m_cColor1 = COLOR_LIGHT_YELLOW; break;
            case 0x08: m_cColor1 = COLOR_LIGHT_RED;    break;
            default:   m_cColor1 = COLOR_LIGHT_BLACK;  break;
            }
            switch (m_nArrowState & 0xf0) {
            case 0x10: m_cColor2 = COLOR_LIGHT_RED;    break;
            case 0x20: m_cColor2 = COLOR_LIGHT_GREEN;  break;
            case 0x40: m_cColor2 = COLOR_LIGHT_YELLOW; break;
            case 0x80: m_cColor2 = COLOR_LIGHT_RED;    break;
            default:   m_cColor2 = COLOR_LIGHT_BLACK;  break;
            }
        }

        void StaAutoBlock::OnButtonClick()
        {

        }

        void StaAutoBlock::setVollover(const QPoint& ptBase)
        {

        }

        void StaAutoBlock::ResetDevState()
        {

        }
    }
}