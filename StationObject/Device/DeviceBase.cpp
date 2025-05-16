#include "DeviceBase.h"
#include "Global.h"
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>
#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#pragma execution_character_set("utf-8")

namespace Station {
    namespace Device {
        QMap<QString, ClickEventMap> DeviceBase::m_mapClickEvent;
        QMap<QString, AttrMap> DeviceBase::m_mapAttribute;
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

        QRectF DeviceScale::QStringToQRectF(const QString& strRect)
        {
            if (strRect.isEmpty()) {
                return QRectF();
            }

            QStringList strlist;
            QString strRectT = strRect;
            strRectT.replace(" ", "");
            if (strRectT.startsWith("R(") && strRectT.endsWith(")")) {
                strlist = strRectT.mid(2, strRectT.length() - 3).split(",");
            }
            else {
                strlist = strRectT.split(",");
            }

            if (strlist.size() == 4) {
                return QRectF(QPointF(strlist[0].toInt(), strlist[1].toInt()), QPointF(strlist[2].toInt(), strlist[3].toInt()));
            }
            else {
                return QRectF();
            }
        }

        QPointF DeviceScale::QStringToQPointF(const QString& strPoint)
        {
            if (strPoint.isEmpty()) {
                return QPointF();
            }
            QStringList strlist;
            QString strPointT = strPoint;
            strPointT.replace(" ", "");
            if (strPointT.startsWith("P(") && strPointT.endsWith(")")) {
                strlist = strPointT.mid(2, strPointT.length() - 3).split(",");
            }
            else {
                strlist = strPointT.split(",");
            }

            return QPointF(strlist[0].toInt(), strlist[1].toInt());
        }

        QRectF DeviceScale::OutSideRect(const QRectF& rect, int rx, int ry)
        {
            return QRectF(rect.x() - rx, rect.y() - ry, rect.width() + (2 * (qreal)rx), rect.height() + (2 * (qreal)ry));
        }

        DeviceBase::DeviceBase(QObject* pParent)
        {
            m_pParent = pParent;
            m_bMainStation = dynamic_cast<StationObject*>(m_pParent)->IsMainStation();
        }

        DeviceBase::~DeviceBase()
        {

        }

        void DeviceBase::InitAttributeMap()
        {
            m_mapAttribute[m_strType].insert("m_nType", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_nType = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("m_strName", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_strName = strElement; });
            m_mapAttribute[m_strType].insert("m_nCode", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_nCode = strElement.toInt(nullptr, 16); });

            m_mapAttribute[m_strType].insert("m_textRect", [](DeviceBase* pDevice, const QString& strElement) {
                pDevice->m_rcTextRect = QStringToQRectF(strElement);
                QFont font;
                font.setFamily("微软雅黑");
                font.setPixelSize(pDevice->Scale(pDevice->m_nFontSize));//字号
                QFontMetrics  fontMetrics(font);
                pDevice->m_ptName = pDevice->m_rcTextRect.topLeft();
                pDevice->m_rcTextRect = QRectF(pDevice->m_rcTextRect.topLeft(), fontMetrics.size(Qt::TextSingleLine, pDevice->m_strName));
            });
            m_mapAttribute[m_strType].insert("pName", [](DeviceBase* pDevice, const QString& strElement) {
                pDevice->m_ptName = QStringToQPointF(strElement);
                QFont font;
                font.setFamily("微软雅黑");
                font.setPixelSize(pDevice->Scale(pDevice->m_nFontSize));//字号
                QFontMetrics  fontMetrics(font);
                pDevice->m_rcTextRect = QRectF(pDevice->m_ptName, fontMetrics.size(Qt::TextSingleLine, pDevice->m_strName));
            });

