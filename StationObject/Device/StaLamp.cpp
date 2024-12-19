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
            int width = m_rcTextRect.width();
            int height = m_rcTextRect.height();
            m_rcTextRect.setX(m_rcLamp.x() + ((m_rcLamp.width() - m_rcTextRect.width()) / 2));
            m_rcTextRect.setWidth(width);
            m_rcTextRect.setHeight(height);
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

            m_nState = 0;
            if (m_nType == 508 || m_nType == 509 || m_nType == 511) {
                m_nState = 1;
            }
            if (m_nType >= 501 && m_nType <= 506) {
                if (MainStation()->getStaLimits(Station::Limits::ExStaControl)) {
                    if (m_nType == 501) {
                        m_nState = 0x01;
                    }
                }
                else {
                    if (m_nType == 502) {
                        m_nState = 0x03;
                    }
                    if (m_nType == 503) {
                        m_nState = 0x01;
                    }
                    if (m_nType >= 504 && m_nType <= 507) {
                        if (m_nType - 504 == MainStation()->getStaLimits(Station::Limits::ControlMode)) {
                            m_nState = 0x01;
                        }
                        if (m_nType - 504 == MainStation()->getStaLimits(Station::Limits::ApplyControlMode)) {
                            m_nState = 0x23;
                        }
                    }
                }
            }
            QColor cColor;
            switch (m_nState & 0x0F)
            {
            case 1:  cColor = COLOR_LIGHT_GREEN;  break;
            case 2:  cColor = COLOR_LIGHT_RED;    break;
            case 3:  cColor = COLOR_LIGHT_YELLOW; break;
            case 4:  cColor = COLOR_LIGHT_GREEN;  break;
            default: cColor = Qt::NoBrush;        break;
            }

            if (m_nState & 0x20) {
                m_pPainter.setBrush(m_bElapsed ? cColor : Qt::NoBrush);
            }
            else {
                m_pPainter.setBrush(cColor);
            }
            
            m_pPainter.drawEllipse(Scale(m_rcLamp));
            m_pPainter.setRenderHint(QPainter::Antialiasing, false);
        }
    }
}

            