#pragma once
#include <QWidget>
#include "../CustomControl/RichTableView.h"

namespace CTCWindows {
    namespace BaseWnd {
        //行车日志界面基类
        class StationLogDisp : public QWidget
        {
            Q_OBJECT
        public:
            StationLogDisp(QWidget* parent);
            ~StationLogDisp();

        public:
            void InitTrafficLogTable();
            virtual void InitTrafficLogTableHead() = 0;
            virtual void AddTrafficLogTable() = 0;

        protected:
            Control::RichTableView* pTrafficLogTable = nullptr;
        };
    }
}