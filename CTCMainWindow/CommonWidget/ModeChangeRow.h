#pragma once

#include <QWidget>
#include "ui_ModeChangeRow.h"
#include "StationObject/StationObject.h"

#define		MODO_CHANGE				3
#define		MANNER_CHANGE			4
#define		MODO_CHANGE_AGREE		5

namespace CTCWindows {
	class ModeChangeRow : public QWidget
	{
		Q_OBJECT

	public:
		ModeChangeRow(QWidget* parent = nullptr);
		~ModeChangeRow();

		void InitRow(Station::MainStationObject* pStation, int nType);
		void SetRadioBtnChecked(int nIndex, bool bChecked);
		void SetCheckBtnChecked(bool bChecked);
		int GetState(int nType);

	public:
		int getCurrState() { return m_nCurrState; }

	signals:
		void SubRadioBtnChecked();
		void SubCheckBtnChecked();

	private:
		Ui::ModeChangeRow ui;
		int m_nCurrState = 0;
	};
}
