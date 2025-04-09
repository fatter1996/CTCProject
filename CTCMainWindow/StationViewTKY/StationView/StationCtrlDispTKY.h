#pragma once

#include "BaseWndClass/StationView/StationCtrlDisp.h"
#include "ui_StationCtrlDispTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StationCtrlDispTKY : public BaseWnd::StationCtrlDisp
		{
			Q_OBJECT

		public:
			StationCtrlDispTKY(QWidget* parent = nullptr);
			~StationCtrlDispTKY();

		public:
			void CreatStaFunBtnToolBar() override;
			void resizeEvent(QResizeEvent* event);
		public slots:
			void setCountdown();
		signals:
			void CountdownEnd();

		public:
			static void CilckDevicCountdownend() { m_nCountdown = 0; };
			static int m_nCountdown;
			QWidget* StaPaintView() const override { return ui.widget; }
			void timerEvent(QTimerEvent* event);
		private:
	
			int m_accumulatedTime = 0;
			int m_nTimerId = 0;
			QPushButton* m_pLeftButton = nullptr;
			QPushButton* m_pRightButton = nullptr;
			Ui::StationCtrlDispTKY ui;
		};
	}
}
