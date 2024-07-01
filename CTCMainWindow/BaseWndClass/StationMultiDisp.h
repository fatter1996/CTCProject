#pragma once
#include <QWidget>

namespace CTCWindows {
    namespace BaseWnd {
        //站间透明界面基类
        class StationMultiDisp : public QWidget
        {
            Q_OBJECT
        public:
            StationMultiDisp(QWidget* parent) {}
        };
    }
}