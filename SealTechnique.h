#pragma once

#include <QWidget>
#include "ui_SealTechnique.h"
#include <QMap>


enum class StationSeal : int {
	XZ,		//X总辅助
	XJ,		//X接车辅助
	XF,		//X发车辅助
	XFZ,	//XF总辅助
	XFJ,	//XF接车辅助
	XFF,	//XF发车辅助
	SFZ,	//SF总辅助
	SFJ,	//SF接车辅助
	SFF,	//SF发车辅助
	SZ,		//S总辅助
	SJ,		//S接车辅助
	SF,		//S发车辅助
	XY,		//X引导总锁
	XG,		//X允许改方
	SY,		//S引导总锁
	SG,		//S允许改方
	GZ,		//故障通知
	ZRJ,	//总人解
	QGJ,	//区故解
	ZDW,	//总定位
	ZFW,	//总反位
	DS,		//单锁
	DJ,		//单解
	FS,		//封锁
	JF,		//解封
	PDJS,	//坡道解锁
	FLBL,	//分路不良
	DD,		//点灯
	MD		//灭灯
};

class SealTechnique : public QWidget
{
	Q_OBJECT

public:
	SealTechnique(QWidget *parent = nullptr);
	~SealTechnique();
	void SetTable();
	void SetTableWidget(QMap<QString, int> SealTechnique);
	void SetMapSeal(StationSeal m_nStationSeal, int frequency);
	void SetStationSeal(QString station);
private:
	static StationSeal m_nStationSeal;
	 QMap<QString, int> m_mSealTechnique;
	static QMap<QString, QMap<QString, int>> m_mStationSeal;
	Ui::SealTechniqueClass ui;
};
