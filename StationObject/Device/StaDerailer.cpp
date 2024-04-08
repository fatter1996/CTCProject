#include "StaDerailer.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaDerailer::StaDerailer(QObject* parent)
        {
            m_mapAttribute.insert("m_RelayDCnode", [&](const QString& strElement) { m_nRelayDCnode = strElement.toInt(nullptr, 16); });
            m_mapAttribute.insert("ISDD", [&](const QString& strElement) { m_bISDD = strElement.toInt(); });
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p3", [&](const QString& strElement) { p3 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("IsDCorTGQ", [&](const QString& strElement) { m_bIsDCorTGQ = strElement.toUInt(); });
        }

        StaDerailer::~StaDerailer()
        {

        }

        bool StaDerailer::eventFilter(QObject* obj, QEvent* event)
        {
            return DeviceBase::eventFilter(obj, event);
        }

        void StaDerailer::Draw(const bool& bElapsed, const bool& isMulti)
        {
            if (!m_pPainter)
                return;
            
            DrawDerailer();

            return DeviceBase::Draw(bElapsed, isMulti);
        }

        void StaDerailer::DrawDerailer()
        {
            m_pPainter->setRenderHint(QPainter::Antialiasing, true);
            m_pPainter->setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter->setBrush(COLOR_BTN_GRAY);
            m_pPainter->drawPolygon(QPolygon() << Scale(p1) << Scale(p2) << Scale(p3));
            m_pPainter->setRenderHint(QPainter::Antialiasing, false);
        }

        QPen StaDerailer::getDeviceNameColor(const bool& bElapsed)
        {
            return QPen(Qt::red);
        }

        void StaDerailer::setVollover(const QPoint& ptBase)
        {

        }

        void StaDerailer::ResetDevState()
        {

        }
    }
}