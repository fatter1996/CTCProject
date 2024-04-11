#include "DeviceBase.h"
#include "Global.h"
#include <QPainter>
#include <QMouseEvent>

namespace Station {
    namespace Device {

        QPainter DeviceBase::m_pPainter;
        int DeviceBase::m_nTimerId_500 = -1;
        bool DeviceBase::m_bElapsed = false;

        DeviceBase::DeviceBase(QObject* parent)
        {
            m_nTimerId_500 = startTimer(500);

            m_mapAttribute.insert("m_nType", [&](const QString& strElement) { m_nType = strElement.toUInt(); });
            m_mapAttribute.insert("m_strName", [&](const QString& strElement) { m_strName = strElement; });
            m_mapAttribute.insert("m_nCode", [&](const QString& strElement) { m_nCode = strElement.toUInt(nullptr, 16); });

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

            m_mapAttribute.insert("m_nSX", [&](const QString& strElement) { m_nSX = strElement.toUInt(); });
            m_mapAttribute.insert("FontHeight", [&](const QString& strElement) { m_nFontSize = strElement.toUInt(); });
            m_mapAttribute.insert("center", [&](const QString& strElement) { m_ptCenter = QStringToQPoint(strElement); });
            m_mapAttribute.insert("InterlockBus", [&](const QString& strElement) { m_nInterlockBus = strElement.toInt(); });
            m_mapAttribute.insert("attr", [&](const QString& strElement) { m_nAttr = strElement.toULong(nullptr, 16); });
            m_mapAttribute.insert("Module_Code", [&](const QString& strElement) { m_nModuleCode = strElement.toULong(nullptr, 16); });
        }

        DeviceBase::~DeviceBase()
        {
            killTimer(m_nTimerId_500);
        }

        bool DeviceBase::eventFilter(QObject* obj, QEvent* event)
        {
            if (event->type() == QEvent::Paint) {
                m_pPainter.begin((QWidget*)obj);
                Draw(m_bElapsed);
                m_pPainter.end();
            }

            if (event->type() == QEvent::MouseMove) {
                QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
                if (m_rcRespondRect.contains(mouseEvent->pos())) {
                    m_bRangeVisible = true;
                }
                else {
                    m_bRangeVisible = false;
                }
            }

            return QObject::eventFilter(obj, event);
        }

        void DeviceBase::timerEvent(QTimerEvent* event)
        {
            if (event->timerId() == m_nTimerId_500) {
                m_bElapsed = !m_bElapsed;
            }
            return QObject::timerEvent(event);
        }

        void DeviceBase::InitDeviceInfo(QXmlStreamReader* m_pDeviceInfoReader, QString strDeviceType)
        {
            while (!m_pDeviceInfoReader->atEnd()) {
                m_pDeviceInfoReader->readNext();
                if (m_pDeviceInfoReader->isStartElement()) {
                    ReadDeviceAttribute(m_pDeviceInfoReader);
                }
                else if (m_pDeviceInfoReader->isEndElement() && m_pDeviceInfoReader->name() == strDeviceType) {
                    InitDeviceAttribute();
                    return;
                }
            }
        }

        void DeviceBase::ReadDeviceAttribute(QXmlStreamReader* m_pDeviceInfoReader)
        {
            QString str = m_pDeviceInfoReader->name().toString();
            if (m_mapAttribute.contains(str)) {
                if (m_pDeviceInfoReader->attributes().hasAttribute("index")) {
                    nIndex = m_pDeviceInfoReader->attributes().value("index").toInt();
                }
                m_mapAttribute[str](m_pDeviceInfoReader->readElementText());
            }
        }

