#include "StaSemiAutoBlock.h"
#include "Global.h"
#include <QJsonDocument>
#include <QJsonObject>

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaSemiAutoBlock::StaSemiAutoBlock(QObject* pParent) 
            : StaDistant(pParent)
        {
            
        }

        StaSemiAutoBlock::~StaSemiAutoBlock()
        {

        }

        void StaSemiAutoBlock::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute.insert(m_strType, mapAttrFun);
            m_mapAttribute[m_strType].insert("BSType", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSemiAutoBlock*>(pDevice)->m_nBlockType = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("m_Direction", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSemiAutoBlock*>(pDevice)->m_strDirection = strElement; });
            m_mapAttribute[m_strType].insert("m_FrameRect", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSemiAutoBlock*>(pDevice)->m_rcFrame = QStringToQRectF(strElement); });
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

            m_mapAttribute[m_strType].insert("BS", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSemiAutoBlock*>(pDevice)->AddBlockBtn("闭塞", strElement); });
            m_mapAttribute[m_strType].insert("SG", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSemiAutoBlock*>(pDevice)->AddBlockBtn("事故", strElement); });
            m_mapAttribute[m_strType].insert("FY", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSemiAutoBlock*>(pDevice)->AddBlockBtn("复原", strElement); });
            InitArrowAttributeMap(m_strType, m_mapAttribute);
            return StaDistant::InitAttributeMap();
        }
        void StaSemiAutoBlock::DrawRoutePreviewWnd()
        {
            QFont font = m_pPainter.font();
            font.setPointSizeF(14);
            QPen pen;
            pen.setColor(Qt::white);
            pen.setStyle(Qt::SolidLine);
            m_pPainter.setPen(pen);
            m_pPainter.setFont(font);
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
     
            while (nIndex < 3) {
                m_pPainter.setBrush(Qt::NoBrush);
                m_pPainter.drawRect(Scale(rcTeainNum[nIndex]));
                if (nIndex < vecTrainRoute.size()) {
                    pTrain = MainStation()->getStaTrainById(vecTrainRoute[nIndex]->m_nTrainId);
                    StaTrainRoute* pRoute = vecTrainRoute[nIndex];
                    if (!pRoute->m_bAutoTouch) {//人工触发-false
                        m_pPainter.setPen(QPen(COLOR_LIGHT_RED, 1));
                    }
                    else if (pRoute->m_bAutoTouch) {//自动触发-true
                        m_pPainter.setPen(QPen(COLOR_BTN_YELLOW, 1));
                    }

                    if (pRoute->m_nRouteState == 2) {//触发成功
                        m_pPainter.setPen(QPen(COLOR_BTN_GREEN_TG, 1));
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

        void StaSemiAutoBlock::Draw(bool isMulti)
        {
            DrawArrow(m_pPainter);
            DrawRoutePreviewWnd();
            if (m_bMainStation) {
                int nState = 0x01;
                for (StaBlockBtn& btnBlock : m_vecBlockBtn) {
                   
                    DrawButton(m_pPainter, Scale(btnBlock.m_rcBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & 0x01);
                    nState *= 2;
                }
            }
            if (!m_rcFrame.isEmpty()) {
                m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1));
                m_pPainter.setBrush(Qt::NoBrush);
                m_pPainter.drawRect(m_rcFrame);
            }
            return StaDistant::Draw(isMulti);
        }

        void StaSemiAutoBlock::DrawLight()
        {
        
        }

        void StaSemiAutoBlock::DrawText()
        {
            if (!MainStation()->IsVisible(VisibleDev::funButtonName)) {
                return;
            }
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            m_pPainter.setFont(font);//设置字体

            QFontMetrics  fontMetrics(font);
            int nState = 0x01;
            for (StaBlockBtn& btnBlock : m_vecBlockBtn) {

                m_pPainter.setPen(btnBlock.m_cTextColor);
                m_pPainter.drawText(Scale(btnBlock.m_rcName), btnBlock.m_strName, QTextOption(Qt::AlignCenter));
                nState *= 2;
            }
        }

        bool StaSemiAutoBlock::Contains(const QPoint& ptPos)
        {
            bool bContains = false;
            for (StaBlockBtn& btnBlock : m_vecBlockBtn) {
                bContains |= Scale(btnBlock.m_rcBtn).contains(ptPos);
            }
            return bContains;
        }

        bool StaSemiAutoBlock::IsMouseWheel(const QPoint& ptPos)
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                int nState = 0x01;
                for (StaBlockBtn& btnBlock : m_vecBlockBtn) {
                    if (Scale(btnBlock.m_rcBtn).contains(ptPos)) {
                        m_nSelectBtnType = nState;
                        return true;
                    }
                    nState *= 2;
                }
            }
            return false;
        }

        void StaSemiAutoBlock::InitClickEvent()
        {
            m_mapClickEvent[m_strType].insert(CTCWindows::FunType::FunBtn, [](DeviceBase* pDevice) {
                dynamic_cast<StaSemiAutoBlock*>(pDevice)->OnButtonClick();
            });
        }

        void StaSemiAutoBlock::SetBtnState()
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                m_nBtnState = m_nSelectBtnType;
                m_nFirstBtnType = 5;
                switch (m_nBtnState)
                {
                case 0x01: CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Blockage);    break;
                case 0x02: CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Restoration); break;
                case 0x04: CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Accident);    break;
                default: break;
                }
            }
        }

        void StaSemiAutoBlock::getArrowColor()
        {
            QColor* cColor1 = &m_cColor1;
            QColor* cColor2 = &m_cColor2;
            if (m_bUpDown) {
                cColor2 = &m_cColor1;
                cColor1 = &m_cColor2;
            }
            
            switch (m_nArrowState & 0x0f) {
            case 0x01: *cColor1 = COLOR_LIGHT_YELLOW; break;
            case 0x02: *cColor1 = COLOR_LIGHT_GREEN;  break;
            case 0x04: *cColor1 = COLOR_LIGHT_RED;    break;
            case 0x08: *cColor1 = COLOR_LIGHT_RED;    break;
            default:   *cColor1 = COLOR_LIGHT_BLACK;  break;
            }
            switch (m_nArrowState & 0xf0) {
            case 0x10: *cColor2 = COLOR_LIGHT_YELLOW; break; 
            case 0x20: *cColor2 = COLOR_LIGHT_GREEN;  break;
            case 0x40: *cColor2 = COLOR_LIGHT_RED;    break;
            case 0x80: *cColor2 = COLOR_LIGHT_RED;    break;
            default:   *cColor2 = COLOR_LIGHT_BLACK;  break;
            }
        }

        void StaSemiAutoBlock::OrderClear(bool bClearTwinkle)
        {
            BtnStateReset();
        }

        void StaSemiAutoBlock::setVollover(const QPointF& ptBase)
        {

        }

        void StaSemiAutoBlock::ResetDevState()
        {

        }

        void StaSemiAutoBlock::AddBlockBtn(QString strType, const QString& strElement)
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
    }
}