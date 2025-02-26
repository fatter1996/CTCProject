﻿#include "StaButton.h"
#include "Global.h"
#include <QMouseEvent>

namespace Station {
    namespace Device {

        StaButton::StaButton(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("p1", [=](const QString& strElement) {p1 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p2", [=](const QString& strElement) {p2 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("m_nTZB", [=](const QString& strElement) {m_nTZB = strElement.toUInt(nullptr, 16); });
            m_mapAttribute.insert("ButLong", [=](const QString& strElement) {m_nButLong = strElement.toUInt(); });
        }

        StaButton::~StaButton()
        {

        }

        void StaButton::InitDeviceAttribute()
        {
            if (m_ptCenter.isNull()) {
                m_ptCenter = QPointF(p1.x() + 8, p1.y() + 8);
            }
            if (p1.isNull()) {
                p1 = QPointF(m_ptCenter.x() - 8, m_ptCenter.y() - 8);
            }
            if (p2.isNull()) {
                p2 = QPointF(p1.x() + 16, p1.y() + 16);
            }
            n_rcButton = QRectF(p1, p2);
        }

        void StaButton::Draw(bool isMulti)
        {
            if (m_nType != 81) {   //引导总锁
                m_bShowName = MainStation()->IsVisible(VisibleDev::funButtonName);
            }

            if (MainStation()->IsVisible(VisibleDev::button) && m_bMainStation) {
                DrawStaButton();
            }
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
            else if (m_nType == 381) {
                DrawButton(m_pPainter, Scale(n_rcButton), COLOR_BTN_DEEPGRAY, m_nBtnState, 2);
            }
            else if (m_nType == 540) {
                DrawButton(m_pPainter, Scale(n_rcButton), m_nBtnState & 0x10 ? COLOR_BTN_RED : COLOR_BTN_DEEPGRAY, m_nBtnState & 0x01);
            }
            else {
                DrawButton(m_pPainter, Scale(n_rcButton), COLOR_BTN_DEEPGRAY, m_nBtnState);
            }
            
        }

        bool StaButton::Contains(const QPoint& ptPos)
        {
            return Scale(n_rcButton).contains(ptPos);
        }
        
        bool StaButton::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RouteBuild && 
                (m_nType == 81 || m_nType == 380 || m_nType == 381)) {
                return Scale(n_rcButton).contains(ptPos);
            }
            else if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn && m_nType == 540) {
                return Scale(n_rcButton).contains(ptPos);
            }
            return false;
        }

        void StaButton::InitClickEvent()
        {
            m_mapClickEvent.insert(CTCWindows::FunType::RouteBuild, [&]() {
                OnButtonClick(this);
            });
            m_mapClickEvent.insert(CTCWindows::FunType::FunBtn, [&]() {
                OnButtonClick(this);
            });
        }

        void StaButton::SetBtnState()
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RouteBuild) {
                if (m_nType == 81 && m_nFirstBtnType == 0) {    //通过按钮
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Though);
                    m_nBtnState = 1;
                    m_nFirstBtnType = 3;
                }
                if ((m_nType == 380 || m_nType == 381) && m_nFirstBtnType == 1) {   //虚信号按钮
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Flexibility);
                    m_nBtnState = 1;
                    m_nFirstBtnType = 1;
                }
            }

            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                m_nBtnState &= 0xF0;
                if (m_nType == 540) {   //引导总锁
                    if (m_bUpDown == 0) {   //下行
                        CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(
                            m_nBtnState ? CTCWindows::OperObjType::UnDescend : CTCWindows::OperObjType::Descend);
                    }
                    else {  //上行
                        CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(
                            m_nBtnState ? CTCWindows::OperObjType::UnAscend : CTCWindows::OperObjType::Ascend);
                    }
                    m_nFirstBtnType = 5;
                }
                m_nBtnState |= 0x01;
                m_nBtnState ^= 0x10;
            }
        }

        void StaButton::OrderClear(int nType)
        {
            if (nType == 1) {
                m_nBtnState ^= 0x10;
            }
            BtnStateReset();
        }
        
        void StaButton::setVollover(const QPointF& ptBase)
        {
        
        }
        
        void StaButton::ResetDevState()
        {
        
        }
    }
}