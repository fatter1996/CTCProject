#pragma once

#include <QDialog>
#include "ui_AuxiliaryMenuWnd.h"
#include <QPushButton>
#include <QStringList>

namespace CTCWindows {
	class AuxiliaryMenuWnd : public QDialog
	{
		Q_OBJECT

	public:
		AuxiliaryMenuWnd(QWidget* parent = nullptr);
		~AuxiliaryMenuWnd();

		QPushButton* AddNewAuxiliaryBtn(QString strName, std::function<void()> CallBack);
		void ShowSealTechnique();

	private:
		Ui::AuxiliaryMenuWndClass ui;
		int m_nButtonNum = 0;
	};
}
