#pragma once
#include <QDialog>
#include "ModeChangeRow.h"
#include "ui_ModeChangeWnd.h"

#define		MODO_CHANGE				1
#define		MODO_CHANGE_AGREE		2
#define		MANNER_CHANGE			3

namespace CTCWindows {
	class ModeChangeWnd : public QDialog
	{
		Q_OBJECT

	public:
		ModeChangeWnd(QWidget* parent = nullptr);
		~ModeChangeWnd();

		void Init(int nType);

	public slots:
		void onSubRadioBtnChecked();
		void onSubCheckBtnChecked();

	private:
		Ui::ModeChange ui;
		QVector<ModeChangeRow*> m_vecModeChangeRow;
	};
}
