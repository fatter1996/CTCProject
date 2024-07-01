#pragma once

#include "../BaseWndClass/StationMultiDisp.h"
#include "ui_StationMultiDispTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StationMultiDispTKY : public BaseWnd::StationMultiDisp
		{
			Q_OBJECT

		public:
			StationMultiDispTKY(QWidget* parent = nullptr);
			~StationMultiDispTKY();

		private:
			Ui::StationMultiDispTKY ui;
		};
	}
}
