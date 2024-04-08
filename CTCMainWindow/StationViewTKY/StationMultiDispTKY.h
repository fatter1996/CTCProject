#pragma once

#include "../BaseWndClass/StationMultiDisp.h"
#include "ui_StationMultiDispTKY.h"

namespace CTCWindows {
	class StationMultiDispTKY : public StationMultiDisp
	{
		Q_OBJECT

	public:
		StationMultiDispTKY(QWidget* parent = nullptr);
		~StationMultiDispTKY();

	private:
		Ui::StationMultiDispTKYClass ui;
	};
}
