#pragma once

#include <QWidget>
#include "BaseWndClass/ModuleWidget/StaDispatchOrder.h"
#include "StationObject/GlobalStruct.h"
#include "ui_StaDispatchOrderKSK.h"
#include <CustomControl/TableView.h>
namespace CTCWindows {
	namespace CASCO {
		enum class TableSpecies : int {
			station,				//��վ
			Dispatcher,				//����̨
			Locomot,				//����
			CopyTable,				//��֪�������
			ReadTable,				//�Ķ����
			DispatchTable,			//����̨���
			LocomotTable			//�������
		};
		enum class DataType : int {
			RetakeOrder,			//������
			PrecollectionOrder,		//Ԥ������
			ReserveOrder,			//������
			PendingOrder,			//��������
			IssueAnOrder,			//������

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
			QVector<QStringList> GetStationOrderData(QString strTrainNum, Station::StaDispatchOrder* pRoute, DataType type);				//	��ȡ��վ��������
			QVector<QStringList> GetDispatcherReceivingData(QString strTrainNum, Station::StaDispatchOrder* pRoute, DataType type);		//	��ȡ����̨��������
			QVector<QStringList> GetTheLocomotiveCollectionData(QString strTrainNum, Station::StaDispatchOrder* pRoute, DataType type);	//	��ȡ������������
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
			CTCWindows::Control::TableView* m_pStationOrder = nullptr;	//��վ������
			CTCWindows::Control::TableView* m_pDispatcherCommand = nullptr;	//����̨������
			CTCWindows::Control::TableView* m_pLocomotiveOrder = nullptr;		//����������
			CTCWindows::Control::TableView* m_pCopyTable = nullptr;	//��֪�������
			CTCWindows::Control::TableView* m_pReadTable = nullptr;	//�Ķ����
			CTCWindows::Control::TableView* m_pdispatchTable = nullptr;	//����̨���ձ��
			CTCWindows::Control::TableView* m_pLocomotiveTable = nullptr;	//�������ձ��
		};
	}
}
