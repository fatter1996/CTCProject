#pragma once
#include <QWidget>
#include "CustomControl/TableView.h"
#include "StationObject/GlobalStruct.h"

namespace CTCWindows {
	namespace BaseWnd {
		class StaRoutePlan : public QWidget
		{
			Q_OBJECT
		public:
			explicit StaRoutePlan(QWidget* parent = nullptr);
			~StaRoutePlan();

		public:
			//列车进路序列
			void InitTrainRouteTable();
			virtual QVector<Control::TableViewHeadInfo> GetTrainRouteTableHeadInfo() = 0;
			virtual QStringList GetTrainRouteTableDataByRoute(QString strTrainNum, Station::StaTrainRoute* pRoute) = 0;
			virtual void UpDataRouteLimits() = 0;
			//调车进路序列
			void InitShuntRouteTable();
			virtual QVector<Control::TableViewHeadInfo> GetShuntRouteTableHeadInfo() {
				return QVector<Control::TableViewHeadInfo>();
			}
			//勾计划
			void InitHookPlanTable();
			virtual QVector<Control::TableViewHeadInfo> GetHookPlanTableHeadInfo() {
				return QVector<Control::TableViewHeadInfo>();
			}

			QSize sizeHint() const override { return QSize(width(), minimumHeight()); }

		public:
			void timerEvent(QTimerEvent* event);

		private:
			void RouteOrderSort(QVector<Station::StaTrainRoute*>::iterator low, QVector<Station::StaTrainRoute*>::iterator high);
			bool RouteOrderCompare(Station::StaTrainRoute* pRoute1, Station::StaTrainRoute* pRoute2);
			void TrainRouteColorUpData();
			void InitTrackChangeMenu(const QModelIndex& index, Station::StaTrainRoute* pTrainRoute);
			void InitTriggerMenu(const QModelIndex& index, Station::StaTrainRoute* pTrainRoute);
			void InitRouteChangeMenu(const QModelIndex& index, Station::StaTrainRoute* pTrainRoute);

		public slots:
			void OnTrainRouteUpData();
			void OnFontSizeChange(int nIndex);
			void TrainRouteTableLeftMenu(const QModelIndex& index);
			void TrainRouteTableRightMenu(const QPoint& pt);
			void OnTrackChange(Station::StaTrainRoute* pTrainRoute, QString strTrack);
			void OnTriggerTypeChange(Station::StaTrainRoute* pTrainRoute, bool bAutoTouch);

		protected:
			Control::TableView* m_pTrainRouteTable = nullptr;	//列车进路表格
			Control::TableView* m_pShuntRouteTable = nullptr;	//调车进路表格
			Control::TableView* m_pHookPlanTable = nullptr;		//勾计划表格

			int m_nTrackRow = -1;
			int m_nTriggerRow = -1;
			int m_nRouteRow = -1;
			int m_nTimerId_15 = -1;
		};
	}
}

