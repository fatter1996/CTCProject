#include "StaFunBtnToolBar.h"
#include "Global.h"

namespace CTCWindows {

	FunType StaFunBtnToolBar::m_SelectFunType = FunType::RouteBuild;
	QMap<FunType, int> StaFunBtnToolBar::m_mapStaOrderCode;

	StaFunBtnToolBar::StaFunBtnToolBar(QWidget* parentr)
	{
		m_mapStaOrderCode.insert(CTCWindows::FunType::RouteBuild, 0x01);
	}

	StaFunBtnToolBar::~StaFunBtnToolBar()
	{
	
	}

	void StaFunBtnToolBar::onButtonClicked(QAbstractButton* pButton)
	{
		m_SelectFunType = m_mapFunBtnType[pButton];
		emit OrderClear();
	}

	void StaFunBtnToolBar::onOrderClear(bool checked)
	{
		FunBtnStateReset();
		emit OrderClear();
	}

	void StaFunBtnToolBar::onOrderIssued(bool checked)
	{
		FunBtnStateReset();
		emit OrderIssued();
	}
	
	FunType getCurrFunType() 
	{ 
		return StaFunBtnToolBar::m_SelectFunType; 
	}

	int getFunBtnOrderCode()
	{
		return StaFunBtnToolBar::m_mapStaOrderCode[StaFunBtnToolBar::m_SelectFunType];
	}
}