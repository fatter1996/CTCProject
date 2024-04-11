#pragma once

#include <QWidget>
#include "../BaseWndClass/StaFunBtnToolBar.h"
#include "ui_StaFunBtnToolBarTKY.h"

namespace CTCWindows {
	class StaFunBtnToolBarTKY : public StaFunBtnToolBar
	{
		Q_OBJECT

	public:
		StaFunBtnToolBarTKY(QWidget* parent = nullptr);
		~StaFunBtnToolBarTKY();

	public:
		void FunBtnStateReset();

	private:
		Ui::StaFunBtnToolBarTKYClass ui;
	};
}
