#pragma once

#include <QWidget>
#include "BaseWndClass/ModuleWidget/StaDispatchOrder.h"
#include "StationObject/GlobalStruct.h"
#include "ui_StaDispatchOrderKSK.h"

namespace CTCWindows {
	namespace CASCO {
		class StaDispatchOrderKSK : public BaseWnd::StaDispatchOrder
		{
			Q_OBJECT

		public:
			StaDispatchOrderKSK(QWidget* parent = nullptr);
			~StaDispatchOrderKSK();

		public:
			void DispatchOrderListUpData() {}
			void CurDispatchOrderUpData() {}
			void TrainDispatchOrderListUpData() {}
			void CurTrainDispatchOrderUpData() {}
			void GetTrainInfo(Station::StaTrainDispatch* pTrainDispatch) {}

		private:
			Ui::StaDispatchOrderKSK ui;
		};
	}
}
