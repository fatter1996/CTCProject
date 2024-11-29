#include "StaDerailer.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaDerailer::StaDerailer(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("m_RelayDCnode", [&](const QString& strElement) { m_nRelayDCnode = strElement.toInt(nullptr, 16); });
            m_mapAttribute.insert("ISDD", [&](const QString& strElement) { m_bISDD = strElement.toInt(); });
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p3", [&](const QString& strElement) { p3 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("IsDCorTGQ", [&](const QString& strElement) { m_bIsDCorTGQ = strElement.toUInt(); });
        }

        StaDerailer::~StaDerailer()
        {

        }

        void StaDerailer::Draw(bool isMulti)
        {
            DrawDerailer();
            return DeviceBase::Draw(isMulti);
        }

        void StaDerailer::DrawDerailer()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.setBrush(COLOR_BTN_GRAY);
            m_pPainter.drawPolygon(QPolygonF() << Scale(p1) << Scale(p2) << Scale(p3));
        }

        QPen StaDerailer::getDeviceNameColor()
        {
            return QPen(Qt::red);
        }

        void StaDerailer::setVollover(const QPointF& ptBase)
        {

        }

        void StaDerailer::ResetDevState()
        {

        }
    }
}