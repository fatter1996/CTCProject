#include "StaHump.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        StaHump::StaHump(QObject* parent)
        {
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p3", [&](const QString& strElement) { p3 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p4", [&](const QString& strElement) { p4 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p5", [&](const QString& strElement) { p5 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p6", [&](const QString& strElement) { p6 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p7", [&](const QString& strElement) { p7 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p8", [&](const QString& strElement) { p8 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p9", [&](const QString& strElement) { p9 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("Pqd", [&](const QString& strElement) { m_ptCutOff = QStringToQPoint(strElement); });
            m_mapAttribute.insert("Pyxts", [&](const QString& strElement) { m_ptAllow = QStringToQPoint(strElement); });
            m_mapAttribute.insert("Pdczc", [&](const QString& strElement) { m_ptTakeLook = QStringToQPoint(strElement); });

            m_mapAttribute.insert("anRect1", [&](const QString& strElement) {
                m_rcButton = QStringToQRect(strElement);
                m_rcButton.setWidth(17);
                m_rcButton.setHeight(17);
            });

            m_mapAttribute.insert("yxtsRect", [&](const QString& strElement) { m_rcAllow = QStringToQRect(strElement); });
            m_mapAttribute.insert("dczcRect", [&](const QString& strElement) { m_rcTakeLook = QStringToQRect(strElement); });
            m_mapAttribute.insert("qdRect", [&](const QString& strElement) { m_rcCutOff = QStringToQRect(strElement); });
            m_mapAttribute.insert("xhRect", [&](const QString& strElement) { m_rcSignal = QStringToQRect(strElement); });
        }

        StaHump::~StaHump()
        {

        }

        bool StaHump::eventFilter(QObject* obj, QEvent* event)
        {
            return DeviceBase::eventFilter(obj, event);
        }

        void StaHump::Draw(const bool& bElapsed, const bool& isMulti)
        {
            if (!m_pPainter)
                return;

            DrawHump();
            DrawButton(m_pPainter, bElapsed, Scale(m_rcButton), COLOR_BTN_DEEPGRAY);

            return StaDistant::Draw(bElapsed, isMulti);
        }

        void StaHump::DrawHump()
        {
            m_pPainter->setPen(QPen(COLOR_TRACK_BLUE, Scale(TRACK_WIDTH)));
            m_pPainter->drawLine(Scale(p1), Scale(QPoint(p2.x() - TRACK_WIDTH, p2.y())));
                 
            m_pPainter->setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter->setBrush(COLOR_TRACK_BLUE);
            m_pPainter->drawPolygon(QPolygon() << Scale(p3) << Scale(p4) << Scale(p5));
            m_pPainter->drawLine(Scale(p6), Scale(p7));
            m_pPainter->drawLine(Scale(p8), Scale(p9));
        }

        void StaHump::DrawLight()
        {
            m_pPainter->setRenderHint(QPainter::Antialiasing, true);
            m_pPainter->setPen(QPen(COLOR_LIGHT_WHITE, 1));

            //«–∂œÕ∆ÀÕ
            m_pPainter->setBrush((m_nState & 0x10) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter->drawEllipse(Scale(m_rcCutOff));
            //’’≤È
            m_pPainter->setBrush((m_nState & 0x20) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter->drawEllipse(Scale(m_rcTakeLook));
            //‘ –ÌÕ∆ÀÕ
            m_pPainter->setBrush((m_nState & 0x40) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter->drawEllipse(Scale(m_rcAllow));
            //–≈∫≈
            m_pPainter->setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter->setBrush((m_nState & 0x80) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter->drawEllipse(Scale(m_rcSignal));

            m_pPainter->setRenderHint(QPainter::Antialiasing, false);
        }

        void StaHump::DrawText()
        {
            QFont font;
            font.setFamily("Œ¢»Ì—≈∫⁄");
            font.setPixelSize(Scale(m_nFontSize));//◊÷∫≈

            m_pPainter->setFont(font);//…Ë÷√◊÷ÃÂ
            m_pPainter->setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            //«–∂œÕ∆ÀÕ
            m_pPainter->drawText(Scale(QRect(m_ptCutOff, fontMetrics.size(Qt::TextSingleLine, "«–∂œÕ∆ÀÕ"))), "«–∂œÕ∆ÀÕ", QTextOption(Qt::AlignCenter));
            //‘ –ÌÕ∆ÀÕ
            m_pPainter->drawText(Scale(QRect(m_ptAllow, fontMetrics.size(Qt::TextSingleLine, "‘ –ÌÕ∆ÀÕ"))), "‘ –ÌÕ∆ÀÕ", QTextOption(Qt::AlignCenter));
            //’’≤È  
            m_pPainter->drawText(Scale(QRect(m_ptTakeLook, fontMetrics.size(Qt::TextSingleLine, "’’≤È"))), "’’≤È", QTextOption(Qt::AlignCenter));
        }

        void StaHump::OnButtonClick()
        {

        }

        void StaHump::setVollover(const QPoint& ptBase)
        {

        }

        void StaHump::ResetDevState()
        {

        }
    }
}