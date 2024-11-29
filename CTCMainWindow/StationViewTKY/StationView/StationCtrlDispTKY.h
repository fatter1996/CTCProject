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

		public:
			QWidget* StaPaintView() const override { return ui.widget; }

		private:
			Ui::StationCtrlDispTKY ui;
		};
	}
}
