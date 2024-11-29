#include "StaText.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaText::StaText(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("isTitle", [&](const QString& strElement) { m_bIsTitle = strElement.toUInt(); });
        }

        StaText::~StaText()
        {

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