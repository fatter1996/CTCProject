
#include "StationCtrlDispTKY.h"
#include "ModuleWidget/StaFunBtnToolBar.h"
#include "StationViewTKY/ModuleWidget/StaFunBtnToolBarTKY.h"
#include <QDateTime>
#include "Global.h"

#pragma execution_character_set("utf-8")
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