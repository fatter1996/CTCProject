#include "StaSwitch.h"
#include "Global.h"
#include <QMessageBox>
#include "CommonWidget/LeadSealDlg.h"
#pragma execution_character_set("utf-8")

#define MAX(a, b)  (a > b ? a : b)
#define MIN(a, b)  (a < b ? a : b)
#define MAX2(a, b, c)  (a > b ? (a > c ? a : c) : (b > c ? b : c))
#define MIN2(a, b, c)  (a < b ? (a < c ? a : c) : (b < c ? b : c))

namespace Station {
    namespace Device {

        StaSwitch::StaSwitch(QObject* pParent)
            : StaSection(pParent)
        {
            m_mapAttribute.insert("m_nQDCode", [&](const QString& strElement) { m_nQDCode = strElement.toUInt(nullptr, 16); });
            m_mapAttribute.insert("m_nCxjy", [&](const QString& strElement) { m_nCxjy = strElement.toUInt(); });
            m_mapAttribute.insert("m_nDSCode", [&](const QString& strElement) { m_nDSCode = strElement.toInt(); });
            m_mapAttribute.insert("QDMKInterlockBus", [&](const QString& strElement) { m_nQDMKInterlockBus = strElement.toInt(); });
            m_mapAttribute.insert("m_nQDMKCode", [&](const QString& strElement) { m_nQDMKInterlockBus = strElement.toInt(nullptr, 16); });
            m_mapAttribute.insert("m_nJyj", [&](const QString& strElement) { m_nJyj = strElement.toInt(); });
            m_mapAttribute.insert("oneToMore", [&](const QString& strElement) { m_nOneToMore = strElement.toInt(); });
            m_mapAttribute.insert("p5", [&](const QString& strElement) { p5 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p6", [&](const QString& strElement) { p6 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p56", [&](const QString& strElement) { p56 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("pZ12", [&](const QString& strElement) { pz12 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("pZ34", [&](const QString& strElement) { pz34 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("pZ56", [&](const QString& strElement) { pz56 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("m_MainDCQD", [&](const QString& strElement) { m_nMainDCQD = strElement.toInt(); });
            m_mapAttribute.insert("m_bMainGD", [&](const QString& strElement) { m_nMainDCQD = strElement.toInt(); });
            m_mapAttribute.insert("isSafetySwitch", [&](const QString& strElement) { m_bSafetySwitch = strElement.toInt(); });
            m_mapAttribute.insert("m_nCQ", [&](const QString& strElement) { m_nQ = strElement.toInt(); });
            m_mapAttribute.insert("m_nQ", [&](const QString& strElement) { m_nQ = strElement.toInt(); });
            m_mapAttribute.insert("m_nDW", [&](const QString& strElement) { m_nD = strElement.toInt(); });
            m_mapAttribute.insert("m_nD", [&](const QString& strElement) { m_nD = strElement.toInt(); });
            m_mapAttribute.insert("m_nFW", [&](const QString& strElement) { m_nF = strElement.toInt(); });
            m_mapAttribute.insert("m_nF", [&](const QString& strElement) { m_nF = strElement.toInt(); });
        }

        StaSwitch::~StaSwitch()
        {
            
        }

        void StaSwitch::InitDeviceAttribute()
        {
            //三角函数求出岔心坐标
            double a, b, c;
            a = (double)m_ptCenter.x() - (double)pz34.x();
            b = (double)m_ptCenter.y() - (double)pz34.y();
            c = sqrt(a * a + b * b);
            p34C.setX(m_ptCenter.x() - 16 * a / c);
            p34C.setY(m_ptCenter.y() - 16 * b / c);

            a = (double)m_ptCenter.x() - (double)pz56.x();
            b = (double)m_ptCenter.y() - (double)pz56.y();
            c = sqrt(a * a + b * b);
            p56C.setX(m_ptCenter.x() - 16 * a / c);
            p56C.setY(m_ptCenter.y() - 16 * b / c);

            m_rcRespondRect = QRectF(QPointF(MIN2(p34C.x(), p56C.x(), m_ptCenter.x()) - 2, MIN2(p34C.y(), p56C.y(), m_ptCenter.y()) - 2),
                QPointF(MAX2(p34C.x(), p56C.x(), m_ptCenter.x()) + 2, MAX2(p34C.y(), p56C.y(), m_ptCenter.y()) + 2));
        }

        void StaSwitch::Draw(bool isMulti)
        {
            m_bShowName = MainStation()->IsVisible(VisibleDev::switchName);
            //绘制股道
            DrawTrack(QPen(getTrackColor(), Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nSwitchState | SWITCH_DRAW_CQ);
            DrawTrack(QPen(COLOR_TRACK_BLUE, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), 0x07 ^ (m_nSwitchState | SWITCH_DRAW_CQ));
            //绘制岔心
            DrawSwitchCenterTrack();
            //绘制股道状态
            DrawSwitchState();

            return StaSection::Draw(isMulti);
        }

        void StaSwitch::DrawTrack(const QPen& pen, const uint nPosition, const bool bOutSide, const int nOffset)
        {
            //岔前
            if (nPosition & SWITCH_DRAW_CQ) {
                if (m_nZ & SWITCH_KINK_CQ) {
                    DrawTrackLine(pen, p12, pz12, bOutSide, nOffset);
                    DrawTrackLine(pen, pz12, m_ptCenter, bOutSide, nOffset);
                }
                else {
                    DrawTrackLine(pen, p12, m_ptCenter, bOutSide, nOffset);
                }
            }
            //定位
            if (nPosition & SWITCH_DRAW_DW) {
                if (m_nZ & SWITCH_KINK_DW) {
                    DrawTrackLine(pen, p34C, pz34, bOutSide, nOffset);
                    DrawTrackLine(pen, pz34, p34, bOutSide, nOffset);
                }
                else {
                    DrawTrackLine(pen, p34C, p34, bOutSide, nOffset);
                }
            }
            //反位
            if (nPosition & SWITCH_DRAW_FW) {
                if (m_nZ & SWITCH_KINK_FW) {
                    DrawTrackLine(pen, p56C, pz56, bOutSide, nOffset);
                    DrawTrackLine(pen, pz56, p56, bOutSide, nOffset);
                }
                else {
                    DrawTrackLine(pen, p56C, p56, bOutSide, nOffset);
                }
            }
        }

        void StaSwitch::DrawSwitchCenterTrack()
        {
            if (m_bRangeVisible) { //选中状态
                DrawSwitchCenter(QPen(COLOR_TRACK_SELECT_BLUE, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), SWITCH_DRAW_DW | SWITCH_DRAW_FW);
            }
            else if (m_nShuntFault) { //分路不良
                if (m_bElapsed && !m_bShuntFaultIdle) {
                    DrawSwitchCenter(QPen(COLOR_TRACK_YELLOW, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nShuntFault & m_nSwitchState);
                }
            }
            else {
                QColor cColor = getSwitchCenterColor();
                DrawSwitchCenter(QPen(cColor == Qt::black ? Qt::NoPen : cColor, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nSwitchState & 0x03);
            }

            if (m_nSpeedLimit) { //临时限速
                DrawSwitchCenter(QPen(COLOR_TRACK_OUTSIDE_YELLOW, Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nSpeedLimit & m_nSwitchState, true, 2);
            }
        }

        void StaSwitch::DrawSwitchCenter(const QPen& pen, const uint nPosition, const bool bOutSide, const int nOffset)
        {
            if (nPosition == SWITCH_STATE_DW) {  //定位
                DrawTrackLine(pen, m_ptCenter, p34C, bOutSide, nOffset);
            }
            else if (nPosition == SWITCH_STATE_FW) { //反位
                DrawTrackLine(pen, m_ptCenter, p56C, bOutSide, nOffset);
            }
            else {  //四开
                DrawTrackLine(pen, m_ptCenter, p34C, bOutSide, nOffset);
                DrawTrackLine(pen, m_ptCenter, p56C, bOutSide, nOffset);
            }
        }

        void StaSwitch::DrawDeviceOutSide()
        {
            //绘制股道外边缘-分路不良
            if (m_nShuntFault && (m_bShuntFaultIdle || m_bElapsed)) {
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_RED, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nShuntFault & (m_nSpeedLimit | m_nPowerCut), true, 4);
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_RED, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), m_nShuntFault & (~(m_nSpeedLimit | m_nPowerCut)), true, 2);
            }
            //股道无电
            if (m_nPowerCut) { 
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_GREEN, 2), m_nPowerCut & (~m_nSpeedLimit), true, 2);
            }
            //临时限速
            if (m_nSpeedLimit) { 
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_YELLOW, 2), m_nSpeedLimit & m_nPowerCut, true, 2);
                DrawTrack(QPen(COLOR_TRACK_OUTSIDE_YELLOW, 2), m_nSpeedLimit & (~m_nPowerCut), true, 2);
            }
        }

        void StaSwitch::DrawSelectRange()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            //道岔虚线框
            int minX = MIN(MIN(p12.x(), p34.x()), p56.x());
            int minY = MIN(MIN(p12.y(), p34.y()), p56.y());
            int maxX = MAX(MAX(p12.x(), p34.x()), p56.x());
            int maxY = MAX(MAX(p12.y(), p34.y()), p56.y());
            m_rcDevRect = QRectF(QPointF(minX - 10, minY - 10), QPointF(maxX + 10, maxY + 10));
            m_pPainter.drawRect(Scale(m_rcDevRect));

            //设备名称虚线框
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            m_pPainter.setBrush(COLOR_TRACK_SELECT_BLUE);
            m_pPainter.drawRect(Scale(OutSideRect(m_rcTextRect, 2, 0)));
        }

        void StaSwitch::DrawInsulateNode()
        {
            m_pPainter.setRenderHint(QPainter::Antialiasing, true);
            
            //岔前
            if (m_nJyj & SWITCH_KINK_CQ) {
                if (m_nCxjy & SWITCH_KINK_CQ) {
                    m_pPainter.setPen(QPen(COLOR_TRACK_RED, 1));
                    m_pPainter.drawEllipse(Scale(p12), Scale(5), Scale(5));
                }
                m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
                m_pPainter.drawLine(Scale(p1), Scale(p2));
            }
            //定位
            if (m_nJyj & SWITCH_KINK_DW) {
                if (m_nCxjy & SWITCH_KINK_DW) {
                    m_pPainter.setPen(QPen(COLOR_TRACK_RED, 1));
                    m_pPainter.drawEllipse(Scale(p34), Scale(5), Scale(5));
                }
                m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
                m_pPainter.drawLine(Scale(p3), Scale(p4));
            }
            //反位
            if (m_nJyj & SWITCH_KINK_FW) {
                if (m_nCxjy & SWITCH_KINK_FW) {
                    m_pPainter.setPen(QPen(COLOR_TRACK_RED, 1));
                    m_pPainter.drawEllipse(Scale(p56), Scale(5), Scale(5));
                }
                m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
                m_pPainter.drawLine(Scale(p5), Scale(p6));
            }

            m_pPainter.setRenderHint(QPainter::Antialiasing, false);
        }

        void StaSwitch::DrawSwitchState()
        {
            if (m_nSwitchState & SWITCH_STATE_LOCK) { //单锁
                //反走样,防止出现锯齿状线条
                m_pPainter.setRenderHint(QPainter::Antialiasing, true);

                m_pPainter.setPen(QPen(isSwitchSK() ? COLOR_TRACK_RED : COLOR_TRACK_GREEN, Scale(1)));
                m_pPainter.drawEllipse(m_ptCenter, Scale(8), Scale(8));

                m_pPainter.setRenderHint(QPainter::Antialiasing, false);
            }
            if (m_nSwitchState & SWITCH_STATE_BLOCK) { //封锁
                m_pPainter.setPen(QPen(Qt::red, Scale(1)));
                m_pPainter.drawRect(Scale(m_rcTextRect));
            }
        }

        //绘制培训提示信息
        void StaSwitch::DrawCultivateTips()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_YELLOW, 2));
            m_pPainter.setBrush(COLOR_LIGHT_BLACK);

            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(14));//字号
            QRectF rcTips = QRectF(m_ptCenter.x() - 12, m_ptCenter.y() - 12, 24, 24);
            QString strTips;
            //道岔 0x01-道岔单操,0x02-道岔单锁,0x04-道岔封锁,0x08-分路不良,
            if (m_nTipsType & 0x01) {
                strTips = QString("使用功能按钮或右键菜单将%1道岔设为\"%2\"").arg(m_strName).arg(m_nSwitchState == 0x01 ? "定位" : "反位");
            }
            if (m_nTipsType & 0x02) {
                strTips = QString("使用功能按钮或右键菜单将%1道岔\"%2\"").arg(m_strName).arg(m_nSwitchState & SWITCH_STATE_LOCK ? "单锁" : "解锁");
            }
            if (m_nTipsType & 0x04) {
                strTips = QString("使用功能按钮或右键菜单将%1道岔\"%2\"").arg(m_strName).arg(m_nSwitchState & SWITCH_STATE_BLOCK ? "封锁" : "解封");
            }
            if (m_nTipsType & 0x08) {
                strTips = QString("使用功能按钮或右键菜单将%1道岔设为\"分路不良\"").arg(m_strName);
            }
            m_pPainter.drawRect(Scale(rcTips));

            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(COLOR_TRACK_YELLOW);
            QRectF rcTipsText = QRectF(rcTips.x() - 72, rcTips.y() + 36, rcTips.width() + 144, 20);
            m_pPainter.drawText(Scale(rcTipsText), strTips);
        }

        bool StaSwitch::Contains(const QPoint& ptPos)
        {
            return Scale(m_rcTextRect).contains(ptPos) || Scale(m_rcRespondRect).contains(ptPos);
        }

        //鼠标是否在设备上
        bool StaSwitch::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RegionRelieve ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::TotalPosition ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::TotalReverse ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::SingleLock ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::SingleUnlock ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::Blockade ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::UnBlockade) {
                return Contains(ptPos);
            }
            return false;
        }

