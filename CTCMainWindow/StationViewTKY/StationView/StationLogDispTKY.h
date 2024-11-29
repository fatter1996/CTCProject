#pragma once

#include "BaseWndClass/StationView/StationLogDisp.h"
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
			void InitTrafficLogTableHead() override;
			void AddTrafficLogTable() override;
			void OnTrafficLogTableUpData() override;

		private:
			Ui::StationLogDispTKY ui;
		};
	}
}
