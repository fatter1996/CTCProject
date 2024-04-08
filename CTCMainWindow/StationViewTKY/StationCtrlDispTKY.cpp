
#include "StationCtrlDispTKY.h"
#include "Global.h"
namespace CTCWindows {
	StationCtrlDispTKY::StationCtrlDispTKY(QWidget* parent)
		: StationCtrlDisp(parent)
	{
		ui.setupUi(this);
		ui.widget->setFixedSize(m_pStationObject->getStaFixedSize());
	}

	StationCtrlDispTKY::~StationCtrlDispTKY()
	{

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