#pragma once

#include <QWidget>
#include "ui_StaAddNewTrainKSK.h"
#include <QCloseEvent>
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
private:
	bool begin = true;
	bool end = true;
	QMap<int, QString> m_mapPassengeTrain;
	QMap<int, QString> m_mapFreighTrain;
	QMap<int, QString> m_mapTrainType;
	Ui::StaAddNewTrainKSKClass ui;
};
