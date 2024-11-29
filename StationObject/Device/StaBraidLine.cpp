#include "StaBraidLine.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        StaBraidLine::StaBraidLine(QObject* pParent)
            : StaDistant(pParent)
        {
            m_mapAttribute.insert("TYFCBSD_rect", [&](const QString& strElement) { m_rcAllowLamp = QStringToQRectF(strElement); });
            m_mapAttribute.insert("TYFCText_rect", [&](const QString& strElement) { m_rcAllowText = QStringToQRectF(strElement); });
        }

        StaBraidLine::~StaBraidLine()
        {

        }

        void StaBraidLine::Draw(bool isMulti)
        {
            return StaDistant::Draw(isMulti);
        }

        void StaBraidLine::DrawLight()
        {
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));
            m_pPainter.setBrush((m_nState & 0x04) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);

            //������
            m_pPainter.setBrush((m_nState & 0x01) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcAllowLamp));
        }

        void StaBraidLine::DrawText()
        {
            QFont font;
            font.setFamily("΢���ź�");
            font.setPixelSize(Scale(m_nFontSize));//�ֺ�

            m_pPainter.setFont(font);//��������
            m_pPainter.setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            //������
            m_pPainter.drawText(Scale(QRectF(m_rcAllowText.topLeft(), fontMetrics.size(Qt::TextSingleLine, "������"))), "������", QTextOption(Qt::AlignCenter));
        }

        void StaBraidLine::setVollover(const QPointF& ptBase)
        {

        }

        void StaBraidLine::ResetDevState()
        {

        }
    }
}