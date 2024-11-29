#include "StationCtrlDispKSK.h"
#include "../ModuleWidget/StaFunBtnToolBarKSK.h"

namespace CTCWindows {
	namespace CASCO {
		StationCtrlDispKSK::StationCtrlDispKSK(QWidget* parent)
			: StationCtrlDisp(parent)
		{
			ui.setupUi(this);
			ui.scrollArea->setMouseTracking(true);
			ui.widget->installEventFilter(this);
			ui.widget->setMouseTracking(true);
		}

		StationCtrlDispKSK::~StationCtrlDispKSK()
		{

		}

		void StationCtrlDispKSK::CreatStaFunBtnToolBar()
		{
			m_pStaFunBtnToolBar = new StaFunBtnToolBarKSK();
			this->layout()->addWidget(m_pStaFunBtnToolBar);
		}

		bool StationCtrlDispKSK::eventFilter(QObject* obj, QEvent* event)
		{
			return StationCtrlDisp::eventFilter(obj, event);
		}
	}
}
