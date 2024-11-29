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

		public slots:
			void ShowTableHead(bool bShow = true) override;

		private:
			Ui::StationLogDispKSK ui;
		};
	}
}
