#pragma once

#include "../BaseWndClass/StationLogDisp.h"
#include "ui_StationLogDispTKY.h"

namespace CTCWindows {
	class StationLogDispTKY : public StationLogDisp
	{
		Q_OBJECT

	public:
		StationLogDispTKY(QWidget* parent = nullptr);
		~StationLogDispTKY();

	private:
		Ui::StationLogDispTKYClass ui;
	};
}
