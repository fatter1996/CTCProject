#pragma once

#include <QWidget>
#include "ui_ModeChangeRow.h"

#define		MODO_CHANGE				1
#define		MODO_CHANGE_AGREE		2
#define		MANNER_CHANGE			3

namespace CTCWindows {
	class ModeChangeRow : public QWidget
	{
		Q_OBJECT

	public:
		ModeChangeRow(QWidget* parent = nullptr);
		~ModeChangeRow();

		void InitRow(QString strName, int nType);
		void SetRadioBtnChecked(int nIndex, bool bChecked);
		void SetCheckBtnChecked(bool bChecked);
		int GetState(int nType, bool* bChecked = nullptr);

	signals:
		void SubRadioBtnChecked();
		void SubCheckBtnChecked();

	private:
		Ui::ModeChangeRow ui;
	};
}
