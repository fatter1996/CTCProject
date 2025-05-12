#pragma once

#include <QWidget>
#include "ui_StaAddNewTrainKSK.h"
#include <QCloseEvent>
#include "Global.h"
namespace CTCWindows {
	namespace CASCO {
		class StaAddNewTrainKSK : public QWidget
		{
			Q_OBJECT

		public:
			StaAddNewTrainKSK(QWidget* parent = nullptr);
			~StaAddNewTrainKSK();
			void InitAddView(Station::StaTrafficLog* m_pCurTrafficLog);
			void InitAddView();
			void ConnectEvent();
			void AddNewTrafficLog();
			void GetNowTime();
			void ClearWidget();
			void closeEvent(QCloseEvent* event);
		private:
			bool begin = true;
			bool end = true;
			QMap<int, QString> m_mapPassengeTrain;
			QMap<int, QString> m_mapFreighTrain;
			QMap<int, QString> m_mapTrainType;
			Ui::StaAddNewTrainKSKClass ui;
		};
	}
}