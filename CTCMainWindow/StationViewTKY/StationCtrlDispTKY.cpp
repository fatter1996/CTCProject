
#include "StationCtrlDispTKY.h"
#include "Global.h"
namespace CTCWindows {
	namespace CARS {
		StationCtrlDispTKY::StationCtrlDispTKY(QWidget* parent)
			: StationCtrlDisp(parent)
		{
			ui.setupUi(this);
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