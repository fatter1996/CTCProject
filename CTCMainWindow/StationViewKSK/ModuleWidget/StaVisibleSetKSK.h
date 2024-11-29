#pragma once

#include "ui_StaVisibleSetKSK.h"
#include "BaseWndClass/ModuleWidget/StaVisibleSet.h"

namespace CTCWindows {
	namespace CASCO {
		class StaVisibleSetKSK : public BaseWnd::StaVisibleSet
		{
			Q_OBJECT
		public:
			StaVisibleSetKSK(QWidget* parent = nullptr);
			~StaVisibleSetKSK();

		private:
			Ui::StaVisibleSetKSK ui;
		};
	}
}
