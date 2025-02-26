#include "StaTrack.h"
#include "Global.h"
#include <QMessageBox>
#include "CommonWidget/LeadSealDlg.h"
#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaTrack::StaTrack(QObject* pParent)
            : StaSection(pParent)
        {
            m_mapAttribute.insert("GD_Type", [&](const QString& strElement) { m_strTrackType = strElement; });
        }

        StaTrack::~StaTrack()
        {

        }

        void StaTrack::InitDeviceAttribute()
        {
            m_rcRespondRect = QRectF(p1, p4);
            TrainFrame* pTrainFrame = new TrainFrame;
            int nWidth = m_rcRespondRect.width() > 64 ? 64 : m_rcRespondRect.width();
            pTrainFrame->m_rcFrame = QRectF(m_ptCenter.x() - nWidth / 2, m_ptCenter.y() - 16, nWidth, 32);
            m_vecTrainFrame.append(pTrainFrame);
        }

        void StaTrack::Draw(bool isMulti)
        {
            if (m_strTrackType == "GD_QD" || m_strTrackType == "ZX_GD") {
                m_bShowName = MainStation()->IsVisible(VisibleDev::trackName);
            }
            else if (m_strTrackType == "JJ_QD") {
                m_bShowName = MainStation()->IsVisible(VisibleDev::sectionName);
            }
            else {
                m_bShowName = MainStation()->IsVisible(VisibleDev::unswitchSectionName);
            }
            //绘制股道底部的灰色区域
            DrawTrackBack();
            //绘制股道
            DrawTrack(QPen(getTrackColor(), Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            //绘制车次窗
            DrawTrainFrame(m_pPainter);
            if (m_strTrackType == "GD_QD" || m_strTrackType == "ZX_GD") {

            }
            
            //绘制车次
            DrawTrain(m_pPainter);
            return StaSection::Draw(isMulti);
        }

        void StaTrack::DrawTrackBack()
        {
            if (m_strTrackType == "ZX_GD" || m_strTrackType == "GD_QD") {
                QRectF bkRect(
                    Scale(QPointF(p12.x() + (p34.x() - p12.x()) / 4, p12.y() - 12)),
                    QSizeF(Scale((p34.x() - p12.x()) / 2), Scale(p34.y() - p12.y() + 24)));
                m_pPainter.setPen(QPen(COLOR_TRACK_DEEPBLUE));
                m_pPainter.setBrush(QBrush(COLOR_TRACK_DEEPBLUE, Qt::SolidPattern));
                m_pPainter.drawRect(bkRect);
            }
        }

        void StaTrack::DrawTrack(const QPen& pen, const bool bOutSide, const int nOffset)
        {
            if (m_nZ == 0) {
                DrawTrackLine(pen, p12, p34, bOutSide, nOffset);
            }
            else if (m_nZ == 1) {
                DrawTrackLine(pen, p12, m_ptCenter, bOutSide, nOffset);
                DrawTrackLine(pen, m_ptCenter, p34, bOutSide, nOffset);
            }
            else {
                DrawTrackLine(pen, p12, pz12, bOutSide, nOffset);
                DrawTrackLine(pen, pz12, pz34, bOutSide, nOffset);
                DrawTrackLine(pen, pz34, p34, bOutSide, nOffset);
            }
        }

        void StaTrack::DrawDeviceOutSide()
        {
            //绘制股道外边缘-分路不良
            if (m_bShuntFault) {
                if (m_bSpeedLimit || m_nPowerCut) {
                    if (m_bShuntFaultIdle || m_bElapsed) {
                        DrawTrack(QPen(COLOR_TRACK_OUTSIDE_RED, 2), true, 4);
                    }
                }
                else {
                    if (m_bShuntFaultIdle || m_bElapsed) {
                        DrawTrack(QPen(COLOR_TRACK_OUTSIDE_RED, 2), true, 2);
                    }
                }
            }
            //绘制股道外边缘-临时限速,股道无电
            if (m_bSpeedLimit) { //临时限速
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_YELLOW, 2), true, 2);
            }
            else if (m_nPowerCut) { //股道无电
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_GREEN, 2), true, 2);
            }
        }
        
        void StaTrack::DrawSelectRange()
        {
            //股道虚线框
            DrawTrack(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine), true, 5);
            //设备名称虚线框
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            m_pPainter.setBrush(COLOR_TRACK_SELECT_BLUE);
            m_pPainter.drawRect(Scale(OutSideRect(m_rcTextRect, 2, 0)));
        }

        void StaTrack::DrawInsulateNode()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.drawLine(Scale(QPointF(p1.x(), p1.y())), Scale(QPointF(p2.x(), p2.y()))); //绘制轨道区段左侧绝缘节
            m_pPainter.drawLine(Scale(QPointF(p3.x(), p3.y())), Scale(QPointF(p4.x(), p4.y()))); //绘制轨道区段右侧绝缘节
        }

        bool StaTrack::Contains(const QPoint& ptPos)
        {
            return Scale(m_rcTextRect).contains(ptPos) || Scale(m_rcRespondRect).contains(ptPos);
        }

        bool StaTrack::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RegionRelieve ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::Blockade ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::UnBlockade ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RampUnlock ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::PoorRoute ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::IdleConfirm) {
                return Contains(ptPos);
            }
            return false;
        }

        void StaTrack::onMouseMoveToDevice(const QPoint& ptPos)
        {
            onMouseMoveToTrainFrame(ptPos);
            return DeviceBase::onMouseMoveToDevice(ptPos);
        }

        void StaTrack::InitClickEvent()
        {
            for (int i = 0; i < static_cast<int>(CTCWindows::FunType::MethodConvert); ++i) {
                switch (i)
                {
                case static_cast<int>(CTCWindows::FunType::RegionRelieve):      //区故解
                case static_cast<int>(CTCWindows::FunType::Blockade):           //封锁
                case static_cast<int>(CTCWindows::FunType::UnBlockade):         //解封
                case static_cast<int>(CTCWindows::FunType::RampUnlock):         //坡道解锁
                case static_cast<int>(CTCWindows::FunType::PoorRoute):          //分路不良
                case static_cast<int>(CTCWindows::FunType::IdleConfirm): {      //确认空闲
                    m_mapClickEvent.insert(static_cast<CTCWindows::FunType>(i), [&]() {
                        CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Track);
                        MainStation()->AddSelectDevice(this);
                    });
                    break;
                }
                }
            }
        }

        void StaTrack::ShowDeviceMenu(const QPoint& ptPos)
        {
            if (m_vecTrainFrame[0]->m_rcFrame.contains(ptPos)) {   //车次号
                ShowTrainMenu(QCursor::pos(), m_nCode);
            }
            else {
                QMenu* pMenu = new QMenu();
                pMenu->setAttribute(Qt::WA_DeleteOnClose);
                QAction* pAction1 = new QAction("封锁/解封");
                pMenu->addAction(pAction1);
                QObject::connect(pAction1, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"封锁/解封[股道:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, (m_nState & SECTION_STATE_BLOCK) ? 0x0b : 0x0a, 0x12);
                    }
                });
                QAction* pAction2 = new QAction("区故解");
                pMenu->addAction(pAction2);
                QObject::connect(pAction2, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"区故解[股道:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                            MainStation()->AddSelectDevice(this);
                            MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x0d, 0x12);
                        }
                    }
                });

                QAction* pAction3 = new QAction("分路不良");
                pMenu->addAction(pAction3);
                QObject::connect(pAction3, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"分路不良[股道:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                            MainStation()->AddSelectDevice(this);
                            MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x11, 0x12);
                        }
                    }
                });

                QAction* pAction4 = new QAction("股道无电");
                pMenu->addAction(pAction4);
                QObject::connect(pAction4, &QAction::triggered, [=]() {
                    m_nPowerCut = true;
                });

                pMenu->exec(QCursor::pos());
            }
        }

        void StaTrack::setVollover(const QPointF& ptBase)
        {

        }

        void StaTrack::ResetDevState()
        {

        }
    }
}