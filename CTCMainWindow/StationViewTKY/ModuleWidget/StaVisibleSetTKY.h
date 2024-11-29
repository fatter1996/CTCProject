#pragma once

#include "BaseWndClass/ModuleWidget/StaVisibleSet.h"
#include "ui_StaVisibleSetTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StaVisibleSetTKY : public BaseWnd::StaVisibleSet
		{
			Q_OBJECT
		public:
			StaVisibleSetTKY(QWidget* parent = nullptr);
			~StaVisibleSetTKY();

		private:
			Ui::StaVisibleSetTKYClass ui;
		};
	}
}