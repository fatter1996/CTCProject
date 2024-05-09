#include "StaLocomotive.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaLocomotive::StaLocomotive(QObject* parent)
        {
            m_mapAttribute.insert("bsdRect", [&](const QString& strElement) { m_rcLight = QStringToQRect(strElement); });
        }

        StaLocomotive::~StaLocomotive()
        {

        }

        bool StaLocomotive::eventFilter(QObject* obj, QEvent* event)
        {
            return DeviceBase::eventFilter(obj, event);
        }

        void StaLocomotive::Draw(const bool& isMulti)
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

        void StaLocomotive::setVollover(const QPoint& ptBase)
        {

        }

        void StaLocomotive::ResetDevState()
        {

        }
    }
}