#include "StaFunBtnToolBar.h"
#include "Global.h"

namespace CTCWindows {

	FunType StaFunBtnToolBar::m_SelectFunType = FunType::RouteBuild;
	QMap<FunType, int> StaFunBtnToolBar::m_mapStaOrderCode;
	int StaFunBtnToolBar::m_nRouteType = 1;

	StaFunBtnToolBar::StaFunBtnToolBar(QWidget* parentr)
	{
		m_mapStaOrderCode.insert(CTCWindows::FunType::TotalCancel, 0x04);
		m_mapStaOrderCode.insert(CTCWindows::FunType::TotalPosition, 0x05);
		m_mapStaOrderCode.insert(CTCWindows::FunType::TotalReverse, 0x06);
		m_mapStaOrderCode.insert(CTCWindows::FunType::SingleLock, 0x07);
		m_mapStaOrderCode.insert(CTCWindows::FunType::SingleUnlock, 0x08);
		m_mapStaOrderCode.insert(CTCWindows::FunType::Blockade, 0x09);
		m_mapStaOrderCode.insert(CTCWindows::FunType::UnBlockade, 0x0a);
		m_mapStaOrderCode.insert(CTCWindows::FunType::TotalRelieve, 0x0b);
		m_mapStaOrderCode.insert(CTCWindows::FunType::RegionRelieve, 0x0c);
		m_mapStaOrderCode.insert(CTCWindows::FunType::Lighting, 0x0d);
		m_mapStaOrderCode.insert(CTCWindows::FunType::UnLighting, 0x0e);
		m_mapStaOrderCode.insert(CTCWindows::FunType::GuideBtn, 0x0f);
		m_mapStaOrderCode.insert(CTCWindows::FunType::GuideClock, 0x10);
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
		if (getCurrFunType() == FunType::RouteBuild) {
			return StaFunBtnToolBar::m_nRouteType; 
		}
		else {
			return StaFunBtnToolBar::m_mapStaOrderCode[StaFunBtnToolBar::m_SelectFunType];
		}
	}

	void setRouteType(int nType)
	{
		StaFunBtnToolBar::m_nRouteType = nType;
	}
}