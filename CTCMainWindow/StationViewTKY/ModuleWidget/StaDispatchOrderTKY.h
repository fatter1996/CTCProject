#pragma once

#include <QWidget>
#include "BaseWndClass/ModuleWidget/StaDispatchOrder.h"
#include "StationObject/GlobalStruct.h"
#include "ui_StaDispatchOrderTKY.h"
#include <StationObject/GlobalStruct.h>

namespace CTCWindows {
	namespace CARS {

		class StaDispatchOrderTKY : public BaseWnd::StaDispatchOrder
		{
			Q_OBJECT
		public:
			StaDispatchOrderTKY(QWidget* parent = nullptr);
			~StaDispatchOrderTKY();

		public:
			void ViewPermission(Station::LimitsOfAuthority operate) override;
			void DispatchOrderListUpData() override;
			void CurDispatchOrderUpData() override;
			void TrainDispatchOrderListUpData() override;
			void CurTrainDispatchOrderUpData() override;
			void GetTrainInfo(Station::StaTrainDispatch* pTrainDispatch) override;

		private:
			Ui::StaDispatchOrderTKY ui;
			QVector<Station::StaDispatchOrder*> m_vecDispatchOrder;
			QVector<Station::StaDispatchOrder*> m_vecImpDispatchOrder;
		};
	}
}
