#include "StaTraindiagramwidgetKSK.h"
#include <QScrollBar>

StaTraindiagramwidgetKSK::StaTraindiagramwidgetKSK(QWidget *parent)
	: CTCWindows::BaseWnd::StaTraindiagramwidget(parent)
{
	ui.setupUi(this);
    ui.diagramWidget->installEventFilter(this);
    ui.stationWidget->installEventFilter(this);
    TrainDiagram = Station::MainStation()->getTrainDiagram();
    connect(CTCWindows::MainWindow(), &CTCWindows::CTCMainWindow::DrawLine, [=] {
        InitTraindiagramw();
        });
}

StaTraindiagramwidgetKSK::~StaTraindiagramwidgetKSK()
{
 
}

void StaTraindiagramwidgetKSK::InitTraindiagramw()
{
    ui.diagramWidget->setFixedSize(6 * 24 * TrainDiagram->miniteDistance + (SPACING_LEFTORRIGHT * 2),
        SPACING_TOPORBOTTOM * 2
        + TrainDiagram->vectRailwayLine.size() * (TrainDiagram->stationDistance * 2)
        + (TrainDiagram->vectRailwayLine.size() - 1) * TrainDiagram->lintDistance
        + SPACING_TIMEAXIS * 2);
    ui.stationWidget->setFixedHeight(ui.diagramWidget->height());
    ui.scrollArea_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui.scrollArea_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui.scrollArea_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    Timer_ID_60000 = startTimer(100);
    nowTime = QDateTime::currentDateTime().time();
    nowTimeLinePx = getPointXByTime(nowTime);

    int startY = SPACING_TOPORBOTTOM + SPACING_TIMEAXIS;
    for (int i = 0; i < TrainDiagram->vectRailwayLine.size(); i++) {
        TrainDiagram->vectRailwayLine[i].startY = startY;
        TrainDiagram->vectRailwayLine[i].middleY = startY + TrainDiagram->stationDistance;
        TrainDiagram->vectRailwayLine[i].endY = startY + TrainDiagram->stationDistance * 2;
        startY += TrainDiagram->stationDistance * 2 + TrainDiagram->lintDistance;
    }
}
void StaTraindiagramwidgetKSK::drawTimeAxis(bool isTop)
{
    QPainter painter(ui.diagramWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);

    QPen pen; //画笔
    pen.setColor(Qt::green);
    pen.setWidth(2);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green); //画刷
    painter.setPen(pen); //添加画笔
    painter.setBrush(brush); //添加画刷

    int startY = 0;
    if (isTop)
        startY = SPACING_TOPORBOTTOM;
    else startY = SPACING_TOPORBOTTOM
        + TrainDiagram->vectRailwayLine.size() * (TrainDiagram->stationDistance * 2)
        + (TrainDiagram->vectRailwayLine.size() - 1) * TrainDiagram->lintDistance
        + SPACING_TIMEAXIS * 2;


    painter.drawLine(SPACING_LEFTORRIGHT, startY, 6 * 24 * TrainDiagram->miniteDistance + SPACING_LEFTORRIGHT, startY);
    int hours = 3;

    for (int i = 0; i < 6 * 24 + 1; i++)
    {
        if (hours >= 24)
            hours = 0;
        if (i % 6 == 0)//小时
        {
            QPen pen; //画笔
            pen.setColor(Qt::green);
            pen.setWidth(2);
            QFont font("微软雅黑", 11, QFont::Bold);
            painter.setFont(font);
            painter.setPen(pen);

            if (isTop)
            {
                painter.drawLine(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                    TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY - 30);

                painter.drawText(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT - 20, startY - 35,
                    QString("%1:00").arg(hours, 2, 10, QLatin1Char('0')));
            }
            else
            {
                painter.drawLine(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                    TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY + 30);

                painter.drawText(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT - 20, startY + 48,
                    QString("%1:00").arg(hours, 2, 10, QLatin1Char('0')));
            }


            hours++;
        }
        else if (i % 3 == 0) //半小时
        {
            QPen pen; //画笔
            pen.setColor(Qt::green);
            pen.setWidth(1);
            painter.setPen(pen);
            QFont font("微软雅黑", 10, QFont::Bold);
            painter.setFont(font);
            if (isTop)
            {
                painter.drawLine(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                    TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY - 25);
                painter.drawText(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT - 8, startY - 30, "30");
            }
            else
            {
                painter.drawLine(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                    TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY + 25);
                painter.drawText(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT - 8, startY + 42, "30");
            }

        }
        else //十分钟
        {
            QPen pen; //画笔
            pen.setColor(Qt::green);
            QVector<qreal>dashes;
            dashes << 3 << 4;
            pen.setDashPattern(dashes);
            pen.setWidth(1);
            painter.setPen(pen);
            if (isTop)
            {
                painter.drawLine(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                    TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY - 20);
            }
            else
            {
                painter.drawLine(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                    TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY + 20);
            }
        }
    }
}

