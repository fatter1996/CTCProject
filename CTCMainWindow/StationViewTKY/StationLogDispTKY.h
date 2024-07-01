#pragma once

#include "../BaseWndClass/StationLogDisp.h"
#include "ui_StationLogDispTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StationLogDispTKY : public BaseWnd::StationLogDisp
		{
			Q_OBJECT

		public:
			StationLogDispTKY(QWidget* parent = nullptr);
			~StationLogDispTKY();

		public:
			void InitTrafficLogTableHead();
			void AddTrafficLogTable();

		private:
			Ui::StationLogDispTKY ui;
		};
	}
}
