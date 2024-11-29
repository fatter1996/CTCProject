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
            //熔丝报警
            m_pPainter.setBrush((m_nState & 0x01) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcFuseAlarmLamp));
            //主副电源
            m_pPainter.drawEllipse(Scale(m_rcMainAuxPowerLamp));
            //上行轨道停电
            m_pPainter.drawEllipse(Scale(m_rcTrackOutageSLamp));
            //下行轨道停电
            m_pPainter.drawEllipse(Scale(m_rcTrackOutageXLamp));

            //上行总锁闭
            m_pPainter.drawEllipse(Scale(m_rcTotalLockSLamp));
            //下行总锁闭
            m_pPainter.drawEllipse(Scale(m_rcTotalLockXLamp));

            //上行灯丝断丝
            m_pPainter.drawEllipse(Scale(m_rcFilamentBreakSLamp));
            //下行灯丝断丝
            m_pPainter.drawEllipse(Scale(m_rcFilamentBreakXLamp));

            //挤岔报警
            m_pPainter.drawEllipse(Scale(m_rcCrowdAlarmLamp));
            //电码化报警
            m_pPainter.drawEllipse(Scale(m_rcCodeAlarmLamp));
        }
        

        void StaScattered::DrawText()
        {
            return;
            if (!MainStation()->IsVisible(VisibleDev::stateLempName)) {
                return;
            }
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号

            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(Qt::white);

            m_pPainter.drawText(Scale(m_rcFuseAlarmText), "熔丝报警", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcMainAuxPowerText), "主副电源", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcTrackOutageSText), "上行轨道停电", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcTrackOutageXText), "下行轨道停电", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcTotalLockSText), "上行总锁闭", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcTotalLockXText), "下行总锁闭", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcFilamentBreakSText), "上行灯丝断丝", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcFilamentBreakXText), "下行灯丝断丝", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcCrowdAlarmText), "挤岔报警", QTextOption(Qt::AlignCenter));
            m_pPainter.drawText(Scale(m_rcCodeAlarmText), "电码化报警", QTextOption(Qt::AlignCenter));
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