#include "DeviceBase.h"
#include "Global.h"
#include <QPainter>
#include <QMouseEvent>
#include <QCursor>
#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
namespace Station {
    namespace Device {

        int DeviceBase::m_rcWheelDevCode = -1;
        QPainter DeviceBase::m_pPainter;
        bool DeviceBase::m_bElapsed = false;
        int DeviceBase::m_nWheelDevCode = -1;

        DeviceBase::DeviceBase(QObject* pParent)
        {
            m_pParent = pParent;
            m_mapAttribute.insert("m_nType", [&](const QString& strElement) { m_nType = strElement.toUInt(); });
            m_mapAttribute.insert("m_strName", [&](const QString& strElement) { m_strName = strElement; });
            m_mapAttribute.insert("m_nCode", [&](const QString& strElement) { m_nCode = strElement.toInt(nullptr, 16); });

            m_mapAttribute.insert("m_textRect", [&](const QString& strElement) {
                m_rcTextRect = QStringToQRect(strElement);
                QFont font;
                font.setFamily("微软雅黑");
                font.setPixelSize(Scale(m_nFontSize));//字号
                QFontMetrics  fontMetrics(font);
                m_rcTextRect = QRect(m_rcTextRect.topLeft(), fontMetrics.size(Qt::TextSingleLine, m_strName));
            });
            m_mapAttribute.insert("pName", [&](const QString& strElement) {
                m_ptName = QStringToQPoint(strElement);
                QFont font;
                font.setFamily("微软雅黑");
                font.setPixelSize(Scale(m_nFontSize));//字号
                QFontMetrics  fontMetrics(font);
                m_rcTextRect = QRect(m_ptName, fontMetrics.size(Qt::TextSingleLine, m_strName));
            });

            m_mapAttribute.insert("m_nSX", [&](const QString& strElement) { m_bUpDown = strElement.toUInt(); });
            m_mapAttribute.insert("FontHeight", [&](const QString& strElement) { m_nFontSize = strElement.toUInt(); });
            m_mapAttribute.insert("center", [&](const QString& strElement) { m_ptCenter = QStringToQPoint(strElement); });
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

            if (event->type() == QEvent::MouseMove) {
                QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
                onMouseMoveToDevice(mouseEvent->pos());
                //if (Contains(mouseEvent->pos()) && (m_rcWheelDevCode == -1 || m_rcWheelDevCode == m_nCode)) {
                //    m_bRangeVisible = true;
                //    m_rcWheelDevCode = m_nCode;
                //}
                //else {
                //    m_bRangeVisible = false;
                //    if (m_rcWheelDevCode == m_nCode) {
                //        m_rcWheelDevCode = -1;
                //    }
                //}
            }

            if (event->type() == QEvent::MouseButtonRelease) {  //鼠标点击事件
                QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
                if (MainStation()->IsAllowStaOperation() && Contains(mouseEvent->pos())) {
                    if (mouseEvent->button() == Qt::LeftButton) {   //鼠标左键点击事件
                        onDeviceClick();
                    }
                    else if (mouseEvent->button() == Qt::RightButton) {   //鼠标右键点击事件
                        //onDeviceClick();
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
            InitClickEvent();
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
                if (lampObject.value(strKey).isString()) {
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
            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(getDeviceNameColor());
            m_pPainter.drawText(Scale(m_rcTextRect), m_strName, QTextOption(Qt::AlignCenter));
        }

        void DeviceBase::Draw(const bool& isMulti)
        {
            //绘制设备选中虚线框
            if (m_bRangeVisible) {
                DrawSelectRange();
            }
            //绘制设备名称
            if (m_bShowName) {
                DrawDeviceName();
            }
            //绘制培训提示信息
            if (m_bShowTips) {
                DrawCultivateTips();
            }
        }

        void DeviceBase::onMouseMoveToDevice(const QPoint& ptPos)
        {
            if ((m_nCode != -1) && Contains(ptPos) && (m_rcWheelDevCode == -1 || m_rcWheelDevCode == m_nCode)) {
                m_bRangeVisible = true;
                m_rcWheelDevCode = m_nCode;
            }
            else {
                m_bRangeVisible = false;
                if (m_rcWheelDevCode == m_nCode) {
                    m_rcWheelDevCode = -1;
                }
            }

            if (m_rcWheelDevCode == -1) {
                QApplication::restoreOverrideCursor();
            }
            else if (MainStation()->IsAllowStaOperation() && IsMouseWheel(ptPos) && (!MainStation()->getDevSelected())) {
                QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
            }
        }

        void DeviceBase::onDeviceClick()
        {
            if (m_mapClickEvent.contains(CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType())) {
                m_mapClickEvent[CTCWindows::BaseWnd::StaFunBtnToolBar::getCurrFunType()]();
            }
        }

        QRect DeviceBase::QStringToQRect(QString strRect)
        {
            QStringList strlist = strRect.split(",");
            if (strlist.size() == 4) {
                return QRect(QPoint(strlist[0].toInt(), strlist[1].toInt()), QPoint(strlist[2].toInt(), strlist[3].toInt()));
            }
            else {
                return QRect();
            }
        }
        
        QPoint DeviceBase::QStringToQPoint(QString strRect)
        {
            QStringList strlist = strRect.split(",");
            return QPoint(strlist[0].toInt(), strlist[1].toInt());
        }
        
        int DeviceBase::Scale(const int& value, const int& nOffset)
        {
            return (value + nOffset) * StationObject::getDiploid(1);
        }

        QPoint DeviceBase::Scale(const QPoint& pt, QObject* pParent, const bool bOutSide, const bool bTopLine)
        {
            QPoint ptOffset;
            if (pParent) {
                ptOffset = dynamic_cast<StationObject*>(pParent)->getOffset();
            }
            
            if (!bOutSide) {
                return QPoint(Scale(pt.x(), ptOffset.x()), Scale(pt.y(), ptOffset.y()));
            }
            else {
                if (bTopLine) {
                    return QPoint(Scale(pt.x(), ptOffset.x()), Scale(pt.y() - TRACK_WIDTH / 2, ptOffset.y()) + 1);
                }
                else {
                    return QPoint(Scale(pt.x(), ptOffset.x()), Scale(pt.y() + TRACK_WIDTH / 2, ptOffset.y()));
                } 
            }
        }

        QRect DeviceBase::Scale(const QRect& rect, QObject* pParent)
        {
            QPoint ptOffset;
            if (pParent) {
                ptOffset = dynamic_cast<StationObject*>(pParent)->getOffset();
            }

            return QRect(Scale(rect.x(), ptOffset.x()),
                    Scale(rect.y(), ptOffset.y()),
                    Scale(rect.width()), Scale(rect.height()));
        }

        QRect DeviceBase::OutSideRect(const QRect& rect, int rx, int ry)
        {
            return QRect(rect.x() - rx, rect.y() - ry, rect.width() + 2 * rx, rect.height() + 2 * ry);
        }
        


        StaSection::StaSection(QObject* pParent)
        {
            m_mapAttribute.insert("m_nZ", [&](const QString& strElement) { m_nZ = strElement.toUInt(); });
            m_mapAttribute.insert("p1", [&](const QString& strElement) { p1 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p2", [&](const QString& strElement) { p2 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p3", [&](const QString& strElement) { p3 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p4", [&](const QString& strElement) { p4 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p12", [&](const QString& strElement) { p12 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p34", [&](const QString& strElement) { p34 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("pz12", [&](const QString& strElement) { pz12 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("pz34", [&](const QString& strElement) { pz34 = QStringToQPoint(strElement); });
        }

        StaSection::~StaSection()
        {

        }

        void StaSection::Draw(const bool& isMulti)
        {
            //绘制股道外边缘
            DrawDeviceOutSide();
            //绘制绝缘节
            if (MainStation()->IsVisible(VisibleDev::insulationNode)) {
                DrawInsulateNode();
            }
            return DeviceBase::Draw(isMulti);
        }

        void StaSection::DrawTrackLine(const QPen& pen, const QPoint& ptStart, const QPoint& ptEnd, const bool bOutSide, const int nOffset)
        {
            m_pPainter.setPen(pen);

            if (bOutSide) {
                QPoint ptStartTemp;
                QPoint ptEndTemp;
                //计算三角函数,对绘制位置进行微调
                double a = ((double)ptEnd.x() - (double)ptStart.x());
                double b = ((double)ptEnd.y() - (double)ptStart.y());
                double c = a > 0 ? sqrt(a * a + b * b) : -sqrt(a * a + b * b);

                ptStartTemp = QPoint(ptStart.x() + nOffset * b / c, ptStart.y() - nOffset * a / c);
                ptEndTemp = QPoint(ptEnd.x() + nOffset * b / c, ptEnd.y() - nOffset * a / c);
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
                m_pPainter.drawLine(Scale(ptStartTemp, m_pParent, bOutSide, true), Scale(ptEndTemp, m_pParent, bOutSide, true));
                ptStartTemp = QPoint(ptStart.x() - nOffset * b / c, ptStart.y() + nOffset * a / c);
                ptEndTemp = QPoint(ptEnd.x() - nOffset * b / c, ptEnd.y() + nOffset * a / c);
                if (b != 0) {  //微调位置,以免产生空隙
                    if ((a < 0 && b > 0) || (a > 0 && b < 0)) {
                        ptStartTemp.setX(ptStartTemp.x() + 2);
                        ptEndTemp.setX(ptEndTemp.x() + 2);
                    }
                }
                m_pPainter.drawLine(Scale(ptStartTemp, m_pParent, bOutSide, false), Scale(ptEndTemp, m_pParent, bOutSide, false));
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

        void DeviceBtn::DrawButton(QPainter& pPainter, const QRect rcButton, const QColor& cBtnColor, bool bBtnDown, int nType, const QColor cBtnDownColor, const QColor cBtnElapsedColor)
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
            if (m_nBtnState != 0) {
                return;
            }

            SetBtnState();

            if (m_nBtnState) {
                MainStation()->AddSelectDevice(pDevice);
                MainStation()->setDevSelected();
            }
        }

        //void DeviceBtn::onMouseMoveToButton(const QPoint& ptPos, const int& nCode)
        //{
        //    if (StationObject::IsAllowStaOperation() &&  IsMouseWheel(ptPos)) {
        //        QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
        //        m_nSelectDevCode = nCode;
        //    }
        //    else if (m_nSelectDevCode == nCode) {   //鼠标离开时清空
        //        m_nSelectDevCode = -1;
        //    }
        //
        //    if (m_nSelectDevCode == -1) {
        //        QApplication::restoreOverrideCursor();
        //    }
        //}

        void DeviceBtn::BtnStateReset() 
        { 
            m_nBtnState = 0; 
            m_nFirstBtnType = 0; 
        }


        DeviceArrow::DeviceArrow(QMap<QString, std::function<void(const QString& strElement)>> mapAttribute)
        {
            mapAttribute.insert("p11", [&](const QString& strElement) { p11 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p12", [&](const QString& strElement) { p12 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p13", [&](const QString& strElement) { p13 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p14", [&](const QString& strElement) { p14 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p15", [&](const QString& strElement) { p15 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p16", [&](const QString& strElement) { p16 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p17", [&](const QString& strElement) { p17 = DeviceBase::QStringToQPoint(strElement); });
            
            mapAttribute.insert("p21", [&](const QString& strElement) { p21 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p22", [&](const QString& strElement) { p22 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p23", [&](const QString& strElement) { p23 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p24", [&](const QString& strElement) { p24 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p25", [&](const QString& strElement) { p25 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p26", [&](const QString& strElement) { p26 = DeviceBase::QStringToQPoint(strElement); });
            mapAttribute.insert("p27", [&](const QString& strElement) { p27 = DeviceBase::QStringToQPoint(strElement); });
        }

        DeviceArrow::~DeviceArrow()
        {
        
        }

        void DeviceArrow::DrawArrow(QPainter& pPainter)
        {
            getArrowColor();
            pPainter.setPen(QPen(COLOR_BTN_WHITE, 1));
            pPainter.setBrush(m_cColor1);
            QPolygon polygon1;
            polygon1 << p11 << p12 << p13 << p14 << p15 << p16 << p17;
            for (QPoint& pt : polygon1) {
                pt = DeviceBase::Scale(pt);
            }
            pPainter.drawPolygon(polygon1);

            pPainter.setBrush(m_cColor2);
            QPolygon polygon2;
            polygon2 << p21 << p22 << p23 << p24 << p25 << p26 << p27;
            for (QPoint& pt : polygon2) {
                pt = DeviceBase::Scale(pt);
            }
            pPainter.drawPolygon(polygon2);
        }


        StaDistant::StaDistant(QObject* pParent)
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
        void StaDistant::Draw(const bool& isMulti)
        {
            //绘制信号灯
            DrawLight();
            //绘制文字
            DrawText();
            return DeviceBase::Draw(isMulti);
        }
    }
}