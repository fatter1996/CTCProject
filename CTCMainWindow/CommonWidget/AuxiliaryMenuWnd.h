#pragma once

#include <QDialog>
#include "ui_AuxiliaryMenuWnd.h"
#include <QPushButton>

namespace CTCWindows {
	class AuxiliaryMenuWnd : public QDialog
	{
		Q_OBJECT

	public:
		AuxiliaryMenuWnd(QWidget* parent = nullptr);
		~AuxiliaryMenuWnd();

		QPushButton* AddNewAuxiliaryBtn(QString strName, std::function<void()> CallBack);

	private:
		Ui::AuxiliaryMenuWndClass ui;
		int m_nButtonNum = 0;
	};
}
