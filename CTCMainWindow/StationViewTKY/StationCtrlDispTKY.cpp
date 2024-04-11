
#include "StationCtrlDispTKY.h"
#include "Global.h"
namespace CTCWindows {
	StationCtrlDispTKY::StationCtrlDispTKY(QWidget* parent)
		: StationCtrlDisp(parent)
	{
		ui.setupUi(this);
		ui.widget->setFixedSize(m_staMainStation->getStaFixedSize());
	}

	StationCtrlDispTKY::~StationCtrlDispTKY()
	{

	}

	void StationCtrlDispTKY::CreatStaFunBtnToolBar()
	{
		m_pStaFunBtnToolBar = new StaFunBtnToolBarTKY();
		this->layout()->addWidget(m_pStaFunBtnToolBar);
	}

	QWidget* StationCtrlDispTKY::GetStationPaintView()
	{
		return ui.widget;
	}

	void StationCtrlDispTKY::timerEvent(QTimerEvent* event)
	{
		return StationCtrlDisp::timerEvent(event);
	}
}