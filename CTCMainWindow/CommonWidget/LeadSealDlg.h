#pragma once

#include <QDialog>
#include "ui_LeadSealDlg.h"

#define PASSWORD	"123"

namespace CTCWindows {
	class LeadSealDlg : public QDialog
	{
		Q_OBJECT

	public:
		LeadSealDlg(QWidget* parent = nullptr);
		~LeadSealDlg();

	public:
		static bool LeadSealPassword();

	private:
		bool eventFilter(QObject* obj, QEvent* event);

	private slots:
		void onLetterButtonClicked();
		void onNumberButtonClicked();
		void onOtherButtonClicked();
		void onEnter();

	private:
		Ui::LeadSealDlgClass ui;
		bool m_bDragging = false; // 拖动标志
		bool m_bCapsLock = false; 
		QPoint m_ptDragPos; // 鼠标按下时的位置
		QVector<QPushButton*> m_vecLetterKeys;

		static bool m_bCorrect; 
	};
}