            m_mapAttribute[m_strType].insert("m_nSX", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_bUpDown = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("FontHeight", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_nFontSize = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("m_nSize", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_nFontSize = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("center", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_ptCenter = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("InterlockBus", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_nInterlockBus = strElement.toInt(); });
            m_mapAttribute[m_strType].insert("attr", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_nAttr = strElement.toULong(nullptr, 16); });
            m_mapAttribute[m_strType].insert("Module_Code", [](DeviceBase* pDevice, const QString& strElement) { pDevice->m_nModuleCode = strElement.toULong(nullptr, 16); });
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
                    if (IsAllowOperation(mouseEvent->pos()) && Contains(mouseEvent->pos())) {
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
                    if (m_bMainStation) {
                        InitClickEvent();
                    }
                    return;
                }
            }
        }

        void DeviceBase::InitDeviceInfoFromTxt(QStringList& strInfoList, const QString& strDeviceType)
        {
            QString strKey;
            QString strValue;
            for (QString strLine : strInfoList) {
                strKey = strLine.mid(strLine.indexOf(".") + 1, strLine.indexOf("=") - strLine.indexOf(".") - 1);
                strValue = strLine.mid(strLine.indexOf("=") + 1, strLine.indexOf(";") - strLine.indexOf("=") - 1);
                strValue.replace("\"", "");
                ReadDeviceAttributeFromTxt(strKey, strValue);
            }
            InitDeviceAttribute();
            if (m_bMainStation) {
                InitClickEvent();
            }
        }

        void DeviceBase::InitDeviceInfoFromJson(const QJsonObject& object)
        {
            for (QString key : object.keys()) {
                if (key.contains("#")) {
                    continue;
                }
                ReadDeviceAttributeFromJson(object, key);
            }
            InitDeviceAttribute();
            if (m_bMainStation) {
                InitClickEvent();
            }
        }

        void DeviceBase::ReadDeviceAttributeFromXml(QXmlStreamReader* m_pDeviceInfoReader)
        {
            QString strKey = m_pDeviceInfoReader->name().toString();
            if (m_mapAttribute[m_strType].contains(strKey)) {
                if (m_pDeviceInfoReader->attributes().hasAttribute("index")) {
                    nIndex = m_pDeviceInfoReader->attributes().value("index").toInt();
                }
                m_mapAttribute[m_strType][strKey](this, m_pDeviceInfoReader->readElementText());
            }
        }

        void DeviceBase::ReadDeviceAttributeFromJson(const QJsonObject& lampObject, const QString& strKey)
        {
            if (m_mapAttribute[m_strType].contains(strKey)) {
                if (lampObject.value(strKey).isObject()) {
                    QJsonDocument jsonDoc(lampObject.value(strKey).toObject());
                    m_mapAttribute[m_strType][strKey](this, jsonDoc.toJson());
                }
                else if (lampObject.value(strKey).isArray()) {
                    QJsonDocument jsonDoc(lampObject.value(strKey).toArray());
                    m_mapAttribute[m_strType][strKey](this, jsonDoc.toJson());
                }
                else if (lampObject.value(strKey).isString()) {
                    m_mapAttribute[m_strType][strKey](this, lampObject.value(strKey).toString());
                }
                else {
                    m_mapAttribute[m_strType][strKey](this, QString::number(lampObject.value(strKey).toInt()));
                }
            }
        }

        void DeviceBase::ReadDeviceAttributeFromTxt(const QString& strKey, const QString& strValue)
        {
            QString strKeyTemp = strKey;
            if (strKey.indexOf("[")) {
                nIndex = strKey.mid(strKey.indexOf("["), strKey.indexOf("]") - strKey.indexOf("[") - 1).toInt();
                strKeyTemp = strKey.left(strKey.indexOf("["));
            }
            if (m_mapAttribute[m_strType].contains(strKeyTemp)) {
                m_mapAttribute[m_strType][strKeyTemp](this, strValue);
            }
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
            
            DrawCountDown();
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
            QSize sz = fm.size(Qt::TextSingleLine, m_strName);
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

        void DeviceBase::DrawCountDown()
        {
            if (m_nCountDown <= 0 || m_ptCountDown.isNull()) {
                return;
            }
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(12));//字号
            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(Qt::red);
            m_pPainter.setBrush(Qt::NoBrush);
            m_pPainter.drawText(QRectF(m_ptCountDown, QSizeF(24, 16)), QString::number(m_nCountDown), QTextOption(Qt::AlignCenter));
            if (m_bElapsed) {
                m_nCountDown--;
            }
        }

