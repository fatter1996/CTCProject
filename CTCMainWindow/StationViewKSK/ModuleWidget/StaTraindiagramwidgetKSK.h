#pragma once

#include <QWidget>
#include "ui_StaTraindiagramwidgetKSK.h"
#include "CTCMainWindow/BaseWndClass/StationView/StaTraindiagramwidget.h"
#include <QTime>
#include "Global.h"
#define SPACING_LEFTORRIGHT 2
#define SPACING_TOPORBOTTOM 60
#define SPACING_TIMEAXIS 50
class StaTraindiagramwidgetKSK : public CTCWindows::BaseWnd::StaTraindiagramwidget
{
	Q_OBJECT

public:
	StaTraindiagramwidgetKSK(QWidget *parent = nullptr);
	~StaTraindiagramwidgetKSK();
	void InitTraindiagramw();
	void drawTimeAxis(bool isTop);
	void drawGrid();
	void drawNowTimeLine();
	void drawStationName();
	void drawTrainPlan();
	int getDirectionIndex(Station::StaTrafficLog* pTrafficLog); 
	void setPenInfoByPoint(QPainter* painter, bool isNull, int nLHFlg);
	void drawEndFlag(QPainter* painter, int X, int Y, bool isUpgoing);
	void drawTrainNum(QPainter* painter, int X, int Y, QString trainNum, bool isUpgoing);
	int getPointXByTime(QTime time);
	void timerEvent(QTimerEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
private:
	Ui::StaTraindiagramwidgetKSKClass ui;
	Station::TrainDiagramInfo* TrainDiagram = nullptr;
	int maxStationWidgetWidth = 0;
	int Timer_ID_60000 = 0; //进路序列刷新计时器
	int nowTimeLinePx = 0;
	int oldTimeLinePx = 0;
	QTime nowTime;
};
