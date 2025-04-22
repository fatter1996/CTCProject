#pragma once

#include <QDialog>
#include "ui_LeadSealDlg.h"
#include <QLineEdit>
#include <QCheckBox>
#define PASSWORD	"123"

namespace CTCWindows {
	enum class KeyInputType : int {
		LeadSeal = 1,	//铅封窗口


		AddTrain = 10,			//添加车次
		ConfirmTrain = 11,		//确认车次号
		AmendTrain = 12,		//修正车次号
		ChangeTrain = 13,		//变更车次号
		ChangeTrainAttr = 14,	//修改车次属性
		InputTrain = 15,			//输入车次号
		InputShuntingTime = 16			//输入调车时长
	};
	class LeadSealDlg : public QDialog
	{
		Q_OBJECT

	public:
		LeadSealDlg(QWidget* parent = nullptr);
		~LeadSealDlg();

		void Init(KeyInputType type, void* pAttrObject);
		void InitAddTrain(void* pAttrObject);
		void InitChangeTrainNum(QString strTrainNum);
		void InitChangeTrainAttr(void* pAttrObject);

	public:
		static bool LeadSealPassword(KeyInputType type, void* pAttrObject = nullptr);

	private:
		bool eventFilter(QObject* obj, QEvent* event);

	private slots:
		void onLetterButtonClicked();
		void onNumberButtonClicked();
		void onOtherButtonClicked();

	private:
		Ui::LeadSealDlg ui;
		QCheckBox* pCheck = nullptr;
		bool m_bDragging = false; // 拖动标志
		bool m_bCapsLock = false; 
		QPoint m_ptDragPos; // 鼠标按下时的位置
		QVector<QPushButton*> m_vecLetterKeys;
		QLineEdit* m_pCurrLineEdit = nullptr;
		static bool m_bResult;
	};
}
