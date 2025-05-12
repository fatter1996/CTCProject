#pragma once
#include <QWidget>
#include "../../StationObject/GlobalStruct.h"

namespace CTCWindows {
    namespace BaseWnd {
        class StaTrainDiagramWidget :
            public QWidget
        {
        public:
            StaTrainDiagramWidget(QWidget* parent = nullptr);
            ~StaTrainDiagramWidget();

        public:
            virtual void InitTraindiagramw() {};

        protected:
            Station::TrainDiagramInfo TrainDiagram;
        };
    }
}
