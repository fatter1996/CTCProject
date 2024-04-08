#pragma once
#include <QWidget>
#include <QEvent>
namespace CTCWindows {
    //单站界面基类
    class StationCtrlDisp : public QWidget
    {
        Q_OBJECT
    public:
        StationCtrlDisp(QWidget* parent = nullptr);
        ~StationCtrlDisp();
    public:
        virtual QWidget* GetStationPaintView() = 0;

    public:
        virtual void timerEvent(QTimerEvent* event);


    private:
        int m_nTimerId_500 = -1;
    };
}