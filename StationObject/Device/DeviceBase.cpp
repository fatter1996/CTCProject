﻿#include "DeviceBase.h"
#include "Global.h"
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>
#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include "CommonWidget/LeadSealDlg.h"
#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        QPainter DeviceBase::m_pPainter;
        bool DeviceBase::m_bElapsed = false;
        int DeviceBase::m_nWheelDevCode = -1;

        DeviceScale::DeviceScale() 
        {
        }

        double DeviceScale::Scale(double value, double nOffset)
        {
            if (m_bMainStation) {
                return (value + nOffset) * MainStation()->getDiploid(DiploidRatio::StaDiploid);
            }
            else {
                return (value + nOffset) * MainStation()->getDiploid(DiploidRatio::MultiDiploid);
            }
        }

        QPointF DeviceScale::Scale(const QPointF& pt, const bool bOutSide, const bool bTopLine)
        {
            QPointF ptOffset = dynamic_cast<StationObject*>(m_pParent)->getOffset();
            if (!bOutSide) {
                return QPointF(Scale(pt.x(), ptOffset.x()), Scale(pt.y(), ptOffset.y()));
            }
            else {
                if (bTopLine) {
                    return QPointF(Scale(pt.x(), ptOffset.x()), Scale(pt.y() - TRACK_WIDTH / 2, ptOffset.y()) + 1);
                }
                else {
                    return QPointF(Scale(pt.x(), ptOffset.x()), Scale(pt.y() + TRACK_WIDTH / 2, ptOffset.y()));
                }
            }
        }

        QRectF DeviceScale::Scale(const QRectF& rect)
        {
            QPointF ptOffset = dynamic_cast<StationObject*>(m_pParent)->getOffset();
            return QRectF(Scale(rect.x(), ptOffset.x()),
                Scale(rect.y(), ptOffset.y()),
                Scale(rect.width()), Scale(rect.height()));
        }

        DeviceBase::DeviceBase(QObject* pParent)
        {
            m_pParent = pParent;
            m_bMainStation = dynamic_cast<StationObject*>(m_pParent)->IsMainStation();
            m_mapAttribute.insert("m_nType", [&](const QString& strElement) { m_nType = strElement.toUInt(); });
            m_mapAttribute.insert("m_strName", [&](const QString& strElement) { m_strName = strElement; });
            m_mapAttribute.insert("m_nCode", [&](const QString& strElement) { m_nCode = strElement.toInt(nullptr, 16); });

            m_mapAttribute.insert("m_textRect", [&](const QString& strElement) {
                m_rcTextRect = QStringToQRectF(strElement);
                QFont font;
                font.setFamily("微软雅黑");
                font.setPixelSize(Scale(m_nFontSize));//字号
                QFontMetrics  fontMetrics(font);
                m_rcTextRect = QRectF(m_rcTextRect.topLeft(), fontMetrics.size(Qt::TextSingleLine, m_strName));
            });
            m_mapAttribute.insert("pName", [&](const QString& strElement) {
                m_ptName = QStringToQPointF(strElement);
                QFont font;
                font.setFamily("微软雅黑");
                font.setPixelSize(Scale(m_nFontSize));//字号
                QFontMetrics  fontMetrics(font);
                m_rcTextRect = QRectF(m_ptName, fontMetrics.size(Qt::TextSingleLine, m_strName));
            });

            m_mapAttribute.insert("m_nSX", [&](const QString& strElement) { m_bUpDown = strElement.toUInt(); });
            m_mapAttribute.insert("FontHeight", [&](const QString& strElement) { m_nFontSize = strElement.toUInt(); });
            m_mapAttribute.insert("center", [&](const QString& strElement) { m_ptCenter = QStringToQPointF(strElement); });
            m_mapAttribute.insert("InterlockBus", [&](const QString& strElement) { m_nInterlockBus = strElement.toInt(); });
            m_mapAttribute.insert("attr", [&](const QString& strElement) { m_nAttr = strElement.toULong(nullptr, 16); });
            m_mapAttribute.insert("Module_Code", [&](const QString& strElement) { m_nModuleCode = strElement.toULong(nullptr, 16); });
        }

        DeviceBase::~DeviceBase()
        {
            
        }

        bool DeviceBase::eventFilter(QObject* obj, QEvent* event)
        {
            if (event->type() == QEvent::Paint) {
                m_pPainter.begin((QWidget*)obj);
                Draw();
                m_pPainter.end();
            }
            if (m_bMainStation) {
                if (event->type() == QEvent::MouseMove) {
                    QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
                    onMouseMoveToDevice(mouseEvent->pos());
                }

                if (event->type() == QEvent::MouseButtonRelease) {  //鼠标点击事件
                    QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
                    if (MainStation()->IsAllowStaOperation() && Contains(mouseEvent->pos())) {
                        if (mouseEvent->button() == Qt::LeftButton) {   //鼠标左键点击事件
                            onDeviceClick();
                        }
                        else if (mouseEvent->button() == Qt::RightButton) {   //鼠标右键点击事件
                            MainStation()->onOrderClear();
                            ShowDeviceMenu(mouseEvent->pos());
                            event->accept();
                            return true;
                        }
                    }
                }
            }
            return QObject::eventFilter(obj, event);
        }

        void DeviceBase::InitDeviceInfoFromXml(QXmlStreamReader* pDeviceInfoReader, const QString& strDeviceType)
        {
            while (!pDeviceInfoReader->atEnd()) {
                pDeviceInfoReader->readNext();
                if (pDeviceInfoReader->isStartElement()) {
                    ReadDeviceAttributeFromXml(pDeviceInfoReader);
                }
                else if (pDeviceInfoReader->isEndElement() && pDeviceInfoReader->name() == strDeviceType) {
                    InitDeviceAttribute();
                    InitClickEvent();
                    return;
                }
            }
        }

        void DeviceBase::InitInitDeviceInfoFromJson(const QJsonObject& object, const QString& strKey)
        {
            for (QString key : object.keys()) {
                ReadDeviceAttributeFromJson(object, key);
            }
            InitDeviceAttribute();
            if (m_bMainStation) {
                InitClickEvent();
            }
        }

        void DeviceBase::ReadDeviceAttributeFromXml(QXmlStreamReader* m_pDeviceInfoReader)
        {
            QString str = m_pDeviceInfoReader->name().toString();
            if (m_mapAttribute.contains(str)) {
                if (m_pDeviceInfoReader->attributes().hasAttribute("index")) {
                    nIndex = m_pDeviceInfoReader->attributes().value("index").toInt();
                }
                m_mapAttribute[str](m_pDeviceInfoReader->readElementText());
            }
        }

        void DeviceBase::ReadDeviceAttributeFromJson(const QJsonObject& lampObject, const QString& strKey)
        {
            if (m_mapAttribute.contains(strKey)) {
                if (lampObject.value(strKey).isObject()) {
                    QJsonDocument jsonDoc(lampObject.value(strKey).toObject());
                    m_mapAttribute[strKey](jsonDoc.toJson());
                }
                else if (lampObject.value(strKey).isArray()) {
                    QJsonDocument jsonDoc(lampObject.value(strKey).toArray());
                    m_mapAttribute[strKey](jsonDoc.toJson());
                }
                else if (lampObject.value(strKey).isString()) {
                    m_mapAttribute[strKey](lampObject.value(strKey).toString());
                }
                else {
                    m_mapAttribute[strKey](QString::number(lampObject.value(strKey).toInt()));
                }
            }
        }

        void DeviceBase::DrawDeviceName()
        {
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            QPen pen = getDeviceNameColor();
            if (pen.color() == Qt::NoPen) {
                return;
            }
            QFontMetrics fm(font);
            int textWidth = fm.horizontalAdvance(m_strName);
            int textHeight = fm.height();

            // 根据范围调整字体大小
            QRectF rcTextRect = Scale(m_rcTextRect);
            if (m_rcTextRect != QRectF()) {
                while (textWidth > rcTextRect.width() || textHeight > rcTextRect.height()) {
                    font.setPixelSize(font.pixelSize() - 1);

                    m_pPainter.setFont(font);
                    fm = QFontMetrics(font);
                    textWidth = fm.horizontalAdvance(m_strName);
                    textHeight = fm.height();
                }
            }

            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(getDeviceNameColor());
            m_pPainter.setBrush(Qt::NoBrush);
            m_pPainter.drawText(rcTextRect, m_strName, QTextOption(Qt::AlignCenter));
        }

        void DeviceBase::Draw(bool isMulti)
        {
            if (m_bMainStation) {
                //绘制设备选中虚线框
                if (m_bRangeVisible) {
                    DrawSelectRange();
                }
                //绘制培训提示信息
                if (m_bShowTips) {
                    DrawCultivateTips();
                }
            }
            //绘制设备名称
            if (m_bShowName) {
                DrawDeviceName();
            }
        }

        void DeviceBase::onMouseMoveToDevice(const QPoint& ptPos)
        {
            if ((m_nCode != -1) && Contains(ptPos) && (m_nWheelDevCode == -1 || m_nWheelDevCode == m_nCode)) {
                m_bRangeVisible = true;
                m_nWheelDevCode = m_nCode;
            }
            else {
                m_bRangeVisible = false;
                if (m_nWheelDevCode == m_nCode) {
                    m_nWheelDevCode = -1;
                }
            }

            if (m_nWheelDevCode == -1) {
                QApplication::restoreOverrideCursor();
            }
            else if (MainStation()->IsAllowStaOperation() && IsMouseWheel(ptPos)) {
                if (!MainStation()->getSelectDevice().size()) {
                    QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
                }
                else if (CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType() == CTCWindows::FunType::RouteBuild) {
                    QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor)); 
                }
                else {
                    QApplication::restoreOverrideCursor();
                }
            }
        }

        void DeviceBase::onDeviceClick()
        {
            if (m_mapClickEvent.contains(CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType())) {
                m_mapClickEvent[CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType()]();
            }
        }

        QRectF DeviceBase::QStringToQRectF(QString strRect)
        {
            QStringList strlist = strRect.split(",");
            if (strlist.size() == 4) {
                return QRectF(QPointF(strlist[0].toInt(), strlist[1].toInt()), QPointF(strlist[2].toInt(), strlist[3].toInt()));
            }
            else {
                return QRectF();
            }
        }
        
        QPointF DeviceBase::QStringToQPointF(QString strRect)
        {
            QStringList strlist = strRect.split(",");
            return QPointF(strlist[0].toInt(), strlist[1].toInt());
        }

        QRectF DeviceBase::OutSideRect(const QRectF& rect, int rx, int ry)
        {
            return QRectF(rect.x() - rx, rect.y() - ry, rect.width() + (2 * (qreal)rx), rect.height() + (2 * (qreal)ry));
        }
        


        StaSection::StaSection(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("m_nZ", [&](const QString& strElement) { m_nZ = strElement.toUInt(); });
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p3", [&](const QString& strElement) { p3 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p4", [&](const QString& strElement) { p4 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p12", [&](const QString& strElement) { p12 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("p34", [&](const QString& strElement) { p34 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("pz12", [&](const QString& strElement) { pz12 = QStringToQPointF(strElement); });
            m_mapAttribute.insert("pz34", [&](const QString& strElement) { pz34 = QStringToQPointF(strElement); });
        }

        StaSection::~StaSection()
        {

        }

        void StaSection::Draw(bool isMulti)
        {
            //绘制股道外边缘
            DrawDeviceOutSide();
            //绘制绝缘节
            if (MainStation()->IsVisible(VisibleDev::insulationNode)) {
                DrawInsulateNode();
            }
            return DeviceBase::Draw(isMulti);
        }

        void StaSection::DrawTrackLine(const QPen& pen, const QPointF& ptStart, const QPointF& ptEnd, const bool bOutSide, const int nOffset)
        {
            m_pPainter.setPen(pen);

            if (bOutSide) {
                QPointF ptStartTemp;
                QPointF ptEndTemp;
                //计算三角函数,对绘制位置进行微调
                double a = ((double)ptEnd.x() - (double)ptStart.x());
                double b = ((double)ptEnd.y() - (double)ptStart.y());
                double c = a > 0 ? sqrt(a * a + b * b) : -sqrt(a * a + b * b);

                ptStartTemp = QPointF(ptStart.x() + nOffset * b / c, ptStart.y() - nOffset * a / c);
                ptEndTemp = QPointF(ptEnd.x() + nOffset * b / c, ptEnd.y() - nOffset * a / c);
                if (b != 0) {  //微调位置,以免产生空隙
                    if ((a < 0 && b < 0) || (a > 0 && b > 0)) {
                        ptStartTemp.setX(ptStartTemp.x() + 2);
                        ptEndTemp.setX(ptEndTemp.x() + 2);
                        ptStartTemp.setY(ptStartTemp.y() + 2);
                        ptEndTemp.setY(ptEndTemp.y() + 2);
                    }
                    if ((a < 0 && b > 0) || (a > 0 && b < 0)) {
                        ptStartTemp.setY(ptStartTemp.y() + 1);
                        ptEndTemp.setY(ptEndTemp.y() + 1);
                    }
                }
                m_pPainter.drawLine(Scale(ptStartTemp, bOutSide, true), Scale(ptEndTemp, bOutSide, true));
                ptStartTemp = QPointF(ptStart.x() - nOffset * b / c, ptStart.y() + nOffset * a / c);
                ptEndTemp = QPointF(ptEnd.x() - nOffset * b / c, ptEnd.y() + nOffset * a / c);
                if (b != 0) {  //微调位置,以免产生空隙
                    if ((a < 0 && b > 0) || (a > 0 && b < 0)) {
                        ptStartTemp.setX(ptStartTemp.x() + 2);
                        ptEndTemp.setX(ptEndTemp.x() + 2);
                    }
                }
                m_pPainter.drawLine(Scale(ptStartTemp, bOutSide, false), Scale(ptEndTemp, bOutSide, false));
            }
            else {
                m_pPainter.drawLine(Scale(ptStart), Scale(ptEnd));
            }
        }

        QColor StaSection::getTrackColor()
        {
            QColor cTrackColor = COLOR_TRACK_BLUE;

            if (m_nState & SECTION_STATE_PRELOCK) {
                cTrackColor = COLOR_TRACK_PRELOCK_BLUE;
            }
            else if (m_nState & SECTION_STATE_LOCK) {
                cTrackColor = COLOR_TRACK_WHITE;
            }
            else if (m_nState & SECTION_STATE_FAULTLOCK) {
                cTrackColor = COLOR_TRACK_GREEN;
            }

            if (m_nState & SECTION_STATE_BLOCK) {
                cTrackColor = m_bElapsed ? COLOR_TRACK_RED : cTrackColor;
            }

            if (m_nState & SECTION_STATE_TAKEUP) {
                cTrackColor = COLOR_TRACK_RED;
            }

            if (m_bRangeVisible) {
                cTrackColor = COLOR_TRACK_SELECT_BLUE;
            }
            return cTrackColor;
        }


        int DeviceBtn::m_nFirstBtnType = 0;
        int DeviceBtn::m_nSelectDevCode = -1;
        DeviceBtn::DeviceBtn()
        {

        }

        DeviceBtn::~DeviceBtn()
        {
        
        }

        void DeviceBtn::DrawButton(QPainter& pPainter, const QRectF rcButton, const QColor& cBtnColor, bool bBtnDown, int nType, const QColor cBtnDownColor, const QColor cBtnElapsedColor)
        {
            //按钮边框默认颜色
            QColor btnClrTopLeft = bBtnDown ? COLOR_BTN_GRAY : COLOR_BTN_WHITE;      //按钮边框-TL
            QColor btnClrBottomRight = bBtnDown ? COLOR_BTN_WHITE : COLOR_BTN_GRAY;   //按钮边框-BR
            
            pPainter.setRenderHint(QPainter::Antialiasing, true);
            //绘制按钮边框
            QLinearGradient linearGradient(rcButton.topLeft(), rcButton.bottomRight());
            linearGradient.setColorAt(0.0, btnClrTopLeft);
            linearGradient.setColorAt(1.0, btnClrBottomRight);

            pPainter.setBrush(linearGradient);
            pPainter.setPen(Qt::NoPen);
            if (nType == 1) {   //矩形按钮 
                pPainter.drawRect(rcButton);
            }
            else if (nType == 2) { //圆形按钮 
                pPainter.drawEllipse(rcButton);
            }
            //绘制按钮
            if (nType == 1) {   //矩形按钮 
                pPainter.setBrush(QBrush(bBtnDown ? (DeviceBase::getElapsed() ? cBtnDownColor : cBtnElapsedColor) : cBtnColor, Qt::SolidPattern));
                pPainter.drawRect(rcButton.x() + 2, rcButton.y() + 2, rcButton.width() - 4, rcButton.height() - 4);
            }
            else if (nType == 2) { //圆形按钮 
                pPainter.setBrush(QBrush(bBtnDown ? (DeviceBase::getElapsed() ? cBtnDownColor : cBtnElapsedColor) : cBtnColor, Qt::SolidPattern));
                pPainter.drawEllipse(rcButton.x() + 2, rcButton.y() + 2, rcButton.width() - 4, rcButton.height() - 4);  
            }
            pPainter.setRenderHint(QPainter::Antialiasing, false);
        }

        void DeviceBtn::OnButtonClick(DeviceBase* pDevice)
        {
            if ((m_nBtnState & 0x0F) != 0) {
                return;
            }

            SetBtnState();

            if (m_nBtnState) {
                MainStation()->AddSelectDevice(pDevice);
            }
        }

        void DeviceBtn::BtnStateReset() 
        { 
            m_nBtnState &= 0xF0; 
            m_nFirstBtnType = 0; 
        }


        DeviceArrow::DeviceArrow(QMap<QString, std::function<void(const QString& strElement)>>& mapAttribute)
        {
            mapAttribute.insert("p11", [&](const QString& strElement) { p11 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p12", [&](const QString& strElement) { p12 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p13", [&](const QString& strElement) { p13 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p14", [&](const QString& strElement) { p14 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p15", [&](const QString& strElement) { p15 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p16", [&](const QString& strElement) { p16 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p17", [&](const QString& strElement) { p17 = DeviceBase::QStringToQPointF(strElement); });
            
            mapAttribute.insert("p21", [&](const QString& strElement) { p21 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p22", [&](const QString& strElement) { p22 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p23", [&](const QString& strElement) { p23 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p24", [&](const QString& strElement) { p24 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p25", [&](const QString& strElement) { p25 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p26", [&](const QString& strElement) { p26 = DeviceBase::QStringToQPointF(strElement); });
            mapAttribute.insert("p27", [&](const QString& strElement) { p27 = DeviceBase::QStringToQPointF(strElement); });
        }

        DeviceArrow::~DeviceArrow()
        {
        
        }

        void DeviceArrow::DrawArrow(QPainter& pPainter)
        {
            getArrowColor();
            pPainter.setPen(QPen(COLOR_BTN_WHITE, 1));
            pPainter.setBrush(m_cColor1);
            QPolygonF polygon1;
            polygon1 << Scale(p11) << Scale(p12) << Scale(p13) << Scale(p14) << Scale(p15) << Scale(p16) << Scale(p17);
            pPainter.drawPolygon(polygon1);

            pPainter.setBrush(m_cColor2);
            QPolygonF polygon2;
            polygon2 << Scale(p21) << Scale(p22) << Scale(p23) << Scale(p24) << Scale(p25) << Scale(p26) << Scale(p27);
            pPainter.drawPolygon(polygon2);
        }


        StaDistant::StaDistant(QObject* pParent)
            : DeviceBase(pParent)
        {
            m_mapAttribute.insert("RelayQD", [&](const QString& strElement) { m_strRelayQD = strElement; });
            m_mapAttribute.insert("CJ_qModule", [&](const QString& strElement) { m_strCJ_qModule = strElement; });
            m_mapAttribute.insert("CJ_hModule", [&](const QString& strElement) { m_strCJ_hModule = strElement; });
            m_mapAttribute.insert("CJ_qModule2", [&](const QString& strElement) { m_strCJ_qModule2 = strElement; });
            m_mapAttribute.insert("CJ_hModule2", [&](const QString& strElement) { m_strCJ_hModule2 = strElement; });
            m_mapAttribute.insert("inStart", [&](const QString& strElement) { m_nInStart = strElement.toUInt(); });
            m_mapAttribute.insert("inStart2", [&](const QString& strElement) { m_nInStart2 = strElement.toUInt(); });
            m_mapAttribute.insert("outStart", [&](const QString& strElement) { m_nOutStart = strElement.toUInt(); });
            m_mapAttribute.insert("outStart2", [&](const QString& strElement) { m_nOutStart2 = strElement.toUInt(); });
        }

        StaDistant::~StaDistant()
        {
        
        }

        //站场绘制
        void StaDistant::Draw(bool isMulti)
        {
            //绘制信号灯
            DrawLight();
            //绘制文字
            DrawText();
            return DeviceBase::Draw(isMulti);
        }

        int DeviceTrain::m_nInTrainFrame = false;
        DeviceTrain::DeviceTrain()
        {
        
        }
        DeviceTrain::~DeviceTrain()
        {
        
        }

        void DeviceTrain::DrawTrainFrame(QPainter& pPainter)
        {
            pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            pPainter.setBrush(Qt::NoBrush);
            double nDiploid = m_bMainStation ?
                MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid) :
                MainStation()->getDiploid(DiploidRatio::MultiTrainNumDiploid);

            QRectF rcFrame;
            for (TrainFrame* pTrainFrame : m_vecTrainFrame) {
                if (pTrainFrame->m_bContains || MainStation()->IsVisible(VisibleDev::trainNumWnd)) {
                    rcFrame = QRectF(
                        pTrainFrame->m_rcFrame.center().x() - pTrainFrame->m_rcFrame.width() * nDiploid / 2,
                        pTrainFrame->m_rcFrame.center().y() - pTrainFrame->m_rcFrame.height() * nDiploid / 2,
                        pTrainFrame->m_rcFrame.width() * nDiploid,
                        pTrainFrame->m_rcFrame.height() * nDiploid);
                    pPainter.drawRect(Scale(rcFrame));
                }
            }
        }

        void DeviceTrain::DrawTrain(QPainter& pPainter)
        {
            if (m_pTrain) {
                double nDiploid = 0;
                if (m_bMainStation) {
                    nDiploid = MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid);
                }
                else {
                    nDiploid = MainStation()->getDiploid(DiploidRatio::MultiTrainNumDiploid);
                }
                QRectF rcFrame;
                for (TrainFrame* pTrainFrame : m_vecTrainFrame) {
                    rcFrame = QRectF(
                        pTrainFrame->m_rcFrame.center().x() - pTrainFrame->m_rcFrame.width() * nDiploid / 2,
                        pTrainFrame->m_rcFrame.center().y() - pTrainFrame->m_rcFrame.height() * nDiploid / 2,
                        pTrainFrame->m_rcFrame.width() * nDiploid,
                        pTrainFrame->m_rcFrame.height() * nDiploid);
                    DrawTrainFrame(pPainter, rcFrame);
                    DrawTrainHead(pPainter, rcFrame);
                    DrawTrainTail(pPainter, rcFrame);
                }
            }
            else {
                m_pTrain = nullptr;
                m_bTrainLeave = false;
            }

            if (m_bTrainLeave) {
                m_pTrain = nullptr;
                m_bTrainLeave = false;
            }
        }

        void DeviceTrain::DrawTrainFrame(QPainter& pPainter, QRectF rcFrame)
        {
            pPainter.setPen(Qt::NoPen);
            if (m_pTrain->m_bElectric) {
                pPainter.setBrush(Qt::green);
            }
            else {
                pPainter.setBrush(Qt::white);
            }

            pPainter.drawRect(Scale(rcFrame));
            if (m_pTrain->m_bFreightTrain) {
                pPainter.setPen(Qt::blue);
            }
            else {
                pPainter.setPen(Qt::red);
            }
            double nDiploid = 0;
            if (m_bMainStation) {
                nDiploid = MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid);
            }
            else {
                nDiploid = MainStation()->getDiploid(DiploidRatio::MultiTrainNumDiploid);
            }
            QFont font = pPainter.font();
            font.setPointSizeF(16 * nDiploid);
            pPainter.setFont(font);
            pPainter.drawText(Scale(rcFrame), m_pTrain->m_strTrainNum, QTextOption(Qt::AlignCenter));
        }

        void DeviceTrain::DrawTrainHead(QPainter& pPainter, QRectF rcFrame)
        {
            pPainter.setPen(Qt::NoPen);
            if (m_pTrain->m_bRunning) {
                QPolygonF triangle;
                double nDiploid = MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid);
                pPainter.setRenderHint(QPainter::Antialiasing, true);
                if (m_pTrain->m_bRight) {
                    triangle << Scale(rcFrame.topRight()) << Scale(rcFrame.bottomRight())
                        << Scale(QPointF(rcFrame.right() + 16 * nDiploid, rcFrame.top() + 16 * nDiploid));
                }
                else {
                    triangle << Scale(rcFrame.topLeft()) << Scale(rcFrame.bottomLeft())
                        << Scale(QPointF(rcFrame.left() - 16 * nDiploid, rcFrame.top() + 16 * nDiploid));
                }
                pPainter.drawPolygon(triangle);
                pPainter.setRenderHint(QPainter::Antialiasing, false);
            }
        }

        void DeviceTrain::DrawTrainTail(QPainter& pPainter, QRectF rcFrame)
        {
            QBrush brush;
            int nMinutes = 0;
            if (m_pTrain->m_nEarlyOrLateTime > 0) { //晚点
                nMinutes = m_pTrain->m_nEarlyOrLateTime / 60;
                brush.setColor(Qt::blue);
            }
            else if (m_pTrain->m_nEarlyOrLateTime > 0) {    //早点
                nMinutes = -m_pTrain->m_nEarlyOrLateTime / 60;
                brush.setColor(Qt::red);
            }

            if (nMinutes > 0) {
                double nDiploid = MainStation()->getDiploid(DiploidRatio::StaTrainNumDiploid);
                QRectF rcLabel;
                if (m_pTrain->m_bRight) {
                    rcLabel = QRectF(rcFrame.left() - 40 * nDiploid, rcFrame.top(), 40 * nDiploid, rcFrame.height());
                }
                else {
                    rcLabel = QRectF(rcFrame.right() + 40 * nDiploid, rcFrame.top(), 40 * nDiploid, rcFrame.height());
                }
                pPainter.setPen(Qt::NoPen);
                pPainter.setBrush(brush);
                pPainter.drawRect(Scale(rcLabel));

                QString strTime;
                if (nMinutes < 99) {
                    strTime = QString::number(nMinutes).rightJustified(2, '0');
                }
                else {
                    strTime = QString("%1:%2")
                        .arg(QString::number(nMinutes / 60).rightJustified(2, '0'))
                        .arg(QString::number(nMinutes % 60).rightJustified(2, '0'));
                }
                pPainter.setPen(Qt::white);
                pPainter.drawText(Scale(rcLabel), strTime);
            }
        }

        void DeviceTrain::ShowTrainMenu(QPoint ptPos, int nCode)
        {
            QMenu* pMenu = new QMenu();
            pMenu->setAttribute(Qt::WA_DeleteOnClose);
            QAction* pAction1 = new QAction("加车次号");
            pAction1->setEnabled(!m_pTrain);
            pMenu->addAction(pAction1);
            QObject::connect(pAction1, &QAction::triggered, [=]() {
                StaTrain* pTrain = new StaTrain;
                pTrain->m_nPosCode = nCode;
                if (CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::AddTrain, pTrain)) {
                    m_pTrain = pTrain;
                }
            });
            QAction* pAction2 = new QAction("删除车次号");
            pAction2->setEnabled(m_pTrain);
            pMenu->addAction(pAction2);
            QObject::connect(pAction2, &QAction::triggered, [=]() {
                QDialog* pDialog = new QDialog;
                pDialog->setWindowIcon(QIcon("icon/icon.ico"));
                pDialog->setWindowTitle("车次号删除");
                pDialog->setAttribute(Qt::WA_DeleteOnClose);
                pDialog->setFixedSize(200, 120);
                QVBoxLayout* pLayout = new QVBoxLayout(pDialog);
                pLayout->setMargin(8);
                pLayout->setSpacing(8);
                pDialog->setLayout(pLayout);

                QHBoxLayout* pLayout1 = new QHBoxLayout(pDialog);
                pLayout1->setMargin(8);
                pLayout1->setSpacing(8);
                QLabel* pLabel = new QLabel(pDialog);
                pLabel->setFixedSize(56, 24);
                pLabel->setText("车次号");
                pLayout1->addWidget(pLabel);
                QLineEdit* pLineEdit = new QLineEdit(pDialog);
                pLineEdit->setFixedHeight(24);
                pLineEdit->setEnabled(false);
                pLineEdit->setText(m_pTrain->m_strTrainNum);
                QFont font = pLineEdit->font();
                font.setPointSize(11);
                pLineEdit->setFont(font);
                pLineEdit->setStyleSheet("QLineEdit:disabled { background-color: rgb(250, 250, 250); }");
                pLayout1->addWidget(pLineEdit);
                pLayout->addLayout(pLayout1);

                QHBoxLayout* pLayout2 = new QHBoxLayout(pDialog);
                pLayout2->setMargin(0);
                pLayout2->setSpacing(8);
                QPushButton* pPushButton1 = new QPushButton(pDialog);
                pPushButton1->setFixedSize(64, 24);
                pPushButton1->setText("确定");
                QObject::connect(pPushButton1, &QPushButton::clicked, [=]() {
                    if (MainStation()->DeleteTrain(m_pTrain)) {
                        m_pTrain = nullptr;
                    }
                    pDialog->close();
                });
                pLayout2->addWidget(pPushButton1);
                QPushButton* pPushButton2 = new QPushButton(pDialog);
                pPushButton2->setFixedSize(64, 24);
                pPushButton2->setText("取消");
                QObject::connect(pPushButton1, &QPushButton::clicked, [=]() {
                    pDialog->close();
                });
                pLayout2->addWidget(pPushButton2);

                pLayout->addLayout(pLayout2);

                pDialog->exec();
            });
            QAction* pAction3 = new QAction("车次号确认");
            pAction3->setEnabled(m_pTrain);
            pMenu->addAction(pAction3);
            QObject::connect(pAction3, &QAction::triggered, [=]() {
                CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::ConfirmTrain, m_pTrain);
            });
            QAction* pAction4 = new QAction("修正车次号");
            pAction4->setEnabled(m_pTrain);
            pMenu->addAction(pAction4);
            QObject::connect(pAction4, &QAction::triggered, [=]() {
                CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::AmendTrain, m_pTrain);
            });
            QAction* pAction5 = new QAction("变更车次号");
            pAction5->setEnabled(m_pTrain);
            pMenu->addAction(pAction5);
            QObject::connect(pAction5, &QAction::triggered, [=]() {
                CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::ChangeTrain, m_pTrain);
                });
            QAction* pAction6 = new QAction("该车次属性");
            pAction6->setEnabled(m_pTrain);
            pMenu->addAction(pAction6);
            QObject::connect(pAction6, &QAction::triggered, [=]() {
                CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::ChangeTrainAttr, m_pTrain); 
                });
            QAction* pAction7 = new QAction("设置车次停稳");
            pAction7->setEnabled(m_pTrain);
            pMenu->addAction(pAction7);
            QObject::connect(pAction7, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("确定要设置车次%1停稳?").arg(m_pTrain->m_strTrainNum), "是", "否") == 0) {
                    MainStation()->SetTrainRunning(m_pTrain, false);
                }
            });
            QAction* pAction8 = new QAction("设置车次启动");
            pAction8->setEnabled(m_pTrain);
            pMenu->addAction(pAction8);
            QObject::connect(pAction8, &QAction::triggered, [=]() {
                if (QMessageBox::question(nullptr, MSGBOX_TITTLE, QString("确定要设置车次%1启动?").arg(m_pTrain->m_strTrainNum), "是", "否") == 0) {
                    MainStation()->SetTrainRunning(m_pTrain, true);
                }
            });
            pMenu->exec(ptPos);
        }

        void DeviceTrain::onMouseMoveToTrainFrame(const QPoint& ptPos)
        {
            for (TrainFrame* pTrainFrame : m_vecTrainFrame) {
                if (pTrainFrame->m_rcFrame.contains(ptPos)) {
                    CTCWindows::MainWindow()->setMouseState(CTCWindows::MouseState::AddTrain);
                    if (!pTrainFrame->m_bContains) {
                        m_nInTrainFrame++;
                    }
                    pTrainFrame->m_bContains = true;
                }
                else {
                    if (pTrainFrame->m_bContains) {
                        m_nInTrainFrame--;
                    }
                    pTrainFrame->m_bContains = false;
                }
            }
            if (m_nInTrainFrame <= 0) {
                CTCWindows::MainWindow()->setMouseState(CTCWindows::MouseState::Default);
            }
        }

        void DeviceTrain::MoveTo(DeviceTrain* pNext)
        {
            if (pNext) {
                qDebug() << "MoveTo";
                pNext->SetTrain(m_pTrain);
                m_bTrainLeave = true;
            }
        }
    }
}