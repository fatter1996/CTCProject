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
            else if (m_nType == 380 || m_nType == 342) {
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
            else if (m_nType == 342) {
                DrawButton(m_pPainter, Scale(n_rcButton), COLOR_BTN_DEEPGRAY, m_nBtnState);
            }
        }

        bool StaButton::Contains(const QPoint& ptPos)
        {
            if (CTCWindows::getCurrFunType() == CTCWindows::FunType::RouteBuild && (m_nType == 81 || m_nType == 380)) {
                return n_rcButton.contains(ptPos); 
            }
            return false;
        }
        
        bool StaButton::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::getCurrFunType() == CTCWindows::FunType::RouteBuild && (m_nType == 81 || m_nType == 380)) {
                return n_rcButton.contains(ptPos);
            }
            else if (CTCWindows::getCurrFunType() == CTCWindows::FunType::FunBtn && m_nType == 342) {
                return n_rcButton.contains(ptPos);
            }
            return false;
        }

        void StaButton::InitClickEvent()
        {
            m_mapClickEvent.insert(CTCWindows::FunType::RouteBuild, [&]() {
                OnButtonClick();
            });
            m_mapClickEvent.insert(CTCWindows::FunType::FunBtn, [&]() {
                OnButtonClick();
            });
        }

        void StaButton::OnButtonClick()
        {
            if (m_nBtnState != 0) { //同一信号机不能重复点击
                return;
            }

            if (CTCWindows::getCurrFunType() == CTCWindows::FunType::RouteBuild) {
                if (m_nType == 81 && m_nFirstBtnType == 0) {    //通过按钮
                    CTCWindows::setOperObjType(CTCWindows::OperObjType::Though);
                    m_nBtnState |= true;
                    m_nFirstBtnType = 3;
                }
                if (m_nType == 380 && m_nFirstBtnType == 1) {   //虚信号按钮
                    CTCWindows::setOperObjType(CTCWindows::OperObjType::Flexibility);
                    m_nBtnState |= true;
                    m_nFirstBtnType = 1;
                }
            }

            if (CTCWindows::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                
                if (m_nType == 342) {   //引导总锁
                    if (m_nSX == 0) {   //下行
                        CTCWindows::setOperObjType(CTCWindows::OperObjType::Descend);
                    }
                    else {  //上行
                        CTCWindows::setOperObjType(CTCWindows::OperObjType::Ascend);
                    }
                    m_nFirstBtnType = 5;
                }
                m_nBtnState |= true;
            }

            if (m_nBtnState) {
                StationObject::AddSelectDevice(this);
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