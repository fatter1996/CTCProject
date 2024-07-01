#pragma once

#include <QWidget>
#include "../BaseWndClass/StaFunBtnToolBar.h"
#include "ui_StaFunBtnToolBarTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StaFunBtnToolBarTKY : public BaseWnd::StaFunBtnToolBar
		{
			Q_OBJECT
		public:
			StaFunBtnToolBarTKY(QWidget* parent = nullptr);
			~StaFunBtnToolBarTKY();

		public slots:
			void onFunBtnStateReset();

		private:
			Ui::StaFunBtnToolBarTKY ui;
		};
	}
}
