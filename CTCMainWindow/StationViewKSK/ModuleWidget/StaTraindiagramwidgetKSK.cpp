#include "StaTrainDiagramWidgetKSK.h"
#include <QScrollBar>
#include "Global.h"

namespace CTCWindows {
    namespace CASCO {
        StaTrainDiagramWidgetKSK::StaTrainDiagramWidgetKSK(QWidget* parent)
            : CTCWindows::BaseWnd::StaTrainDiagramWidget(parent)
        {
            ui.setupUi(this);
            ui.diagramWidget->installEventFilter(this);
            ui.stationWidget->installEventFilter(this);

        }

        StaTrainDiagramWidgetKSK::~StaTrainDiagramWidgetKSK()
        {

        }

        void StaTrainDiagramWidgetKSK::InitTraindiagramw()
        {
            ui.diagramWidget->setFixedSize(6 * 24 * TrainDiagram.miniteDistance + (SPACING_LEFTORRIGHT * 2),
                SPACING_TOPORBOTTOM * 2
                + TrainDiagram.vectRailwayLine.size() * (TrainDiagram.stationDistance * 2)
                + (TrainDiagram.vectRailwayLine.size() - 1) * TrainDiagram.lintDistance
                + SPACING_TIMEAXIS * 2);
            ui.stationWidget->setFixedHeight(ui.diagramWidget->height());
            ui.scrollArea_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            ui.scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            ui.scrollArea_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            ui.scrollArea_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

            Timer_ID_60000 = startTimer(60000);
            nowTime = QDateTime::currentDateTime().time();
            nowTimeLinePx = getPointXByTime(nowTime);

            int startY = SPACING_TOPORBOTTOM + SPACING_TIMEAXIS;
            for (int i = 0; i < TrainDiagram.vectRailwayLine.size(); i++) {
                TrainDiagram.vectRailwayLine[i].startY = startY;
                TrainDiagram.vectRailwayLine[i].middleY = startY + TrainDiagram.stationDistance;
                TrainDiagram.vectRailwayLine[i].endY = startY + TrainDiagram.stationDistance * 2;
                startY += TrainDiagram.stationDistance * 2 + TrainDiagram.lintDistance;
            }
        }

        void StaTrainDiagramWidgetKSK::drawTimeAxis(bool isTop)
        {
            QPainter painter(ui.diagramWidget);
            painter.setRenderHint(QPainter::Antialiasing, false);

            QPen pen; //ª≠± 
            pen.setColor(Qt::green);
            pen.setWidth(2);
            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(Qt::green); //ª≠À¢
            painter.setPen(pen); //ÃÌº”ª≠± 
            painter.setBrush(brush); //ÃÌº”ª≠À¢

            int startY = 0;
            if (isTop)
                startY = SPACING_TOPORBOTTOM;
            else startY = SPACING_TOPORBOTTOM
                + TrainDiagram.vectRailwayLine.size() * (TrainDiagram.stationDistance * 2)
                + (TrainDiagram.vectRailwayLine.size() - 1) * TrainDiagram.lintDistance
                + SPACING_TIMEAXIS * 2;


            painter.drawLine(SPACING_LEFTORRIGHT, startY, 6 * 24 * TrainDiagram.miniteDistance + SPACING_LEFTORRIGHT, startY);
            int hours = 3;

            for (int i = 0; i < 6 * 24 + 1; i++)
            {
                if (hours >= 24)
                    hours = 0;
                if (i % 6 == 0)//–° ±
                {
                    QPen pen; //ª≠± 
                    pen.setColor(Qt::green);
                    pen.setWidth(2);
                    QFont font("Œ¢»Ì—≈∫⁄", 11, QFont::Bold);
                    painter.setFont(font);
                    painter.setPen(pen);

                    if (isTop)
                    {
                        painter.drawLine(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                            TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY - 30);

                        painter.drawText(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT - 20, startY - 35,
                            QString("%1:00").arg(hours, 2, 10, QLatin1Char('0')));
                    }
                    else
                    {
                        painter.drawLine(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                            TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY + 30);

                        painter.drawText(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT - 20, startY + 48,
                            QString("%1:00").arg(hours, 2, 10, QLatin1Char('0')));
                    }


                    hours++;
                }
                else if (i % 3 == 0) //∞Î–° ±
                {
                    QPen pen; //ª≠± 
                    pen.setColor(Qt::green);
                    pen.setWidth(1);
                    painter.setPen(pen);
                    QFont font("Œ¢»Ì—≈∫⁄", 10, QFont::Bold);
                    painter.setFont(font);
                    if (isTop)
                    {
                        painter.drawLine(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                            TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY - 25);
                        painter.drawText(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT - 8, startY - 30, "30");
                    }
                    else
                    {
                        painter.drawLine(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                            TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY + 25);
                        painter.drawText(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT - 8, startY + 42, "30");
                    }

                }
                else // Æ∑÷÷”
                {
                    QPen pen; //ª≠± 
                    pen.setColor(Qt::green);
                    QVector<qreal>dashes;
                    dashes << 3 << 4;
                    pen.setDashPattern(dashes);
                    pen.setWidth(1);
                    painter.setPen(pen);
                    if (isTop)
                    {
                        painter.drawLine(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                            TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY - 20);
                    }
                    else
                    {
                        painter.drawLine(TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                            TrainDiagram.miniteDistance * i + SPACING_LEFTORRIGHT, startY + 20);
                    }
                }
            }
        }

