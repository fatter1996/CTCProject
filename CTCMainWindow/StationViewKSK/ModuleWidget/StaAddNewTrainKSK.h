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
	StaAddNewTrainKSK(QWidget *parent = nullptr);
	~StaAddNewTrainKSK();
	void InitAddView();
	void ConnectEvent();
	void closeEvent(QCloseEvent* event);
	void GetNowTime();
	void ClearWidget();
	void SetTrainValue(Station::StaTrafficLog* m_pCurTrafficLog);
private:
	bool begin = true;
	bool end = true;
	QMap<int, QString> m_mapPassengeTrain;
	QMap<int, QString> m_mapFreighTrain;
	QMap<int, QString> m_mapTrainType;
	Ui::StaAddNewTrainKSKClass ui;
};
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
