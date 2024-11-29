#include "StaLamp.h"
#include "Global.h"
#include <QMouseEvent>


namespace Station {
    namespace Device {

        StaLamp::StaLamp(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("m_lampRect", [=](const QString& strElement) { m_rcLamp = QStringToQRectF(strElement); });
        }

        StaLamp::~StaLamp()
        {

        }

        void StaLamp::InitDeviceAttribute()
        {
            int width = m_rcLamp.width();
            int height = m_rcLamp.height();
            m_rcLamp.setX(m_rcTextRect.x() + ((m_rcTextRect.width() - m_rcLamp.width()) / 2));
            //m_rcLamp.setY(m_rcTextRect.y() - 24);
            m_rcLamp.setWidth(width);
            m_rcLamp.setHeight(height);
        }

        void StaLamp::Draw(bool isMulti)
        {
            m_bShowName = MainStation()->IsVisible(VisibleDev::stateLempName);
            DrawLight();
            return DeviceBase::Draw(isMulti);
        }

        void StaLamp::DrawLight()
        {
            m_pPainter.setRenderHint(QPainter::Antialiasing, true);
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 2));

            m_pPainter.setBrush((m_nState & 0x10) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
            m_pPainter.drawEllipse(Scale(m_rcLamp));
            
            m_pPainter.setRenderHint(QPainter::Antialiasing, false);
        }
    }
}

            