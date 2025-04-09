#include "StaTrack.h"
#include "Global.h"
#include <QMessageBox>
#include "qmath.h"
#include "CommonWidget/LeadSealDlg.h"


#pragma execution_character_set("utf-8")

Station::Device::LowFrequency Station::Device::StaTrack::m_nLowFrequency;
Station::Device::Direction Station::Device::StaTrack::m_nDirection;
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
        int index = 0;
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
            
            DrawLowTriangulation();
            //绘制车次
            DrawTrain(m_pPainter);
            index++;
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
            Station::MainStationObject* Station = Station::MainStation();
            QString stationName = Station->getStationName();
          
            
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
                            SealTechnique::InsertSealRecord(stationName, "区故解");
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
                            SealTechnique::InsertSealRecord(stationName, "分路不良");
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
        void StaTrack::DrawLowTriangulation()
        {

            if (m_strTrackType == "JJ_QD") {
                switch (m_nLowFrequency)
                {
                case LowFrequency::L6:
                    DrawTriangulation(COLOR_TRACK_GREEN, COLOR_LIGHT_BLACK, m_nDirection, "6");
                    return;
                case LowFrequency::L5:
                    DrawTriangulation(COLOR_TRACK_GREEN, COLOR_LIGHT_BLACK, m_nDirection, "5");
                    return;
                case LowFrequency::L4:
                    DrawTriangulation(COLOR_TRACK_GREEN, COLOR_LIGHT_BLACK, m_nDirection, "4");
                    return;
                case LowFrequency::L3:
                    DrawTriangulation(COLOR_TRACK_GREEN, COLOR_LIGHT_BLACK, m_nDirection, "3");
                    return;
                case LowFrequency::L2:
                    DrawTriangulation(COLOR_TRACK_GREEN, COLOR_LIGHT_BLACK, m_nDirection, "2");
                    return;
                case LowFrequency::L:
                    DrawTriangulation(COLOR_TRACK_GREEN, COLOR_LIGHT_BLACK, m_nDirection);
                    return;
                case LowFrequency::LU:
                    DrawTriangulation(COLOR_BTN_YELLOW, COLOR_LIGHT_BLACK, m_nDirection);
                    DrawMinTriangulation(COLOR_TRACK_GREEN, m_nDirection);
                    return;
                case LowFrequency::LU2:
                    DrawTriangulation(COLOR_BTN_YELLOW, COLOR_LIGHT_BLACK, m_nDirection);
                    return;
                case LowFrequency::U:
                    DrawTriangulation(COLOR_BTN_YELLOW, COLOR_LIGHT_BLACK, m_nDirection);
                    return;
                case LowFrequency::U2:
                    DrawTriangulation(COLOR_BTN_YELLOW, COLOR_LIGHT_BLACK, m_nDirection, "2");
                    return;
                case LowFrequency::U2S:
                    DrawTriangulation(COLOR_BTN_YELLOW, COLOR_LIGHT_BLACK, m_nDirection, "2");
                    DrawTriangulationLine();
                    return;
                case LowFrequency::UU:
                    DrawTriangulation(COLOR_BTN_YELLOW, COLOR_LIGHT_BLACK, m_nDirection);
                    DrawMinTriangulation(COLOR_BTN_YELLOW, m_nDirection);
                    return;
                case LowFrequency::UUS:
                    DrawTriangulationLine();
                    DrawTriangulation(COLOR_TRACK_GREEN, COLOR_LIGHT_BLACK, m_nDirection);
                    DrawMinTriangulation(COLOR_BTN_YELLOW, m_nDirection);
                  
                    return;
                case LowFrequency::HU:
                    DrawTriangulation(COLOR_TRACK_RED, COLOR_LIGHT_BLACK, m_nDirection);
                    DrawMinTriangulation(COLOR_BTN_YELLOW, m_nDirection);
                    return;
                case LowFrequency::HB:
                    DrawTriangulationLine();
                    DrawTriangulation(COLOR_TRACK_RED, COLOR_LIGHT_BLACK, m_nDirection);
                    DrawMinTriangulation(COLOR_BTN_YELLOW, m_nDirection);
                
                    return;
                case LowFrequency::H:
                    DrawTriangulation(COLOR_TRACK_RED, COLOR_LIGHT_BLACK, m_nDirection);
                    return;
                case LowFrequency::O:
                    DrawTriangulation(COLOR_BTN_GRAY, COLOR_LIGHT_BLACK, m_nDirection);
                    return;
                default: 
                    break;
                }
            }
        }
        void StaTrack::DrawTriangulation(QColor Pencolor, QColor Textcolor, Direction m_nDirection, QString Text)
        {
            QRectF trRect1(
                Scale(QPointF(p12.x() + (p34.x() - p12.x()) / 4, p12.y() - 12)),
                QSizeF(Scale((p34.x() - p12.x()) / 3), Scale(p34.y() - p12.y() + 24))
            );
            QPointF trPoint1[3];
            if (m_nDirection == Direction::Right) {
                trPoint1[0] = trRect1.topLeft();
                trPoint1[1] = trRect1.bottomLeft();
                trPoint1[2] = QPointF(trRect1.right(), trRect1.center().y());
            }
            else if (m_nDirection == Direction::Left) {
                trPoint1[0] = trRect1.topRight();
                trPoint1[1] = trRect1.bottomRight();
                trPoint1[2] = QPointF(trRect1.left(), trRect1.center().y());
            }

            qreal lineLength = QLineF(trPoint1[0], trPoint1[1]).length(); // 边1：左上角到左下角
            m_pPainter.setPen(QPen(Pencolor));
            m_pPainter.setBrush(QBrush(Pencolor, Qt::SolidPattern));
            m_pPainter.drawPolygon(trPoint1, 3);
            QString text = Text; // 要显示的文字
            QFont font("Arial", 8); // 字体样式
            m_pPainter.setFont(font);
            m_pPainter.setPen(QPen(Textcolor)); // 文字颜色（与背景对比）

            // 计算文字居中位置
            QRectF textRect = m_pPainter.boundingRect(trRect1, Qt::AlignCenter, text);
            m_pPainter.drawText(textRect, Qt::AlignCenter, text);
        }

        void StaTrack::DrawMinTriangulation(QColor Pencolor, Direction m_nDirection) {

            QPen pen(Qt::black, 2);
            m_pPainter.setPen(pen);
            m_pPainter.setBrush(QBrush(Pencolor, Qt::SolidPattern));

            if (m_nDirection == Direction::Right) {
                QRectF trRect2(
                    Scale(QPointF(p12.x() + (p34.x() - p12.x() + 35) / 4, p12.y() - 8)),
                    QSizeF(Scale((p34.x() - p12.x() - 16) / 4), Scale(p34.y() - p12.y() + 16))
                );
                QPointF trPoint2[3];
                trPoint2[0] = trRect2.topLeft();
                trPoint2[1] = trRect2.bottomLeft();
                trPoint2[2] = QPointF(trRect2.right(), trRect2.center().y());
                m_pPainter.drawPolygon(trPoint2, 3);
            }
            else if (m_nDirection == Direction::Left) {
                QRectF trRect2(
                    Scale(QPointF(p12.x() + (p34.x() - p12.x() - 10) / 4, p12.y() - 8)),
                    QSizeF(Scale((p34.x() - p12.x()) / 4), Scale(p34.y() - p12.y() + 16))
                );
                QPointF trPoint2[3];
                trPoint2[0] = trRect2.topRight();
                trPoint2[1] = trRect2.bottomRight();
                trPoint2[2] = QPointF(trRect2.left(), trRect2.center().y());
                m_pPainter.drawPolygon(trPoint2, 3);
            }
        }
        void StaTrack::DrawTriangulationLine()
        {
            // 定义三角形
            QRectF trRect1(
                Scale(QPointF(p12.x() + (p34.x() - p12.x()) / 4, p12.y() - 12)),
                QSizeF(Scale((p34.x() - p12.x()) / 3), Scale(p34.y() - p12.y() + 24))
            );
            qreal offsetDistance=0.0;
            QPointF trPoint1[3];
            if (m_nDirection == Direction::Right) {
                offsetDistance = 5.0; // 边外偏移量
                trPoint1[0] = trRect1.topLeft();
                trPoint1[1] = trRect1.bottomLeft();
                trPoint1[2] = QPointF(trRect1.right(), trRect1.center().y());
            }
            else if (m_nDirection == Direction::Left) {
                offsetDistance = -10.0; // 边外偏移量
                trPoint1[0] = trRect1.topRight();
                trPoint1[1] = trRect1.bottomRight();
                trPoint1[2] = QPointF(trRect1.left(), trRect1.center().y());
            }

            QPointF midPoints[3] = {
                QPointF((trPoint1[0].x() + trPoint1[1].x()) / 2, (trPoint1[0].y() + trPoint1[1].y()) / 2), // 左侧边中点
                QPointF((trPoint1[1].x() + trPoint1[2].x()) / 2, (trPoint1[1].y() + trPoint1[2].y()) / 2), // 底边中点
                QPointF((trPoint1[2].x() + trPoint1[0].x()) / 2, (trPoint1[2].y() + trPoint1[0].y()) / 2)  // 右侧边中点
            };
           
            qreal lineLength = 5.0;     // 线条长度
            int linesPerEdge = 4;        // 每边线条数量
            qreal angles[] = { 30, 30 ,15, 15 }; // 每条线的角度（相对于法线方向）
            QPointF p1;
            QPointF p2;
            QPointF edgeVector;
            qreal t = 0;
            QPointF basePoint;
            QPointF startPoint;
            qreal angle; 
            QPointF endPoint;
            // 遍历三角形的每条边
            for (int edge = 0; edge < 3; ++edge) {
                // 当前边的两个顶点
                 p1 = trPoint1[edge];
                 p2 = trPoint1[(edge + 1) % 3];

                // 计算边的方向向量和法向量（指向外侧）
                edgeVector = p2 - p1;
                QPointF edgeDirection(edgeVector.x() / edgeVector.manhattanLength(),
                    edgeVector.y() / edgeVector.manhattanLength());
                QPointF outerNormal(-edgeDirection.y(), edgeDirection.x()); // 法向量（外侧）

                for (int line = 0; line < linesPerEdge; ++line) {
                    t = 0.2 + 0.2 * line;
                     basePoint = p1 + t * edgeVector;

                     startPoint = basePoint + outerNormal * offsetDistance;

                     angle = angles[line] * M_PI / 180; // 转为弧度
                    QPointF direction(
                        outerNormal.x() * qCos(angle) - outerNormal.y() * qSin(angle),
                        outerNormal.x() * qSin(angle) + outerNormal.y() * qCos(angle)
                    );

                    endPoint = startPoint + direction * lineLength;
                    m_pPainter.drawLine(QLineF(startPoint, endPoint));
                }
            }
        }
        void StaTrack::SetTrack()
        {
            m_nPowerCut = true;
        }
    }
}