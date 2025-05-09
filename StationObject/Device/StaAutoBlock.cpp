﻿
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include "CommonWidget/LeadSealDlg.h"
#include "CommonWidget/AuxiliaryMenuWnd.h"
#include "CommonWidget/SealTechnique.h"
#include "StaAutoBlock.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaAutoBlock::StaAutoBlock(QObject* pParent) 
            : StaDistant(pParent)
        {
            
        }

        StaAutoBlock::~StaAutoBlock()
        {

        }

        void StaAutoBlock::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute.insert(m_strType, mapAttrFun);
            m_mapAttribute[m_strType].insert("LampNum", [](DeviceBase* pDevice, const QString& strElement) {
                dynamic_cast<StaAutoBlock*>(pDevice)->m_nLampNum = strElement.toUInt();
                for (int i = 0; i < dynamic_cast<StaAutoBlock*>(pDevice)->m_nLampNum; i++) {
                    dynamic_cast<StaAutoBlock*>(pDevice)->m_vecStaLeaveTrack.append(StaLeaveTrack());
                    dynamic_cast<StaAutoBlock*>(pDevice)->m_vecStaLeaveTrack[i].m_nIndex = i;
                }
            });

            m_mapAttribute[m_strType].insert("isLeave", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_bLeave = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("LampCenter0", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_ptLampCenter = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("InterUsed", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_ptInterUsed = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("ZDBS_Type", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_strAutoBlockType = strElement; });

            m_mapAttribute[m_strType].insert("trackName", [](DeviceBase* pDevice, const QString& strElement) {
                if (dynamic_cast<StaAutoBlock*>(pDevice)->m_vecStaLeaveTrack.size() > dynamic_cast<StaAutoBlock*>(pDevice)->nIndex) {
                    dynamic_cast<StaAutoBlock*>(pDevice)->m_vecStaLeaveTrack[dynamic_cast<StaAutoBlock*>(pDevice)->nIndex].m_strName = strElement;
                }
            });

            m_mapAttribute[m_strType].insert("trackNamePoint", [](DeviceBase* pDevice, const QString& strElement) {
                if (dynamic_cast<StaAutoBlock*>(pDevice)->m_vecStaLeaveTrack.size() > dynamic_cast<StaAutoBlock*>(pDevice)->nIndex) {
                    dynamic_cast<StaAutoBlock*>(pDevice)->m_vecStaLeaveTrack[dynamic_cast<StaAutoBlock*>(pDevice)->nIndex].m_ptName = QStringToQPointF(strElement);
                }
            });

            m_mapAttribute[m_strType].insert("CommLeaveTrack", [](DeviceBase* pDevice, const QString& strElement) {
                if (dynamic_cast<StaAutoBlock*>(pDevice)->m_vecStaLeaveTrack.size() > dynamic_cast<StaAutoBlock*>(pDevice)->nIndex) {
                    dynamic_cast<StaAutoBlock*>(pDevice)->m_vecStaLeaveTrack[dynamic_cast<StaAutoBlock*>(pDevice)->nIndex].m_rcTrack = QStringToQRectF(strElement);
                }
            });

            m_mapAttribute[m_strType].insert("m_Direction", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_strDirection = strElement; });
            m_mapAttribute[m_strType].insert("m_RoutePoint", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_ptRouteWnd = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("m_FrameRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_rcFrame = QStringToQRectF(strElement); });

            m_mapAttribute[m_strType].insert("m_Button", [](DeviceBase* pDevice, const QString& strElement) {
                QJsonParseError error;
                QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
                if (josnDoc.isNull()) {
                    qDebug() << "无效的JSON格式:" << error.errorString();
                    return;
                }
                if (josnDoc.isObject()) {
                    for (const QString& key : josnDoc.object().keys()) {
                        m_mapAttribute[pDevice->getStrType()][key](pDevice, QJsonDocument(josnDoc.object().value(key).toObject()).toJson());
                    }
                }
            });
            m_mapAttribute[m_strType].insert("ZFZ", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->AddBlockBtn(0x01, "总辅助", strElement); });
            m_mapAttribute[m_strType].insert("JCFZ", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->AddBlockBtn(0x02, "接车辅助", strElement); });
            m_mapAttribute[m_strType].insert("FCFZ", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->AddBlockBtn(0x04, "发车辅助", strElement); });
            m_mapAttribute[m_strType].insert("YXGF", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->AddBlockBtn(0x08, "允许改方", strElement); });

            m_mapAttribute[m_strType].insert("m_Lamp", [](DeviceBase* pDevice, const QString& strElement) {
                QJsonParseError error;
                QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
                if (josnDoc.isNull()) {
                    qDebug() << "无效的JSON格式:" << error.errorString();
                    return;
                }
                if (josnDoc.isObject()) {
                    for (const QString& key : josnDoc.object().keys()) {
                        m_mapAttribute[pDevice->getStrType()][key](pDevice, QJsonDocument(josnDoc.object().value(key).toObject()).toJson());
                    }
                }
            });
            m_mapAttribute[m_strType].insert("Allow", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->AddBlockLamp("允许发车", strElement); });
            m_mapAttribute[m_strType].insert("Assisted", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->AddBlockLamp("辅助办理", strElement); });
            m_mapAttribute[m_strType].insert("Supervise", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->AddBlockLamp("监督区间", strElement); });
            m_mapAttribute[m_strType].insert("Section", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->AddBlockLamp("区间逻辑检查", strElement); });
            InitArrowAttributeMap(m_strType, m_mapAttribute);
            return StaDistant::InitAttributeMap();
        }

        void StaAutoBlock::timerEvent(QTimerEvent* event)
        {
            if (event->timerId() == m_nTimerIdJF) {
                m_nBtnState &= (0xFF ^ 0x20);
                killTimer(m_nTimerIdJF);
            }
            if (event->timerId() == m_nTimerIdFF) {
                m_nBtnState &= (0xFF ^ 0x40);
                killTimer(m_nTimerIdFF);
            }
            return DeviceBase::timerEvent(event);
        }

        void StaAutoBlock::InitDeviceAttribute()
        {
            TrainFrame* pTrainFrame = nullptr;
            for (StaLeaveTrack& track : m_vecStaLeaveTrack) {
                pTrainFrame = new TrainFrame();
                int nWidth = track.m_rcTrack.width() > 64 ? 64 : track.m_rcTrack.width();
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
                for (StaBlockBtn& btnBlock : m_vecBlockBtn) {
                    DrawButton(m_pPainter, Scale(btnBlock.m_rcBtn), (m_nBtnState & (btnBlock.m_nState << 4)) ? COLOR_BTN_RED : COLOR_BTN_DEEPGRAY, m_nBtnState & btnBlock.m_nState);
                }
                
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
            if (!m_rcFrame.isEmpty()) {
                m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1));
                m_pPainter.setBrush(Qt::NoBrush);
                m_pPainter.drawRect(m_rcFrame);
            }
            
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
            for (StaBlockLamp& lamp : m_vecBlockLamp) { 
                m_pPainter.drawText(Scale(lamp.m_rcName), lamp.m_strName, QTextOption(Qt::AlignCenter));
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
            
            QFontMetrics  fontMetrics(font);
            for (StaBlockBtn& btnBlock : m_vecBlockBtn) {
                m_pPainter.setPen(btnBlock.m_cTextColor);
                m_pPainter.drawText(Scale(btnBlock.m_rcName), btnBlock.m_strName, QTextOption(Qt::AlignCenter));
            }
        }

        void StaAutoBlock::DrawRoutePreviewWnd()
        {
            QRectF rcTeainNum[3];
            rcTeainNum[0] = { m_ptRouteWnd.x(), m_ptRouteWnd.y(), 120, 32 };
            rcTeainNum[1] = { m_ptRouteWnd.x(), m_ptRouteWnd.y() + 32, 120, 32 };
            rcTeainNum[2] = { m_ptRouteWnd.x(), m_ptRouteWnd.y() + 64, 120, 32 };
            
            QVector<StaTrainRoute*> vecTrainRoute;
            for (StaTrainRoute* pRoute : MainStation()->TrainRouteList()) {
                if (pRoute->m_strSignal == m_strDirection) {
                    vecTrainRoute.append(pRoute);
                }
            }
            int nIndex = 0;
            StaTrain* pTrain = nullptr;
            QFont font = m_pPainter.font();
            font.setPointSizeF(14);
            m_pPainter.setFont(font);
            StaTrainRoute* pRoute = nullptr;
            while (nIndex < 3) {
                m_pPainter.setPen(QPen(Qt::white));
                m_pPainter.setBrush(Qt::NoBrush);
                m_pPainter.drawRect(Scale(rcTeainNum[nIndex]));
                if (nIndex < vecTrainRoute.size()) {
                    
                    pRoute = vecTrainRoute[nIndex];
                    if (!pRoute->m_bAutoTouch) {//人工触发-false
                        m_pPainter.setPen(QPen(COLOR_LIGHT_RED, 1));
                    }
                    else if(pRoute->m_bAutoTouch) {//自动触发-true
                        m_pPainter.setPen(QPen(COLOR_BTN_YELLOW, 1));
                    }
               
                    if (pRoute->m_nRouteState == 2) {//触发成功
                        m_pPainter.setPen(QPen(COLOR_BTN_GREEN_TG, 1));
                    }
                    pTrain = MainStation()->getStaTrainById(vecTrainRoute[nIndex]->m_nTrainId);
                    if (!pTrain) {
                        nIndex++;
                        continue;
                    }
                    if (pRoute->m_bArrivaRoute)//接发类型 (接车-true 发车-false 通过)
                    {
                        m_pPainter.drawText(Scale(rcTeainNum[nIndex]), pTrain->m_strTrainNum + "  J" + pRoute->m_strTrack, QTextOption(Qt::AlignCenter));
                    }
                    else if (!pRoute->m_bArrivaRoute) {
                        m_pPainter.drawText(Scale(rcTeainNum[nIndex]), pTrain->m_strTrainNum + "  F" + pRoute->m_strTrack, QTextOption(Qt::AlignCenter));
                    }
                    else {
                        m_pPainter.drawText(Scale(rcTeainNum[nIndex]), pTrain->m_strTrainNum + "  T" + pRoute->m_strTrack, QTextOption(Qt::AlignCenter));
                    }     
                }
                nIndex++;
            } 
        }

        bool StaAutoBlock::Contains(const QPoint& ptPos)
        {
            bool bContains = false;
            for (StaBlockBtn& btnBlock : m_vecBlockBtn) {
                bContains |= Scale(btnBlock.m_rcBtn).contains(ptPos);
            }
            return bContains;
        }

        bool StaAutoBlock::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                bool bContains = false;
                for (StaBlockBtn& btnBlock : m_vecBlockBtn) {
                    if (Scale(btnBlock.m_rcBtn).contains(ptPos)) {
                        m_nSelectBtnType = btnBlock.m_nState;
                        return true;
                    }
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
            m_mapClickEvent[m_strType].insert(CTCWindows::FunType::FunBtn, [](DeviceBase* pDevice) {
                if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                    dynamic_cast<StaAutoBlock*>(pDevice)->OnButtonClick();
                }
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
                                Station::MainStationObject* Station = Station::MainStation();
                                QString stationName = Station->getStationName();
                                CTCWindows::SealTechnique::InsertSealRecord(stationName, "分路不良");
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
                m_nFirstBtnType = 5;
                qDebug() << "BtnState" << m_nBtnState << m_nSelectBtnType << (m_nBtnState ^ (m_nSelectBtnType << 4)) + m_nSelectBtnType;
                m_nBtnState &= 0xF0;
                m_nBtnState |= m_nSelectBtnType;
                
                
                switch (m_nBtnState & 0x0F)
                {
                case 0x01: 
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType((m_nBtnState & 0x10) ? 
                        CTCWindows::OperObjType::TotalAuxUp : CTCWindows::OperObjType::TotalAux);  
                    break;
                case 0x02 : 
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::PickUpAux);      
                    m_nTimerIdJF = startTimer(25000);
                    break;
                case 0x04 : 
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::DepartureAux);   
                    m_nTimerIdFF = startTimer(25000);
                    break;
                case 0x08 : 
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::DirectionChange); 
                    break;
                default: break;
                }
                m_nBtnState ^= m_nSelectBtnType << 4;
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
            case 0x01: *cColor1 = COLOR_LIGHT_YELLOW;   break;
            case 0x02: *cColor1 = COLOR_LIGHT_GREEN;    break;
            default:   *cColor1 = COLOR_LIGHT_BLACK;    break;
            }
            switch (m_nArrowState & 0x0c) {
            case 0x04: *cColor2 = COLOR_LIGHT_GREEN;    break;
            case 0x08: *cColor2 = COLOR_LIGHT_YELLOW;   break;
            default:   *cColor2 = COLOR_LIGHT_BLACK;    break;
            }
        }

        void StaAutoBlock::OrderClear(bool bClearTwinkle)
        {
            if (bClearTwinkle) {
                m_nBtnState ^= m_nSelectBtnType << 4;
                killTimer(m_nTimerIdJF);
                killTimer(m_nTimerIdFF);
            }
            BtnStateReset();
        }

        void StaAutoBlock::setVollover(const QPointF& ptBase)
        {

        }

        void StaAutoBlock::ResetDevState()
        {

        }

        void StaAutoBlock::AddBlockBtn(int nState, QString strType, const QString& strElement)
        {
            QJsonParseError error;
            QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
            if (josnDoc.isNull()) {
                qDebug() << "无效的JSON格式:" << error.errorString();
                return;
            }
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            QFontMetricsF  fontMetrics(font);
            StaBlockBtn blockBtn;
            blockBtn.m_nState = nState;
            blockBtn.m_strName = m_strDirection + strType;
            blockBtn.m_rcName = QRectF(QStringToQPointF(josnDoc.object().value("pName").toString()), fontMetrics.size(Qt::TextSingleLine, blockBtn.m_strName));
            blockBtn.m_rcBtn = QRectF(QStringToQPointF(josnDoc.object().value("m_ptBtn").toString()), QSizeF(15, 15));
            blockBtn.m_ptCountdown = QStringToQPointF(josnDoc.object().value("m_ptCountdown").toString());
            switch (josnDoc.object().value("m_textColor").toInt())
            {
            case 0:  blockBtn.m_cTextColor = Qt::white;  break;
            case 1:  blockBtn.m_cTextColor = Qt::red;    break;
            case 2:  blockBtn.m_cTextColor = Qt::yellow; break;
            case 3:  blockBtn.m_cTextColor = Qt::green;  break;
            default: blockBtn.m_cTextColor = Qt::white;  break;
            }
            m_vecBlockBtn.append(blockBtn);
        }

        void StaAutoBlock::AddBlockLamp(QString strType, const QString& strElement)
        {
            QJsonParseError error;
            QJsonDocument josnDoc = QJsonDocument::fromJson(strElement.toUtf8(), &error);
            if (josnDoc.isNull()) {
                qDebug() << "无效的JSON格式:" << error.errorString();
                return;
            }
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            QFontMetricsF  fontMetrics(font);
            StaBlockLamp blockLamp;
            blockLamp.m_strName = m_strDirection + strType;
            blockLamp.m_rcName = QRectF(QStringToQPointF(josnDoc.object().value("pName").toString()), fontMetrics.size(Qt::TextSingleLine, blockLamp.m_strName));
            blockLamp.m_rcLamp = QRectF(QStringToQPointF(josnDoc.object().value("m_ptLamp").toString()), QSizeF(15, 15));
            switch (josnDoc.object().value("m_textColor").toInt())
            {
            case 0:  blockLamp.m_cTextColor = Qt::white;  break;
            case 1:  blockLamp.m_cTextColor = Qt::red;    break;
            case 2:  blockLamp.m_cTextColor = Qt::yellow; break;
            case 3:  blockLamp.m_cTextColor = Qt::green;  break;
            default: blockLamp.m_cTextColor = Qt::white;  break;
            }
            m_vecBlockLamp.append(blockLamp);
        }

        void StaAutoBlock::setLeaveTrackState(int nState)
        {
            for (StaLeaveTrack& pTrack : m_vecStaLeaveTrack) {
                pTrack.m_nState = nState & 0x03;
                nState = nState >> 2;
            }
        }

        bool StaAutoBlock::IsHaveAllowBtn()
        {
            bool bHave = false;
            for (const StaBlockBtn& btnBlock : m_vecBlockBtn) {
                if (btnBlock.m_strName.contains("允许发车")) {
                    bHave = true;
                }
            }
            return bHave;
        }
    }
}