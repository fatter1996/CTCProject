#include "StaScattered.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaScattered::StaScattered(QObject* pParent)
            : StaDistant(pParent)
        {
            
        }

        StaScattered::~StaScattered()
        {

        }

        void StaScattered::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute.insert(m_strType, mapAttrFun);
            m_mapAttribute[m_strType].insert("mRsbjRect", [](DeviceBase* pDevice, const QString& strElement) {
                StaScattered* pStaScattered = dynamic_cast<StaScattered*>(pDevice);
                pStaScattered->m_rcFuseAlarmText = QStringToQRectF(strElement);
                pStaScattered->m_rcFuseAlarmLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcFuseAlarmText);
            });

            m_mapAttribute[m_strType].insert("mZFdyRect", [](DeviceBase* pDevice, const QString& strElement) {
                StaScattered* pStaScattered = dynamic_cast<StaScattered*>(pDevice);
                pStaScattered->m_rcMainAuxPowerText = QStringToQRectF(strElement);
                pStaScattered->m_rcMainAuxPowerLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcMainAuxPowerText);
            });

            m_mapAttribute[m_strType].insert("mGdtdRect", [](DeviceBase* pDevice, const QString& strElement) {
                StaScattered* pStaScattered = dynamic_cast<StaScattered*>(pDevice);
                if (pStaScattered->nIndex == 0) {
                    pStaScattered->m_rcTrackOutageSText = QStringToQRectF(strElement);
                    pStaScattered->m_rcTrackOutageSLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcTrackOutageSText);
                }
                else if (pStaScattered->nIndex == 1) {
                    pStaScattered->m_rcTrackOutageXText = QStringToQRectF(strElement);
                    pStaScattered->m_rcTrackOutageXLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcTrackOutageXText);
                }
            });

            m_mapAttribute[m_strType].insert("mYdzsbRect", [](DeviceBase* pDevice, const QString& strElement) {
                StaScattered* pStaScattered = dynamic_cast<StaScattered*>(pDevice);
                if (pStaScattered->nIndex == 0) {
                    pStaScattered->m_rcTotalLockSText = QStringToQRectF(strElement);
                    pStaScattered->m_rcTotalLockSLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcTotalLockSText);
                }
                else if (pStaScattered->nIndex == 1) {
                    pStaScattered->m_rcTotalLockXText = QStringToQRectF(strElement);
                    pStaScattered->m_rcTotalLockXLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcTotalLockXText);
                }
            });

            m_mapAttribute[m_strType].insert("mDsdsRect", [](DeviceBase* pDevice, const QString& strElement) {
                StaScattered* pStaScattered = dynamic_cast<StaScattered*>(pDevice);
                if (pStaScattered->nIndex == 0) {
                    pStaScattered->m_rcFilamentBreakSText = QStringToQRectF(strElement);
                    pStaScattered->m_rcFilamentBreakSLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcFilamentBreakSText);
                }
                else if (pStaScattered->nIndex == 1) {
                    pStaScattered->m_rcFilamentBreakXText = QStringToQRectF(strElement);
                    pStaScattered->m_rcFilamentBreakXLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcFilamentBreakXText);
                }
            });

            m_mapAttribute[m_strType].insert("mJcbjRect", [](DeviceBase* pDevice, const QString& strElement) {
                StaScattered* pStaScattered = dynamic_cast<StaScattered*>(pDevice);
                pStaScattered->m_rcCrowdAlarmText = QStringToQRectF(strElement);
                pStaScattered->m_rcCrowdAlarmLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcCrowdAlarmText);
            });

            m_mapAttribute[m_strType].insert("mDmhbjRect", [](DeviceBase* pDevice, const QString& strElement) {
                StaScattered* pStaScattered = dynamic_cast<StaScattered*>(pDevice);
                pStaScattered->m_rcCodeAlarmText = QStringToQRectF(strElement);
                pStaScattered->m_rcCodeAlarmLamp = pStaScattered->getLampRectByTextRect(pStaScattered->m_rcCodeAlarmText);
            });

            m_mapAttribute[m_strType].insert("SGDTDJ", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaScattered*>(pDevice)->m_strSGDTDJ = strElement; });
            m_mapAttribute[m_strType].insert("SGDTDJ_bus", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaScattered*>(pDevice)->m_nSGDTDJ = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("XGDTDJ", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaScattered*>(pDevice)->m_strXGDTDJ = strElement; });
            m_mapAttribute[m_strType].insert("XGDTDJ_bus", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaScattered*>(pDevice)->m_nXGDTDJ = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("trackType", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaScattered*>(pDevice)->m_strTrackType = strElement; });
            return StaDistant::InitAttributeMap();
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