#include "StaEndLine.h"
#include "Global.h"

namespace Station {
    namespace Device {
        StaEndLine::StaEndLine(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p3", [&](const QString& strElement) { p3 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p4", [&](const QString& strElement) { p4 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p5", [&](const QString& strElement) { p5 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p6", [&](const QString& strElement) { p6 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p7", [&](const QString& strElement) { p7 = QStringToQPointF(strElement); });
        }

        StaEndLine::~StaEndLine()
        {

        }

        void StaEndLine::Draw(bool isMulti)
        {
            if (MainStation()->IsVisible(VisibleDev::endMarker)) {
                DrawEndLine();
            }
            return DeviceBase::Draw(isMulti);
        }

        void StaEndLine::DrawEndLine()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 2));
            m_pPainter.drawLine(Scale(p1), Scale(p6));
            m_pPainter.drawLine(Scale(p2), Scale(p7));
            m_pPainter.drawLine(Scale(p6), Scale(p7));
            m_pPainter.drawLine(Scale(p4), Scale(p5));
            m_pPainter.drawLine(Scale(p3), Scale(QPointF(p4.x(), p3.y())));
        }

        void StaEndLine::setVollover(const QPointF& ptBase)
        {

        }

        void StaEndLine::ResetDevState()
        {

        }
    }
}