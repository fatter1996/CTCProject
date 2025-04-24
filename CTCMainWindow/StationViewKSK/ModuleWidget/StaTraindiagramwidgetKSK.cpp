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
    qDebug() << ui.diagramWidget->width() << ui.diagramWidget->height();
    ui.scrollArea_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui.scrollArea_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui.scrollArea_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    Timer_ID_60000 = startTimer(100);
    nowTime = QDateTime::currentDateTime().time();
    nowTimeLinePx = getPointXByTime(nowTime);
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

    for (int i = 0; i < TrainDiagram->vectRailwayLine.size(); i++)
    {
        QPen pen; //画笔
        pen.setColor(Qt::green);
        pen.setWidth(2);
        QBrush brush;
        brush.setStyle(Qt::NoBrush);
        painter.setPen(pen); //添加画笔
        painter.setBrush(brush); //添加画刷
        painter.drawRect(QRect({ SPACING_LEFTORRIGHT, startY, endX - SPACING_LEFTORRIGHT, TrainDiagram->stationDistance * 2 }));
        painter.drawLine(SPACING_LEFTORRIGHT, startY + TrainDiagram->stationDistance, endX, startY + TrainDiagram->stationDistance);

        for (int i = 0; i < 6 * 24 + 1; i++)
        {
            if (i % 6 == 0)//小时
            {
                QPen pen; //画笔
                pen.setColor(Qt::green);
                pen.setWidth(2);
                painter.setPen(pen);
            }
            else if (i % 3 == 0) //半小时
            {
                QPen pen; //画笔
                pen.setColor(Qt::green);
                QVector<qreal>dashes;
                dashes << 16 << 8;
                pen.setDashPattern(dashes);
                pen.setWidth(1);
                painter.setPen(pen);
            }
            else //十分钟
            {
                QPen pen; //画笔
                pen.setColor(Qt::green);
                pen.setWidth(1);
                painter.setPen(pen);
            }
            painter.drawLine(TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY,
                TrainDiagram->miniteDistance * i + SPACING_LEFTORRIGHT, startY + TrainDiagram->stationDistance * 2);
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
        painter.drawText(16, startY + 4, TrainDiagram->vectRailwayLine.at(i).startStation);
        painter.drawText(16, startY + TrainDiagram->stationDistance + 4, TrainDiagram->vectRailwayLine.at(i).middleStation);
        painter.drawText(16, startY + TrainDiagram->stationDistance * 2 + 4, TrainDiagram->vectRailwayLine.at(i).endStation);
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
    for (int i = 0; i < TrainDiagram->vectRailwayLine.size(); i++)
    {
        if ((TrainDiagram->vectRailwayLine[i].startStation == pTrafficLog->m_strArrivaSignal
            || TrainDiagram->vectRailwayLine[i].endStation == pTrafficLog->m_strArrivaSignal)
            && (TrainDiagram->vectRailwayLine[i].endStation == pTrafficLog->m_strDepartSignal
                || TrainDiagram->vectRailwayLine[i].startStation == pTrafficLog->m_strDepartSignal))
        {
            return i + 1;
        }
    }
    return 1;
}

void StaTraindiagramwidgetKSK::drawTrainPlan()
{
    QPainter painter(ui.diagramWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);
    Station::StaTrain* m_pStaTrain = nullptr;
    QBrush brush;
    brush.setStyle(Qt::NoBrush);

    painter.setBrush(brush); //添加画刷
    Station::StaTrafficLog* m_pstatraff = new Station::StaTrafficLog();
    //m_pstatraff->m_nTrainId = 147;
    //m_pstatraff->m_nPlanType = x;//计划类型 (接发-0x01,始发-0x02, 终到-0x03, 通过-0x04)
    //
    //m_pstatraff->m_strArrivalTrainNum = "G1447";
    //m_pstatraff->m_nArrivalTrackCode = 2;
    //m_pstatraff->m_strArrivalTrack = "23G";
    //m_pstatraff->m_nArrivalSignalCode = 2;
    //m_pstatraff->m_strArrivaSignal = "XHD2";
    //QDateTime dt1(
    //    QDate(2025, 4, 17),    // 年, 月, 日
    //    QTime(hour, 15, 0)       // 时, 分, 秒
    //);
    //QDateTime dt2(
    //    QDate(2025, 4, 17),    // 年, 月, 日
    //    QTime(hour, 20, 0)       // 时, 分, 秒
    //);
    //QDateTime dt3(
    //    QDate(2025, 4, 17),    // 年, 月, 日
    //    QTime(hour, 10, 0)       // 时, 分, 秒
    //);
    //QDateTime dt4(
    //    QDate(2025, 4, 17),    // 年, 月, 日
    //    QTime(hour, 00, 0)       // 时, 分, 秒
    //);
    //m_pstatraff->m_tProvArrivalTime = dt3;//计划到站时间
    //m_pstatraff->m_tAdjDepartTime = dt4;//邻站发车时间
    //m_pstatraff->m_strDepartTrainNum = "33G";
    //m_pstatraff->m_nDepartTrackCode = 4;
    //m_pstatraff->m_strDepartTrack = "33G";
    //m_pstatraff->m_nDepartSignalCode = 3;
    //m_pstatraff->m_strDepartSignal = "XHD22";
    //m_pstatraff->m_tProvDepartTime = dt1;//计划发车
    //m_pstatraff->m_tAdjArrivalTime = dt2;//邻站到达时间
    //QVector<Station::StaTrafficLog*> mptraa = Station::MainStation()->getvecTrafficLog();
    //mptraa.append(m_pstatraff);
    //for (int i = 0; i < mptraa.size(); i++) {
    for (int i = 0; i < Station::MainStation()->getvecTrafficLog().size(); i++) {
        Station::StaTrafficLog* pTrafficLog = Station::MainStation()->getvecTrafficLog().at(i);
     //   Station::StaTrafficLog* pTrafficLog = mptraa.at(i);
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

        //m_pStaTrain = new Station::StaTrain();
        //m_pStaTrain->m_nTrainId = 123;
        //m_pStaTrain->m_strTrainNum = "123";
        //m_pStaTrain->m_bRight = true;
        //m_pStaTrain->m_bFreightTrain = 0;
        //pTrafficLog->m_bUpDown = true;
        startY = SPACING_TOPORBOTTOM + SPACING_TIMEAXIS;
        if (pTrafficLog->m_nPlanType == 0x02) {
            if (pTrafficLog->m_bUpDown)
            {
                offset1 = TrainDiagram->stationDistance;
                startY = startY + offset1;
                offset2 = TrainDiagram->stationDistance / 2;
            }
            else
            {
                offset1 = -1 * (TrainDiagram->stationDistance);
                startY = startY + TrainDiagram->stationDistance;
                offset2 = -1 * TrainDiagram->stationDistance / 2;
            }
            if (pTrafficLog->m_tRealDepartTime.isNull())
            {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tProvDepartTime.time().toString("mm").toInt();
                startX = getPointXByTime(pTrafficLog->m_tProvDepartTime.time()) - 15;
            }
            else
            {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tRealDepartTime.time().toString("mm").toInt();
                startX = getPointXByTime(pTrafficLog->m_tRealDepartTime.time());
            }

            painter.drawLine(startX - 10, startY - 15, startX, startY);
            drawTrainNum(&painter, startX - 10, startY - 15, m_pStaTrain->m_strTrainNum, pTrafficLog->m_bUpDown);
            painter.drawText(startX + 10, startY - 10, QString("%1").arg(min));


            if (pTrafficLog->m_tAdjArrivalTime.isNull()) {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                middleX = startX + 15;
                middleY = startY + offset2;
                painter.drawLine(startX, startY, middleX, middleY);
            }
            else {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tAdjArrivalTime.time().minute();
                middleX = getPointXByTime(pTrafficLog->m_tAdjArrivalTime.time());
                middleY = startY + offset1;
                painter.drawLine(startX, startY, middleX, middleY);

            }
            painter.drawText(middleX + 10, middleY - 10, QString("%1").arg(min));

            painter.setPen(Qt::blue);
            painter.drawText(startX - 30, startY + 16, QString("%1").arg(pTrafficLog->m_strDepartTrack));

            drawEndFlag(&painter, middleX, middleY, pTrafficLog->m_bUpDown);

        }
        else if (pTrafficLog->m_nPlanType == 0x03)
        {
            if (pTrafficLog->m_bUpDown)
            {
                offset1 = TrainDiagram->stationDistance;
                offset2 = TrainDiagram->stationDistance / 2;
            }
            else
            {
                offset1 = -1 * (TrainDiagram->stationDistance);
                startY = startY + TrainDiagram->stationDistance * 2;
                offset2 = -1 * TrainDiagram->stationDistance / 2;
            }
            if (pTrafficLog->m_tAdjDepartTime.isNull())
            {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                startX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time()) - 15;
                startY = startY + offset1 / 2;
            }
            else
            {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tAdjDepartTime.time().toString("mm").toInt();
                startX = getPointXByTime(pTrafficLog->m_tAdjDepartTime.time());
            }
            painter.drawLine(startX - 10, startY - 15, startX, startY);
            drawTrainNum(&painter, startX - 10, startY - 15, m_pStaTrain->m_strTrainNum, pTrafficLog->m_bUpDown);
            painter.drawText(startX + 10, startY - 10, QString("%1").arg(min));

            if (pTrafficLog->m_tRealArrivalTime.isNull()) {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                if (pTrafficLog->m_tAdjDepartTime.isNull()) {
                    middleX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time());
                    min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                    middleY = startY + offset2;
                }
                else {
                    middleX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time());
                    min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                    middleY = startY + offset1;
                }
            }

            else {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tRealArrivalTime.time().minute();
                middleX = getPointXByTime(pTrafficLog->m_tRealArrivalTime.time());
                middleY = startY + offset1;
            }
            painter.drawLine(startX, startY, middleX, middleY);
            painter.drawText(middleX + 10, middleY - 10, QString("%1").arg(min));
            painter.setPen(Qt::blue);
            painter.drawText(middleX - 30, middleY + 16, QString("%1").arg(pTrafficLog->m_strDepartTrack));
            drawEndFlag(&painter, middleX, middleY, pTrafficLog->m_bUpDown);
        }
        else if (pTrafficLog->m_nPlanType == 0x04) {
            if (pTrafficLog->m_bUpDown)
            {
                offset1 = TrainDiagram->stationDistance;
                offset2 = TrainDiagram->stationDistance / 2;
            }
            else
            {
                offset1 = -1 * (TrainDiagram->stationDistance);
                startY = startY + TrainDiagram->stationDistance * 2;
                offset2 = -1 * TrainDiagram->stationDistance / 2;
            }

            if (pTrafficLog->m_tAdjDepartTime.isNull())
            {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                startX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time()) - 15;
                startY = startY + offset2;
            }
            else
            {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tAdjDepartTime.time().toString("mm").toInt();
                startX = getPointXByTime(pTrafficLog->m_tAdjDepartTime.time());
            }
            painter.drawLine(startX - 10, startY - 15, startX, startY);
            drawTrainNum(&painter, startX - 10, startY - 15, m_pStaTrain->m_strTrainNum, pTrafficLog->m_bUpDown);
            painter.drawText(startX + 10, startY - 10, QString("%1").arg(min));
            painter.setPen(Qt::blue);
            if (pTrafficLog->m_tRealArrivalTime.isNull())
            {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                if (pTrafficLog->m_tAdjDepartTime.isNull()) {
                    middleX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time());
                    min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                    middleY = startY + offset2;
                }
                else {
                    middleX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time());
                    min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                    middleY = startY + offset1;
                }
            }
            else
            {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tRealArrivalTime.time().toString("mm").toInt();
                middleX = getPointXByTime(pTrafficLog->m_tRealArrivalTime.time());
                middleY = startY + offset1;
            }

            painter.drawLine(startX, startY, middleX, middleY);
            painter.drawText(middleX + 10, middleY - 10, QString("%1").arg(min));
            painter.setPen(Qt::blue);
            painter.drawText(middleX - 25, middleY + 15, QString("%1").arg(pTrafficLog->m_strDepartTrack));

            if (pTrafficLog->m_tAdjArrivalTime.isNull()) {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                endX = middleX + 15;
                min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                endY = middleY + offset2;
            }
            else {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tAdjArrivalTime.time().minute();
                endX = getPointXByTime(pTrafficLog->m_tAdjArrivalTime.time());
                endY = middleY + offset1;
            }

            painter.drawLine(middleX, middleY, endX, endY);

            painter.drawText(endX + 10, endY - 10, QString("%1").arg(min));
            painter.setPen(Qt::blue);
            drawEndFlag(&painter, endX, endY, pTrafficLog->m_bUpDown);

        }
        else
        {
            if (pTrafficLog->m_bUpDown)
            {
                offset1 = TrainDiagram->stationDistance;
                offset2 = TrainDiagram->stationDistance / 2;
            }
            else
            {
                offset1 = -1 * (TrainDiagram->stationDistance);
                startY = startY + TrainDiagram->stationDistance * 2;
                offset2 = -1 * TrainDiagram->stationDistance / 2;
            }

            if (pTrafficLog->m_tAdjDepartTime.isNull())
            {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                startX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time()) - 15;
                startY = startY + offset2;
            }
            else
            {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tAdjDepartTime.time().toString("mm").toInt();
                startX = getPointXByTime(pTrafficLog->m_tAdjDepartTime.time());
            }
            painter.drawLine(startX - 10, startY - 15, startX, startY);
            drawTrainNum(&painter, startX - 10, startY - 15, m_pStaTrain->m_strTrainNum, pTrafficLog->m_bUpDown);
            painter.drawText(startX + 10, startY - 10, QString("%1").arg(min));
            painter.setPen(Qt::blue);
            if (pTrafficLog->m_tRealArrivalTime.isNull())
            {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                if (pTrafficLog->m_tAdjDepartTime.isNull()) {
                    middleX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time());
                    min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                    middleY = startY + offset2;
                }
                else {
                    middleX = getPointXByTime(pTrafficLog->m_tProvArrivalTime.time());
                    min = pTrafficLog->m_tProvArrivalTime.time().toString("mm").toInt();
                    middleY = startY + offset1;
                }
            }
            else
            {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tRealArrivalTime.time().toString("mm").toInt();
                middleX = getPointXByTime(pTrafficLog->m_tRealArrivalTime.time());
                middleY = startY + offset1;
            }

            painter.drawLine(startX, startY, middleX, middleY);
            painter.drawText(middleX + 10, middleY - 10, QString("%1").arg(min));
            painter.setPen(Qt::blue);
            painter.drawText(middleX - 25, middleY + 15, QString("%1").arg(pTrafficLog->m_strDepartTrack));
            if (pTrafficLog->m_tRealDepartTime.isNull())
            {
                setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                stopX = getPointXByTime(pTrafficLog->m_tProvDepartTime.time()) - middleX + middleX;
                stopY = middleY;
            }
            else
            {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                stopX = getPointXByTime(pTrafficLog->m_tRealDepartTime.time());
                stopY = middleY + offset1;
            }

            painter.drawLine(middleX, middleY, stopX, stopY);
            if (pTrafficLog->m_tAdjArrivalTime.isNull()) {
                if (pTrafficLog->m_tRealDepartTime.isNull()) {
                    setPenInfoByPoint(&painter, true, m_pStaTrain->m_bFreightTrain);
                    endX = stopX + 15;
                    endY = stopY + offset2;
                }
                else
                {
                    setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                    endX = getPointXByTime(pTrafficLog->m_tAdjArrivalTime.time());
                    endY = stopY + offset1;
                }
            }
            else {
                setPenInfoByPoint(&painter, false, m_pStaTrain->m_bFreightTrain);
                min = pTrafficLog->m_tAdjArrivalTime.time().minute();
                endX = getPointXByTime(pTrafficLog->m_tAdjArrivalTime.time());
                endY = stopY + offset1;
            }
            painter.drawLine(stopX, stopY, endX, endY);

            painter.drawText(endX + 10, endY - 10, QString("%1").arg(min));
            painter.setPen(Qt::blue);
            drawEndFlag(&painter, endX, endY, pTrafficLog->m_bUpDown);
        }
    }

}

