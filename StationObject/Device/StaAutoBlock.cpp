#include "StaAutoBlock.h"
#include "Global.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include "CommonWidget/LeadSealDlg.h"
#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaAutoBlock::StaAutoBlock(QObject* pParent) 
            : DeviceArrow(m_mapAttribute), StaDistant(pParent)
        {
            m_pParent = pParent;
            m_mapAttribute.insert("ZFZ1_rect", [&](const QString& strElement) {
                m_rcZFZBtn = QStringToQRectF(strElement);
                m_rcZFZBtn.setWidth(17);
                m_rcZFZBtn.setHeight(17);
            });

            m_mapAttribute.insert("FFZ1_rect", [&](const QString& strElement) {
                m_rcFCFZBtn = QStringToQRectF(strElement);
                m_rcFCFZBtn.setWidth(17);
                m_rcFCFZBtn.setHeight(17);
            });

            m_mapAttribute.insert("JFZ1_rect", [&](const QString& strElement) {
                m_rcJCFZBtn = QStringToQRectF(strElement);
                m_rcJCFZBtn.setWidth(17);
                m_rcJCFZBtn.setHeight(17);
            });

            m_mapAttribute.insert("GF1_rect", [&](const QString& strElement) {
                m_rcGFBtn = QStringToQRectF(strElement);
                m_rcGFBtn.setWidth(17);
                m_rcGFBtn.setHeight(17);
            });

            m_mapAttribute.insert("FZBSD_rect", [&](const QString& strElement) {
                m_rcFZLight = QStringToQRectF(strElement);
                m_rcFZLight.setWidth(13);
                m_rcFZLight.setHeight(13);
            });

            m_mapAttribute.insert("ZFZ_Text", [&](const QString& strElement) { m_ptZFZText = QStringToQPointF(strElement); });
            m_mapAttribute.insert("FFZ_Text", [&](const QString& strElement) { m_ptFCFZText = QStringToQPointF(strElement); });
            m_mapAttribute.insert("JFZ_Text", [&](const QString& strElement) { m_ptJCFZText = QStringToQPointF(strElement); });
            m_mapAttribute.insert("FZ_Text", [&](const QString& strElement) { m_ptFZText = QStringToQPointF(strElement); });
            m_mapAttribute.insert("GF_Text", [&](const QString& strElement) { m_ptGFText = QStringToQPointF(strElement); });

            m_mapAttribute.insert("LampNum", [&](const QString& strElement) {
                m_nLampNum = strElement.toUInt();
                for (int i = 0; i < m_nLampNum; i++) {
                    m_vecStaLeaveTrack.append(StaLeaveTrack());
                    m_vecStaLeaveTrack[i].m_nIndex = i;
                }
            });

            m_mapAttribute.insert("isLeave", [&](const QString& strElement) { m_bLeave = strElement.toUInt(); });
            m_mapAttribute.insert("LampCenter0", [&](const QString& strElement) { m_ptLampCenter = QStringToQPointF(strElement); });
            m_mapAttribute.insert("InterUsed", [&](const QString& strElement) { m_ptInterUsed = QStringToQPointF(strElement); });
            m_mapAttribute.insert("ZDBS_Type", [&](const QString& strElement) { m_strAutoBlockType = strElement; });
            
            m_mapAttribute.insert("trackName", [&](const QString& strElement) {
                if (m_vecStaLeaveTrack.size() > nIndex) {
                    m_vecStaLeaveTrack[nIndex].m_strName = strElement;
                }
            });

            m_mapAttribute.insert("trackNamePoint", [&](const QString& strElement) {
                if (m_vecStaLeaveTrack.size() > nIndex) {
                    m_vecStaLeaveTrack[nIndex].m_ptName = QStringToQPointF(strElement);
                }
            });

            m_mapAttribute.insert("CommLeaveTrack", [&](const QString& strElement) {
                if (m_vecStaLeaveTrack.size() > nIndex) {
                    m_vecStaLeaveTrack[nIndex].m_rcTrack = QStringToQRectF(strElement);
                }
            });

            m_mapAttribute.insert("ZFZ_JS", [&](const QString& strElement) { m_ptZFZ_JS = QStringToQPointF(strElement); });
            m_mapAttribute.insert("FFZ_JS", [&](const QString& strElement) { m_ptJCFZ_JS = QStringToQPointF(strElement); });
            m_mapAttribute.insert("JFZ_JS", [&](const QString& strElement) { m_ptFCFZ_JS = QStringToQPointF(strElement); });

            m_mapAttribute.insert("m_direction", [&](const QString& strElement) { m_strDirection = strElement; });
            m_mapAttribute.insert("m_routePoint", [&](const QString& strElement) { m_ptRouteWnd = QStringToQPointF(strElement); });
            
            m_mapAttribute.insert("Allow", [&](const QString& strElement) {
                QJsonParseError error;
                QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
                if (josnDoc.isNull()) {
                    qDebug() << "无效的JSON格式:" << error.errorString();
                    return;
                }
                StaBlockLamp blockLamp;
                blockLamp.m_strName = "允许发车"; 
                blockLamp.m_rcName = QStringToQRectF(josnDoc.object().value("m_textRect").toString());
                blockLamp.m_rcLamp = QStringToQRectF(josnDoc.object().value("m_lampRect").toString());
                m_mapBlockLamp.insert("Allow", blockLamp);
            });

            m_mapAttribute.insert("Assisted", [&](const QString& strElement) {
                QJsonParseError error;
                QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
                if (josnDoc.isNull()) {
                    qDebug() << "无效的JSON格式:" << error.errorString();
                    return;
                }
                StaBlockLamp blockLamp;
                blockLamp.m_strName = "辅助办理";
                blockLamp.m_rcName = QStringToQRectF(josnDoc.object().value("m_textRect").toString());
                blockLamp.m_rcLamp = QStringToQRectF(josnDoc.object().value("m_lampRect").toString());
                m_mapBlockLamp.insert("Assisted", blockLamp);
            });

            m_mapAttribute.insert("Supervise", [&](const QString& strElement) {
                QJsonParseError error;
                QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
                if (josnDoc.isNull()) {
                    qDebug() << "无效的JSON格式:" << error.errorString();
                    return;
                }
                StaBlockLamp blockLamp;
                blockLamp.m_strName = "监督区间";
                blockLamp.m_rcName = QStringToQRectF(josnDoc.object().value("m_textRect").toString());
                blockLamp.m_rcLamp = QStringToQRectF(josnDoc.object().value("m_lampRect").toString());
                m_mapBlockLamp.insert("Supervise", blockLamp);
            });

            m_mapAttribute.insert("Section", [&](const QString& strElement) {
                QJsonParseError error;
                QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
                if (josnDoc.isNull()) {
                    qDebug() << "无效的JSON格式:" << error.errorString();
                    return;
                }
                StaBlockLamp blockLamp;
                blockLamp.m_strName = "区间逻辑检查";
                blockLamp.m_rcName = QStringToQRectF(josnDoc.object().value("m_textRect").toString());
                blockLamp.m_rcLamp = QStringToQRectF(josnDoc.object().value("m_lampRect").toString());
                m_mapBlockLamp.insert("Section", blockLamp);
            });
        }

        StaAutoBlock::~StaAutoBlock()
        {

        }

        void StaAutoBlock::InitDeviceAttribute()
        {
            if (m_rcFZLight.x() < p11.x()) {
                p11.setX(p11.x() + 48);
                p12.setX(p12.x() + 48);
                p13.setX(p13.x() + 48);
                p14.setX(p14.x() + 48);
                p15.setX(p15.x() + 48);
                p16.setX(p16.x() + 48);
                p17.setX(p17.x() + 48);
            }
            else if (m_rcFZLight.x() > p11.x()) {
                p21.setX(p21.x() - 48);
                p22.setX(p22.x() - 48);
                p23.setX(p23.x() - 48);
                p24.setX(p24.x() - 48);
                p25.setX(p25.x() - 48);
                p26.setX(p26.x() - 48);
                p27.setX(p27.x() - 48);
            }
            TrainFrame* pTrainFrame = nullptr;
            for (StaLeaveTrack& track : m_vecStaLeaveTrack) {
                pTrainFrame = new TrainFrame();
                int nWidth = track.m_rcTrack.width() > 80 ? 80 : track.m_rcTrack.width();
                pTrainFrame->m_rcFrame = QRect(track.m_rcTrack.center().x() - nWidth / 2,
                    track.m_rcTrack.center().y() - 15, nWidth, 30);
                m_vecTrainFrame.append(pTrainFrame);
            }
        }

        void StaAutoBlock::Draw(bool isMulti)
        {
            m_bShowName = MainStation()->IsVisible(VisibleDev::direction);
            DrawArrow(m_pPainter);
            if (m_bMainStation) {
                DrawButton(m_pPainter, Scale(m_rcZFZBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x01);
                DrawButton(m_pPainter, Scale(m_rcJCFZBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x02);
                DrawButton(m_pPainter, Scale(m_rcFCFZBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x04);
                DrawButton(m_pPainter, Scale(m_rcGFBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x08);
            }
            for (StaLeaveTrack& track : m_vecStaLeaveTrack) {
                DrawLeaveTrack(track);
            }
            //绘制车次窗
            DrawTrainFrame(m_pPainter);
            //绘制车次
            DrawTrain(m_pPainter);
            //车次预告窗
            DrawRoutePreviewWnd();
            
            return StaDistant::Draw(isMulti);
        }

        void StaAutoBlock::DrawLeaveTrack(const StaLeaveTrack& track)
        {
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            QFontMetrics  fontMetrics(font);
            m_pPainter.setFont(font);//设置字体
            if (MainStation()->IsVisible(VisibleDev::sectionName)) {
                //绘制股道名称
                m_pPainter.setPen(Qt::white);
                m_pPainter.drawText(Scale(QRectF(track.m_ptName, fontMetrics.size(Qt::TextSingleLine, track.m_strName))), track.m_strName, QTextOption(Qt::AlignCenter));
            }
            //绘制股道
            m_pPainter.setPen(QPen(getTrackColor(track.m_nIndex), Scale(TRACK_WIDTH), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            m_pPainter.drawLine(Scale(QPoint(track.m_rcTrack.left(), m_ptInterUsed.y())),
                Scale(QPoint(track.m_rcTrack.right(), m_ptInterUsed.y())));
            //绘制绝缘节
            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            m_pPainter.drawLine(Scale(track.m_rcTrack.topLeft()), Scale(track.m_rcTrack.bottomLeft())); //绘制轨道区段左侧绝缘节
            m_pPainter.drawLine(Scale(track.m_rcTrack.topRight()), Scale(track.m_rcTrack.bottomRight())); //绘制轨道区段右侧绝缘节
        }

        void StaAutoBlock::DrawLight()
        {
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 2));

            m_pPainter.setRenderHint(QPainter::Antialiasing, true);
            for (StaBlockLamp& lamp : m_mapBlockLamp) {
                m_pPainter.drawText(Scale(lamp.m_rcName), m_strDirection + lamp.m_strName, QTextOption(Qt::AlignCenter));
                //m_pPainter.drawRect(Scale(lamp.m_rcName));
                m_pPainter.setBrush((lamp.m_nState & 0x10) ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK);
                m_pPainter.drawEllipse(Scale(lamp.m_rcLamp));
            }
            m_pPainter.setRenderHint(QPainter::Antialiasing, false);
        }

        void StaAutoBlock::DrawText()
        {
            if (!MainStation()->IsVisible(VisibleDev::funButtonName)) {
                return;
            }
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(Qt::yellow);

            QFontMetrics  fontMetrics(font);
            //总辅助
            m_pPainter.drawText(Scale(QRectF(m_ptZFZText, fontMetrics.size(Qt::TextSingleLine, "总辅助"))), "总辅助", QTextOption(Qt::AlignCenter));
            //接车辅助
            m_pPainter.drawText(Scale(QRectF(m_ptJCFZText, fontMetrics.size(Qt::TextSingleLine, "接车辅助"))), "接车辅助", QTextOption(Qt::AlignCenter));
            //发车辅助
            m_pPainter.drawText(Scale(QRectF(m_ptFCFZText, fontMetrics.size(Qt::TextSingleLine, "发车辅助"))), "发车辅助", QTextOption(Qt::AlignCenter));
            if (m_ptGFText != QPointF()) {
                //改方
                m_pPainter.drawText(Scale(QRectF(m_ptGFText, fontMetrics.size(Qt::TextSingleLine, "允许改方"))), "允许改方", QTextOption(Qt::AlignCenter));
            }
            
        }

        void StaAutoBlock::DrawRoutePreviewWnd()
        {
            QFont font = m_pPainter.font();
            font.setPointSizeF(14);
            m_pPainter.setFont(font);
            QRectF rcTeainNum[3];
            rcTeainNum[0] = { m_ptRouteWnd.x(), m_ptRouteWnd.y(), 120, 32 };
            rcTeainNum[1] = { m_ptRouteWnd.x(), m_ptRouteWnd.y() + 32, 120, 32 };
            rcTeainNum[2] = { m_ptRouteWnd.x(), m_ptRouteWnd.y() + 64, 120, 32 };
            
            int m_nSignalCode = -1; //进/出站信号机编号
            QString m_strSignal; //进/出站信号机
            QVector<StaTrainRoute*> vecTrainRoute;
            for (StaTrainRoute* pRoute : MainStation()->TrainRouteList()) {
                if (pRoute->m_strSignal == m_strDirection) {
                    vecTrainRoute.append(pRoute);
                }
            }
            int nIndex = 0;
            StaTrain* pTrain = nullptr;
            
            while (nIndex < 3) {
                m_pPainter.setPen(QPen(COLOR_LIGHT_WHITE, 1));
                m_pPainter.drawRect(Scale(rcTeainNum[nIndex]));
                if (nIndex < vecTrainRoute.size()) {
                    pTrain = MainStation()->getStaTrainById(vecTrainRoute[nIndex]->m_nTrainId);
                    m_pPainter.setPen(QPen(COLOR_LIGHT_RED, 1));
                    m_pPainter.drawText(Scale(rcTeainNum[nIndex]), pTrain->m_strTrainNum + "  T", QTextOption(Qt::AlignCenter));
                }
                nIndex++;
            }
        }

        bool StaAutoBlock::Contains(const QPoint& ptPos)
        {
            return m_rcZFZBtn.contains(ptPos) || m_rcJCFZBtn.contains(ptPos) || m_rcFCFZBtn.contains(ptPos);
        }

        bool StaAutoBlock::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                if (m_rcZFZBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x01;
                    return true;
                }
                else if (m_rcJCFZBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x02;
                    return true;
                }
                else if (m_rcFCFZBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x04;
                    return true;
                }
                else if (m_rcGFBtn.contains(ptPos)) {
                    m_nSelectBtnType = 0x08;
                    return true;
                }
            }
            return false;
        }

        void StaAutoBlock::onMouseMoveToDevice(const QPoint& ptPos)
        {
            onMouseMoveToTrainFrame(ptPos);
            return DeviceBase::onMouseMoveToDevice(ptPos);
        }

        void StaAutoBlock::InitClickEvent()
        {
            m_mapClickEvent.insert(CTCWindows::FunType::FunBtn, [&]() {
                OnButtonClick(this);
            });
        }

        void StaAutoBlock::ShowDeviceMenu(const QPoint& ptPos)
        {
            for (TrainFrame* pTrainFrame : m_vecTrainFrame) {
                if (pTrainFrame->m_rcFrame.contains(ptPos)) {   //车次号
                    ShowTrainMenu(QCursor::pos(), m_nCode);
                    return;
                }
            }
            for (StaLeaveTrack& track : m_vecStaLeaveTrack) {
                if (track.m_rcTrack.contains(ptPos)) {
                    QMenu* pMenu = new QMenu();
                    pMenu->setAttribute(Qt::WA_DeleteOnClose);
                    QAction* pAction1 = new QAction("封锁/解封");
                    pMenu->addAction(pAction1);
                    QObject::connect(pAction1, &QAction::triggered, [=]() {
                        if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"封锁/解封[区间轨道:%1]\"命令吗?").arg(track.m_strName), "确定", "取消") == 0) {
                            MainStation()->AddSelectDevice(this);
                            MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, (track.m_nState & SECTION_STATE_BLOCK) ? 0x0b : 0x0a, 0x12);
                        }
                        });

                    QAction* pAction2 = new QAction("分路不良");
                    pMenu->addAction(pAction2);
                    QObject::connect(pAction2, &QAction::triggered, [=]() {
                        if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"分路不良[区间轨道:%1]\"命令吗?").arg(track.m_strName), "确定", "取消") == 0) {
                            if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                                MainStation()->AddSelectDevice(this);
                                MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x11, 0x12);
                            }
                        }
                    });
                    pMenu->exec(QCursor::pos());
                }
            }
        }

        void StaAutoBlock::SetBtnState()
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                m_nBtnState = m_nSelectBtnType;
                m_nFirstBtnType = 5;
                switch (m_nBtnState)
                {
                case 0x01 : CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::TotalAux);        break;
                case 0x02 : CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::PickUpAux);       break;
                case 0x04 : CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::DepartureAux);    break;
                case 0x08 : CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::DirectionChange); break;
                default: break;
                }
            }
        }

        QColor StaAutoBlock::getTrackColor(int nIndex)
        {
            if (m_vecStaLeaveTrack.at(nIndex).m_nState == 0x02) {
                return COLOR_TRACK_RED;
            }
            else {
                return COLOR_TRACK_BLUE;
            }
        }

        void StaAutoBlock::getArrowColor()
        {
            QColor* cColor1 = &m_cColor1;
            QColor* cColor2 = &m_cColor2;
            if (m_bUpDown) {
                cColor2 = &m_cColor1;
                cColor1 = &m_cColor2;
            }

            switch (m_nArrowState & 0x03) {
            case 0x01: *cColor1 = COLOR_LIGHT_GREEN;    break;
            case 0x02: *cColor1 = COLOR_LIGHT_YELLOW;   break;
            default:   *cColor1 = COLOR_LIGHT_BLACK;    break;
            }
            switch (m_nArrowState & 0x0c) {
            case 0x04: *cColor2 = COLOR_LIGHT_GREEN;    break;
            case 0x08: *cColor2 = COLOR_LIGHT_YELLOW;   break;
            default:   *cColor2 = COLOR_LIGHT_BLACK;    break;
            }
        }

        void StaAutoBlock::OrderClear()
        {
            BtnStateReset();
        }

        void StaAutoBlock::setVollover(const QPointF& ptBase)
        {

        }

        void StaAutoBlock::ResetDevState()
        {

        }

        void StaAutoBlock::setLeaveTrackState(int nState)
        {
            for (StaLeaveTrack& pTrack : m_vecStaLeaveTrack) {
                pTrack.m_nState = nState & 0x03;
                nState = nState >> 2;
            }
        }
    }
}