#include "StaEndLine.h"
#include "Global.h"

namespace Station {
    namespace Device {
        StaEndLine::StaEndLine(QObject* parent)
        {
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p3", [&](const QString& strElement) { p3 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p4", [&](const QString& strElement) { p4 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p5", [&](const QString& strElement) { p5 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p6", [&](const QString& strElement) { p6 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p7", [&](const QString& strElement) { p7 = QStringToQPoint(strElement); });
        }

        StaEndLine::~StaEndLine()
        {

        }

        bool StaEndLine::eventFilter(QObject* obj, QEvent* event)
        {
            return DeviceBase::eventFilter(obj, event);
        }

        void StaEndLine::Draw(const bool& bElapsed, const bool& isMulti)
        {
            if (!m_pPainter)
                return;

            DrawEndLine();

            return DeviceBase::Draw(bElapsed, isMulti);
        }

        void StaEndLine::DrawEndLine()
        {
            m_pPainter->setPen(QPen(COLOR_TRACK_WHITE, 2));
            m_pPainter->drawLine(Scale(p1), Scale(p6));
            m_pPainter->drawLine(Scale(p2), Scale(p7));
            m_pPainter->drawLine(Scale(p6), Scale(p7));
            m_pPainter->drawLine(Scale(p4), Scale(p5));
            m_pPainter->drawLine(Scale(p3), Scale(QPoint(p4.x(), p3.y())));
        }

        void StaEndLine::setVollover(const QPoint& ptBase)
        {

        }

        void StaEndLine::ResetDevState()
        {

        }
    }
}