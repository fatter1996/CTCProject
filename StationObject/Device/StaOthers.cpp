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


        StaMark::StaMark(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("m_Color", [&](const QString& strElement) { 
                switch (strElement.toInt())
                {
                case 0:  m_cColor = Qt::white;  break;
                case 1:  m_cColor = Qt::red;    break;
                case 2:  m_cColor = Qt::yellow; break;
                case 3:  m_cColor = Qt::green;  break;
                default: m_cColor = Qt::white;  break;
                }
            });
            m_mapAttribute.insert("m_Rect", [&](const QString& strElement) { m_rcMark = QStringToQRectF(strElement); });
        }

        StaMark::~StaMark()
        {
        
        }

        void StaMark::Draw(bool isMulti)
        {
            DrawMark();
            return DeviceBase::Draw(isMulti);
        }

        void StaMark::DrawMark()
        {
            m_pPainter.setPen(QPen(m_cColor, 3));
            if (m_nType == 601) {
                QPointF points[4] = {
                    Scale(m_rcMark.topLeft()),
                    Scale(m_rcMark.bottomLeft()),
                    Scale(m_rcMark.bottomRight()),
                    Scale(m_rcMark.topRight())
                };
                m_pPainter.drawPolyline(points, 4);
            }
            else if (m_nType == 602) {
                QPointF points[4] = {
                        Scale(m_rcMark.bottomLeft()),
                        Scale(m_rcMark.topLeft()),
                        Scale(m_rcMark.topRight()),
                        Scale(m_rcMark.bottomRight())
                };
                m_pPainter.drawPolyline(points, 4);
            }
            else if (m_nType == 611) {
                m_pPainter.drawRect(Scale(m_rcMark));
                m_pPainter.drawLine(QPointF(m_rcMark.left() + m_rcMark.width() * 0.15, m_rcMark.top() + m_rcMark.height() * 0.25),
                    QPointF(m_rcMark.left() + m_rcMark.width() * 0.85, m_rcMark.top() + m_rcMark.height() * 0.25));
                m_pPainter.drawLine(QPointF(m_rcMark.left() + m_rcMark.width() * 0.3, m_rcMark.top() + m_rcMark.height() * 0.5),
                    QPointF(m_rcMark.left() + m_rcMark.width() * 0.7, m_rcMark.top() + m_rcMark.height() * 0.5));
            }
            else if (m_nType == 612) {
                m_pPainter.drawRect(Scale(m_rcMark));
                m_pPainter.drawLine(QPointF(m_rcMark.left() + m_rcMark.width() * 0.15, m_rcMark.top() + m_rcMark.height() * 0.75),
                    QPointF(m_rcMark.left() + m_rcMark.width() * 0.85, m_rcMark.top() + m_rcMark.height() * 0.75));
                m_pPainter.drawLine(QPointF(m_rcMark.left() + m_rcMark.width() * 0.3, m_rcMark.top() + m_rcMark.height() * 0.5),
                    QPointF(m_rcMark.left() + m_rcMark.width() * 0.7, m_rcMark.top() + m_rcMark.height() * 0.5));
            }
        }
        //站场翻转
        void StaMark::setVollover(const QPointF& ptBase)
        {
        
        }
    }
}