        void StaTrainDiagramWidgetKSK::drawGrid()
        {
            int startY = SPACING_TOPORBOTTOM + SPACING_TIMEAXIS;
            int endX = 6 * 24 * TrainDiagram.miniteDistance + SPACING_LEFTORRIGHT;
            QPainter painter(ui.diagramWidget);
            painter.setRenderHint(QPainter::Antialiasing, false);
            QPen pen; //ª≠± 
            pen.setColor(Qt::green);
            pen.setWidth(2);
            QVector<qreal>dashes;
            dashes << 16 << 8;
            for (int i = 0; i < TrainDiagram.vectRailwayLine.size(); i++)
            {
                pen.setStyle(Qt::SolidLine);
                painter.setPen(pen); //ÃÌº”ª≠± 
                painter.drawLine(SPACING_LEFTORRIGHT, TrainDiagram.vectRailwayLine[i].startY, endX, TrainDiagram.vectRailwayLine[i].startY);
                painter.drawLine(SPACING_LEFTORRIGHT, TrainDiagram.vectRailwayLine[i].middleY, endX, TrainDiagram.vectRailwayLine[i].middleY);
                painter.drawLine(SPACING_LEFTORRIGHT, TrainDiagram.vectRailwayLine[i].endY, endX, TrainDiagram.vectRailwayLine[i].endY);

                for (int j = 0; j < 6 * 24 + 1; j++)
                {
                    if (j % 6 == 0)//–° ±
                    {
                        pen.setStyle(Qt::SolidLine);
                        pen.setWidth(2);
                        painter.setPen(pen);
                    }
                    else if (j % 3 == 0) //∞Î–° ±
                    {

                        pen.setDashPattern(dashes);
                        pen.setWidth(1);
                        painter.setPen(pen);
                    }
                    else // Æ∑÷÷”
                    {
                        pen.setStyle(Qt::SolidLine);
                        pen.setWidth(1);
                        painter.setPen(pen);
                    }
                    painter.drawLine(TrainDiagram.miniteDistance * j + SPACING_LEFTORRIGHT, TrainDiagram.vectRailwayLine[i].startY,
                        TrainDiagram.miniteDistance * j + SPACING_LEFTORRIGHT, TrainDiagram.vectRailwayLine[i].endY);
                }
                startY += TrainDiagram.stationDistance * 2 + TrainDiagram.lintDistance;
            }
        }