void StaTraindiagramwidgetKSK::drawGrid()
{
    int startY = SPACING_TOPORBOTTOM + SPACING_TIMEAXIS;
    int endX = 6 * 24 * TrainDiagram->miniteDistance + SPACING_LEFTORRIGHT;
    QPainter painter(ui.diagramWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);
    QPen pen; //画笔
    pen.setColor(Qt::green);
    pen.setWidth(2);
    QVector<qreal>dashes;
    dashes << 16 << 8;
    for (int i = 0; i < TrainDiagram->vectRailwayLine.size(); i++)
    {
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen); //添加画笔
        painter.drawLine(SPACING_LEFTORRIGHT, TrainDiagram->vectRailwayLine[i].startY, endX, TrainDiagram->vectRailwayLine[i].startY);
        painter.drawLine(SPACING_LEFTORRIGHT, TrainDiagram->vectRailwayLine[i].middleY, endX, TrainDiagram->vectRailwayLine[i].middleY);
        painter.drawLine(SPACING_LEFTORRIGHT, TrainDiagram->vectRailwayLine[i].endY, endX, TrainDiagram->vectRailwayLine[i].endY);

        for (int j = 0; j < 6 * 24 + 1; j++)
        {
            if (j % 6 == 0)//小时
            {
                pen.setStyle(Qt::SolidLine);
                pen.setWidth(2);
                painter.setPen(pen);
            }
            else if (j % 3 == 0) //半小时
            {
                
                pen.setDashPattern(dashes);
                pen.setWidth(1);
                painter.setPen(pen);
            }
            else //十分钟
            {
                pen.setStyle(Qt::SolidLine);
                pen.setWidth(1);
                painter.setPen(pen);
            }
           painter.drawLine(TrainDiagram->miniteDistance * j + SPACING_LEFTORRIGHT, TrainDiagram->vectRailwayLine[i].startY,
               TrainDiagram->miniteDistance * j + SPACING_LEFTORRIGHT, TrainDiagram->vectRailwayLine[i].endY);
        }
        startY += TrainDiagram->stationDistance * 2 + TrainDiagram->lintDistance;
    }
}

void StaTraindiagramwidgetKSK::drawStationName()
{
    QPainter painter(ui.stationWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);

    QPen pen; //画笔
    pen.setColor(Qt::green);
    pen.setWidth(2);
    painter.setPen(pen); //添加画笔
    QFont font("楷体", 13, QFont::Bold);
    painter.setFont(font);
    QFontMetrics metrics(font);
    int startY = SPACING_TOPORBOTTOM + SPACING_TIMEAXIS;
    int strWidth = 0;
    for (int i = 0; i < TrainDiagram->vectRailwayLine.size(); i++)
    {
        painter.drawText(16, TrainDiagram->vectRailwayLine[i].startY + 4, TrainDiagram->vectRailwayLine.at(i).startStation);
        painter.drawText(16, TrainDiagram->vectRailwayLine[i].middleY + 4, TrainDiagram->vectRailwayLine.at(i).middleStation);
        painter.drawText(16, TrainDiagram->vectRailwayLine[i].endY + 4, TrainDiagram->vectRailwayLine.at(i).endStation);
        startY += TrainDiagram->stationDistance * 2 + TrainDiagram->lintDistance;

        strWidth = metrics.width(TrainDiagram->vectRailwayLine.at(i).endStation);
        if (strWidth > maxStationWidgetWidth)
            maxStationWidgetWidth = strWidth;
        strWidth = metrics.width(TrainDiagram->vectRailwayLine.at(i).middleStation);
        if (strWidth > maxStationWidgetWidth)
            maxStationWidgetWidth = strWidth;
        strWidth = metrics.width(TrainDiagram->vectRailwayLine.at(i).startStation);
        if (strWidth > maxStationWidgetWidth)
            maxStationWidgetWidth = strWidth;
    }

    if (maxStationWidgetWidth > ui.stationWidget->width())
    {
        ui.stationWidget->setFixedWidth(maxStationWidgetWidth);
    }
}

