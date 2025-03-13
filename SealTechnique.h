#pragma once

#include <QWidget>
#include "ui_SealTechnique.h"
#include <QMap>


enum class StationSeal : int {
	XZ,		//X�ܸ���
	XJ,		//X�ӳ�����
	XF,		//X��������
	XFZ,	//XF�ܸ���
	XFJ,	//XF�ӳ�����
	XFF,	//XF��������
	SFZ,	//SF�ܸ���
	SFJ,	//SF�ӳ�����
	SFF,	//SF��������
	SZ,		//S�ܸ���
	SJ,		//S�ӳ�����
	SF,		//S��������
	XY,		//X��������
	XG,		//X����ķ�
	SY,		//S��������
	SG,		//S����ķ�
	GZ,		//����֪ͨ
	ZRJ,	//���˽�
	QGJ,	//���ʽ�
	ZDW,	//�ܶ�λ
	ZFW,	//�ܷ�λ
	DS,		//����
	DJ,		//����
	FS,		//����
	JF,		//���
	PDJS,	//�µ�����
	FLBL,	//��·����
	DD,		//���
	MD		//���
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
