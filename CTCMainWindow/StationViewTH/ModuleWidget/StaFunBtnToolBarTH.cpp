#include "StaFunBtnToolBarTH.h"

namespace CTCWindows {
	namespace TH {
		StaFunBtnToolBarTH::StaFunBtnToolBarTH(QWidget* parent)
			: StaFunBtnToolBar(parent)
		{
			ui.setupUi(this);
			m_mapFunBtnType = {
				{ ui.FunBtn1_RouteBuild, FunType::RouteBuild },
				{ ui.FunBtn2_STrackBlock, FunType::STrackBlock },
				{ ui.FunBtn3_XTrackBlock, FunType::XTrackBlock },
				{ ui.FunBtn4_RampUnlock, FunType::RampUnlock },
				{ ui.FunBtn5_CommandClear, FunType::CommandClear }
			};
			m_pButtonGroup = ui.buttonGroup;
			m_pRouteBuildBtn = ui.FunBtn1_RouteBuild;
			m_pCommandClearBtn = ui.FunBtn5_CommandClear;

			//ÃüÁîÇå³ý
			QObject::connect(m_pCommandClearBtn, &QPushButton::clicked, [&]() { emit OrderClear(1); });
		}

		StaFunBtnToolBarTH::~StaFunBtnToolBarTH()
		{
		}
	}
}
