#include "StaLocomotive.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaLocomotive::StaLocomotive(QObject* pParent)
            : StaDistant(pParent)
        {
            m_mapAttribute.insert("bsdRect", [&](const QString& strElement) { m_rcLight = QStringToQRectF(strElement); });
        }

        StaLocomotive::~StaLocomotive()
        {

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