        void DeviceBase::DrawDeviceName(const bool& bElapsed)
        {
            QFont font;
            font.setFamily("微软雅黑");
            font.setPixelSize(Scale(m_nFontSize));//字号
            QPen pen = getDeviceNameColor(bElapsed);
            if (pen.color() == Qt::NoPen) {
                return;
            }
            m_pPainter.setFont(font);//设置字体
            m_pPainter.setPen(getDeviceNameColor(bElapsed));
            
            m_pPainter.drawText(Scale(m_rcTextRect), m_strName, QTextOption(Qt::AlignCenter));

            //if (m_rcTextRect != QRect()) {
            //    m_pPainter.drawText(Scale(m_rcTextRect), m_strName, QTextOption(Qt::AlignCenter));
            //}
            //else if (m_ptName != QPoint()) {
            //    m_pPainter.drawText(Scale(QRect(m_ptName, fontMetrics.size(Qt::TextSingleLine, m_strName))), m_strName, QTextOption(Qt::AlignCenter));
            //}
        }

        void DeviceBase::InitDeviceAttribute()
        {
        
        }

        void DeviceBase::Draw(const bool& bElapsed, const bool& isMulti)
        {
            //绘制设备选中虚线框
            if (m_bRangeVisible) {
                DrawSelectRange();
            }
            //绘制设备名称
            DrawDeviceName(bElapsed);
        }

        void DeviceBase::DrawSelectRange()
        {
            //设备名称虚线框
            m_pPainter.setPen(QPen(COLOR_TRACK_WHITE, 1, Qt::DashLine));
            m_pPainter.setBrush(COLOR_TRACK_SELECT_BLUE);
            m_pPainter.drawRect(Scale(OutSideRect(m_rcTextRect, 2, 0)));
        }

        QPen DeviceBase::getDeviceNameColor(const bool& bElapsed)
        {
            return QPen(Qt::white);
        }

        void DeviceBase::setState(const uint& nState) 
        { 
            m_nState = nState;
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
            return (value + nOffset) * StationObject::m_nDiploid;
        }

        QPoint DeviceBase::Scale(const QPoint& pt, const bool bOutSide, const bool bTopLine)
        {
            if (!bOutSide) {
                return QPoint(Scale(pt.x(), StationObject::m_ptOffset.x()), Scale(pt.y(), StationObject::m_ptOffset.y()));
            }
            else {
                if (bTopLine) {
                    return QPoint(Scale(pt.x(), StationObject::m_ptOffset.x()), Scale(pt.y() - TRACK_WIDTH / 2, StationObject::m_ptOffset.y()) + 1);
                }
                else {
                    return QPoint(Scale(pt.x(), StationObject::m_ptOffset.x()), Scale(pt.y() + TRACK_WIDTH / 2, StationObject::m_ptOffset.y()));
                } 
            }
        }

        QRect DeviceBase::Scale(const QRect& rect)
        {
            return QRect(Scale(rect.x(), StationObject::m_ptOffset.x()), 
                    Scale(rect.y(), StationObject::m_ptOffset.y()),
                    Scale(rect.width()), Scale(rect.height()));
        }

        QRect DeviceBase::OutSideRect(const QRect& rect, int rx, int ry)
        {
            return QRect(rect.x() - rx, rect.y() - ry, rect.width() + 2 * rx, rect.height() + 2 * ry);
        }
        


