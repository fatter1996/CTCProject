#pragma once

#include "../BaseWndClass/StationLogDisp.h"
#include "ui_StationLogDispKSK.h"

namespace CTCWindows {
	namespace CASCO {
		class StationLogDispKSK : public BaseWnd::StationLogDisp
		{
			Q_OBJECT

		public:
			StationLogDispKSK(QWidget* parent = nullptr);
			~StationLogDispKSK();

		private:
			Ui::StationLogDispKSK ui;
		};
	}
}
