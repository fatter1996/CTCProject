#include "StaLocomotive.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaLocomotive::StaLocomotive(QObject* pParent)
            : StaDistant(pParent)
        {
            
        }

        StaLocomotive::~StaLocomotive()
        {

        }

        void StaLocomotive::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute.insert(m_strType, mapAttrFun);
            m_mapAttribute[m_strType].insert("bsdRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaLocomotive*>(pDevice)->m_rcLight = QStringToQRectF(strElement); });
            return DeviceBase::InitAttributeMap();
        }

        void StaLocomotive::Draw(bool isMulti)
        {
            return StaDistant::Draw(isMulti);
        }

        void StaLocomotive::DrawLight()
        {
            m_pPainter.setPen(QPen(COLOR_BTN_WHITE, 1));
            m_pPainter.setBrush(((m_nState & 0x0f) == 0x05) ? COLOR_LIGHT_WHITE : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(m_rcLight);
        }

        void StaLocomotive::DrawText()
        {
        
        }

        void StaLocomotive::setVollover(const QPointF& ptBase)
        {

        }

        void StaLocomotive::ResetDevState()
        {

        }
    }
}