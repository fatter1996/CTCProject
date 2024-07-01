#pragma once

#include "../BaseWndClass/StationCtrlDisp.h"
#include "ui_StationCtrlDispKSK.h"

namespace CTCWindows {
	namespace CASCO {
		class StationCtrlDispKSK : public BaseWnd::StationCtrlDisp
		{
			Q_OBJECT

		public:
			StationCtrlDispKSK(QWidget* parent = nullptr);
			~StationCtrlDispKSK();

		private:
			Ui::StationCtrlDispKSK ui;
		};
	}
}
