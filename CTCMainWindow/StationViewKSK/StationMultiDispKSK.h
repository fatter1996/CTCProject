#pragma once

#include "../BaseWndClass/StationMultiDisp.h"
#include "ui_StationMultiDispKSK.h"

namespace CTCWindows {
	class StationMultiDispKSK : public StationMultiDisp
	{
		Q_OBJECT

	public:
		StationMultiDispKSK(QWidget* parent = nullptr);
		~StationMultiDispKSK();

	private:
		Ui::StationMultiDispKSKClass ui;
	};
}