        bool DeviceBase::IsAllowOperation(const QPoint& ptPos)
        {
            return MainStation()->IsAllowStaOperation();
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
            else if (IsAllowOperation(ptPos) && IsMouseWheel(ptPos)) {
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
            else if (m_strType == SIGNALLAMP) {
            
            }
        }

        void DeviceBase::onDeviceClick()
        {
            if (m_mapClickEvent[m_strType].contains(CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType())) {
                m_mapClickEvent[m_strType][CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType()](this);
            }
        }

        StaSection::StaSection(QObject* pParent)
            : DeviceBase(pParent)
        {
            
        }

        StaSection::~StaSection()
        {

        }

        void StaSection::InitAttributeMap()
        {
            m_mapAttribute[m_strType].insert("m_nZ", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSection*>(pDevice)->m_nZ = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("p1", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSection*>(pDevice)->p1 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p2", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSection*>(pDevice)->p2 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p3", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSection*>(pDevice)->p3 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p4", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSection*>(pDevice)->p4 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p12", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSection*>(pDevice)->p12 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("p34", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSection*>(pDevice)->p34 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("pz12", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSection*>(pDevice)->pz12 = QStringToQPointF(strElement); });
            m_mapAttribute[m_strType].insert("pz34", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaSection*>(pDevice)->pz34 = QStringToQPointF(strElement); });
            return DeviceBase::InitAttributeMap();
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
            if (m_bRangeVisible) {
                return COLOR_TRACK_SELECT_BLUE;
            }

            if (m_bPutThrough) {
                return COLOR_TRACK_WHITE;
            }

            QColor cTrackColor = COLOR_TRACK_BLUE;

            if (m_nState & SECTION_STATE_PRELOCK) {
                cTrackColor = COLOR_TRACK_WHITE;
            }
            else if (m_nState & SECTION_STATE_LOCK) {
                cTrackColor = COLOR_TRACK_WHITE;
            }
            else if (m_nState & SECTION_STATE_FAULTLOCK) {
                cTrackColor = COLOR_TRACK_GREEN;
            }
            else if (m_nState & SECTION_STATE_BLOCK) {
                cTrackColor = m_bElapsed ? COLOR_TRACK_RED : cTrackColor;
            }
            else if (m_nState & SECTION_STATE_TAKEUP) {
                cTrackColor = COLOR_TRACK_RED;
            }
            return cTrackColor;
        }

        void StaSection::OrderClear(bool bClearTwinkle)
        {
            m_bSelect = false;
        }

        void StaSection::SetShuntFault(int nState)
        {
            switch (nState)
            {
            case 0x12:  m_nShuntFault ^= 0x01;  break;
            case 0x23:  m_nShuntFault ^= 0x04;  break;
            case 0x24:  m_nShuntFault ^= 0x01;  break;
            case 0x25:  m_nShuntFault ^= 0x02;  break;
            default:    m_nShuntFault ^= 0x01;  break;
            }
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
            if (rcButton.isEmpty()) {
                return;
            }
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

        void DeviceBtn::OnButtonClick()
        {
            if ((m_nBtnState & 0x0F)) {
                return;
            }
            SetBtnState();
            if (m_nBtnState) {
                MainStation()->AddSelectDevice(dynamic_cast<DeviceBase*>(this));
            }
        }

        void DeviceBtn::BtnStateReset() 
        { 
            m_nBtnState &= 0xF0; 
            m_nFirstBtnType = 0; 
        }


        DeviceArrow::DeviceArrow()
        {
            
        }

        DeviceArrow::~DeviceArrow()
        {
        
        }

        void DeviceArrow::InitArrowAttributeMap(QString strType, QMap<QString, AttrMap>& mapAttribute)
        {
            mapAttribute[strType].insert("p11", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p11 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p12", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p12 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p13", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p13 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p14", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p14 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p15", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p15 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p16", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p16 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p17", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p17 = QStringToQPointF(strElement); });

            mapAttribute[strType].insert("p21", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p21 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p22", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p22 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p23", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p23 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p24", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p24 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p25", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p25 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p26", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p26 = QStringToQPointF(strElement); });
            mapAttribute[strType].insert("p27", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<DeviceArrow*>(pDevice)->p27 = QStringToQPointF(strElement); });

            mapAttribute[strType].insert("m_ArrowPoint", [](DeviceBase* pDevice, const QString& strElement) {
                DeviceArrow* pArrow = dynamic_cast<DeviceArrow*>(pDevice);
                pArrow->ptArrow = QStringToQPointF(strElement);
                pArrow->p11 = QPointF(pArrow->ptArrow.x() - 10, pArrow->ptArrow.y());
                pArrow->p12 = QPointF(pArrow->p11.x() - 8, pArrow->p11.y() - 8);
                pArrow->p13 = QPointF(pArrow->p12.x(), pArrow->p12.y() + 4);
                pArrow->p14 = QPointF(pArrow->p13.x() - 16, pArrow->p13.y());
                pArrow->p15 = QPointF(pArrow->p14.x(), pArrow->p14.y() + 8);
                pArrow->p16 = QPointF(pArrow->p15.x() + 16, pArrow->p15.y());
                pArrow->p17 = QPointF(pArrow->p16.x(), pArrow->p16.y() + 4);

                pArrow->p21 = QPointF(pArrow->ptArrow.x() + 10, pArrow->ptArrow.y());
                pArrow->p22 = QPointF(pArrow->p21.x() + 8, pArrow->p21.y() - 8);
                pArrow->p23 = QPointF(pArrow->p22.x(), pArrow->p22.y() + 4);
                pArrow->p24 = QPointF(pArrow->p23.x() + 16, pArrow->p23.y());
                pArrow->p25 = QPointF(pArrow->p24.x(), pArrow->p24.y() + 8);
                pArrow->p26 = QPointF(pArrow->p25.x() - 16, pArrow->p25.y());
                pArrow->p27 = QPointF(pArrow->p26.x(), pArrow->p26.y() + 4);
            });
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
            
        }

        StaDistant::~StaDistant()
        {
        
        }

        void StaDistant::InitAttributeMap()
        {
            AttrMap mapAttrFun;
            m_mapAttribute[m_strType].insert("RelayQD", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_strRelayQD = strElement; });
            m_mapAttribute[m_strType].insert("CJ_qModule", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_strCJ_qModule = strElement; });
            m_mapAttribute[m_strType].insert("CJ_hModule", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_strCJ_hModule = strElement; });
            m_mapAttribute[m_strType].insert("CJ_qModule2", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_strCJ_qModule2 = strElement; });
            m_mapAttribute[m_strType].insert("CJ_hModule2", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_strCJ_hModule2 = strElement; });
            m_mapAttribute[m_strType].insert("inStart", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_nInStart = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("inStart2", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_nInStart2 = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("outStart", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_nOutStart = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("outStart2", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_nOutStart2 = strElement.toUInt(); });
            m_mapAttribute[m_strType].insert("m_Direction", [](DeviceBase* pDevice, const QString& strElement) { dynamic_cast<StaDistant*>(pDevice)->m_strDirection = strElement; });
            m_mapAttribute[m_strType].insert("m_FrameRect", [=](DeviceBase* pDevice, const QString& strElement) { 
                
                dynamic_cast<StaDistant*>(pDevice)->m_rcFrame = QStringToQRectF(strElement); 
                qDebug() << m_strType << dynamic_cast<StaDistant*>(pDevice)->getName() << strElement << QStringToQRectF(strElement) << dynamic_cast<StaDistant*>(pDevice)->m_rcFrame;
                });
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
            return DeviceBase::InitAttributeMap();
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
    }
}