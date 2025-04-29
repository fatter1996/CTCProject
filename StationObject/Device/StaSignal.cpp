#include "StaSignal.h"
#include "StaButton.h"
#include "Global.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainterPath>
#include <QMessageBox>
#include "CommonWidget/LeadSealDlg.h"
#include "CommonWidget/SealTechnique.h"
#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        QList<SignalBtn*> StaSignal::m_listFlashSignal;
        StaSignal::StaSignal(QObject* pParent)
            : DeviceBase(pParent)
        {
            InitSignalLightColor();
        }

        StaSignal::~StaSignal()
        {

        }

        void StaSignal::InitAttributeMap()
        {
            if (m_mapAttribute.contains(m_strType)) {
                return;
            }
            AttrMap mapAttrFun;
            m_mapAttribute.insert(m_strType, mapAttrFun);
            m_mapAttribute[m_strType].insert("p12", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->p12 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p7", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->p7 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p8", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->p8 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p9", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->p9 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p10", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->p10 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p13", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->p13 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p14", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->p14 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("m_nXHDType", [](DeviceBase* pDevice, const QString& strElement) {
                dynamic_cast<StaSignal*>(pDevice)->m_nXHDType = strElement.toUInt();
                dynamic_cast<StaSignal*>(pDevice)->m_strXHDType = strElement;
            });
            m_mapAttribute[m_strType].insert("radius", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_nRadius = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("SignalType", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_nSignalType = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("D_B_C_Signa", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_nD_B_C_Signal = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("DC_LC_Signa", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_nDC_LC_Signal = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("safeLamp", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_strSafeLamp = strElement; });
            m_mapAttribute[m_strType].insert("isHigh", [](DeviceBase* pDevice, const QString& strElement) {
                if (strElement == "TRUE") {
                    dynamic_cast<StaSignal*>(pDevice)->m_bHigh = true;
                }
                else if (strElement == "FALSE") {
                    dynamic_cast<StaSignal*>(pDevice)->m_bHigh = true;
                }
                else {
                    dynamic_cast<StaSignal*>(pDevice)->m_bHigh = strElement.toInt();
                }

            });
            m_mapAttribute[m_strType].insert("IsHaveBSQ", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_bIsIsHaveBSQ = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("bsqNum", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_nBSQNum = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("bsqInterlockBus", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_nBSQInterlockBus = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("m_nBSQModuAddr", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_nBSQModuAddr = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("isMD", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_bMD = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("DC_LC_Signa", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_bSingleDeng = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("isYDSD", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSignal*>(pDevice)->m_bYDSD = strElement.toInt(); });

            return DeviceBase::InitAttributeMap();
        }

        void StaSignal::InitDeviceAttribute()
        {
            m_bRight = (m_nType == 31 || m_nType == 32);
            if (m_nRadius == 0) {
                m_nRadius = 7;
            }
            if (p12.isNull()) {
                p12 = QPointF((m_bRight ? m_ptCenter.x() - 10 : m_ptCenter.x() + 10), m_ptCenter.y());
            }
            if (p7.isNull() || p8.isNull()) {
                p7 = QPointF(m_bRight ? m_ptCenter.x() - m_nBtnRadius * 4 : m_ptCenter.x() + m_nBtnRadius * 2, m_ptCenter.y() - m_nBtnRadius);
                p8 = QPointF(m_bRight ? m_ptCenter.x() - m_nBtnRadius * 2 : m_ptCenter.x() + m_nBtnRadius * 4, m_ptCenter.y() + m_nBtnRadius);
            }
            if (p9.isNull() || p10.isNull()) {
                p9 = QPointF(m_bRight ? p7.x() - m_nBtnRadius * 2 - 4 : p8.x() + 4, p7.y());
                p10 = QPointF(m_bRight ? p7.x() - 4 : p8.x() + m_nBtnRadius * 2 + 4, p8.y());
            }
            if (p13.isNull() || p14.isNull()) {
                p13 = QPointF(m_bRight ? p9.x() - m_nBtnRadius * 2 - 4 : p10.x() + 4, p9.y());
                p14 = QPointF(m_bRight ? p9.x() - 4 : p10.x() + m_nBtnRadius * 2 + 4, p10.y());
            }
            //列车按钮
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD) || m_strXHDType == "JZ_XHJ" || m_strXHDType == "CZ_XHJ") {
                m_rcTrainBtn = QRectF(p7, p8);
            }
            //调车按钮
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD) || m_strXHDType == "CZ_XHJ") {
                if (!(m_nAttr & SIGNAL_ISXXH || m_nSignalType)) {
                    m_rcShuntBtn = QRectF(p9, p10);
                }
            }
            else if (((m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) && !(m_nAttr & SIGNAL_ISXXH || m_nSignalType)) || m_strXHDType == "DC_XHJ") {
                m_rcShuntBtn = QRectF(p7, p8);
            }

            //引导按钮,仅可作为始端
            m_bYDSD |= ((m_nAttr & SIGNAL_LCSD) && (m_nAttr & SIGNAL_JCXH || m_nAttr & SIGNAL_FCXH || m_nAttr & SIGNAL_FCJLXH));
            if (m_bYDSD) {
                if (m_strXHDType == "JZ_XHJ") {
                    m_rcGuideBtn = QRectF(p9, p10);
                }
                else {
                    m_rcGuideBtn = QRectF(p13, p14);
                }
            }

            //第一灯位
            m_rcLight1 = QRectF((m_bRight ? (p12.x() + m_nRadius) : (p12.x() - 3 * m_nRadius)), p12.y() - m_nRadius, Scale(m_nRadius * 2), Scale(m_nRadius * 2));
            //第二灯位
            m_rcLight2 = QRectF(m_bRight ? m_rcLight1.x() + 2 * m_nRadius : m_rcLight1.x() - 2 * m_nRadius, p12.y() - m_nRadius, Scale(m_nRadius * 2), Scale(m_nRadius * 2));
            if (m_nType == 32 || m_nType == 34) {
                m_rcLightTotal = QRectF(m_bRight ? m_rcLight1.topLeft() : m_rcLight2.bottomRight(), QSizeF(4 * m_nRadius, 2 * m_nRadius));
            }
            else {
                m_rcLightTotal = m_rcLight1;
            }
        }

        void StaSignal::Draw(bool isMulti)
        {
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {
                if (m_nAttr & SIGNAL_JCXH) {    //进站信号机
                    m_bShowName = MainStation()->IsVisible(VisibleDev::entrySignalName);
                }   
                else if (m_nAttr & SIGNAL_FCXH) {    //出站信号机
                    m_bShowName = MainStation()->IsVisible(VisibleDev::exitSignalName);
                }
            }
            else if (m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) {
                m_bShowName = MainStation()->IsVisible(VisibleDev::shuntSignalName);
            }
            if (!isMulti && MainStation()->IsVisible(VisibleDev::button) && m_bMainStation) {
                DrawSignalButton();
            }
            DrawSignalLight();
            DrawSignalButtonLock();
            return DeviceBase::Draw(isMulti);
        }

        void StaSignal::DrawSignalButton()
        {
            Station::Device::StaSignal* pSignal = nullptr;
            Station::Device::DeviceBtn* pBtnType = nullptr;

            //列车按钮
            if ((m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) || m_strXHDType == "JZ_XHJ" || m_strXHDType == "CZ_XHJ") {
                DrawButton(m_pPainter, Scale(m_rcTrainBtn), COLOR_BTN_GREEN, m_nBtnState & BTNDOWN_TRAIN | (m_bFlash && m_nFirstBtnType == 1), 1, COLOR_BTN_BLUE, m_bFlash ? COLOR_BTN_YELLOW : COLOR_BTN_WHITE);
            }
            if (m_nFirstBtnType == 3) {
                DrawButton(m_pPainter, Scale(m_rcTrainBtn), COLOR_BTN_GREEN, m_nBtnState & BTNDOWN_TRAIN | (m_bFlash && m_nFirstBtnType == 3), 1, COLOR_BTN_BLUE, m_bFlash ? COLOR_BTN_YELLOW : COLOR_BTN_WHITE);
            }
            //调车按钮
            if (m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD) || m_strXHDType == "DC_XHJ" || m_strXHDType == "CZ_XHJ") {
                DrawButton(m_pPainter, Scale(m_rcShuntBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & BTNDOWN_SHUNT | (m_bFlash && m_nFirstBtnType == 2), 2, COLOR_BTN_BLUE, m_bFlash ? COLOR_BTN_YELLOW : COLOR_BTN_WHITE );
            }
            //引导按钮,仅可作为始端
            if (m_bYDSD) {
                DrawButton(m_pPainter, Scale(m_rcGuideBtn), COLOR_BTN_BLUE_YD, m_nBtnState & BTNDOWN_GUIDE);
            }
        }

        void StaSignal::DrawSignalLight()
        {
            if (m_nAttr & SIGNAL_ISXXH || m_nSignalType) {  //虚信号机,不绘制灯位
                return;
            }

            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            //绘制灯柱
            m_pPainter.drawLine(Scale(QPointF(p12.x(), p12.y() + m_nRadius)), Scale(QPointF(p12.x(), p12.y() - m_nRadius)));
            m_pPainter.drawLine(Scale(QPointF(p12.x(), p12.y())), Scale(QPointF(m_bRight ? (p12.x() + m_nRadius) : (p12.x() - m_nRadius), p12.y())));

            GetSignalLightColor();
            m_pPainter.setRenderHint(QPainter::Antialiasing, true);
            //第一灯位
            m_pPainter.setBrush(m_cLightColor1);
            m_pPainter.drawEllipse(Scale(m_rcLight1));
            //第二灯位
            if (m_nType == 32 || m_nType == 34) {
                m_pPainter.setBrush(m_cLightColor2);
                m_pPainter.drawEllipse(Scale(m_rcLight2));
            }
            m_pPainter.setRenderHint(QPainter::Antialiasing, false);
        }

        void StaSignal::DrawSignalButtonLock()
        {
            m_pPainter.setBrush(COLOR_LIGHT_BLACK);
            m_pPainter.setPen(QPen(COLOR_BTN_RED, 1));
            if (m_nState & 0x10) {  //调车按钮封锁
                m_pPainter.drawRect(Scale(m_rcShuntBtn));
                m_pPainter.drawLine(Scale(m_rcShuntBtn.topLeft()), Scale(m_rcShuntBtn.bottomRight()));
                m_pPainter.drawLine(Scale(m_rcShuntBtn.bottomLeft()), Scale(m_rcShuntBtn.topRight()));
            }
            if (m_nState & 0x20) {  //列车按钮封锁
                m_pPainter.drawRect(Scale(m_rcTrainBtn));
                m_pPainter.drawLine(Scale(m_rcTrainBtn.topLeft()), Scale(m_rcTrainBtn.bottomRight()));
                m_pPainter.drawLine(Scale(m_rcTrainBtn.bottomLeft()), Scale(m_rcTrainBtn.topRight()));
            }
        }

        void StaSignal::DrawSelectRange()
        {
            m_pPainter.setRenderHint(QPainter::Antialiasing, false);
            DrawLightRange();
            if (m_nSelectType & 0x01) {  //信号机名称
                m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
                m_pPainter.setBrush(COLOR_TRACK_SELECT_BLUE);
                m_pPainter.drawRect(Scale(OutSideRect(m_rcTextRect, 2, 0)));
            }
            DrawButtonRange();
        }

        void StaSignal::DrawLightRange()
        {
            if (m_nAttr & SIGNAL_ISXXH || m_nSignalType) {  //虚信号机,不绘制灯位
                return;
            }

            m_pPainter.setBrush(COLOR_LIGHT_BLACK);
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            //第一灯位
            m_pPainter.drawRect(Scale(m_rcLight1));
            //第二灯位
            /*(m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) && ((m_nAttr & (SIGNAL_LCZH | SIGNAL_AQH)) != SIGNAL_LCZH)*/
            if (m_nType == 32 || m_nType == 34) {
                m_pPainter.drawRect(Scale(m_rcLight2));
            }
        }

        void StaSignal::DrawDeviceNameRange()
        {
            
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            m_pPainter.setBrush(COLOR_TRACK_SELECT_BLUE);
            m_pPainter.drawRect(Scale(OutSideRect(m_rcTextRect, 2, 0)));
        }

        void StaSignal::DrawButtonRange()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            m_pPainter.setBrush(COLOR_LIGHT_BLACK);

            if (m_nSelectType & 0x02) {   //列车信号机按钮
                m_pPainter.drawRect(Scale(OutSideRect(m_rcTrainBtn, 1, 1)));
            }
            if (m_nSelectType & 0x04) {   //调车信号机按钮
                m_pPainter.drawRect(Scale(OutSideRect(m_rcShuntBtn, 1, 1)));
            }
            if (m_pRelatedBtn&& m_nSelectType & 0x08) {   //通过信号机按钮
                StaButton* pButton = dynamic_cast<StaButton*>(m_pRelatedBtn);
                m_pPainter.drawRect(Scale(OutSideRect(pButton->getButtonRect(), 1, 1)));
            }
            if (m_nSelectType & 0x10) {   //引导信号机按钮
                m_pPainter.drawRect(Scale(OutSideRect(m_rcGuideBtn, 1, 1)));
            }
        }

        void StaSignal::DrawCultivateTips()
        {
            m_pPainter.setPen(QPen(COLOR_TRACK_YELLOW, 2));
            m_pPainter.setBrush(COLOR_LIGHT_BLACK);

            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(14));//字号
            QRectF rcTips;
            QString strTips;
            int index = 0;
            if (m_nTipsType == 0x01) {
                index = (m_nTipsType & 0xF0) >> 4;
                rcTips = QRectF(m_rcTrainBtn.x() - 4, m_rcTrainBtn.y() - 4, m_rcTrainBtn.width() + 8, m_rcTrainBtn.height() + 8);
                strTips = QString("%1点击%2信号机列车按钮").arg(index == 1 ? "先" : "后").arg(m_strName);
            }
            if (m_nTipsType == 0x02) {
                index = (m_nTipsType & 0xF0) >> 4;
                rcTips = QRectF(m_rcShuntBtn.x() - 4, m_rcShuntBtn.y() - 4, m_rcShuntBtn.width() + 8, m_rcShuntBtn.height() + 8);
                strTips = QString("%1点击%2信号机调车按钮").arg(index == 1 ? "先" : "后").arg(m_strName);
            }
            if (m_nTipsType == 0x03) {
                index = (m_nTipsType & 0xF0) >> 4;
                QRectF rcThoughBtn = dynamic_cast<StaButton*>(m_pRelatedBtn)->getButtonRect();
                rcTips = QRectF(rcThoughBtn.x() - 4, rcThoughBtn.y() - 4, rcThoughBtn.width() + 8, rcThoughBtn.height() + 8);
                strTips = QString("%1点击%2信号机通过按钮").arg(index == 1 ? "先" : "后").arg(m_strName);
            }
            if (m_nTipsType == 0x04) {
                if (m_rcLight1.x() < m_rcLight2.x()) {
                    rcTips = QRectF(m_rcLight1.x() - 4, m_rcLight1.y() - 4, m_nRadius * 4 + 8, m_rcLight1.height() + 8);
                }
                rcTips = QRectF(m_rcLight1.x() - 4, m_rcLight1.y() - 4, m_rcLight1.width() + 8, m_rcLight1.height() + 8);
                strTips = QString("使用功能按钮或右键菜单将%1信号机\"封锁\"").arg(m_strName);
            }
            if (m_nTipsType == 0x05) {
                rcTips = QRectF(m_rcTrainBtn.x() - 4, m_rcTrainBtn.y() - 4, m_rcTrainBtn.width() + 8, m_rcTrainBtn.height() + 8);
                strTips = QString("使用功能按钮或右键菜单对%1信号机进行\"总取消\"").arg(m_strName);
            }
            if (m_nTipsType == 0x06) {
                rcTips = QRectF(m_rcTrainBtn.x() - 4, m_rcTrainBtn.y() - 4, m_rcTrainBtn.width() + 8, m_rcTrainBtn.height() + 8);
                strTips = QString("使用功能按钮或右键菜单对%1信号机进行\"总人解\"").arg(m_strName);
            }

            m_pPainter.drawRect(Scale(rcTips));

            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(COLOR_TRACK_YELLOW);
            m_pPainter.drawText(QRectF(rcTips.x() - 72, rcTips.y() + 36, rcTips.width() + 144, 20), strTips);
        }

        void StaSignal::InitSignalLightColor()
        {
            //m_mapLightColor.insert(SignalState::DS, [&]() { m_cLightColor1 = COLOR_LIGHT_WHITE; m_cLightColor2 = COLOR_LIGHT_BLACK; });
            m_mapLightColor.insert(SignalState::A, [&]() { m_cLightColor1 = COLOR_LIGHT_BLUE; m_cLightColor2 = COLOR_LIGHT_BLACK; });
            m_mapLightColor.insert(SignalState::H, [&]() { m_cLightColor1 = COLOR_LIGHT_RED; m_cLightColor2 = COLOR_LIGHT_BLACK; });
            m_mapLightColor.insert(SignalState::L, [&]() { m_cLightColor1 = COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_GREEN; });
            m_mapLightColor.insert(SignalState::LL, [&]() { m_cLightColor1 = COLOR_LIGHT_GREEN; m_cLightColor2 = COLOR_LIGHT_GREEN; });
            m_mapLightColor.insert(SignalState::U, [&]() { m_cLightColor1 = COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_YELLOW; });
            m_mapLightColor.insert(SignalState::UU, [&]() {m_cLightColor1 = COLOR_LIGHT_YELLOW; m_cLightColor2 = COLOR_LIGHT_YELLOW; });
            m_mapLightColor.insert(SignalState::U2, [&]() {m_cLightColor1 = COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_YELLOW; });
            m_mapLightColor.insert(SignalState::HB, [&]() {m_cLightColor1 = COLOR_LIGHT_WHITE; m_cLightColor2 = COLOR_LIGHT_RED; });
            m_mapLightColor.insert(SignalState::LU, [&]() {m_cLightColor1 = COLOR_LIGHT_GREEN; m_cLightColor2 = COLOR_LIGHT_YELLOW; });
            m_mapLightColor.insert(SignalState::BS, [&]() { m_cLightColor1 = m_bElapsed ? COLOR_LIGHT_WHITE : COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_BLACK; });
            m_mapLightColor.insert(SignalState::HS, [&]() { m_cLightColor1 = m_bElapsed ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_BLACK; });
            m_mapLightColor.insert(SignalState::LS, [&]() { m_cLightColor1 = COLOR_LIGHT_BLACK;   m_cLightColor2 = m_bElapsed ? COLOR_LIGHT_GREEN : COLOR_LIGHT_BLACK; });
            m_mapLightColor.insert(SignalState::USU, [&]() {m_cLightColor1 = m_bElapsed ? COLOR_LIGHT_YELLOW : COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_YELLOW; });
            m_mapLightColor.insert(SignalState::US, [&]() { m_cLightColor1 = m_bElapsed ? COLOR_LIGHT_YELLOW : COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_BLACK; });
            m_mapLightColor.insert(SignalState::DS, [&]() { 
                if (m_nType == 32 || m_nType == 34) {
                    m_cLightColor1 = m_bElapsed ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_BLACK;
                }
                else {
                    m_cLightColor1 = m_bElapsed ? COLOR_LIGHT_BLUE : COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_BLACK;
                }
                
            });
        }

        void StaSignal::GetSignalLightColor()
        {
            if (m_mapLightColor.contains(static_cast<SignalState>(m_nState & 0x0f))) {
                m_mapLightColor[static_cast<SignalState>(m_nState & 0x0f)]();
            }
            else {
                m_cLightColor1 = COLOR_LIGHT_RED; 
                m_cLightColor2 = COLOR_LIGHT_BLACK;
            }
        }

        bool StaSignal::Contains(const QPoint& ptPos)
        {
            StaButton* pButton = dynamic_cast<StaButton*>(m_pRelatedBtn);

            if (Scale(m_rcTextRect).contains(ptPos)) {     //0x01-选中信号机名称
                m_nSelectType = 0x01;
                return true;
            }
            else if (Scale(m_rcTrainBtn).contains(ptPos)) {    //0x02-选中列车按钮
                m_nSelectType = 0x02;
                return true;
            }
            else if (Scale(m_rcShuntBtn).contains(ptPos)) {    //0x04-选中调车按钮
                m_nSelectType = 0x04;
                return true;
            }
            else if (pButton && Scale(pButton->getButtonRect()).contains(ptPos)) {    //0x08-选中通过按钮
                m_nSelectType = 0x08;
                return true;
            }
            else if (Scale(m_rcGuideBtn).contains(ptPos)) {    //0x10-选中引导按钮
                m_nSelectType = 0x10;
                return true;
            }
            else if (Scale(m_rcLightTotal).contains(ptPos)) {    //0x1f-选中信号机灯位
                m_nSelectType = 0x1f;
                return true;
            }
            return false; 
        }

        bool StaSignal::IsMouseWheel(const QPoint& ptPos)
        {
            //进路建立，总取消,总人解,可操作列车按钮，调车按钮和通过按钮
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RouteBuild ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::TotalCancel ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::TotalRelieve) {
                return Scale(m_rcTrainBtn).contains(ptPos) || Scale(m_rcShuntBtn).contains(ptPos);
            }
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::GuideBtn) {
                return Scale(m_rcGuideBtn).contains(ptPos);
            }

            //信号重开,点灯,灭灯操作信号灯或设备名称
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::SignalReopen ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::Lighting ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::UnLighting) {
                return Scale(m_rcLightTotal).contains(ptPos) || Scale(m_rcTextRect).contains(ptPos);
            }

            //封锁/解封 可操作列车按钮,调车按钮,信号灯
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::Blockade ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::UnBlockade) {
                return Scale(m_rcTrainBtn).contains(ptPos) || Scale(m_rcShuntBtn).contains(ptPos) || Scale(m_rcLightTotal).contains(ptPos);
            }
            return false;
        }
        void StaSignal::ButtonStatusSwitching(DeviceBase* pDevice) {
            dynamic_cast<StaSignal*>(pDevice)->OnButtonClick();
            StaSignal* pStaSignal = nullptr;
            StaButton* pStaBtn = nullptr;
            QList<SignalBtn*> listFlashSignalT;
            QList<SignalBtn*>* listFlashSignal = MainStation()->getSignalBtn();
            for (SignalBtn* pSignal : m_listFlashSignal) {
                if (MainStation()->getSelectDevice().size() > 1 && pSignal->Btnname.size() >= MainStation()->getSelectDevice().size()) {
                    pStaSignal = dynamic_cast<StaSignal*>(MainStation()->getDeviceByName(pSignal->Btnname[MainStation()->getSelectDevice().size() - 1], SIGNALLAMP));
                    if (pStaSignal != nullptr) {
                        pStaSignal->setFlash(false);
                    }
                    else { continue; }
                }
            }
            if (MainStation()->getSelectDevice().size() > 1) {
                listFlashSignal = &m_listFlashSignal;
            }
            for (SignalBtn* pSignal : *listFlashSignal) {

                if (MainStation()->getSelectDevice().size() == 0) { return; }
                if (pSignal->Btnname.size() < MainStation()->getSelectDevice().size()) {
                    continue;
                }
                if (m_nFirstBtnType != pSignal->SigType) {
                    continue;
                }
                qDebug() << MainStation()->getSelectDevice().size();
                if (pSignal->Btnname[MainStation()->getSelectDevice().size() - 1] == pDevice->getName()) {
                    listFlashSignalT.append(pSignal);
                }
            }
            m_listFlashSignal.swap(listFlashSignalT);
            for (SignalBtn* pSignal : m_listFlashSignal) {
                
                if (pSignal->Btnname.size() > MainStation()->getSelectDevice().size()) {
                    pStaSignal = dynamic_cast<StaSignal*>(MainStation()->getDeviceByName(pSignal->Btnname[MainStation()->getSelectDevice().size()], SIGNALLAMP));
                    pStaSignal->setFlash();
                }
            }
        }
        void StaSignal::InitClickEvent()
        {
            for (int i = 0; i < static_cast<int>(CTCWindows::FunType::MethodConvert); ++i) {
                switch (i)
                {
                case static_cast<int>(CTCWindows::FunType::RouteBuild):         //进路建立
                    m_mapClickEvent[m_strType].insert(static_cast<CTCWindows::FunType>(i), [=](DeviceBase* pDevice) {
                        ButtonStatusSwitching(pDevice);
                    });
                    break;
                case static_cast<int>(CTCWindows::FunType::GuideBtn): {         //引导按钮
                    m_mapClickEvent[m_strType].insert(static_cast<CTCWindows::FunType>(i), [](DeviceBase* pDevice) {
                        dynamic_cast<StaSignal*>(pDevice)->OnButtonClick();
                    });
                    break;
                }   
                case static_cast<int>(CTCWindows::FunType::SignalReopen): {     //信号重开
                    m_mapClickEvent[m_strType].insert(static_cast<CTCWindows::FunType>(i), [](DeviceBase* pDevice) {
                        if (dynamic_cast<StaSignal*>(pDevice)->m_nSelectType == 0x01 || dynamic_cast<StaSignal*>(pDevice)->m_nSelectType == 0x1f) { //点击信号机名称或信号机灯位
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Signal);
                            MainStation()->AddSelectDevice(pDevice);
                        }
                    });
                    break;
                }
                case static_cast<int>(CTCWindows::FunType::TotalCancel):        //总取消
                case static_cast<int>(CTCWindows::FunType::TotalRelieve):       //总人解
                case static_cast<int>(CTCWindows::FunType::Blockade):           //封锁
                case static_cast<int>(CTCWindows::FunType::UnBlockade): {       //解封
                    m_mapClickEvent[m_strType].insert(static_cast<CTCWindows::FunType>(i), [](DeviceBase* pDevice) {
                        StaSignal* pSignal = dynamic_cast<StaSignal*>(pDevice);
                        if (pSignal->m_nSelectType == 0x02 && (m_nFirstBtnType == 0 || pSignal->m_nFirstBtnType == 1)) { //点击列车按钮
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Train);
                            if (m_nFirstBtnType == 0) {
                                m_nFirstBtnType = 1;
                            }
                        }
                        else if (pSignal->m_nSelectType == 0x04 && (m_nFirstBtnType == 0 || m_nFirstBtnType == 2)) { //点击调车按钮
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Shunt);
                            if (m_nFirstBtnType == 0) {
                                m_nFirstBtnType = 2;
                            }
                        }
                        else if (pSignal->m_nSelectType == 0x1f) { //点击信号灯
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Signal);
                        }
                        MainStation()->AddSelectDevice(pSignal);
                    });
                    break;
                }
                case static_cast<int>(CTCWindows::FunType::Lighting):           //点灯
                case static_cast<int>(CTCWindows::FunType::UnLighting): {       //灭灯
                    m_mapClickEvent[m_strType].insert(static_cast<CTCWindows::FunType>(i), [](DeviceBase* pDevice) {
                        if (dynamic_cast<StaSignal*>(pDevice)->m_nSelectType == 0x1f) { //点击信号机灯位
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Signal);
                            MainStation()->AddSelectDevice(pDevice);
                        }
                    });
                    break;
                }
                }
            }
            
        }

        void StaSignal::ShowDeviceMenu(const QPoint& ptPos)
        {
            QMenu* pMenu = new QMenu();
            pMenu->setAttribute(Qt::WA_DeleteOnClose);
            if (m_pRelatedBtn) {    //通过
                QAction* pAction1 = new QAction("办理 通过进路");
                pAction1->setEnabled(static_cast<SignalState>(m_nState & 0x0f) == SignalState::U);
                pMenu->addAction(pAction1);
                QObject::connect(pAction1, &QAction::triggered, [=]() {
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Though);
                    dynamic_cast<StaButton*>(m_pRelatedBtn)->setBtnState(1);
                    m_nFirstBtnType = 3;
                    MainStation()->AddSelectDevice(m_pRelatedBtn);
                });
                pMenu->addSeparator();
            }
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD) || m_strXHDType == "JZ_XHJ" || m_strXHDType == "CZ_XHJ") {    //列车
                QAction* pAction1 = new QAction("列车 进路办理");
                pAction1->setEnabled(static_cast<SignalState>(m_nState & 0x0f) == SignalState::U);
                pMenu->addAction(pAction1);
                QObject::connect(pAction1, &QAction::triggered, [=]() {
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Train);
                    m_nBtnState |= BTNDOWN_TRAIN;
                    if (m_nFirstBtnType == 0) {
                        m_nFirstBtnType = 1;
                    }
                    MainStation()->AddSelectDevice(m_pRelatedBtn);
                });
                QAction* pAction2 = new QAction("取消进路");
                pAction2->setEnabled(!(static_cast<SignalState>(m_nState & 0x0f) == SignalState::U));
                pMenu->addAction(pAction2);
                QObject::connect(pAction2, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"取消进路[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x04, 0x01);
                    }
                });
                QAction* pAction3 = new QAction("信号重开");
                pMenu->addAction(pAction3);
                QObject::connect(pAction3, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"信号重开[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x03, 0x13);
                    }
                });
                QAction* pAction4 = new QAction("封锁/解封");
                pMenu->addAction(pAction4);
                QObject::connect(pAction4, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"封锁/解封[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, (m_nState & 0x20) ? 0x0b : 0x0a, 0x01);
                    }
                });
                QAction* pAction5 = new QAction("总人解");
                pMenu->addAction(pAction5);
                QObject::connect(pAction5, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"总人解[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                            CTCWindows::SealTechnique::InsertSealRecord(Station::MainStation()->getStationName(), "总人解");
                            MainStation()->AddSelectDevice(this);
                            MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x0c, 0x01);
                        }
                    }
                });
            }
            if (m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD) || m_strXHDType == "DC_XHJ" || m_strXHDType == "CZ_XHJ") {    //调车
                pMenu->addSeparator();
                QAction* pAction1 = new QAction("调车 进路办理");
                pAction1->setEnabled(static_cast<SignalState>(m_nState & 0x0f) == SignalState::B);
                pMenu->addAction(pAction1);
                QObject::connect(pAction1, &QAction::triggered, [=]() {
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Shunt);
                    m_nBtnState |= BTNDOWN_SHUNT;
                    if (m_nFirstBtnType == 0) {
                        m_nFirstBtnType = 2;
                    }
                    MainStation()->AddSelectDevice(m_pRelatedBtn);
                });
                QAction* pAction2 = new QAction("取消进路");
                pAction2->setEnabled(!(static_cast<SignalState>(m_nState & 0x0f) == SignalState::B));
                pMenu->addAction(pAction2);
                QObject::connect(pAction2, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"取消进路[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x04, 0x02);
                    }
                });
                QAction* pAction3 = new QAction("信号重开");
                pMenu->addAction(pAction3);
                QObject::connect(pAction3, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"信号重开[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x03, 0x13);
                    }
                });
                QAction* pAction4 = new QAction("封锁/解封");
                pMenu->addAction(pAction4);
                QObject::connect(pAction4, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"封锁/解封[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, (m_nState & 0x20) ? 0x0b : 0x0a, 0x02);
                    }
                });
                QAction* pAction5 = new QAction("总人解");
                pMenu->addAction(pAction5);
                QObject::connect(pAction5, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"总人解[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                            CTCWindows::SealTechnique::InsertSealRecord(Station::MainStation()->getStationName(), "总人解");
                            MainStation()->AddSelectDevice(this);
                            MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x0c, 0x02);
                        }
                    }
                });
            }
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {
                pMenu->addSeparator();
                if ((m_nAttr & SIGNAL_LCSD) && (m_nAttr & SIGNAL_JCXH || m_nAttr & SIGNAL_FCJLXH) && m_rcGuideBtn != QRectF()) { //引导
                    QAction* pAction1 = new QAction("引导");
                    pAction1->setEnabled(static_cast<SignalState>(m_nState & 0x0f) == SignalState::U);
                    pMenu->addAction(pAction1);
                    QObject::connect(pAction1, &QAction::triggered, [=]() {
                        if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"引导[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                            if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                                CTCWindows::SealTechnique::InsertSealRecord(Station::MainStation()->getStationName(), "引导按钮");
                                MainStation()->AddSelectDevice(this);
                                MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x02, 0x05);
                            }
                        }
                    });
                }
                QAction* pAction1 = new QAction("点灯/灭灯");
                pAction1->setEnabled(static_cast<SignalState>(m_nState & 0x0f) == SignalState::U);
                pMenu->addAction(pAction1);
                QObject::connect(pAction1, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"点灯/灭灯[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
                            CTCWindows::SealTechnique::InsertSealRecord(Station::MainStation()->getStationName(), (static_cast<SignalState>(m_nState & 0x0f) == SignalState::U) ? "点灯" : "灭灯");
                            MainStation()->AddSelectDevice(this);
                            MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, (static_cast<SignalState>(m_nState & 0x0f) == SignalState::U) ? 0x0e : 0x0f, 0x13);
                        }
                    }
                });
            }
            pMenu->exec(QCursor::pos());
        }

        void StaSignal::SetBtnState()
        {
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RouteBuild) {
                if (m_nSelectType == 0x02 && (m_nFirstBtnType == 0 || m_nFirstBtnType == 1 || m_nFirstBtnType == 3)) { //点击列车按钮
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Train);
                    m_nBtnState |= BTNDOWN_TRAIN;
                    if (m_nFirstBtnType == 0) {
                        m_nFirstBtnType = 1;
                    }
                }
                if (m_nSelectType == 0x04 && (m_nFirstBtnType == 0 || m_nFirstBtnType == 2)) { //点击调车按钮
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Shunt);
                    m_nBtnState |= BTNDOWN_SHUNT;
                    if (m_nFirstBtnType == 0) {
                        m_nFirstBtnType = 2;
                    }
                }
            }
            else if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::GuideBtn) {
                if (m_nSelectType == 0x10 && m_nFirstBtnType == 0) {    //点击引导按钮
                    CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Guide);
                    m_nBtnState |= BTNDOWN_GUIDE;
                    m_nFirstBtnType = 4;
                }
            }
        }

        void StaSignal::OrderClear(bool bClearTwinkle)
        {
            BtnStateReset();
        }

        void StaSignal::setVollover(const QPointF& ptBase)
        {

        }

        void StaSignal::ResetDevState()
        {

        }
    }
}