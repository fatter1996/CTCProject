#pragma once
#include <QWidget>
#include <QMap>
#include "ui_SealTechnique.h"
#include "StationObject/StationObject.h"

namespace CTCWindows {
	class SealTechnique : public QWidget
	{
		Q_OBJECT
	public:
		SealTechnique(QWidget* parent = nullptr);
		~SealTechnique();

	public:
		void InitTableWidget();
		void UpdaTableWidget(Station::StationObject* pStation);
		void InitListView();

	public:
		static void InitSealRecord(Station::StationObject* pStation);
		static void InsertSealRecord(QString strStation, QString strStationSeal);

	private:
		Ui::SealTechniqueClass ui;

	private:
		static QMap<QString, QMap<QString, int>> m_mapStationSeal;
	};
}