void StaTraindiagramwidgetKSK::setPenInfoByPoint(QPainter* painter, bool isNull, int nLHFlg)
{
    QFont font;
    QPen pen; //画笔
    pen.setWidth(2);
    pen.setColor(Qt::red);
    if (isNull)
    {
        font = QFont("微软雅黑", 11);
    }
    else
    {
        font = QFont("微软雅黑", 11, QFont::Bold);
    }
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
        qDebug() << nowTimeT.minute() << nowTime.minute();
        if (nowTimeT.minute() != nowTime.minute())
        {
            nowTimeLinePx = getPointXByTime(QDateTime::currentDateTime().time());
            ui.diagramWidget->update();
        }
    }
}

void StaTraindiagramwidgetKSK::drawTrainNum(QPainter* painter, int X, int Y, QString trainNum, bool isUpgoing)
{

    QFontMetrics metrics(painter->font());
    painter->drawLine(X, Y, X - metrics.width(trainNum), Y);
    painter->drawLine(X - metrics.width(trainNum), Y, X - 8 - metrics.width(trainNum), Y - 8);
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

void StaTraindiagramwidgetKSK::drawEndFlag(QPainter* painter, int X, int Y, bool isUpgoing)
{
    QPen pen; 
    pen.setWidth(2);
    pen.setColor(Qt::red);
    painter->setPen(pen);
    if (isUpgoing)
    {
        painter->setBrush(Qt::NoBrush);
        int lineEndY = Y + 15;
        painter->drawLine(X, Y, X, lineEndY);
        int triangleWidth = 8;

        int triangleHeight = 5;

        painter->drawLine(X - triangleWidth / 2, lineEndY, X, lineEndY + triangleHeight);
        painter->drawLine(X, lineEndY + triangleHeight, X + triangleWidth / 2, lineEndY);
        painter->drawLine(X + triangleWidth / 2, lineEndY, X - triangleWidth / 2, lineEndY);

    }
    else
    {
        painter->setBrush(Qt::NoBrush);
        int lineEndY = Y+15;
        painter->drawLine(X, Y, X, lineEndY);
        int triangleWidth = 8;

        int triangleHeight = 5;

        painter->drawLine(X - triangleWidth / 2, lineEndY, X, lineEndY + triangleHeight);
        painter->drawLine(X, lineEndY + triangleHeight, X + triangleWidth / 2, lineEndY);
        painter->drawLine(X + triangleWidth / 2, lineEndY, X - triangleWidth / 2, lineEndY);
    }
}