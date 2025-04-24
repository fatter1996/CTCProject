#pragma once
#include <QWidget>
namespace CTCWindows {

    namespace BaseWnd {

        class StaTraindiagramwidget :
            public QWidget
        {
        public:
            StaTraindiagramwidget(QWidget* parent = nullptr);
            ~StaTraindiagramwidget();
        };
    }
}
