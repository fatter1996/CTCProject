#pragma once

#include <QWidget>
#include "ui_SealTechnique.h"
#include <QMap>


enum class StationSeal : int {
	JFFZ,	//�ӷ�������
	YDAN,	//������ť
	YDZS,	//��������
	ZRJ,	//���˽�
	QGJ,	//���ʽ�
	PDJS,	//�µ�����
	FLBL,	//��·����
	MD		//���
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
