#pragma once
#include <QDialog>
#include "ModeChangeRow.h"
#include "ui_ModeChangeWnd.h"

namespace CTCWindows {
	class ModeChangeWnd : public QDialog
	{
		Q_OBJECT

	public:
		ModeChangeWnd(QWidget* parent = nullptr);
		~ModeChangeWnd();

		void InitModeChange(int nType, const QList<Station::MainStationObject*>& listStation);

	public slots:
		void onSubRadioBtnChecked();
		void onSubCheckBtnChecked();

	private:
		Ui::ModeChange ui;
		QVector<ModeChangeRow*> m_vecModeChangeRow;
		QList<Station::MainStationObject*> m_listStation;
	};
}
