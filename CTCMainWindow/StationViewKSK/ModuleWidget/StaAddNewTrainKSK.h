#pragma once

#include <QWidget>
#include "ui_StaAddNewTrainKSK.h"
#include <QCloseEvent>
namespace CTCWindows {
	namespace CASCO {
		class StaAddNewTrainKSK : public QWidget
		{
			Q_OBJECT

		public:
			StaAddNewTrainKSK(QWidget* parent = nullptr);
			~StaAddNewTrainKSK();

		public:
			void InitAddView();
			void ConnectEvent();

		public slots:
			void AddNewTrafficLog();

		private:
			Ui::StaAddNewTrainKSKClass ui;
		};
	}
}
