#include "StaScattered.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaScattered::StaScattered(QObject* parent)
        {
            m_mapAttribute.insert("mRsbjRect", [&](const QString& strElement) { 
                m_rcFuseAlarmText = QStringToQRect(strElement); 
                m_rcFuseAlarmLemp = getLempRectByTextRect(m_rcFuseAlarmText); 
            });

            m_mapAttribute.insert("mZFdyRect", [&](const QString& strElement) {
                m_rcMainAuxPowerText = QStringToQRect(strElement);
                m_rcMainAuxPowerLemp = getLempRectByTextRect(m_rcMainAuxPowerText);
            });

            m_mapAttribute.insert("mGdtdRect", [&](const QString& strElement) {
                if (nIndex == 0) {
                    m_rcTrackOutageSText = QStringToQRect(strElement);
                    m_rcTrackOutageSLemp = getLempRectByTextRect(m_rcTrackOutageSText);
                }
                else if (nIndex == 1) {
                    m_rcTrackOutageXText = QStringToQRect(strElement);
                    m_rcTrackOutageXLemp = getLempRectByTextRect(m_rcTrackOutageXText);
                }
            });

            m_mapAttribute.insert("mYdzsbRect", [&](const QString& strElement) {
                if (nIndex == 0) {
                    m_rcTotalLockSText = QStringToQRect(strElement);
                    m_rcTotalLockSLemp = getLempRectByTextRect(m_rcTotalLockSText);
                }
                else if (nIndex == 1) {
                    m_rcTotalLockXText = QStringToQRect(strElement);
                    m_rcTotalLockXLemp = getLempRectByTextRect(m_rcTotalLockXText);
                }
            });

            m_mapAttribute.insert("mDsdsRect", [&](const QString& strElement) {
                if (nIndex == 0) {
                    m_rcFilamentBreakSText = QStringToQRect(strElement);
                    m_rcFilamentBreakSLemp = getLempRectByTextRect(m_rcFilamentBreakSText);
                }
                else if (nIndex == 1) {
                    m_rcFilamentBreakXText = QStringToQRect(strElement);
                    m_rcFilamentBreakXLemp = getLempRectByTextRect(m_rcFilamentBreakXText);
                }
            });
            
            m_mapAttribute.insert("mJcbjRect", [&](const QString& strElement) {
                m_rcCrowdAlarmText = QStringToQRect(strElement);
                m_rcCrowdAlarmLemp = getLempRectByTextRect(m_rcCrowdAlarmText);
            });
            
            m_mapAttribute.insert("mDmhbjRect", [&](const QString& strElement) {
                m_rcCodeAlarmText = QStringToQRect(strElement);
                m_rcCodeAlarmLemp = getLempRectByTextRect(m_rcCodeAlarmText);
            });

            m_mapAttribute.insert("SGDTDJ", [&](const QString& strElement) { m_strSGDTDJ = strElement; });
            m_mapAttribute.insert("SGDTDJ_bus", [&](const QString& strElement) { m_nSGDTDJ = strElement.toInt(); });
            m_mapAttribute.insert("XGDTDJ", [&](const QString& strElement) { m_strXGDTDJ = strElement; });
            m_mapAttribute.insert("XGDTDJ_bus", [&](const QString& strElement) { m_nXGDTDJ = strElement.toInt(); });
            m_mapAttribute.insert("trackType", [&](const QString& strElement) { m_strTrackType = strElement; });
        }

        StaScattered::~StaScattered()
        {

        }

        bool StaScattered::eventFilter(QObject* obj, QEvent* event)
        {
            return DeviceBase::eventFilter(obj, event);
        }
    
        void StaScattered::Draw(const bool& bElapsed, const bool& isMulti)
        {
            return DeviceBase::Draw(bElapsed, isMulti);
        }

        void StaScattered::DrawLight()
        {
            m_pPainter.setRenderHint(QPainter::Antialiasing, true);

            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));
            //熔丝报警
            m_pPainter.setBrush((m_nState & 0x01) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcFuseAlarmLemp));
            //主副电源
            m_pPainter.drawEllipse(Scale(m_rcMainAuxPowerLemp));
            //上行轨道停电
            m_pPainter.drawEllipse(Scale(m_rcTrackOutageSLemp));
            //下行轨道停电
            m_pPainter.drawEllipse(Scale(m_rcTrackOutageXLemp));

            //上行总锁闭
            m_pPainter.drawEllipse(Scale(m_rcTotalLockSLemp));
            //下行总锁闭
            m_pPainter.drawEllipse(Scale(m_rcTotalLockXLemp));

            //上行灯丝断丝
            m_pPainter.drawEllipse(Scale(m_rcFilamentBreakSLemp));
            //下行灯丝断丝
            m_pPainter.drawEllipse(Scale(m_rcFilamentBreakXLemp));

            //挤岔报警
            m_pPainter.drawEllipse(Scale(m_rcCrowdAlarmLemp));
            //电码化报警
            m_pPainter.drawEllipse(Scale(m_rcCodeAlarmLemp));

            m_pPainter.setRenderHint(QPainter::Antialiasing, false);
        }
        

        void StaScattered::DrawText()
        {
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

        void StaScattered::setVollover(const QPoint& ptBase)
        {

        }

        void StaScattered::ResetDevState()
        {

        }

        QRect StaScattered::getLempRectByTextRect(const QRect& rcText)
        {
            return QRect(QPoint(rcText.left() + ((rcText.width() - 12) / 2), rcText.top() + 18), QSize(13, 13));
        }
    }
}