void StaTraindiagramwidgetKSK::drawNowTimeLine()
{
    QPainter painter(ui.diagramWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);

    QPen pen; //画笔
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::blue); //画刷
    painter.setPen(pen); //添加画笔
    painter.setBrush(brush); //添加画刷
    int startY = SPACING_TOPORBOTTOM + SPACING_TIMEAXIS;
    int endY = startY + TrainDiagram->vectRailwayLine.size() * (TrainDiagram->stationDistance * 2)
        + (TrainDiagram->vectRailwayLine.size() - 1) * TrainDiagram->lintDistance;
    qDebug() << "NowTimeLine" << nowTimeLinePx << startY << nowTimeLinePx << endY;
    painter.drawLine(nowTimeLinePx, startY, nowTimeLinePx, endY);
}

int StaTraindiagramwidgetKSK::getDirectionIndex(Station::StaTrafficLog* pTrafficLog)
{
    Station::Device::StaSignal* pSignal_1 = nullptr;
    Station::Device::StaSignal* pSignal_2 = nullptr;
    for (int i = 0; i < TrainDiagram->vectRailwayLine.size(); i++)//计划类型 (接发-0x01,始发-0x02, 终到-0x03, 通过-0x04)
    {
        pSignal_1 = nullptr;
        pSignal_2 = nullptr;
        if (pTrafficLog->m_nPlanType == 2) {
            pSignal_1 = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strDepartSignal, SIGNALLAMP));
            if ((TrainDiagram->vectRailwayLine[i].endStation == pSignal_1->getDirection()
                || TrainDiagram->vectRailwayLine[i].middleStation == pSignal_1->getDirection()))
            {
                return i;
            }
        }
        else if (pTrafficLog->m_nPlanType == 3) {
            pSignal_1 = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));
            if ((TrainDiagram->vectRailwayLine[i].middleStation == pSignal_1->getDirection()
                || TrainDiagram->vectRailwayLine[i].startStation == pSignal_1->getDirection()))
            {
                return i;
            }
        }
        else {
            Station::Device::StaSignal* pSignal_1 = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));
            Station::Device::StaSignal* pSignal_2 = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strDepartSignal, SIGNALLAMP));

            if ((TrainDiagram->vectRailwayLine[i].startStation == pSignal_1->getDirection()
                && TrainDiagram->vectRailwayLine[i].endStation == pSignal_2->getDirection()))
            {
                return i;
            }
        }

    }
    return 1;
}


void StaTraindiagramwidgetKSK::drawRouteLine(QPainter* painter, Station::RailwayLine* TrainDiagram, Station::StaTrafficLog* pTrafficLog, Station::StaTrainRoute* pRoute, int& nStartX, int& nEndX, int& nEndY)
{
    int nOffset = 0;
    int nMiddleX = 0;
    int min = 0;

    if (pRoute->m_bArrivaRoute) {

        if (pTrafficLog->m_tRealArrivalTime.isNull()) {
            nStartX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time());
            painter->drawText(nStartX + 10, TrainDiagram->middleY - 10, QString("%1").arg(pTrafficLog->m_tProvArrivalTime.time().minute()));
        }
        else {
            nStartX = getPointXByTime(pTrafficLog->m_tRealArrivalTime.time());
            painter->drawText(nStartX + 10, TrainDiagram->middleY - 10, QString("%1").arg(pTrafficLog->m_tRealArrivalTime.time().minute()));
        }

        if (pTrafficLog->m_tAdjDepartTime.isNull()) {
            Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));
            if ((TrainDiagram->startStation == pSignal->getDirection())) {
                nEndY = TrainDiagram->startY;
            }
            else {
                nEndY = TrainDiagram->endY;
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
            painter->drawText(nMiddleX + 10, TrainDiagram->middleY - 10, QString("%1").arg(pTrafficLog->m_tAdjDepartTime.time().minute()));
        }
        nEndY = nEndY + nOffset;
        painter->drawLine(nStartX, TrainDiagram->middleY, nEndX, nEndY);
        painter->setPen(Qt::blue);
        painter->drawText(nStartX - 30, TrainDiagram->middleY + 16, QString("%1").arg(pTrafficLog->m_strDepartTrack));
        painter->setPen(Qt::red);
    }
    else {
        if (pTrafficLog->m_tRealDepartTime.isNull()) {
            nStartX = getPointXByTime(pTrafficLog->m_tProvDepartTime.time());
            painter->drawText(nStartX + 10, TrainDiagram->middleY - 10, QString("%1").arg(pTrafficLog->m_tProvDepartTime.time().minute()));
        }
        else {
            nStartX = getPointXByTime(pTrafficLog->m_tRealDepartTime.time());
            painter->drawText(nStartX + 10, TrainDiagram->middleY - 10, QString("%1").arg(pTrafficLog->m_tRealDepartTime.time().minute()));
        }

        if (pTrafficLog->m_tAdjArrivalTime.isNull()) {
            Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strDepartSignal, SIGNALLAMP));
            if ((TrainDiagram->startStation == pSignal->getDirection())) {
                nEndY = TrainDiagram->startY;
            }
            else {
                nEndY = TrainDiagram->endY;
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
            painter->drawText(nMiddleX + 10, TrainDiagram->middleY - 10, QString("%1").arg(pTrafficLog->m_tAdjArrivalTime.time().minute()));
        }
        nEndY = nEndY + nOffset;
        painter->drawLine(nStartX, TrainDiagram->middleY, nEndX, nEndY);
        painter->setPen(Qt::blue);
        painter->drawText(nStartX - 30, TrainDiagram->middleY + 16, QString("%1").arg(pTrafficLog->m_strDepartTrack));
        painter->setPen(Qt::red);
    }
}


