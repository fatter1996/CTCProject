#include "StaText.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaText::StaText(QObject* parent)
        {
            m_mapAttribute.insert("isTitle", [&](const QString& strElement) { m_bIsTitle = strElement.toUInt(); });
        }

        StaText::~StaText()
        {

        }

        void StaText::Draw(const bool& isMulti)
        {
            return DeviceBase::Draw(isMulti);
        }

        void StaText::setVollover(const QPoint& ptBase)
        {

        }

        void StaText::ResetDevState()
        {

        }
    }
}