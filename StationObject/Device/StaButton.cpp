#include "StaButton.h"
#include "Global.h"

namespace Station {
    namespace Device {

        StaButton::StaButton(QObject* parent)
        {
            m_mapAttribute.insert("p1", [=](const QString& strElement) { p1 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p2", [=](const QString& strElement) { p2 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("m_nTZB", [=](const QString& strElement) { m_nTZB = strElement.toUInt(nullptr, 16); });
            m_mapAttribute.insert("ButLong", [=](const QString& strElement) { m_nButLong = strElement.toUInt(); });
        }

        StaButton::~StaButton()
        {

        }

        bool StaButton::eventFilter(QObject* obj, QEvent* event)
        {
            return DeviceBase::eventFilter(obj, event);
        }

        void StaButton::InitDeviceAttribute()
        {
            if (m_nType == 81) {
                n_rcButton = QRect(QPoint(m_ptCenter.x() - 8, m_ptCenter.y() - 8), QPoint(m_ptCenter.x() + 8, m_ptCenter.y() + 8));
            }
            else if (m_nType == 380) {
                n_rcButton = QRect(p1, p2);
            }
        }

        void StaButton::Draw(const bool& bElapsed, const bool& isMulti)
        {
            if (!m_pPainter) {
                return;
            }
            DrawStaButton(bElapsed);

            return DeviceBase::Draw(bElapsed, isMulti);
        }

        void StaButton::DrawStaButton(const bool& bElapsed)
        {
            if (m_nType == 81) {
                DrawButton(m_pPainter, bElapsed, Scale(n_rcButton), COLOR_BTN_GREEN_TG);
            }
            else if (m_nType == 380) {
                DrawButton(m_pPainter, bElapsed, Scale(n_rcButton), COLOR_BTN_GREEN);
            } 
        }

        QPen StaButton::getDeviceNameColor(const bool& bElapsed)
        {
            return QPen(m_bRangeVisible ? Qt::black : Qt::white);
        }
        
        void StaButton::OnButtonClick()
        {
        }
        
        void StaButton::setVollover(const QPoint& ptBase)
        {
        
        }
        
        void StaButton::ResetDevState()
        {
        
        }
    }
}