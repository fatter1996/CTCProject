#include "StaTrainDiagramWidget.h"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "Global.h"

namespace CTCWindows {
	namespace BaseWnd {
		StaTrainDiagramWidget::StaTrainDiagramWidget(QWidget* parent)
		{
			TrainDiagram = Station::MainStation()->getTrainDiagram();
			InitTraindiagramw();
		}

		StaTrainDiagramWidget::~StaTrainDiagramWidget()
		{
		}
	}
}
