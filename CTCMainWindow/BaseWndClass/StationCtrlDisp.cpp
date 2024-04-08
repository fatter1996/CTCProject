#include "StationCtrlDisp.h"

namespace CTCWindows {

	StationCtrlDisp::StationCtrlDisp(QWidget* parent)
	{
		m_nTimerId_500 = startTimer(500);
	}

	StationCtrlDisp::~StationCtrlDisp()
	{
		killTimer(m_nTimerId_500);
	}

	void StationCtrlDisp::timerEvent(QTimerEvent* event)
	{
		if (event->timerId() == m_nTimerId_500) {
			GetStationPaintView()->update();
		}
	}
}