
#include "StationCtrlDispTKY.h"
#include "../ModuleWidget/StaFunBtnToolBarTKY.h"

namespace CTCWindows {
	namespace CARS {
		StationCtrlDispTKY::StationCtrlDispTKY(QWidget* parent)
			: StationCtrlDisp(parent)
		{
			ui.setupUi(this);
			ui.scrollArea->setMouseTracking(true);
		}

		StationCtrlDispTKY::~StationCtrlDispTKY()
		{
		}

		void StationCtrlDispTKY::CreatStaFunBtnToolBar()
		{
			m_pStaFunBtnToolBar = new StaFunBtnToolBarTKY();
			this->layout()->addWidget(m_pStaFunBtnToolBar);
		}
	}
}