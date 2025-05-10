#include "AuxiliaryMenuWnd.h"
#include "SealTechnique.h"
#include "Global.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
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

	QPushButton* AuxiliaryMenuWnd::AddNewAuxiliaryBtn(QString strName, std::function<void(AuxiliaryMenuWnd*)> CallBack)
	{
		QPushButton* pBtn = new QPushButton(this);
		pBtn->setFixedHeight(40);
		pBtn->setText(strName);
		QFont font = QFont("Î¢ÈíÑÅºÚ", 14);
		font.setBold(true);
		pBtn->setFont(font);
		this->layout()->addWidget(pBtn);
		connect(pBtn, &QPushButton::clicked, [=]() { CallBack(this); });
		m_nButtonNum++;
		setFixedHeight(m_nButtonNum * 40);
		return pBtn;
	}

	void AuxiliaryMenuWnd::ShowSealTechnique()
	{
		SealTechnique* m_pSealTechnique = new SealTechnique;
		m_pSealTechnique->setAttribute(Qt::WA_DeleteOnClose);
		m_pSealTechnique->UpdaTableWidget(Station::MainStation());
		this->close();
		m_pSealTechnique->show();

	}

	void AuxiliaryMenuWnd::PutThrough15S()
	{
		Station::MainStation()->PutThrough(15);
		this->close();
	}

	void AuxiliaryMenuWnd::PutThrough30S()
	{
		Station::MainStation()->PutThrough(30);
		this->close();
	}
}