#pragma once

#include <QWidget>
#include "ui_ModifyRelevantStationsKSK.h"
#include "Global.h"

namespace CTCWindows {
	namespace CASCO {
		class ModifyRelevantStationsKSK : public QWidget
		{
			Q_OBJECT

		public:
			ModifyRelevantStationsKSK(Station::StaTrafficLog* m_pCurTrafficLog, QWidget* parent = nullptr);
			~ModifyRelevantStationsKSK();

		private:
			Ui::ModifyRelevantStationsKSKClass ui;
		};
	}
}
