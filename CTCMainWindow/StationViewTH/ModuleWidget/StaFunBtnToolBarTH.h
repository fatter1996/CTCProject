#pragma once

#include "BaseWndClass/ModuleWidget/StaFunBtnToolBar.h"
#include "ui_StaFunBtnToolBarTH.h"

namespace CTCWindows {
	namespace TH {
		class StaFunBtnToolBarTH : public BaseWnd::StaFunBtnToolBar
		{
			Q_OBJECT

		public:
			StaFunBtnToolBarTH(QWidget* parent = nullptr);
			~StaFunBtnToolBarTH();

			void InitAuxiliaryMenu(AuxiliaryMenuWnd* pAuxiliary) override {}

		private:
			Ui::StaFunBtnToolBarTH ui;
		};
	}
}