        void StaTrainDiagramWidgetKSK::drawStationName()
        {
            QPainter painter(ui.stationWidget);
            painter.setRenderHint(QPainter::Antialiasing, false);

            QPen pen; //ª≠± 
            pen.setColor(Qt::green);
            pen.setWidth(2);
            painter.setPen(pen); //ÃÌº”ª≠± 
            QFont font("ø¨ÃÂ", 13, QFont::Bold);
            painter.setFont(font);
            QFontMetrics metrics(font);
            int startY = SPACING_TOPORBOTTOM + SPACING_TIMEAXIS;
            int strWidth = 0;
            for (int i = 0; i < TrainDiagram.vectRailwayLine.size(); i++)
            {
                painter.drawText(16, TrainDiagram.vectRailwayLine[i].startY + 4, TrainDiagram.vectRailwayLine.at(i).startStation);
                painter.drawText(16, TrainDiagram.vectRailwayLine[i].middleY + 4, TrainDiagram.vectRailwayLine.at(i).middleStation);
                painter.drawText(16, TrainDiagram.vectRailwayLine[i].endY + 4, TrainDiagram.vectRailwayLine.at(i).endStation);
                startY += TrainDiagram.stationDistance * 2 + TrainDiagram.lintDistance;

                strWidth = metrics.width(TrainDiagram.vectRailwayLine.at(i).endStation);
                if (strWidth > maxStationWidgetWidth)
                    maxStationWidgetWidth = strWidth;
                strWidth = metrics.width(TrainDiagram.vectRailwayLine.at(i).middleStation);
                if (strWidth > maxStationWidgetWidth)
                    maxStationWidgetWidth = strWidth;
                strWidth = metrics.width(TrainDiagram.vectRailwayLine.at(i).startStation);
                if (strWidth > maxStationWidgetWidth)
                    maxStationWidgetWidth = strWidth;
            }

            if (maxStationWidgetWidth > ui.stationWidget->width())
            {
                ui.stationWidget->setFixedWidth(maxStationWidgetWidth);
            }
        }

        void StaTrainDiagramWidgetKSK::drawNowTimeLine()
        {
            QPainter painter(ui.diagramWidget);
            painter.setRenderHint(QPainter::Antialiasing, false);

            QPen pen; //ª≠± 
            pen.setColor(Qt::blue);
            pen.setWidth(2);
            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(Qt::blue); //ª≠À¢
            painter.setPen(pen); //ÃÌº”ª≠± 
            painter.setBrush(brush); //ÃÌº”ª≠À¢
            int startY = SPACING_TOPORBOTTOM + SPACING_TIMEAXIS;
            int endY = startY + TrainDiagram.vectRailwayLine.size() * (TrainDiagram.stationDistance * 2)
                + (TrainDiagram.vectRailwayLine.size() - 1) * TrainDiagram.lintDistance;
            painter.drawLine(nowTimeLinePx, startY, nowTimeLinePx, endY);
        }

