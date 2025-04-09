#include "StaHump.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        StaHump::StaHump(QObject* pParent)
            : StaDistant(pParent)
        {
            
        }

        StaHump::~StaHump()
        {

        }

        void StaHump::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute.insert(m_strType, mapAttrFun);
            m_mapAttribute[m_strType].insert("p1", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->p1 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p2", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->p2 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p3", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->p3 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p4", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->p4 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p5", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->p5 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p6", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->p6 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p7", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->p7 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p8", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->p8 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p9", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->p9 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("Pqd", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->m_ptCutOff = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("Pyxts", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->m_ptAllow = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("Pdczc", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->m_ptTakeLook = QStringToQPointF(strElement); });

            m_mapAttribute[m_strType].insert("anRect1", [](DeviceBase* pDevice, const QString& strElement) {
                dynamic_cast<StaHump*>(pDevice)->m_rcButton = QStringToQRectF(strElement);
                dynamic_cast<StaHump*>(pDevice)->m_rcButton.setWidth(17);
                dynamic_cast<StaHump*>(pDevice)->m_rcButton.setHeight(17);
            });

            m_mapAttribute[m_strType].insert("yxtsRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->m_rcAllow = QStringToQRectF(strElement); });
            m_mapAttribute[m_strType].insert("dczcRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->m_rcTakeLook = QStringToQRectF(strElement); });
            m_mapAttribute[m_strType].insert("qdRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->m_rcCutOff = QStringToQRectF(strElement); });
            m_mapAttribute[m_strType].insert("xhRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaHump*>(pDevice)->m_rcSignal = QStringToQRectF(strElement); });
            return StaDistant::InitAttributeMap();
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
            return Scale(m_rcButton).contains(ptPos);
        }

        bool StaHump::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                return Scale(m_rcButton).contains(ptPos);
            }
            return false;
        }

        void StaHump::InitClickEvent()
        {
            m_mapClickEvent[m_strType].insert(CTCWindows::FunType::FunBtn, [](DeviceBase* pDevice) {
                dynamic_cast<StaHump*>(pDevice)->OnButtonClick();
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