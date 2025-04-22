#pragma once
#include <QDialog>
#include <StationObject/GlobalStruct.h>

namespace CTCWindows {
	namespace BaseWnd {

		class StaDispatchOrder : public QDialog
		{
			Q_OBJECT
		public:
			StaDispatchOrder(QWidget* parent = nullptr);
			~StaDispatchOrder();
		
		public:
			virtual void ViewPermission(Station::LimitsOfAuthority operate) = 0;
			virtual void DispatchOrderListUpData() = 0;
			virtual void CurDispatchOrderUpData() = 0;
			virtual void TrainDispatchOrderListUpData() = 0;
			virtual void CurTrainDispatchOrderUpData() = 0;
			virtual void GetTrainInfo(Station::StaTrainDispatch* pTrainDispatch) = 0;

		public slots:
			void SignForDispatchOrder();
			void AddNewTrainDispatch();
			void ModifyNewTrainDispatch();
			void DeleteNewTrainDispatch();

		protected:
			Station::StaDispatchOrder* m_pCurDispatch = nullptr;
			Station::StaTrainDispatch* m_pCurTrainDispatch = nullptr;
		};
	}
}