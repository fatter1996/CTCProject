#pragma once

#include <QWidget>
#include "ui_ModifyRelevantStationsKSK.h"
#include "Global.h"
class ModifyRelevantStationsKSK : public QWidget
{
	Q_OBJECT

public:
	ModifyRelevantStationsKSK(Station::StaTrafficLog* m_pCurTrafficLog,QWidget *parent = nullptr);
	~ModifyRelevantStationsKSK();

private:
	Ui::ModifyRelevantStationsKSKClass ui;
};
