#include "StaAutoBlock.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaAutoBlock::StaAutoBlock(QObject* pParent) : DeviceArrow(m_mapAttribute)
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
                m_rcFZLight.setWidth(13);
                m_rcFZLight.setHeight(13);
            });

            m_mapAttribute.insert("ZFZ_Text", [&](const QString& strElement) { m_ptZFZText = QStringToQPoint(strElement); });
            m_mapAttribute.insert("FFZ_Text", [&](const QString& strElement) { m_ptJCFZText = QStringToQPoint(strElement); });
            m_mapAttribute.insert("JFZ_Text", [&](const QString& strElement) { m_ptFCFZText = QStringToQPoint(strElement); });
            m_mapAttribute.insert("FZ_Text", [&](const QString& strElement) { m_ptFZText = QStringToQPoint(strElement); });

            m_mapAttribute.insert("LampNum", [&](const QString& strElement) {
                m_nLampNum = strElement.toUInt();
                for (int i = 0; i < m_nLampNum; i++) {
                    m_vecStaLeaveTrack.append(StaLeaveTrack());
                    m_vecStaLeaveTrack[i].m_nIndex = i;
                }
            });

            m_mapAttribute.insert("isLeave", [&](const QString& strElement) { m_bLeave = strElement.toUInt(); });
            m_mapAttribute.insert("LampCenter0", [&](const QString& strElement) { m_ptLampCenter = QStringToQPoint(strElement); });
            m_mapAttribute.insert("InterUsed", [&](const QString& strElement) { m_ptInterUsed = QStringToQPoint(strElement); });
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

        void StaAutoBlock::InitDeviceAttribute()
        {
            if (m_rcFZLight.x() < p11.x()) {
                p11.setX(p11.x() + 48);
                p12.setX(p12.x() + 48);
                p13.setX(p13.x() + 48);
                p14.setX(p14.x() + 48);
                p15.setX(p15.x() + 48);
                p16.setX(p16.x() + 48);
                p17.setX(p17.x() + 48);
            }
            else if (m_rcFZLight.x() > p11.x()) {
                p21.setX(p21.x() - 48);
                p22.setX(p22.x() - 48);
                p23.setX(p23.x() - 48);
                p24.setX(p24.x() - 48);
                p25.setX(p25.x() - 48);
                p26.setX(p26.x() - 48);
                p27.setX(p27.x() - 48);
            }
        }

        void StaAutoBlock::Draw(const bool& isMulti)
        {
            m_bShowName = MainStation()->IsVisible(VisibleDev::direction);
            DrawArrow(m_pPainter);
            DrawButton(m_pPainter, Scale(m_rcZFZBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x01);
            DrawButton(m_pPainter, Scale(m_rcJCFZBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x02);
            DrawButton(m_pPainter, Scale(m_rcFCFZBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x04);
            
            if (MainStation()->IsVisible(VisibleDev::sectionName)) {
                for (StaLeaveTrack& track : m_vecStaLeaveTrack) {
                    DrawLeaveTrack(track);
                }
            }

            return StaDistant::Draw(isMulti);
        }

        void StaAutoBlock::DrawLeaveTrack(const StaLeaveTrack& track)
        {
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            QFontMetrics  fontMetrics(font);
            m_pPainter.setFont(font);//设置字体

            //绘制股道名称
            m_pPainter.setPen(Qt::white);
            m_pPainter.drawText(Scale(QRect(track.m_ptName, fontMetrics.size(Qt::TextSingleLine, track.m_strName))), track.m_strName, QTextOption(Qt::AlignCenter));
            //绘制股道
            m_pPainter.setPen(QPen(getTrackColor(track.m_nIndex), Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            m_pPainter.drawLine(Scale(QPoint(track.m_rcTrack.left(), m_ptInterUsed.y())),
                Scale(QPoint(track.m_rcTrack.right(), m_ptInterUsed.y())));
            //绘制绝缘节
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.drawLine(Scale(track.m_rcTrack.topLeft()), Scale(track.m_rcTrack.bottomLeft())); //绘制轨道区段左侧绝缘节
            m_pPainter.drawLine(Scale(track.m_rcTrack.topRight()), Scale(track.m_rcTrack.bottomRight())); //绘制轨道区段右侧绝缘节
        }

        void StaAutoBlock::DrawLight()
        {
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));

            //辅助闭塞灯
            QColor color;
            if (m_nState & 0x40) {
                color = COLOR_LIGHT_WHITE;
            }
            else if (m_nState & 0x80) {
                color = m_bElapsed ? COLOR_LIGHT_WHITE : COLOR_LIGHT_BLACK;
            }
            m_pPainter.setBrush(color);
            m_pPainter.drawEllipse(Scale(m_rcFZLight));
        }

        void StaAutoBlock::DrawText()
        {
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            //总辅助
            m_pPainter.drawText(Scale(QRect(m_ptZFZText, fontMetrics.size(Qt::TextSingleLine, "总辅助"))), "总辅助", QTextOption(Qt::AlignCenter));
            //接车辅助
            m_pPainter.drawText(Scale(QRect(m_ptJCFZText, fontMetrics.size(Qt::TextSingleLine, "接车辅助"))), "接车辅助", QTextOption(Qt::AlignCenter));
            //发车辅助
            m_pPainter.drawText(Scale(QRect(m_ptFCFZText, fontMetrics.size(Qt::TextSingleLine, "发车辅助"))), "发车辅助", QTextOption(Qt::AlignCenter));
            //发车辅助
            m_pPainter.drawText(Scale(QRect(m_ptFZText, fontMetrics.size(Qt::TextSingleLine, "辅助办理"))), "辅助办理", QTextOption(Qt::AlignCenter));
        }

        bool StaAutoBlock::Contains(const QPoint& ptPos)
        {
            return m_rcZFZBtn.contains(ptPos) || m_rcJCFZBtn.contains(ptPos) || m_rcFCFZBtn.contains(ptPos);
        }

        bool StaAutoBlock::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                if (m_rcZFZBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x01;
                    return true;
                }
                else if (m_rcJCFZBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x02;
                    return true;
                }
                else if (m_rcFCFZBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x04;
                    return true;
                }
            }
            return false;
        }

        void StaAutoBlock::InitClickEvent()
        {
            m_mapClickEvent.insert(CTCWindows::FunType::FunBtn, [&]() {
                OnButtonClick(this);
            });
        }

        void StaAutoBlock::SetBtnState()
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                m_nBtnState = m_nSelectBtnType;
                m_nFirstBtnType = 5;
                switch (m_nBtnState)
                {
                case 0x01 : CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::TotalAux);       break;
                case 0x02: CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::PickUpAux);       break;
                case 0x04: CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::DepartureAux);    break;
                default: break;
                }
            }
        }

        QColor StaAutoBlock::getTrackColor(int nIndex)
        {
            if (m_vecStaLeaveTrack.at(nIndex).m_nState == 0x02) {
                return COLOR_TRACK_RED;
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

        void StaAutoBlock::setVollover(const QPoint& ptBase)
        {

        }

        void StaAutoBlock::ResetDevState()
        {

        }

        void StaAutoBlock::setLeaveTrackState(int nState)
        {
            for (StaLeaveTrack& pTrack : m_vecStaLeaveTrack) {
                pTrack.m_nState = nState & 0x03;
                nState = nState >> 2;
            }
        }
    }
}