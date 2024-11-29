#pragma once

#include "BaseWndClass/ModuleWidget/StaFunBtnToolBar.h"
#include "ui_StaFunBtnToolBarTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StaFunBtnToolBarTKY : public BaseWnd::StaFunBtnToolBar
		{
			Q_OBJECT
		public:
			StaFunBtnToolBarTKY(QWidget* parent = nullptr);
			~StaFunBtnToolBarTKY();

			void InitAuxiliaryMenu(AuxiliaryMenuWnd* pAuxiliary) override;

		private:
			Ui::StaFunBtnToolBarTKY ui;
		};
	}
}
