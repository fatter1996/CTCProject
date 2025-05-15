#pragma once

#include <QWidget>
#include "ui_ModifyTrainNumberKSK.h"
#include "Global.h"

namespace CTCWindows {
	namespace CASCO {
		class ModifyTrainNumberKSK : public QWidget
		{
			Q_OBJECT

		public:
			ModifyTrainNumberKSK(Station::StaTrafficLog* m_pCurTrafficLog, QWidget* parent = nullptr);
			~ModifyTrainNumberKSK();

		private:
			Ui::ModifyTrainNumberKSKClass ui;
		};
	}
}