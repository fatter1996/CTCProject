#pragma once

#include "BaseWndClass/StationView/StationMultiDisp.h"
#include "ui_StationMultiDispTH.h"
#include "StationObject/StationObject.h"

namespace CTCWindows {
	namespace TH {
		class StationMultiDispTH : public BaseWnd::StationMultiDisp
		{
			Q_OBJECT

		public:
			StationMultiDispTH(QWidget* parent = nullptr);
			~StationMultiDispTH();

		public:
			void InitMultiStation(QVector<Station::StationObject*> vecMultiStation) override;

		private:
			Ui::StationMultiDispTH ui;
		};
	}
}
