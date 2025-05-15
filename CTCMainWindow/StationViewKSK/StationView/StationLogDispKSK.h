#pragma once

#include "BaseWndClass/StationView/StationLogDisp.h"
#include "ui_StationLogDispKSK.h"

namespace CTCWindows {
	namespace CASCO {
		class StationLogDispKSK : public  BaseWnd::StationLogDisp
		{
			Q_OBJECT
		public:
			StationLogDispKSK(QWidget* parent = nullptr);
			~StationLogDispKSK();

		public:
			void InitTrafficLogTableHead() override;
			void AddTrafficLogTable() override;
			void OnTrafficLogTableUpData() override;
			void ShowHeadTableClickMenu(QPoint pos, Station::StaTrafficLog* m_pCurTrafficLog) override;

		private:
			void OnTrackItemClicked(int nRow, int nCol, int nType);
			void OnSignalItemClicked(int nRow, int nCol, int nType);

		public slots:
			void ShowTableHead(bool bShow = true) override;

		private:
			Ui::StationLogDispKSK ui;
			QMap<int, std::function<void(int, int)>> m_mapColClickFunction;
		};
	}
}
