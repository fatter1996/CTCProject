#pragma once

#include "BaseWndClass/StationView/StationMultiDisp.h"
#include "ui_StationMultiDispKSK.h"
#include "StationObject/StationObject.h"

namespace CTCWindows {
	namespace CASCO {
		class StationMultiDispKSK : public BaseWnd::StationMultiDisp
		{
			Q_OBJECT

		public:
			StationMultiDispKSK(QWidget* parent = nullptr);
			~StationMultiDispKSK();

		public:
			void InitMultiStation(QVector<Station::StationObject*> vecMultiStation) override;

		private:
			Ui::StationMultiDispKSK ui;
			
		};
	}
}
