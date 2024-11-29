#pragma once

#include "../BaseWndClass/StationMultiDisp.h"
#include "ui_StationMultiDispKSK.h"

namespace CTCWindows {
	namespace CASCO {
		class StationMultiDispKSK : public BaseWnd::StationMultiDisp
		{
			Q_OBJECT

		public:
			StationMultiDispKSK(QWidget* parent = nullptr);
			~StationMultiDispKSK();

		private:
			Ui::StationMultiDispKSK ui;
		};
	}
}
