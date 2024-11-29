#pragma once

#include "BaseWndClass/StationView/StationCtrlDisp.h"
#include "ui_StationCtrlDispKSK.h"

namespace CTCWindows {
	namespace CASCO {
		class StationCtrlDispKSK : public BaseWnd::StationCtrlDisp
		{
			Q_OBJECT
		public:
			StationCtrlDispKSK(QWidget* parent = nullptr);
			~StationCtrlDispKSK();

		public:
			void CreatStaFunBtnToolBar() override;
			bool eventFilter(QObject* obj, QEvent* event) override;

		public:
			QWidget* StaPaintView() const override { return ui.widget; }

		private:
			Ui::StationCtrlDispKSK ui;
		};
	}
}
