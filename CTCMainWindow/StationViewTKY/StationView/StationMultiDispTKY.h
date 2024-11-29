#pragma once

#include "BaseWndClass/StationView/StationMultiDisp.h"
#include "ui_StationMultiDispTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StationMultiDispTKY : public BaseWnd::StationMultiDisp
		{
			Q_OBJECT

		public:
			StationMultiDispTKY(QWidget* parent = nullptr);
			~StationMultiDispTKY();

		public:
			void InitMultiStation(QVector<Station::StationObject*> vecMultiStation) override;

		private:
			Ui::StationMultiDispTKY ui;
		};
	}
}
