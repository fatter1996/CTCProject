#include "StaOthers.h"
#include "StaSwitch.h"
#include "Global.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Station {
    namespace Device {
        StaSwitchSection::StaSwitchSection(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("m_nChildNum", [&](const QString& strElement) { m_nChildNum = strElement.toUInt(); });
            m_mapAttribute.insert("m_nChild", [&](const QString& strElement) {
                if (nIndex < m_nChildNum) {
                    m_vecSectionsCode.append(strElement.toUInt());
                }
            });
        }

        StaSwitchSection::~StaSwitchSection()
        {

        }

        void StaSwitchSection::setVollover(const QPointF& ptBase)
        {

        }



        StaPermillSix::StaPermillSix(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPointF(strElement); });
        }

        StaPermillSix::~StaPermillSix()
        {
        
        }

        void StaPermillSix::Draw(bool isMulti)
        {
            if (MainStation()->IsVisible(VisibleDev::permillSix)) {
                DrawPermillSix();
            }
            
            return DeviceBase::Draw(isMulti);
        }
        
        void StaPermillSix::DrawPermillSix()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.drawLine(p1, m_ptCenter);
            m_pPainter.drawLine(m_ptCenter, p2);
        }

        void StaPermillSix::setVollover(const QPointF& ptBase)
        {
        
        }


        StaNeutral::StaNeutral(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("Line", [&](const QString& strElement) {  
                QJsonParseError error;
                QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
                if (josnDoc.isNull()) {
                    qDebug() << "无效的JSON格式:" << error.errorString();
                    return;
                }
                
                if (josnDoc.isArray()) {
                    NeutralLine* pLine = nullptr;
                    for (const QJsonValue& value : josnDoc.array()) {
                        if (value.isObject()) {
                            pLine = new NeutralLine;
                            pLine->ptStart = QStringToQPointF(value.toObject().value("m_startPos").toString());
                            pLine->ptEnd = QStringToQPointF(value.toObject().value("m_endPos").toString());
                            m_vecNeutralLine.append(pLine);
                        }
                    }
                }
            });

            m_mapAttribute.insert("Text", [&](const QString& strElement) {
                QJsonParseError error;
                QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
                if (josnDoc.isNull()) {
                    qDebug() << "无效的JSON格式:" << error.errorString();
                    return;
                }

                if (josnDoc.isArray()) {
                    NeutralText* pText = nullptr;
                    for (const QJsonValue& value : josnDoc.array()) {
                        if (value.isObject()) {
                            pText = new NeutralText;
                            pText->strContent = value.toObject().value("m_strContent").toString();
                            pText->rcText = QStringToQRectF(value.toObject().value("m_textRect").toString());
                            switch (value.toObject().value("m_textColor").toInt())
                            {
                            case 0:  pText->cTextColor = Qt::white;  break;
                            case 1:  pText->cTextColor = Qt::red;    break;
                            case 2:  pText->cTextColor = Qt::yellow; break;
                            case 3:  pText->cTextColor = Qt::green;  break;
                            default: pText->cTextColor = Qt::white;  break;
                            }
                            m_vecNeutralText.append(pText);
                        }
                    }
                }
            });
        }

        StaNeutral::~StaNeutral()
        {

        }

        void StaNeutral::Draw(bool isMulti)
        {
            DrawNeutral();
            return DeviceBase::Draw(isMulti);
        }
        
        void StaNeutral::DrawNeutral()
        {
            m_pPainter.setPen(QPen(COLOR_BTN_WHITE, 1));
            for (NeutralLine* pLing : m_vecNeutralLine) {
                m_pPainter.drawLine(Scale(pLing->ptStart), Scale(pLing->ptEnd));
            }
            QFont font = m_pPainter.font();
            font.setPointSize(10);
            m_pPainter.setFont(font);
            for (NeutralText* pText : m_vecNeutralText) {
                m_pPainter.setPen(QPen(pText->cTextColor, 1));
                m_pPainter.drawText(Scale(pText->rcText), pText->strContent, QTextOption(Qt::AlignHCenter));
            }
        }
        
        void StaNeutral::setVollover(const QPointF& ptBase)
        {
        
        }
    }
}