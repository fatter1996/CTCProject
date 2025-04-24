#include "AuxiliaryMenuWnd.h"

#pragma execution_character_set("utf-8")

namespace CTCWindows {
	QStringList AuxiliaryMenuWnd::m_stringList;
	AuxiliaryMenuWnd::AuxiliaryMenuWnd(QWidget* parent)
		: QDialog(parent)
	{
		ui.setupUi(this);
		setAttribute(Qt::WA_TranslucentBackground);
		setWindowFlags(Qt::FramelessWindowHint);
	}

	AuxiliaryMenuWnd::~AuxiliaryMenuWnd() 
	{

	}

	QPushButton* AuxiliaryMenuWnd::AddNewAuxiliaryBtn(QString strName, std::function<void()> CallBack)
	{
		QPushButton* pBtn = new QPushButton(this);
		pBtn->setFixedHeight(40);
		pBtn->setText(strName);
		QFont font = QFont("Î¢ÈíÑÅºÚ", 14);
		font.setBold(true);
		pBtn->setFont(font);
		this->layout()->addWidget(pBtn);
		connect(pBtn, &QPushButton::clicked, [=]() { CallBack(); });
		m_nButtonNum++;
		setFixedHeight(m_nButtonNum * 40);
		return pBtn;
	}
	void AuxiliaryMenuWnd::ShowSealTechnique()
	{
	
		m_pSealTechnique = new SealTechnique;
		m_pSealTechnique->SetTable();
		m_pSealTechnique->show();

	}

}