void StaTraindiagramwidgetKSK::drawTrainPlan()
{
    QPainter painter(ui.diagramWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);
    Station::StaTrain* m_pStaTrain = nullptr;
    QBrush brush;
    brush.setStyle(Qt::NoBrush);

    painter.setBrush(brush); //添加画刷

    Station::StaTrainRoute* m_pArrivalRoute;
    Station::StaTrainRoute* m_pDepartRoute;
   // Station::StaTrafficLog* m_pstatraff = new Station::StaTrafficLog();
   // m_pstatraff->m_nTrainId = 147;
   // m_pstatraff->m_nPlanType = i;//计划类型 (接发-0x01,始发-0x02, 终到-0x03, 通过-0x04)
   // 
   // m_pstatraff->m_strArrivalTrainNum = "G1447";
   // m_pstatraff->m_nArrivalTrackCode = 2;
   // m_pstatraff->m_strArrivalTrack = "23G";
   // m_pstatraff->m_nArrivalSignalCode = 2;
   // m_pstatraff->m_strArrivaSignal = "XHD2";
   // QDateTime dt1(
   //     QDate(2025, 4, 17),    // 年, 月, 日
   //     QTime(13, 15, 0)       // 时, 分, 秒
   // );
   // QDateTime dt2(
   //     QDate(2025, 4, 17),    // 年, 月, 日
   //     QTime(13, 20, 0)       // 时, 分, 秒
   // );
   // QDateTime dt3(
   //     QDate(2025, 4, 17),    // 年, 月, 日
   //     QTime(13, 10, 0)       // 时, 分, 秒
   // );
   // QDateTime dt4(
   //     QDate(2025, 4, 17),    // 年, 月, 日
   //     QTime(13, 00, 0)       // 时, 分, 秒
   // );
   // m_pstatraff->m_tProvArrivalTime = dt3;//计划到站时间
   // m_pstatraff->m_tAdjDepartTime = dt4;//邻站发车时间
   // m_pstatraff->m_strDepartTrainNum = "33G";
   // m_pstatraff->m_nDepartTrackCode = 4;
   // m_pstatraff->m_strDepartTrack = "33G";
   // m_pstatraff->m_nDepartSignalCode = 3;
   // m_pstatraff->m_strDepartSignal = "XHD22";
   // m_pstatraff->m_tProvDepartTime = dt1;//计划发车
   // m_pstatraff->m_tAdjArrivalTime = dt2;//邻站到达时间
   // QVector<Station::StaTrafficLog*> mptraa = Station::MainStation()->getvecTrafficLog();
   // mptraa.append(m_pstatraff);
    //for (int i = 0; i < mptraa.size(); i++) {
    for (Station::StaTrafficLog* pTrafficLog : Station::MainStation()->getvecTrafficLog()) {
        m_pArrivalRoute = Station::MainStation()->getStaTrainRouteById(pTrafficLog->m_nArrivalRouteId);
        m_pDepartRoute = Station::MainStation()->getStaTrainRouteById(pTrafficLog->m_nDepartRouteId);
       //Station::StaTrainRoute* m_Route = new Station::StaTrainRoute;
       //m_Route->m_strRouteDescrip = begin;
       //Station::StaTrainRoute* m_Route2 = new Station::StaTrainRoute;
       //m_Route2->m_strRouteDescrip = end;
       //Station::StaTrafficLog* pTrafficLog = mptraa.at(i);
        QString start;
        QString end;
        int min = 0;
        int startX = 0;
        int startY = 0;
        int middleX = 0;
        int middleY = 0;
        int stopX = 0;
        int stopY = 0;
        int endX = 0;
        int endY = 0;
        int offsetX = 0;
        int offset1 = 0;
        int offset2 = 0;
        int nStartX1 = 0;
        int nStartX2 = 0;
        int nEndX = 0;
        int nEndY = 0;
        QFont font;
        QPen pen; //画笔
        pen.setWidth(2);
        pen.setColor(Qt::red);
        font = QFont("微软雅黑", 11);
        painter.setFont(font);
        painter.setPen(pen); //添加画笔

        int nIndex = getDirectionIndex(pTrafficLog);
        TrainDiagram->vectRailwayLine[nIndex];
        m_pStaTrain = Station::MainStation()->getStaTrainById(pTrafficLog->m_nTrainId);

        
        if (pTrafficLog->m_nPlanType == 0x02) {
            Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strDepartSignal, SIGNALLAMP));
            drawRouteLine(&painter, &TrainDiagram->vectRailwayLine[nIndex], pTrafficLog, m_pDepartRoute, nStartX1, nEndX, nEndY);
   

            drawTrainNum(&painter, nStartX1, TrainDiagram->vectRailwayLine[nIndex].middleY, m_pStaTrain->m_strTrainNum);

            drawEndFlag(&painter, !pSignal->getSXThroat(), nEndX, nEndY);
        }
        else if (pTrafficLog->m_nPlanType == 0x03) {
            Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));

            drawRouteLine(&painter, &TrainDiagram->vectRailwayLine[nIndex], pTrafficLog, m_pArrivalRoute, nStartX1, nEndX, nEndY);
            
            drawTrainNum(&painter, nEndX, nEndY, m_pStaTrain->m_strTrainNum);
            drawEndFlag(&painter, pSignal->getSXThroat(), nStartX1, TrainDiagram->vectRailwayLine[nIndex].middleY);
        }
        else {
            Station::Device::StaSignal* pSignal = dynamic_cast<Station::Device::StaSignal*>(Station::MainStation()->getDeviceByName(pTrafficLog->m_strArrivaSignal, SIGNALLAMP));

            drawRouteLine(&painter, &TrainDiagram->vectRailwayLine[nIndex], pTrafficLog, m_pArrivalRoute, nStartX1, nEndX, nEndY);
            drawTrainNum(&painter,nEndX,nEndY, m_pStaTrain->m_strTrainNum);
            drawRouteLine(&painter, &TrainDiagram->vectRailwayLine[nIndex], pTrafficLog, m_pDepartRoute, nStartX2, nEndX, nEndY);
            drawEndFlag(&painter, pSignal->getSXThroat(), nEndX, nEndY);
            painter.drawLine(nStartX1, TrainDiagram->vectRailwayLine[nIndex].middleY, nStartX2, TrainDiagram->vectRailwayLine[nIndex].middleY);
        }
    }


}

