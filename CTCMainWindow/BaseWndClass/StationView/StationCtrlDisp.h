#pragma once
#include <QWidget>
#include <QEvent>
#include "../ModuleWidget/StaFunBtnToolBar.h"

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
            virtual QWidget* StaPaintView() const = 0;
            StaFunBtnToolBar* StaFunBtnBar() const { return m_pStaFunBtnToolBar; }

        public:
            virtual bool eventFilter(QObject* obj, QEvent* event) override;
            virtual void timerEvent(QTimerEvent* event) override;

        protected:
            StaFunBtnToolBar* m_pStaFunBtnToolBar = nullptr;

        private:
            int m_nTimerId_500 = -1;
        };
    }
}