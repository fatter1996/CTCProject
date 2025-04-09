#pragma once

#include <QWidget>
#include "ui_SealTechnique.h"
#include <QMap>


enum class StationSeal : int {
	JFFZ,	//接发车辅助
	YDAN,	//引导按钮
	YDZS,	//引导总锁
	ZRJ,	//总人解
	QGJ,	//区故解
	PDJS,	//坡道解锁
	FLBL,	//分路不良
	MD		//灭灯
};

class SealTechnique : public QWidget
{
	Q_OBJECT

public:
	SealTechnique(QWidget *parent = nullptr);
	~SealTechnique();
	void SetTable();
	void SetTableWidget(QMap<QString, int> SealTechnique, QStringList predefinedTexts);
	static void InsertSealRecord(QString station, QString StationSeal);
private:
	
	static SealTechnique* m_pSealTechnique;
	static StationSeal m_nStationSeal;
	QMap<QString, int> m_mSealTechnique;
	static QMap<QString, QMap<QString, int>> m_mStationSeal;
	Ui::SealTechniqueClass ui;
};
