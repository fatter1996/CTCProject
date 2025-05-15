#pragma once

#include <QWidget>
#include "ui_StaAddNewTrainKSK.h"
#include "StationObject/GlobalStruct.h"

namespace CTCWindows {
	namespace CASCO {
		class StaAddNewTrainKSK : public QWidget
		{
			Q_OBJECT

		public:
			StaAddNewTrainKSK(QWidget* parent = nullptr);
			~StaAddNewTrainKSK();

		public:
			void InitAddView();
			void InitAddView(Station::StaTrafficLog* m_pCurTrafficLog);
			void ConnectEvent();
			void AddNewTrafficLog(Station::StaTrafficLog* pTrafficLog);

		private:
			Station::StaTrafficLog* m_pTrafficLog = nullptr;
			Ui::StaAddNewTrainKSKClass ui;
		};
	}
}