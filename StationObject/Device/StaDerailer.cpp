#include "StaDerailer.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaDerailer::StaDerailer(QObject* pParent)
            : DeviceBase(pParent)
        {
            
        }

        StaDerailer::~StaDerailer()
        {

        }

        void StaDerailer::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute[m_strType].insert("m_RelayDCnode", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDerailer*>(pDevice)->m_nRelayDCnode = strElement.toInt(nullptr, 16); });
            m_mapAttribute[m_strType].insert("ISDD", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDerailer*>(pDevice)->m_bISDD = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("p1", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDerailer*>(pDevice)->p1 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p2", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDerailer*>(pDevice)->p2 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p3", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDerailer*>(pDevice)->p3 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("IsDCorTGQ", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDerailer*>(pDevice)->m_bIsDCorTGQ = strElement.toUInt(); });
            return DeviceBase::InitAttributeMap();
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