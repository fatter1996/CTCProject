#include "StaScattered.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaScattered::StaScattered(QObject* pParent)
            : StaDistant(pParent)
        {
            m_mapAttribute.insert("mRsbjRect", [&](const QString& strElement) { 
                m_rcFuseAlarmText = QStringToQRectF(strElement);
                m_rcFuseAlarmLamp = getLampRectByTextRect(m_rcFuseAlarmText);
            });

            m_mapAttribute.insert("mZFdyRect", [&](const QString& strElement) {
                m_rcMainAuxPowerText = QStringToQRectF(strElement);
                m_rcMainAuxPowerLamp = getLampRectByTextRect(m_rcMainAuxPowerText);
            });

            m_mapAttribute.insert("mGdtdRect", [&](const QString& strElement) {
                if (nIndex == 0) {
                    m_rcTrackOutageSText = QStringToQRectF(strElement);
                    m_rcTrackOutageSLamp = getLampRectByTextRect(m_rcTrackOutageSText);
                }
                else if (nIndex == 1) {
                    m_rcTrackOutageXText = QStringToQRectF(strElement);
                    m_rcTrackOutageXLamp = getLampRectByTextRect(m_rcTrackOutageXText);
                }
            });

            m_mapAttribute.insert("mYdzsbRect", [&](const QString& strElement) {
                if (nIndex == 0) {
                    m_rcTotalLockSText = QStringToQRectF(strElement);
                    m_rcTotalLockSLamp = getLampRectByTextRect(m_rcTotalLockSText);
                }
                else if (nIndex == 1) {
                    m_rcTotalLockXText = QStringToQRectF(strElement);
                    m_rcTotalLockXLamp = getLampRectByTextRect(m_rcTotalLockXText);
                }
            });

            m_mapAttribute.insert("mDsdsRect", [&](const QString& strElement) {
                if (nIndex == 0) {
                    m_rcFilamentBreakSText = QStringToQRectF(strElement);
                    m_rcFilamentBreakSLamp = getLampRectByTextRect(m_rcFilamentBreakSText);
                }
                else if (nIndex == 1) {
                    m_rcFilamentBreakXText = QStringToQRectF(strElement);
                    m_rcFilamentBreakXLamp = getLampRectByTextRect(m_rcFilamentBreakXText);
                }
            });
            
            m_mapAttribute.insert("mJcbjRect", [&](const QString& strElement) {
                m_rcCrowdAlarmText = QStringToQRectF(strElement);
                m_rcCrowdAlarmLamp = getLampRectByTextRect(m_rcCrowdAlarmText);
            });
            
            m_mapAttribute.insert("mDmhbjRect", [&](const QString& strElement) {
                m_rcCodeAlarmText = QStringToQRectF(strElement);
                m_rcCodeAlarmLamp = getLampRectByTextRect(m_rcCodeAlarmText);
            });

            m_mapAttribute.insert("SGDTDJ", [&](const QString& strElement) { m_strSGDTDJ = strElement; });
            m_mapAttribute.insert("SGDTDJ_bus", [&](const QString& strElement) {m_nSGDTDJ = strElement.toInt(); });
            m_mapAttribute.insert("XGDTDJ", [&](const QString& strElement) { m_strXGDTDJ = strElement; });
            m_mapAttribute.insert("XGDTDJ_bus", [&](const QString& strElement) { m_nXGDTDJ = strElement.toInt(); });
            m_mapAttribute.insert("trackType", [&](const QString& strElement) { m_strTrackType = strElement; });
        }

        StaScattered::~StaScattered()
        {

        }

        void StaScattered::DrawLight()
        {
            return;
            if (!MainStation()->IsVisible(VisibleDev::stateLemp)) {
                return;
            }
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));
            //��˿����
            m_pPainter.setBrush((m_nState & 0x01) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcFuseAlarmLamp));
            //������Դ
            m_pPainter.drawEllipse(Scale(m_rcMainAuxPowerLamp));
            //���й��ͣ��
            m_pPainter.drawEllipse(Scale(m_rcTrackOutageSLamp));
            //���й��ͣ��
            m_pPainter.drawEllipse(Scale(m_rcTrackOutageXLamp));

            //����������
            m_pPainter.drawEllipse(Scale(m_rcTotalLockSLamp));
            //����������
            m_pPainter.drawEllipse(Scale(m_rcTotalLockXLamp));

            //���е�˿��˿
            m_pPainter.drawEllipse(Scale(m_rcFilamentBreakSLamp));
            //���е�˿��˿
            m_pPainter.drawEllipse(Scale(m_rcFilamentBreakXLamp));

            //������
            m_pPainter.drawEllipse(Scale(m_rcCrowdAlarmLamp));
            //���뻯����
            m_pPainter.drawEllipse(Scale(m_rcCodeAlarmLamp));
        }
        

        void StaScattered::DrawText()
        {
            return;
            if (!MainStation()->IsVisible(VisibleDev::stateLempName)) {
                return;
            }
            QFont font;
            font.setFamily("΢���ź�");
            font.setPixelSize(Scale(m_nFontSize));//�ֺ�

            m_pPainter.setFont(font);//��������
            m_pPainter.setPen(Qt::white);

            m_pPainter.drawText(Scale(m_rcFuseAlarmText), "��˿����", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcMainAuxPowerText), "������Դ", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcTrackOutageSText), "���й��ͣ��", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcTrackOutageXText), "���й��ͣ��", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcTotalLockSText), "����������", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcTotalLockXText), "����������", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcFilamentBreakSText), "���е�˿��˿", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcFilamentBreakXText), "���е�˿��˿", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcCrowdAlarmText), "������", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcCodeAlarmText), "���뻯����", QTextOption(Qt::AlignCenter));
        }

        void StaScattered::setVollover(const QPointF& ptBase)
        {

        }

        void StaScattered::ResetDevState()
        {

        }

        QRectF StaScattered::getLampRectByTextRect(const QRectF& rcText)
        {
            return QRect(QPoint(rcText.left() + ((rcText.width() - 12) / 2), rcText.top() + 18), QSize(13, 13));
        }
    }
}