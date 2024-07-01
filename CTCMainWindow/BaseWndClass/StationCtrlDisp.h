#pragma once
#include <QWidget>
#include <QEvent>
#include "StaFunBtnToolBar.h"

namespace CTCWindows {
    namespace BaseWnd {
        //单站界面基类
        class StationCtrlDisp : public QWidget
        {
            Q_OBJECT
        public:
            StationCtrlDisp(QWidget* parent = nullptr);
            ~StationCtrlDisp();

        public:
            virtual void CreatStaFunBtnToolBar() = 0;

        public:
            virtual QWidget* StaPaintView() = 0;
            QWidget* StaFunBtnBar() { return m_pStaFunBtnToolBar; }
        public:
            virtual void timerEvent(QTimerEvent* event);

        protected:
            StaFunBtnToolBar* m_pStaFunBtnToolBar = nullptr;
            int m_nTimerId_500 = -1;
        };
    }
}