#pragma once

#include "BaseWndClass/StationView/StationCtrlDisp.h"
#include "ui_StationCtrlDispTH.h"

namespace CTCWindows {
	namespace TH {
		class StationCtrlDispTH : public BaseWnd::StationCtrlDisp
		{
			Q_OBJECT

		public:
			StationCtrlDispTH(QWidget* parent = nullptr);
			~StationCtrlDispTH();

		public:
			void CreatStaFunBtnToolBar() override;
			bool eventFilter(QObject* obj, QEvent* event) override;

		public:
			QWidget* StaPaintView() const override { return ui.widget; }

		private:
			Ui::StationCtrlDispTH ui;
		};
	}
}
