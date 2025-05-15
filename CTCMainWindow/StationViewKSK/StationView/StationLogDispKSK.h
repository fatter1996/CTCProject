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
			void ClickMenu(QPoint pos, Station::StaTrafficLog* m_pCurTrafficLog) override;
			void SetPlanType(bool Type, Station::StaTrafficLog* m_pCurTrafficLog);
		public slots:
			void ShowTableHead(bool bShow = true) override;

		private:
			bool Check = false;
			Ui::StationLogDispKSK ui;
		};
	}
}
