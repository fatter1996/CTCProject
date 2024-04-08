#pragma once

#include "../BaseWndClass/StationLogDisp.h"
#include "ui_StationLogDispKSK.h"

namespace CTCWindows {
	class StationLogDispKSK : public StationLogDisp
	{
		Q_OBJECT

	public:
		StationLogDispKSK(QWidget* parent = nullptr);
		~StationLogDispKSK();

	private:
		Ui::StationLogDispKSKClass ui;
	};
}
