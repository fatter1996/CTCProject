#pragma once

#include <QWidget>
#include "ui_StaAlarmWindowTKY.h"
#include "./CTCMainWindow/BaseWndClass/ModuleWidget/StaAlarmWindow.h"


namespace CTCWindows {
	namespace CARS {
		class StaAlarmWindowTKY : public CTCWindows::BaseWnd::StaAlarmWindow
		{
			Q_OBJECT

		public:
			StaAlarmWindowTKY(QWidget* parent = nullptr);
			~StaAlarmWindowTKY();

		private:
			Ui::StaAlarmWindowTKYClass ui;
		};
	}
}
