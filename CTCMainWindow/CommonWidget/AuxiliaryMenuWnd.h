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

	public:
		QPushButton* AddNewAuxiliaryBtn(QString strName, std::function<void(AuxiliaryMenuWnd*)> CallBack);
		void ShowSealTechnique();
		void PutThrough15S();
		void PutThrough30S();

	private:
		Ui::AuxiliaryMenuWndClass ui;
		int m_nButtonNum = 0;
	};
}
