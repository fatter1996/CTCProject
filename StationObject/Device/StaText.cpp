#include "StaText.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaText::StaText(QObject* pParent)
            : DeviceBase(pParent)
        {
           
        }

        StaText::~StaText()
        {

        }

        void StaText::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute.insert(m_strType, mapAttrFun);
            m_mapAttribute[m_strType].insert("isTitle", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaText*>(pDevice)->m_bIsTitle = strElement.toUInt(); });
            return DeviceBase::InitAttributeMap();
        }

        void StaText::Draw(bool isMulti)
        {
            if (m_bIsTitle) {
                m_bShowName = MainStation()->IsVisible(VisibleDev::stationName);
            }
            else {
                m_bShowName = MainStation()->IsVisible(VisibleDev::text);
            }
            return DeviceBase::Draw(isMulti);
        }
    }
}