void StaTraindiagramwidgetKSK::setPenInfoByPoint(QPainter* painter)
{
    QFont font;
    QPen pen; //画笔
    pen.setWidth(2);
    pen.setColor(Qt::red);
    font = QFont("微软雅黑", 11);
    painter->setFont(font);
    painter->setPen(pen); //添加画笔
}


bool StaTraindiagramwidgetKSK::eventFilter(QObject* obj, QEvent* event)
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
        if (event->type() == QEvent::Show|| oldTimeLinePx != nowTimeLinePx)
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

    return StaTraindiagramwidget::eventFilter(obj, event);
}

void StaTraindiagramwidgetKSK::timerEvent(QTimerEvent* event)
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



void StaTraindiagramwidgetKSK::drawTrainNum(QPainter* painter, int X, int Y, QString trainNum)
{

    QFontMetrics metrics(painter->font());
    painter->drawLine(X- metrics.width(trainNum), Y, X , Y);
    painter->drawText(X - metrics.width(trainNum), Y - 4, trainNum);

}

int StaTraindiagramwidgetKSK::getPointXByTime(QTime time)
{
    int hour = time.hour();
    if (hour < 3)
        hour += 24;
    hour -= 3;

    int minite = time.minute() / 10;
    int min = time.minute() % 10;
    int X = SPACING_LEFTORRIGHT + (hour * 6 + minite) * TrainDiagram->miniteDistance + min * (TrainDiagram->miniteDistance / 9);
    return X;
}

void StaTraindiagramwidgetKSK::drawEndFlag(QPainter* painter, bool UPDown, int X, int Y)
{
    QPen pen; 
    pen.setWidth(2);
    pen.setColor(Qt::red);
    painter->setPen(pen);

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