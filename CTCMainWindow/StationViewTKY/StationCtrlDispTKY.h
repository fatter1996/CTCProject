#pragma once

#include "StaFunBtnToolBarTKY.h"
#include "../BaseWndClass/StationCtrlDisp.h"
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
			void CreatStaFunBtnToolBar();

		public:
			QWidget* StaPaintView() { return ui.widget; }

		private:
			Ui::StationCtrlDispTKY ui;
		};
	}
}
