#pragma once

#include <QWidget>
#include "BaseWndClass/ModuleWidget/StaFunBtnToolBar.h"
#include "ui_StaFunBtnToolBarKSK.h"

namespace CTCWindows {
	namespace CASCO {
		class StaFunBtnToolBarKSK : public BaseWnd::StaFunBtnToolBar
		{
			Q_OBJECT
		public:
			StaFunBtnToolBarKSK(QWidget* parent = nullptr);
			~StaFunBtnToolBarKSK();

			void InitAuxiliaryMenu(AuxiliaryMenuWnd* pAuxiliary) override;

		private:
			Ui::StaFunBtnToolBarKSK ui;
		};
	}
}


