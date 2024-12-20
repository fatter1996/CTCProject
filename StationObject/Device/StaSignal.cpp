#include "StaSignal.h"
#include "StaButton.h"
#include "Global.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainterPath>
#include <QMessageBox>
#include "CommonWidget/LeadSealDlg.h"
#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {

        StaSignal::StaSignal(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("p12", [&](const QString& strElement) { p12 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p7", [&](const QString& strElement) { p7 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p8", [&](const QString& strElement) { p8 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p9", [&](const QString& strElement) { p9 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p10", [&](const QString& strElement) { p10 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p13", [&](const QString& strElement) { p13 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p14", [&](const QString& strElement) { p14 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("m_nXHDType", [&](const QString& strElement) { m_nXHDType = strElement.toUInt(); });
            m_mapAttribute.insert("radius", [&](const QString& strElement) { m_nRadius = strElement.toUInt(); });
            m_mapAttribute.insert("SignalType", [&](const QString& strElement) { m_nSignalType = strElement.toUInt(); });
            m_mapAttribute.insert("D_B_C_Signa", [&](const QString& strElement) { m_nD_B_C_Signal = strElement.toUInt(); });
            m_mapAttribute.insert("DC_LC_Signa", [&](const QString& strElement) { m_nDC_LC_Signal = strElement.toUInt(); });
            m_mapAttribute.insert("safeLamp", [&](const QString& strElement) { m_strSafeLamp = strElement; });
            m_mapAttribute.insert("isHigh", [&](const QString& strElement) { m_bHigh = strElement.toInt(); });
            m_mapAttribute.insert("IsHaveBSQ", [&](const QString& strElement) { m_bIsIsHaveBSQ = strElement.toInt(); });
            m_mapAttribute.insert("bsqNum", [&](const QString& strElement) { m_nBSQNum = strElement.toInt(); });
            m_mapAttribute.insert("bsqInterlockBus", [&](const QString& strElement) { m_nBSQInterlockBus = strElement.toInt(); });
            m_mapAttribute.insert("m_nBSQModuAddr", [&](const QString& strElement) { m_nBSQModuAddr = strElement.toInt(); });
            m_mapAttribute.insert("isMD", [&](const QString& strElement) { m_bMD = strElement.toInt(); });
            m_mapAttribute.insert("DC_LC_Signa", [&](const QString& strElement) { m_bSingleDeng = strElement.toInt(); });

            InitSignalLightColor();
            
        }

        StaSignal::~StaSignal()
        {

        }

        void StaSignal::InitDeviceAttribute()
        {
            //列车按钮
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {
                m_rcTrainBtn = QRectF(p7, p8);
            }
            //调车按钮
            if (m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) {
                if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {
                    m_rcShuntBtn = QRectF(p9, p10);
                }
                else {
                    m_rcShuntBtn = QRectF(p7, p8);
                }
            }
            //引导按钮,仅可作为始端
            if ((m_nAttr & SIGNAL_LCSD) && (m_nAttr & SIGNAL_JCXH || m_nAttr & SIGNAL_FCXH || m_nAttr & SIGNAL_FCJLXH) && p13 != QPointF() && p14 != QPointF()) {
                if ((m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) && (QRectF(p13, p14) == QRectF(p9, p10))) {
                    m_rcGuideBtn = QRectF(
                        QPointF((m_nAttr & SIGNAL_SYH) ? (p13.x() - 20) : (p13.x() + 20), p13.y()),
                        QPointF((m_nAttr & SIGNAL_SYH) ? (p14.x() - 20) : (p14.x() + 20), p14.y()));
                }
                else {
                    m_rcGuideBtn = QRectF(p13, p14);
                }
            }

            int nDiameter = m_nRadius * 2;
            //第一灯位
            if (p12.x() > p8.x()) {    //向右
                m_rcLight1 = QRectF(p12.x() + m_nRadius, p12.y() - m_nRadius, Scale(nDiameter), Scale(nDiameter));
            }
            else {  //向左
                m_rcLight1 = QRectF(p12.x() - 3 * m_nRadius, p12.y() - m_nRadius, Scale(nDiameter), Scale(nDiameter));
            }
            //第二灯位
            if ((m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) && ((m_nAttr & (SIGNAL_LCZH | SIGNAL_AQH)) != SIGNAL_LCZH)) {
                if (p12.x() > p8.x()) {    //向右
                    m_rcLight2 = QRectF(p12.x() + 3 * m_nRadius, p12.y() - m_nRadius, Scale(nDiameter), Scale(nDiameter));
                    m_rcLightTotal = QRectF(m_rcLight1.topLeft(), m_rcLight2.bottomRight());
                }
                else {  //向左
                    m_rcLight2 = QRectF(p12.x() - 5 * m_nRadius, p12.y() - m_nRadius, Scale(nDiameter), Scale(nDiameter));
                    m_rcLightTotal = QRectF(m_rcLight2.topLeft(), m_rcLight1.bottomRight());
                }
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
            //列车按钮
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {   
                DrawButton(m_pPainter, Scale(m_rcTrainBtn), COLOR_BTN_GREEN, m_nBtnState & BTNDOWN_TRAIN);
            }
            //调车按钮
            if (m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) {
                DrawButton(m_pPainter, Scale(m_rcShuntBtn), COLOR_BTN_DEEPGRAY, m_nBtnState & BTNDOWN_SHUNT, 2);
            }
            //引导按钮,仅可作为始端
            if (m_rcGuideBtn != QRectF()) {
                DrawButton(m_pPainter, Scale(m_rcGuideBtn), COLOR_BTN_BLUE_YD, m_nBtnState & BTNDOWN_GUIDE);
            }
        }

        void StaSignal::DrawSignalLight()
        {
            if (m_nAttr & SIGNAL_ISXXH) {  //虚信号机,不绘制灯位
                return;
            }

            m_pPainter.setPen(QPen(COLOR_TRACK_BLUE, 2));
            //绘制灯柱
            m_pPainter.drawLine(Scale(QPointF(p12.x(), p12.y() + m_nRadius)), Scale(QPointF(p12.x(), p12.y() - m_nRadius)));
            if (p12.x() > p8.x()) {    //向右
                m_pPainter.drawLine(Scale(QPointF(p12.x(), p12.y())), Scale(QPointF(p12.x() + m_nRadius, p12.y())));
            }
            else {
                m_pPainter.drawLine(Scale(QPointF(p12.x(), p12.y())), Scale(QPointF(p12.x() - m_nRadius, p12.y())));
            }

            GetSignalLightColor();
            m_pPainter.setRenderHint(QPainter::Antialiasing, true);
            //第一灯位
            m_pPainter.setBrush(m_cLightColor1);
            m_pPainter.drawEllipse(Scale(m_rcLight1));
            //第二灯位
            if ((m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) && ((m_nAttr & (SIGNAL_LCZH | SIGNAL_AQH)) != SIGNAL_LCZH)) {  
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
            if (m_nAttr & SIGNAL_ISXXH) {  //虚信号机,不绘制灯位
                return;
            }

            m_pPainter.setBrush(COLOR_LIGHT_BLACK);
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            //第一灯位
            m_pPainter.drawRect(Scale(m_rcLight1));
            //第二灯位
            if ((m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) && ((m_nAttr & (SIGNAL_LCZH | SIGNAL_AQH)) != SIGNAL_LCZH)) {
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
            m_mapLightColor.insert(SignalState::DS, [&]() { m_cLightColor1 = COLOR_LIGHT_WHITE; m_cLightColor2 = COLOR_LIGHT_BLACK; });
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
            m_mapLightColor.insert(SignalState::DS, [&]() { m_cLightColor1 = m_bElapsed ? COLOR_LIGHT_RED : COLOR_LIGHT_BLACK; m_cLightColor2 = COLOR_LIGHT_BLACK; });
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

            if (m_rcTextRect.contains(ptPos)) {     //0x01-选中信号机名称
                m_nSelectType = 0x01;
                return true;
            }
            else if (m_rcTrainBtn.contains(ptPos)) {    //0x02-选中列车按钮
                m_nSelectType = 0x02;
                return true;
            }
            else if (m_rcShuntBtn.contains(ptPos)) {    //0x04-选中调车按钮
                m_nSelectType = 0x04;
                return true;
            }
            else if (pButton && pButton->getButtonRect().contains(ptPos)) {    //0x08-选中通过按钮
                m_nSelectType = 0x08;
                return true;
            }
            else if (m_rcGuideBtn.contains(ptPos)) {    //0x10-选中引导按钮
                m_nSelectType = 0x10;
                return true;
            }
            else if (m_rcLightTotal.contains(ptPos)) {    //0x1f-选中信号机灯位
                m_nSelectType = 0x1f;
                return true;
            }
            return false; 
        }

        bool StaSignal::IsMouseWheel(const QPoint& ptPos)
        {
            //进路建立，可操作列车按钮，调车按钮和通过按钮
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RouteBuild) {
                return m_rcTrainBtn.contains(ptPos) || m_rcShuntBtn.contains(ptPos);
            }
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::GuideBtn) {
                return m_rcGuideBtn.contains(ptPos);
            }

            //信号重开,总取消,总人解,点灯,灭灯操作信号灯或设备名称
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::SignalReopen ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::TotalCancel || 
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::TotalRelieve ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::Lighting ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::UnLighting) {
                return m_rcLightTotal.contains(ptPos) || m_rcTextRect.contains(ptPos);
            }

            //封锁/解封 可操作列车按钮,调车按钮,信号灯
            if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::Blockade ||
                CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::UnBlockade) {
                return m_rcTrainBtn.contains(ptPos) || m_rcShuntBtn.contains(ptPos) || m_rcLightTotal.contains(ptPos);
            }
            return false;
        }

        void StaSignal::InitClickEvent()
        {
            for (int i = 0; i < static_cast<int>(CTCWindows::FunType::MethodConvert); ++i) {
                switch (i)
                {
                case static_cast<int>(CTCWindows::FunType::RouteBuild):         //进路建立
                case static_cast<int>(CTCWindows::FunType::GuideBtn): {         //引导按钮
                    m_mapClickEvent.insert(static_cast<CTCWindows::FunType>(i), [&]() {
                        OnButtonClick(this);
                    });
                    break;
                }   
                case static_cast<int>(CTCWindows::FunType::SignalReopen): {     //信号重开
                    m_mapClickEvent.insert(static_cast<CTCWindows::FunType>(i), [&]() {
                        if (m_nSelectType == 0x01 || m_nSelectType == 0x1f) { //点击信号机名称或信号机灯位
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Signal);
                            MainStation()->AddSelectDevice(this);
                        }
                    });
                    break;
                }
                case static_cast<int>(CTCWindows::FunType::TotalCancel):        //总取消
                case static_cast<int>(CTCWindows::FunType::TotalRelieve):       //总人解
                case static_cast<int>(CTCWindows::FunType::Blockade):           //封锁
                case static_cast<int>(CTCWindows::FunType::UnBlockade): {       //解封
                    m_mapClickEvent.insert(static_cast<CTCWindows::FunType>(i), [&]() {
                        if (m_nSelectType == 0x02 && (m_nFirstBtnType == 0 || m_nFirstBtnType == 1)) { //点击列车按钮
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Train);
                            if (m_nFirstBtnType == 0) {
                                m_nFirstBtnType = 1;
                            }
                        }
                        else if (m_nSelectType == 0x04 && (m_nFirstBtnType == 0 || m_nFirstBtnType == 2)) { //点击调车按钮
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Shunt);
                            if (m_nFirstBtnType == 0) {
                                m_nFirstBtnType = 2;
                            }
                        }
                        else if (m_nSelectType == 0x1f) { //点击信号灯
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Signal);
                        }
                        MainStation()->AddSelectDevice(this);
                    });
                    break;
                }
                case static_cast<int>(CTCWindows::FunType::Lighting):           //点灯
                case static_cast<int>(CTCWindows::FunType::UnLighting): {       //灭灯
                    m_mapClickEvent.insert(static_cast<CTCWindows::FunType>(i), [&]() {
                        if (m_nSelectType == 0x1f) { //点击信号机灯位
                            CTCWindows::BaseWnd::StaFunBtnToolBar::setOperObjType(CTCWindows::OperObjType::Signal);
                            MainStation()->AddSelectDevice(this);
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
            if (m_nAttr & (SIGNAL_LCZD | SIGNAL_LCSD)) {    //列车
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
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x0c, 0x01);
                    }
                });
            }
            if (m_nAttr & (SIGNAL_DCZD | SIGNAL_DCSD)) {    //调车
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
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x0c, 0x02);
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
                            MainStation()->AddSelectDevice(this);
                            MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, 0x02, 0x05);
                        }
                    });
                }
                QAction* pAction1 = new QAction("点灯/灭灯");
                pAction1->setEnabled(static_cast<SignalState>(m_nState & 0x0f) == SignalState::U);
                pMenu->addAction(pAction1);
                QObject::connect(pAction1, &QAction::triggered, [=]() {
                    if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("下发\"点灯/灭灯[信号机:%1]\"命令吗?").arg(m_strName), "确定", "取消") == 0) {
                        MainStation()->AddSelectDevice(this);
                        MainStation()->SendPacketMsg(TARGET_INTERLOCK, 0x40, (static_cast<SignalState>(m_nState & 0x0f) == SignalState::U) ? 0x0e : 0x0f, 0x13);
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

        void StaSignal::OrderClear()
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