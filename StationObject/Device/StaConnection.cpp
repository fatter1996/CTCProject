#include "StaConnection.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        StaConnection::StaConnection(QObject* pParent) 
            : DeviceArrow(m_mapAttribute), StaDistant(pParent)
        {
            m_mapAttribute.insert("flag", [&](const QString& strElement) { m_nFlag = strElement.toInt(); });
            m_mapAttribute.insert("Pxh", [&](const QString& strElement) { m_ptSignal = QStringToQPointF(strElement); });
            m_mapAttribute.insert("Pjj", [&](const QString& strElement) { m_ptNear = QStringToQPointF(strElement); });
            m_mapAttribute.insert("Pzc", [&](const QString& strElement) { m_ptTakeLook = QStringToQPointF(strElement); });
            m_mapAttribute.insert("Pyx", [&](const QString& strElement) { m_ptAllow = QStringToQPointF(strElement); });
            m_mapAttribute.insert("Pbc", [&](const QString& strElement) { m_ptFileld = QStringToQPointF(strElement); });
            m_mapAttribute.insert("Plc", [&](const QString& strElement) { m_ptAdjFileld = QStringToQPointF(strElement); });
            m_mapAttribute.insert("JJbsdRect", [&](const QString& strElement) { m_rcNearLight = QStringToQRectF(strElement); });
            m_mapAttribute.insert("BCbsdRect", [&](const QString& strElement) { m_rcFileldLight = QStringToQRectF(strElement); });
            m_mapAttribute.insert("LCbsdRect", [&](const QString& strElement) { m_rcAdjFileldLight = QStringToQRectF(strElement); });
            m_mapAttribute.insert("YXANRect1", [&](const QString& strElement) { 
                m_rcAllowBtn = QStringToQRectF(strElement);
                m_rcAllowBtn.setWidth(17);
                m_rcAllowBtn.setHeight(17);
            });
            m_mapAttribute.insert("CL_Type", [&](const QString& strElement) { m_strCLType = strElement; });
        }

        StaConnection::~StaConnection()
        {

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
            //�ӽ�
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));
            m_pPainter.setBrush((m_nState & 0x04) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcNearLight));
            if (m_strCLType.contains("_DC")) {
                //����
                m_pPainter.setBrush((m_nState & 0x40) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
                m_pPainter.drawEllipse(Scale(m_rcFileldLight));
                //�ڳ�
                m_pPainter.setBrush((m_nState & 0x80) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
                m_pPainter.drawEllipse(Scale(m_rcAdjFileldLight));
            }
        }

        void StaConnection::DrawText()
        {
            QFont font;
            font.setFamily("΢���ź�");
            font.setPixelSize(Scale(m_nFontSize));//�ֺ�

            m_pPainter.setFont(font);//��������
            m_pPainter.setPen(Qt::white);

            QFontMetrics  fontMetrics(font);
            
            //�ӽ�
            m_pPainter.drawText(Scale(QRectF(m_ptNear, fontMetrics.size(Qt::TextSingleLine, "�ӽ�"))), "�ӽ�", QTextOption(Qt::AlignCenter));
            
            if (m_bUpDown) {
                //�ź�
                m_pPainter.drawText(Scale(QRectF(m_ptSignal, fontMetrics.size(Qt::TextSingleLine, "�ź�"))), "�ź�", QTextOption(Qt::AlignCenter));
                //�ղ�  
                m_pPainter.drawText(Scale(QRectF(m_ptTakeLook, fontMetrics.size(Qt::TextSingleLine, "�ղ�"))), "�ղ�", QTextOption(Qt::AlignCenter));
            }
            else {
                //�ź�
                m_pPainter.drawText(Scale(QRectF(m_ptTakeLook, fontMetrics.size(Qt::TextSingleLine, "�ź�"))), "�ź�", QTextOption(Qt::AlignCenter));
                //�ղ�  
                m_pPainter.drawText(Scale(QRectF(m_ptSignal, fontMetrics.size(Qt::TextSingleLine, "�ղ�"))), "�ղ�", QTextOption(Qt::AlignCenter));
            }
            
            if (m_strCLType.contains("_DC")) {
                //����
                m_pPainter.drawText(Scale(QRectF(m_ptAllow, fontMetrics.size(Qt::TextSingleLine, "����ٶ�����"))), "����ٶ�����", QTextOption(Qt::AlignCenter));
                //����
                m_pPainter.drawText(Scale(QRectF(m_ptFileld, fontMetrics.size(Qt::TextSingleLine, "����"))), "����", QTextOption(Qt::AlignCenter));
                //�ڳ�
                m_pPainter.drawText(Scale(QRectF(m_ptAdjFileld, fontMetrics.size(Qt::TextSingleLine, "�ڳ�"))), "�ڳ�", QTextOption(Qt::AlignCenter));
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
            m_mapClickEvent.insert(CTCWindows::FunType::FunBtn, [&]() {
                OnButtonClick(this);
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