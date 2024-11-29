#include "StaHump.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        StaHump::StaHump(QObject* pParent)
            : StaDistant(pParent)
        {
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p3", [&](const QString& strElement) { p3 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p4", [&](const QString& strElement) { p4 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p5", [&](const QString& strElement) { p5 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p6", [&](const QString& strElement) { p6 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p7", [&](const QString& strElement) { p7 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p8", [&](const QString& strElement) { p8 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p9", [&](const QString& strElement) { p9 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("Pqd", [&](const QString& strElement) { m_ptCutOff = QStringToQPointF(strElement); });
            m_mapAttribute.insert("Pyxts", [&](const QString& strElement) { m_ptAllow = QStringToQPointF(strElement); });
            m_mapAttribute.insert("Pdczc", [&](const QString& strElement) { m_ptTakeLook = QStringToQPointF(strElement); });

            m_mapAttribute.insert("anRect1", [&](const QString& strElement) {
                m_rcButton = QStringToQRectF(strElement);
                m_rcButton.setWidth(17);
                m_rcButton.setHeight(17);
            });

            m_mapAttribute.insert("yxtsRect", [&](const QString& strElement) { m_rcAllow = QStringToQRectF(strElement); });
            m_mapAttribute.insert("dczcRect", [&](const QString& strElement) { m_rcTakeLook = QStringToQRectF(strElement); });
            m_mapAttribute.insert("qdRect", [&](const QString& strElement) { m_rcCutOff = QStringToQRectF(strElement); });
            m_mapAttribute.insert("xhRect", [&](const QString& strElement) { m_rcSignal = QStringToQRectF(strElement); });
        }

        StaHump::~StaHump()
        {

        }

        void StaHump::Draw(bool isMulti)
        {
            DrawHump();
            if (m_bMainStation) {
                DrawButton(m_pPainter, Scale(m_rcButton), COLOR_BTN_DEEPGRAY, m_nBtnState);
            }
            return StaDistant::Draw(isMulti);
        }

        void StaHump::DrawHump()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, Scale(TRACK_WIDTH)));
            m_pPainter.drawLine(Scale(p1), Scale(QPointF(p2.x() - TRACK_WIDTH, p2.y())));
                 
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.setBrush(COLOR_TRACK_BLUE);
            m_pPainter.drawPolygon(QPolygonF() << Scale(p3) << Scale(p4) << Scale(p5));
            m_pPainter.drawLine(Scale(p6), Scale(p7));
            m_pPainter.drawLine(Scale(p8), Scale(p9));
        }

        void StaHump::DrawLight()
        {
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));

            //切断推送
            m_pPainter.setBrush((m_nState & 0x10) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcCutOff));
            //照查
            m_pPainter.setBrush((m_nState & 0x20) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcTakeLook));
            //允许推送
            m_pPainter.setBrush((m_nState & 0x40) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcAllow));
            //信号
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.setBrush((m_nState & 0x80) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcSignal));
        }

        void StaHump::DrawText()
        {
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号

            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            //切断推送
            m_pPainter.drawText(Scale(QRectF(m_ptCutOff, fontMetrics.size(Qt::TextSingleLine, "切断推送"))), "切断推送", QTextOption(Qt::AlignCenter));
            //允许推送
            m_pPainter.drawText(Scale(QRectF(m_ptAllow, fontMetrics.size(Qt::TextSingleLine, "允许推送"))), "允许推送", QTextOption(Qt::AlignCenter));
            //照查  
            m_pPainter.drawText(Scale(QRectF(m_ptTakeLook, fontMetrics.size(Qt::TextSingleLine, "照查"))), "照查", QTextOption(Qt::AlignCenter));
        }

        bool StaHump::Contains(const QPoint& ptPos)
        {
            return m_rcButton.contains(ptPos);
        }

        bool StaHump::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                return m_rcButton.contains(ptPos);
            }
            return false;
        }

        void StaHump::InitClickEvent()
        {
            m_mapClickEvent.insert(CTCWindows::FunType::FunBtn, [&]() {
                OnButtonClick(this);
            });
        }

        void StaHump::SetBtnState()
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                m_nBtnState = 1;
                m_nFirstBtnType = 5;
                CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Hump);
            }
        }

        void StaHump::setVollover(const QPointF& ptBase)
        {

        }

        void StaHump::ResetDevState()
        {

        }
    }
}