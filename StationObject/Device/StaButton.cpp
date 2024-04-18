#include "StaButton.h"
#include "Global.h"
#include <QMouseEvent>

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
            if (event->type() == QEvent::MouseMove) {
                QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
                onMouseMoveToButton(mouseEvent->pos(), m_nCode);
            }
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

        void StaButton::Draw(const bool& isMulti)
        {
            DrawStaButton();
            return DeviceBase::Draw(isMulti);
        }

        void StaButton::DrawStaButton()
        {
            if (m_nType == 81) {
                DrawButton(m_pPainter, Scale(n_rcButton), COLOR_BTN_GREEN_TG, m_nBtnState);
            }
            else if (m_nType == 380) {
                DrawButton(m_pPainter, Scale(n_rcButton), COLOR_BTN_GREEN, m_nBtnState);
            } 
        }

        bool StaButton::Contains(const QPoint& ptPos)
        {
            return n_rcButton.contains(ptPos);
        }
        
        bool StaButton::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::getCurrFunType() == CTCWindows::FunType::RouteBuild) {
                return n_rcButton.contains(ptPos);
            }
            return false;
        }

        void StaButton::InitClickEvent()
        {
            m_mapClickEvent.insert(CTCWindows::FunType::RouteBuild, [&]() {
                OnButtonClick();
                if (m_nBtnState) {
                    StationObject::AddSelectDevice(this);
                }
            });
        }

        void StaButton::OnButtonClick()
        {
            if (m_nBtnState != 0) { //同一信号机不能重复点击
                return;
            }

            if (CTCWindows::getCurrFunType() == CTCWindows::FunType::RouteBuild) {
                if (m_nType == 81 && m_nFirstBtnType == 0) {    //通过按钮
                    m_nBtnState |= true;
                    m_nFirstBtnType = 3;
                    return;
                }
                if (m_nType == 380 && m_nFirstBtnType == 1) {   //虚信号按钮
                    m_nBtnState |= true;
                    m_nFirstBtnType = 1;
                    return;
                }
            }
        }

        void StaButton::OrderClear()
        {
            BtnStateReset();
        }
        
        void StaButton::setVollover(const QPoint& ptBase)
        {
        
        }
        
        void StaButton::ResetDevState()
        {
        
        }
    }
}