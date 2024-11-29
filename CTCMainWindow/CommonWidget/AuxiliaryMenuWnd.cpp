#include "AuxiliaryMenuWnd.h"

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
}