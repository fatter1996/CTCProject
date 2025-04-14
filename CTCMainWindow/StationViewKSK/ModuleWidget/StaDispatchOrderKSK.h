#pragma once

#include <QWidget>
#include "BaseWndClass/ModuleWidget/StaDispatchOrder.h"
#include "StationObject/GlobalStruct.h"
#include "ui_StaDispatchOrderKSK.h"
#include <CustomControl/TableView.h>
namespace CTCWindows {
	namespace CASCO {
		enum class TableSpecies : int {
			station,				//车站
			Dispatcher,				//调度台
			Locomot,				//机车
			CopyTable,				//抄知处所表格
			ReadTable,				//阅读表格
			DispatchTable,			//调度台表格
			LocomotTable			//机车表格
		};
		enum class DataType : int {
			RetakeOrder,			//收令箱
			PrecollectionOrder,		//预收令箱
			ReserveOrder,			//储令箱
			PendingOrder,			//待发令箱
			IssueAnOrder,			//发令箱

		};
		class StaDispatchOrderKSK : public BaseWnd::StaDispatchOrder
		{
			Q_OBJECT

		public:
			StaDispatchOrderKSK(QWidget* parent = nullptr);
			~StaDispatchOrderKSK();
			bool eventFilter(QObject* watched, QEvent* event);
			void InitControl();
			void SetTableWidgetStation(int index,int flag = 0);
			QVector<QStringList> GetStationOrderData(QString strTrainNum, Station::StaDispatchOrder* pRoute, DataType type);				//	获取车站收令数据
			QVector<QStringList> GetDispatcherReceivingData(QString strTrainNum, Station::StaDispatchOrder* pRoute, DataType type);		//	获取调度台收令数据
			QVector<QStringList> GetTheLocomotiveCollectionData(QString strTrainNum, Station::StaDispatchOrder* pRoute, DataType type);	//	获取机车收令数据
			void ViewPermission(Station::LimitsOfAuthority operate) {}
			void DispatchOrderListUpData() {}
			void CurDispatchOrderUpData() {}
			void TrainDispatchOrderListUpData() {}
			void CurTrainDispatchOrderUpData() {}
			void GetTrainInfo(Station::StaTrainDispatch* pTrainDispatch) {}
			void InitTable();
			QVector<Control::TableViewHeadInfo> GetTrainRouteTableHeadInfo(TableSpecies m_intSpecies);
		private:
			Ui::StaDispatchOrderKSK ui;
			CTCWindows::Control::TableView* m_pStationOrder = nullptr;	//车站命令表格
			CTCWindows::Control::TableView* m_pDispatcherCommand = nullptr;	//调度台命令表格
			CTCWindows::Control::TableView* m_pLocomotiveOrder = nullptr;		//机车命令表格
			CTCWindows::Control::TableView* m_pCopyTable = nullptr;	//抄知处所表格
			CTCWindows::Control::TableView* m_pReadTable = nullptr;	//阅读表格
			CTCWindows::Control::TableView* m_pdispatchTable = nullptr;	//调度台接收表格
			CTCWindows::Control::TableView* m_pLocomotiveTable = nullptr;	//机车接收表格
		};
	}
}
