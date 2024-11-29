#include "StationCtrlDispTH.h"
#include "../ModuleWidget/StaFunBtnToolBarTH.h"

namespace CTCWindows {
	namespace TH {
		StationCtrlDispTH::StationCtrlDispTH(QWidget* parent)
			: StationCtrlDisp(parent)
		{
			ui.setupUi(this);
			ui.scrollArea->setMouseTracking(true);
			ui.widget->installEventFilter(this);
			ui.widget->setMouseTracking(true);
		}

		StationCtrlDispTH::~StationCtrlDispTH()
		{
		}

		void StationCtrlDispTH::CreatStaFunBtnToolBar()
		{
			m_pStaFunBtnToolBar = new StaFunBtnToolBarTH();
		}

		bool StationCtrlDispTH::eventFilter(QObject* obj, QEvent* event)
		{
			return StationCtrlDisp::eventFilter(obj, event);
		}
	}
}
