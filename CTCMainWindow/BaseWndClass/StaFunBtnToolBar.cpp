#include "StaFunBtnToolBar.h"
#include "Global.h"

namespace CTCWindows {

	FunType StaFunBtnToolBar::m_SelectFunType = FunType::RouteBuild;
	OperObjType StaFunBtnToolBar::m_nOperObjType = OperObjType::Defult;

	StaFunBtnToolBar::StaFunBtnToolBar(QWidget* parentr)
	{

	}

	StaFunBtnToolBar::~StaFunBtnToolBar()
	{
	
	}

	void StaFunBtnToolBar::onButtonClicked(QAbstractButton* pButton)
	{
		m_SelectFunType = m_mapFunBtnType[pButton];
		emit OrderClear();
	}
	
	FunType getCurrFunType() 
	{ 
		return StaFunBtnToolBar::m_SelectFunType; 
	}

	void setOperObjType(OperObjType eType)
	{
		StaFunBtnToolBar::m_nOperObjType = eType;
	}

	OperObjType getOperObjType()
	{
		return StaFunBtnToolBar::m_nOperObjType;
	}
}