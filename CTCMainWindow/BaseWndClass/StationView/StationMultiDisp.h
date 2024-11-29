#pragma once
#include <QWidget>
#include "StationObject/StationObject.h"

namespace CTCWindows {
    namespace BaseWnd {
        //站间透明界面基类
        class StationMultiDisp : public QWidget
        {
            Q_OBJECT
        public:
            StationMultiDisp(QWidget* parent = nullptr);
            ~StationMultiDisp();

        protected:
            void InitMultiStation(QWidget* parent, QVector<Station::StationObject*> vecMultiStation);

        public:
            virtual void InitMultiStation(QVector<Station::StationObject*> vecMultiStation) = 0;
            virtual void timerEvent(QTimerEvent* event) override;
            virtual bool eventFilter(QObject* obj, QEvent* event) override;

        protected:
            QVector<Station::StationObject*> m_vecMultiStation;

        private:
            int m_nTimerId_500 = -1;
        };
    }
}