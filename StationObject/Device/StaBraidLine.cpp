#include "StaBraidLine.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        StaBraidLine::StaBraidLine(QObject* pParent)
        {
            m_mapAttribute.insert("TYFCBSD_rect", [&](const QString& strElement) { m_rcAllowLamp = QStringToQRect(strElement); });
            m_mapAttribute.insert("TYFCText_rect", [&](const QString& strElement) { m_rcAllowText = QStringToQRect(strElement); });
        }

        StaBraidLine::~StaBraidLine()
        {

        }

        void StaBraidLine::Draw(const bool& isMulti)
        {
            return StaDistant::Draw(isMulti);
        }

        void StaBraidLine::DrawLight()
        {
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));
            m_pPainter.setBrush((m_nState & 0x04) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);

            //允许发车
            m_pPainter.setBrush((m_nState & 0x01) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcAllowLamp));
        }

        void StaBraidLine::DrawText()
        {
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号

            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            //允许发车
            m_pPainter.drawText(Scale(QRect(m_rcAllowText.topLeft(), fontMetrics.size(Qt::TextSingleLine, "允许发车"))), "允许发车", QTextOption(Qt::AlignCenter));
        }

        void StaBraidLine::setVollover(const QPoint& ptBase)
        {

        }

        void StaBraidLine::ResetDevState()
        {

        }
    }
}