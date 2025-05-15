
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

            m_mapAttribute[m_strType].insert("isLeave", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_bLeave = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("LampCenter0", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_ptLampCenter = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("InterUsed", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_ptInterUsed = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("ZDBS_Type", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaAutoBlock*>(pDevice)->m_strAutoBlockType = strElement; });;

            
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
            InitRoutePreviewAttributeMap(m_strType, m_mapAttribute);
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
            return StaRoutePreview::InitDeviceAttribute();
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

            //车次预告窗
            DrawRoutePreviewWnd(m_pPainter, m_strDirection);
            if (!m_rcFrame.isEmpty()) {
                m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1));
                m_pPainter.setBrush(Qt::NoBrush);
                m_pPainter.drawRect(m_rcFrame);
            }
            
            return StaDistant::Draw(isMulti);
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

        bool StaAutoBlock::Contains(const QPoint& ptPos)
        {
            bool bContains = false;
            for (StaBlockBtn& btnBlock : m_vecBlockBtn) {
                bContains |= Scale(btnBlock.m_rcBtn).contains(ptPos);
            }
            bContains |= StaRoutePreview::Contains(ptPos);
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
            QMenu* pMenu = new QMenu();
            pMenu->setAttribute(Qt::WA_DeleteOnClose);
            ShowRoutePreviewMenu(pMenu, ptPos);
            if (pMenu->actions().size()) {
                pMenu->exec(QCursor::pos());
            }
            else {
                pMenu->close();
            }
        }

        void StaAutoBlock::SetBtnState()
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::FunBtn) {
                m_nFirstBtnType = 5;
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