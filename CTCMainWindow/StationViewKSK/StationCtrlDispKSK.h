#pragma once

#include "../BaseWndClass/StationCtrlDisp.h"
#include "ui_StationCtrlDispKSK.h"

namespace CTCWindows {
	class StationCtrlDispKSK : public StationCtrlDisp
	{
		Q_OBJECT

	public:
		StationCtrlDispKSK(QWidget* parent = nullptr);
		~StationCtrlDispKSK();

	private:
		Ui::StationCtrlDispKSKClass ui;
	};
}
