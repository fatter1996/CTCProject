#pragma once

#include <QDialog>
#include "ui_LeadSealDlg.h"
#include <QLineEdit>

#define PASSWORD	"123"

namespace CTCWindows {
	enum class KeyInputType {
		LeadSeal = 1,	//Ǧ�ⴰ��

		AddTrain = 10,			//��ӳ���
		ConfirmTrain = 11,		//ȷ�ϳ��κ�
		AmendTrain = 12,		//�������κ�
		ChangeTrain = 13,		//������κ�
		ChangeTrainAttr = 14,	//�޸ĳ�������

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
		bool m_bDragging = false; // �϶���־
		bool m_bCapsLock = false; 
		QPoint m_ptDragPos; // ��갴��ʱ��λ��
		QVector<QPushButton*> m_vecLetterKeys;
		QLineEdit* m_pCurrLineEdit = nullptr;
		static bool m_bResult;
	};
}
