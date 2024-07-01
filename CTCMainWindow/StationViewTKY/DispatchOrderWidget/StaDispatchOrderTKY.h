#pragma once

#include <QWidget>
#include "../../BaseWndClass/DispatchOrderWidget/StaDispatchOrder.h"
#include "../../../StationObject/GlobalStruct.h"
#include "ui_StaDispatchOrderTKY.h"

namespace CTCWindows {
	namespace CARS {
		class StaDispatchOrderTKY : public BaseWnd::StaDispatchOrder
		{
			Q_OBJECT
		public:
			StaDispatchOrderTKY(QWidget* parent = nullptr);
			~StaDispatchOrderTKY();

		public:
			void DispatchOrderListUpData();
			void CurDispatchOrderUpData();
			void TrainDispatchOrderListUpData();
			void CurTrainDispatchOrderUpData();
			void GetTrainInfo(Station::StaTrainDispatch* pTrainDispatch);

		private:
			Ui::StaDispatchOrderTKY ui;
			QVector<Station::StaDispatchOrder*> m_vecDispatchOrder;
			QVector<Station::StaDispatchOrder*> m_vecImpDispatchOrder;
		};
	}
}
