#pragma once

#include <QWidget>
#include "ui_StaTrainDiagramWidgetKSK.h"
#include "StationView/StaTrainDiagramWidget.h"
#include <QTime>


#define SPACING_LEFTORRIGHT 2
#define SPACING_TOPORBOTTOM 60
#define SPACING_TIMEAXIS 50
namespace CTCWindows {
	namespace CASCO {
		class StaTrainDiagramWidgetKSK : public BaseWnd::StaTrainDiagramWidget
		{
			Q_OBJECT

		public:
			StaTrainDiagramWidgetKSK(QWidget* parent = nullptr);
			~StaTrainDiagramWidgetKSK();

		public:
			void InitTraindiagramw();
			void drawTimeAxis(bool isTop);
			void drawGrid();
			void drawNowTimeLine();
			void drawStationName();
			void drawTrainPlan();
			int getDirectionIndex(Station::StaTrafficLog* pTrafficLog);
			void drawEndFlag(QPainter* painter, bool UPDown, int X, int Y);
			void drawTrainNum(QPainter* painter, int X, int Y, QString trainNum);
			int getPointXByTime(QTime time);
			void timerEvent(QTimerEvent* event);
			bool eventFilter(QObject* obj, QEvent* event);
			void drawRouteLine(QPainter* painter, Station::RailwayLine RailwayLine, Station::StaTrafficLog* pTrafficLog, Station::StaTrainRoute* pRoute, int& nStartX, int& nEndX, int& nEndY);

		private:
			Ui::StaTrainDiagramWidgetKSKClass ui;
			int maxStationWidgetWidth = 0;
			int Timer_ID_60000 = 0; //进路序列刷新计时器
			int nowTimeLinePx = 0;
			int oldTimeLinePx = 0;
			QTime nowTime;
		};
	}
}