        StaSection::StaSection(QObject* parent)
        {
            m_mapAttribute.insert("m_nZ", [&](QString strElement) { m_nZ = strElement.toUInt(); });
            m_mapAttribute.insert("p1", [&](QString strElement) { p1 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p2", [&](QString strElement) { p2 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p3", [&](QString strElement) { p3 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p4", [&](QString strElement) { p4 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p12", [&](QString strElement) { p12 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("p34", [&](QString strElement) { p34 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("pz12", [&](QString strElement) { pz12 = QStringToQPoint(strElement); });
            m_mapAttribute.insert("pz34", [&](QString strElement) { pz34 = QStringToQPoint(strElement); });
        }

        StaSection::~StaSection()
        {

        }

        void StaSection::Draw(const bool& bElapsed, const bool& isMulti)
        {
            //绘制股道外边缘
            DrawDeviceOutSide(bElapsed);
            //绘制绝缘节
            if (m_bShowInsulateNode) {
                DrawInsulateNode();
            }
            return DeviceBase::Draw(bElapsed, isMulti);
        }

        void StaSection::DrawTrackLine(QPen pen, const QPoint& ptStart, const QPoint& ptEnd, const bool bOutSide, const int nOffset)
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
                m_pPainter.drawLine(Scale(ptStartTemp, bOutSide, true), Scale(ptEndTemp, bOutSide, true));
                ptStartTemp = QPoint(ptStart.x() - nOffset * b / c, ptStart.y() + nOffset * a / c);
                ptEndTemp = QPoint(ptEnd.x() - nOffset * b / c, ptEnd.y() + nOffset * a / c);
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

        QColor StaSection::getTrackColor(const bool& bElapsed)
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
                cTrackColor = bElapsed ? COLOR_TRACK_RED : cTrackColor;
            }

            if (m_nState & SECTION_STATE_TAKEUP) {
                cTrackColor = COLOR_TRACK_RED;
            }

            if (m_bRangeVisible) {
                cTrackColor = COLOR_TRACK_SELECT_BLUE;
            }
            return cTrackColor;
        }



        DeviceBtn::DeviceBtn()
        {

        }

        DeviceBtn::~DeviceBtn()
        {
        
        }

        void DeviceBtn::DrawButton(QPainter& pPainter, const bool& bElapsed, const QRect rcButton, const QColor cBtnColor, int nType, const QColor cBtnDownColor, const QColor cBtnElapsedColor)
        {
            //按钮边框默认颜色
            QColor btnClrTopLeft = m_bBtnDown ? COLOR_BTN_GRAY : COLOR_BTN_WHITE;      //按钮边框-TL
            QColor btnClrBottomRight = m_bBtnDown ? COLOR_BTN_WHITE : COLOR_BTN_GRAY;   //按钮边框-BR
            
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
                pPainter.setBrush(QBrush(m_bBtnDown ? (bElapsed ? cBtnDownColor : cBtnElapsedColor) : cBtnColor, Qt::SolidPattern));
                pPainter.drawRect(rcButton.x() + 2, rcButton.y() + 2, rcButton.width() - 4, rcButton.height() - 4);
            }
            else if (nType == 2) { //圆形按钮 
                pPainter.setBrush(QBrush(m_bBtnDown ? (bElapsed ? cBtnDownColor : cBtnElapsedColor) : cBtnColor, Qt::SolidPattern));
                pPainter.drawEllipse(rcButton.x() + 2, rcButton.y() + 2, rcButton.width() - 4, rcButton.height() - 4);  
            }
            pPainter.setRenderHint(QPainter::Antialiasing, false);
        }



        DeviceArrow::DeviceArrow(QMap<QString, std::function<void(const QString&)>>& m_mapAttribute)
        {
            m_mapAttribute.insert("p11", [&](const QString& strElement) { p11 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p12", [&](const QString& strElement) { p12 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p13", [&](const QString& strElement) { p13 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p14", [&](const QString& strElement) { p14 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p15", [&](const QString& strElement) { p15 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p16", [&](const QString& strElement) { p16 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p17", [&](const QString& strElement) { p17 = DeviceBase::QStringToQPoint(strElement); });

            m_mapAttribute.insert("p21", [&](const QString& strElement) { p21 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p22", [&](const QString& strElement) { p22 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p23", [&](const QString& strElement) { p23 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p24", [&](const QString& strElement) { p24 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p25", [&](const QString& strElement) { p25 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p26", [&](const QString& strElement) { p26 = DeviceBase::QStringToQPoint(strElement); });
            m_mapAttribute.insert("p27", [&](const QString& strElement) { p27 = DeviceBase::QStringToQPoint(strElement); });
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



        StaDistant::StaDistant(QObject* parent)
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
        void StaDistant::Draw(const bool& bElapsed, const bool& isMulti)
        {
            //绘制信号灯
            DrawLight();
            //绘制文字
            DrawText();
            return DeviceBase::Draw(bElapsed, isMulti);
        }
    }
}