        void StaSwitch::InitClickEvent()
        {
            for (int i = 0; i < static_cast<int>(CTCWindows::FunType::MethodConvert); ++i) {
                switch (i)
                {
                case static_cast<int>(CTCWindows::FunType::RegionRelieve):      //区故解
                case static_cast<int>(CTCWindows::FunType::TotalPosition):      //总定位
                case static_cast<int>(CTCWindows::FunType::TotalReverse):       //总反位
                case static_cast<int>(CTCWindows::FunType::SingleLock):         //单锁
                case static_cast<int>(CTCWindows::FunType::SingleUnlock):       //单解
                case static_cast<int>(CTCWindows::FunType::Blockade):           //封锁
                case static_cast<int>(CTCWindows::FunType::UnBlockade): {       //解封
                    m_mapClickEvent.insert(static_cast<CTCWindows::FunType>(i), [&]() {
                        CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Switch);
                        MainStation()->AddSelectDevice(this);
                    });
                break;
                }
                }
            }
        }

        void StaSwitch::ShowDeviceMenu(const QPoint& ptPos)
        {
            QMenu* pMenu = new QMenu();
            pMenu->setAttribute(Qt::WA_DeleteOnClose);
            QAction* pAction1 = new QAction("定操");
            pAction1->setEnabled(m_nSwitchState & SWITCH_STATE_FW);
            pMenu->addAction(pAction1);
            QObject::connect(pAction1, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"定操[道岔:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                    MainStation()->AddSelectDevice(this);
                    MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x06, 0x11);
                }
            });
            QAction* pAction2 = new QAction("反操");
            pAction2->setEnabled(m_nSwitchState & SWITCH_STATE_DW);
            pMenu->addAction(pAction2);
            QObject::connect(pAction2, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"反操[道岔:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                    MainStation()->AddSelectDevice(this);
                    MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x07, 0x11);
                }
            });
            pMenu->addSeparator();
            QAction* pAction3 = new QAction("单锁");
            pAction3->setEnabled(!(m_nSwitchState & SWITCH_STATE_LOCK));
            pMenu->addAction(pAction3);
            QObject::connect(pAction3, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"单锁[道岔:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                    MainStation()->AddSelectDevice(this);
                    MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x08, 0x11);
                }
            });
            QAction* pAction4 = new QAction("单解");
            pAction4->setEnabled(m_nSwitchState & SWITCH_STATE_LOCK);
            pMenu->addAction(pAction4);
            QObject::connect(pAction4, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"单解[道岔:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                    MainStation()->AddSelectDevice(this);
                    MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x08, 0x11);
                }
            });
            pMenu->addSeparator();
            QAction* pAction5 = new QAction("封锁/解封");
            //pAction5->setEnabled(m_nSwitchState & SWITCH_STATE_LOCK);
            pMenu->addAction(pAction5);
            QObject::connect(pAction5, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"封锁/解封[道岔:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                    MainStation()->AddSelectDevice(this);
                    MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, (m_nSwitchState & SECTION_STATE_BLOCK) ? 0x0b : 0x0a, 0x11);
                }
            });
            QAction* pAction6 = new QAction("区故解");
            pAction6->setEnabled(m_nSwitchState & SWITCH_STATE_LOCK);
            pMenu->addAction(pAction6);
            QObject::connect(pAction6, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"区故解[道岔:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                    if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x0d, 0x11);
                    }
                }
            });
            pMenu->addSeparator();
            QAction* pAction7 = new QAction("岔前 分路不良");
            pAction7->setEnabled(m_nSwitchState & SWITCH_STATE_LOCK);
            pMenu->addAction(pAction7);
            QObject::connect(pAction7, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"岔前 分路不良[道岔:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                    if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x11, 0x23);
                    }
                }
            });
            QAction* pAction8 = new QAction("定位 分路不良");
            pAction8->setEnabled(m_nSwitchState & SWITCH_STATE_LOCK);
            pMenu->addAction(pAction8);
            QObject::connect(pAction8, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"定位 分路不良[道岔:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                    if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x11, 0x24);
                    }
                }
            });
            QAction* pAction9 = new QAction("反位 分路不良");
            pAction9->setEnabled(m_nSwitchState & SWITCH_STATE_LOCK);
            pMenu->addAction(pAction9);
            QObject::connect(pAction9, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"反位 分路不良[道岔:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                    if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x11, 0x25);
                    }
                }
            });
            pMenu->addSeparator();
            QAction* pAction10 = new QAction("接触网定位无电");
            pAction10->setEnabled(m_nSwitchState & SWITCH_STATE_LOCK);
            pMenu->addAction(pAction10);
            QObject::connect(pAction10, &QAction::triggered, [=]() {
                m_nPowerCut |= 0x01;
            });
            QAction* pAction11 = new QAction("接触网反位无电");
            pAction11->setEnabled(m_nSwitchState& SWITCH_STATE_LOCK);
            pMenu->addAction(pAction11);
            QObject::connect(pAction11, &QAction::triggered, [=]() {
                m_nPowerCut |= 0x02;
            });
            pMenu->exec(QCursor::pos());
        }

        void StaSwitch::setVollover(const QPointF& ptBase)
        {

        }

        void StaSwitch::ResetDevState()
        {

        }

        QColor StaSwitch::getSwitchCenterColor()
        {
            QColor cTrackColor = Qt::black;

            if (m_nSwitchState & SWITCH_DRAW_DW) {
                cTrackColor = COLOR_TRACK_GREEN;
            }
            else if (m_nSwitchState & SWITCH_DRAW_FW) {
                cTrackColor = COLOR_TRACK_YELLOW;
            }
            else {
                cTrackColor = m_bElapsed ? COLOR_TRACK_RED : cTrackColor;
            }

            if (m_nState & SECTION_STATE_TAKEUP) {
                cTrackColor = COLOR_TRACK_RED;
            }

            if (m_nState & SECTION_STATE_PRELOCK) {
                cTrackColor = COLOR_TRACK_PRELOCK_BLUE;
            }
            else if (m_nState & SECTION_STATE_LOCK) {
                cTrackColor = COLOR_TRACK_WHITE;
            }
            else if (m_nState & SECTION_STATE_FAULTLOCK) {
                cTrackColor = COLOR_TRACK_RED;
            }

            if (m_nState & SECTION_STATE_BLOCK) {
                cTrackColor = m_bElapsed ? COLOR_TRACK_RED : cTrackColor;
            }

            if (m_bRangeVisible) {
                cTrackColor = COLOR_TRACK_SELECT_BLUE;
            }
            return cTrackColor;
        }

        QPen StaSwitch::getDeviceNameColor()
        {
            QPen pen;
            if (isSwitchSK()) {
                pen = QPen(!m_bElapsed ? COLOR_TRACK_RED : Qt::NoPen);
            }
            else if (m_nSwitchState & SWITCH_STATE_DW) {
                pen = QPen(COLOR_TRACK_GREEN);
            }
            else if (m_nSwitchState & SWITCH_STATE_FW) {
                pen = QPen(COLOR_TRACK_YELLOW);
            }

            if (m_bRangeVisible) {
                pen = QPen(Qt::white);
            }

            return pen;
        }

        void StaSwitch::setSwitchState(const uint& nSwitchState)
        { 
            m_nSwitchState = nSwitchState;
        }

        bool StaSwitch::isSwitchSK()
        { 
            return ((m_nSwitchState & SWITCH_STATE_SK) == SWITCH_STATE_SK) || ((m_nSwitchState & SWITCH_STATE_SK) == 0);
        }
    }
}