        int StaTrainDiagramWidgetKSK::getDirectionIndex(Station::StaTrafficLog* pTrafficLog)
        {
            Station::Device::StaSignal* pSignal_1 = nullptr;
            Station::Device::StaSignal* pSignal_2 = nullptr;
            for (int i = 0; i < TrainDiagram.vectRailwayLine.size(); i++)//º∆ªÆ¿‡–Õ (Ω”∑¢-0x01, º∑¢-0x02, ÷’µΩ-0x03, Õ®π˝-0x04)
            {
                pSignal_1 = nullptr;
                pSignal_2 = nullptr;
                if (pTrafficLog->m_nPlanType == 2) {
                    pSignal_1 = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strDepartSignal, SIGNALLAMP));
                    if ((TrainDiagram.vectRailwayLine[i].endStation == pSignal_1->getDirection()
                        || TrainDiagram.vectRailwayLine[i].middleStation == pSignal_1->getDirection()))
                    {
                        return i;
                    }
                }
                else if (pTrafficLog->m_nPlanType == 3) {
                    pSignal_1 = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));
                    if ((TrainDiagram.vectRailwayLine[i].middleStation == pSignal_1->getDirection()
                        || TrainDiagram.vectRailwayLine[i].startStation == pSignal_1->getDirection()))
                    {
                        return i;
                    }
                }
                else {
                    Station::Device::StaSignal* pSignal_1 = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));
                    Station::Device::StaSignal* pSignal_2 = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strDepartSignal, SIGNALLAMP));

                    if ((TrainDiagram.vectRailwayLine[i].startStation == pSignal_1->getDirection()
                        && TrainDiagram.vectRailwayLine[i].endStation == pSignal_2->getDirection()))
                    {
                        return i;
                    }
                }

            }
            return 1;
        }

        void StaTrainDiagramWidgetKSK::drawRouteLine(QPainter* painter, Station::RailwayLine RailwayLine, Station::StaTrafficLog* pTrafficLog, Station::StaTrainRoute* pRoute, int& nStartX, int& nEndX, int& nEndY)
        {
            int nOffset = 0;
            int nMiddleX = 0;
            int min = 0;

            if (pRoute->m_bArrivaRoute) {

                if (pTrafficLog->m_tRealArrivalTime.isNull()) {
                    nStartX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time());
                    painter->drawText(nStartX + 10, RailwayLine.middleY - 10, QString("%1").arg(pTrafficLog->m_tProvArrivalTime.time().minute()));
                }
                else {
                    nStartX = getPointXByTime(pTrafficLog->m_tRealArrivalTime.time());
                    painter->drawText(nStartX + 10, RailwayLine.middleY - 10, QString("%1").arg(pTrafficLog->m_tRealArrivalTime.time().minute()));
                }

                if (pTrafficLog->m_tAdjDepartTime.isNull()) {
                    Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));
                    if ((RailwayLine.startStation == pSignal->getDirection())) {
                        nEndY = RailwayLine.startY;
                    }
                    else {
                        nEndY = RailwayLine.endY;
                    }
                    nEndX = nStartX - 15;
                    if (pSignal->getSXThroat()) {
                        nOffset = 15;
                    }
                    else {
                        nOffset = -15;
                    }
                }
                else {
                    nMiddleX = getPointXByTime(pTrafficLog->m_tAdjDepartTime.time());
                    painter->drawText(nMiddleX + 10, RailwayLine.middleY - 10, QString("%1").arg(pTrafficLog->m_tAdjDepartTime.time().minute()));
                }
                nEndY = nEndY + nOffset;
                painter->drawLine(nStartX, RailwayLine.middleY, nEndX, nEndY);
                painter->setPen(Qt::blue);
                painter->drawText(nStartX - 30, RailwayLine.middleY + 16, QString("%1").arg(pTrafficLog->m_strDepartTrack));
                painter->setPen(Qt::red);
            }
            else {
                if (pTrafficLog->m_tRealDepartTime.isNull()) {
                    nStartX = getPointXByTime(pTrafficLog->m_tProvDepartTime.time());
                    painter->drawText(nStartX + 10, RailwayLine.middleY - 10, QString("%1").arg(pTrafficLog->m_tProvDepartTime.time().minute()));
                }
                else {
                    nStartX = getPointXByTime(pTrafficLog->m_tRealDepartTime.time());
                    painter->drawText(nStartX + 10, RailwayLine.middleY - 10, QString("%1").arg(pTrafficLog->m_tRealDepartTime.time().minute()));
                }

                if (pTrafficLog->m_tAdjArrivalTime.isNull()) {
                    Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strDepartSignal, SIGNALLAMP));
                    if ((RailwayLine.startStation == pSignal->getDirection())) {
                        nEndY = RailwayLine.startY;
                    }
                    else {
                        nEndY = RailwayLine.endY;
                    }
                    nEndX = nStartX + 15;
                    if (pSignal->getSXThroat()) {
                        nOffset = 15;
                    }
                    else {
                        nOffset = -15;
                    }

                }
                else {
                    nMiddleX = getPointXByTime(pTrafficLog->m_tAdjArrivalTime.time());
                    painter->drawText(nMiddleX + 10, RailwayLine.middleY - 10, QString("%1").arg(pTrafficLog->m_tAdjArrivalTime.time().minute()));
                }
                nEndY = nEndY + nOffset;
                painter->drawLine(nStartX, RailwayLine.middleY, nEndX, nEndY);
                painter->setPen(Qt::blue);
                painter->drawText(nStartX - 30, RailwayLine.middleY + 16, QString("%1").arg(pTrafficLog->m_strDepartTrack));
                painter->setPen(Qt::red);
            }
        }

        void StaTrainDiagramWidgetKSK::drawTrainPlan()
        {
            QPainter painter(ui.diagramWidget);
            painter.setRenderHint(QPainter::Antialiasing, false);
            Station::StaTrain* m_pStaTrain = nullptr;
            painter.setBrush(Qt::NoBrush); //ÃÌº”ª≠À¢

            Station::StaTrainRoute* m_pArrivalRoute;
            Station::StaTrainRoute* m_pDepartRoute;

            int nStartX1 = 0;
            int nStartX2 = 0;
            int nEndX = 0;
            int nEndY = 0;
            int nIndex = 0;

            QFont font;
            QPen pen; //ª≠± 
            pen.setWidth(2);
            pen.setColor(Qt::red);
            font = QFont("Œ¢»Ì—≈∫⁄", 11);
            painter.setFont(font);
            painter.setPen(pen); //ÃÌº”ª≠± 
            for (Station::StaTrafficLog* pTrafficLog : Station::MainStation()->getvecTrafficLog()) {
                m_pArrivalRoute = Station::MainStation()->getStaTrainRouteById(pTrafficLog->m_nArrivalRouteId);
                m_pDepartRoute = Station::MainStation()->getStaTrainRouteById(pTrafficLog->m_nDepartRouteId);

                nStartX1 = 0;
                nStartX2 = 0;
                nEndX = 0;
                nEndY = 0;

                nIndex = getDirectionIndex(pTrafficLog);
                m_pStaTrain = Station::MainStation()->getStaTrainById(pTrafficLog->m_nTrainId);


                if (pTrafficLog->m_nPlanType == 0x02) {
                    Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strDepartSignal, SIGNALLAMP));
                    drawRouteLine(&painter, TrainDiagram.vectRailwayLine[nIndex], pTrafficLog, m_pDepartRoute, nStartX1, nEndX, nEndY);
                    drawTrainNum(&painter, nStartX1, TrainDiagram.vectRailwayLine[nIndex].middleY, m_pStaTrain->m_strTrainNum);
                    drawEndFlag(&painter, !pSignal->getSXThroat(), nEndX, nEndY);
                }
                else if (pTrafficLog->m_nPlanType == 0x03) {
                    Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));
                    drawRouteLine(&painter, TrainDiagram.vectRailwayLine[nIndex], pTrafficLog, m_pArrivalRoute, nStartX1, nEndX, nEndY);
                    drawTrainNum(&painter, nEndX, nEndY, m_pStaTrain->m_strTrainNum);
                    drawEndFlag(&painter, pSignal->getSXThroat(), nStartX1, TrainDiagram.vectRailwayLine[nIndex].middleY);
                }
                else {
                    Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));
                    drawRouteLine(&painter, TrainDiagram.vectRailwayLine[nIndex], pTrafficLog, m_pArrivalRoute, nStartX1, nEndX, nEndY);
                    drawTrainNum(&painter, nEndX, nEndY, m_pStaTrain->m_strTrainNum);
                    drawRouteLine(&painter, TrainDiagram.vectRailwayLine[nIndex], pTrafficLog, m_pDepartRoute, nStartX2, nEndX, nEndY);
                    drawEndFlag(&painter, pSignal->getSXThroat(), nEndX, nEndY);
                    painter.drawLine(nStartX1, TrainDiagram.vectRailwayLine[nIndex].middleY, nStartX2, TrainDiagram.vectRailwayLine[nIndex].middleY);
                }
            }
        }



        bool StaTrainDiagramWidgetKSK::eventFilter(QObject* obj, QEvent* event)
        {
            if (obj == ui.diagramWidget)
            {
                if (event->type() == QEvent::Paint)
                {
                    drawTimeAxis(true);
                    drawGrid();
                    drawTimeAxis(false);
                    drawNowTimeLine();
                    drawTrainPlan();
                }
                if (event->type() == QEvent::Show || oldTimeLinePx != nowTimeLinePx)
                {
                    oldTimeLinePx = nowTimeLinePx;
                    ui.scrollArea_3->horizontalScrollBar()->setValue(oldTimeLinePx - 800);
                }
            }
            if (obj == ui.stationWidget)
            {
                if (event->type() == QEvent::Paint)
                {
                    drawStationName();
                }
            }

            return StaTrainDiagramWidget::eventFilter(obj, event);
        }

        void StaTrainDiagramWidgetKSK::timerEvent(QTimerEvent* event)
        {

            if (event->timerId() == Timer_ID_60000)
            {
                QTime nowTimeT = QDateTime::currentDateTime().time();
                if (nowTimeT.minute() != nowTime.minute())
                {
                    nowTimeLinePx = getPointXByTime(QDateTime::currentDateTime().time());
                    ui.diagramWidget->update();
                }
            }
        }

        void StaTrainDiagramWidgetKSK::drawTrainNum(QPainter* painter, int X, int Y, QString trainNum)
        {

            QFontMetrics metrics(painter->font());
            painter->drawLine(X - metrics.width(trainNum), Y, X, Y);
            painter->drawText(X - metrics.width(trainNum), Y - 4, trainNum);

        }

        int StaTrainDiagramWidgetKSK::getPointXByTime(QTime time)
        {
            int hour = time.hour();
            if (hour < 3)
                hour += 24;
            hour -= 3;

            int minite = time.minute() / 10;
            int min = time.minute() % 10;
            int X = SPACING_LEFTORRIGHT + (hour * 6 + minite) * TrainDiagram.miniteDistance + min * (TrainDiagram.miniteDistance / 9);
            return X;
        }

        void StaTrainDiagramWidgetKSK::drawEndFlag(QPainter* painter, bool UPDown, int X, int Y)
        {

            if (UPDown) {
                painter->setBrush(Qt::NoBrush);
                int lineEndY = Y + 15;
                painter->drawLine(X, Y, X, lineEndY);
                int triangleWidth = 8;

                int triangleHeight = 5;

                painter->drawLine(X - triangleWidth / 2, lineEndY, X, lineEndY + triangleHeight);
                painter->drawLine(X, lineEndY + triangleHeight, X + triangleWidth / 2, lineEndY);
                painter->drawLine(X + triangleWidth / 2, lineEndY, X - triangleWidth / 2, lineEndY);
            }
            else {
                painter->setBrush(Qt::NoBrush);
                int lineStartY = Y - 15;
                painter->drawLine(X, Y, X, lineStartY);

                int triangleWidth = 8;
                int triangleHeight = 5;
                painter->drawLine(X - triangleWidth / 2, lineStartY, X, lineStartY - triangleHeight);
                painter->drawLine(X, lineStartY - triangleHeight, X + triangleWidth / 2, lineStartY);
                painter->drawLine(X + triangleWidth / 2, lineStartY, X - triangleWidth / 2, lineStartY);
            }


        }
    }
}
