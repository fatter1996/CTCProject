#include "AuxiliaryMenuWnd.h"

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
		SealTechnique* se = new SealTechnique;
		se->SetMapSeal(StationSeal::ZRJ, 88);
		se->SetStationSeal("³àË®");
		m_pSealTechnique = new SealTechnique;
		m_pSealTechnique->SetMapSeal(StationSeal::GZ, 6);
		m_pSealTechnique->SetMapSeal(StationSeal::SFZ, 6);
		m_pSealTechnique->SetMapSeal(StationSeal::XZ, 6);
		m_pSealTechnique->SetMapSeal(StationSeal::SF, 6);
		m_pSealTechnique->SetMapSeal(StationSeal::XF, 6);
		m_pSealTechnique->SetMapSeal(StationSeal::SFJ, 6);

		m_pSealTechnique->SetStationSeal("±ê×¼Õ¾");
		m_pSealTechnique->SetTable();
		m_pSealTechnique->show();

	}
}