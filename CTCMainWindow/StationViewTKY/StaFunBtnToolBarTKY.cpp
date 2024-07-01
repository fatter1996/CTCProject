#include "StaFunBtnToolBarTKY.h"
#include <QButtonGroup>
#include <QAbstractButton>
#include "Global.h"

#include "CTCMainWindow/CommonWidget/LeadSealDlg.h"

namespace CTCWindows {
	namespace CARS {
		StaFunBtnToolBarTKY::StaFunBtnToolBarTKY(QWidget* parent)
			: StaFunBtnToolBar(parent)
		{
			ui.setupUi(this);
			m_mapFunBtnType = {
				{ ui.FunBtn1_RouteBuild, FunType::RouteBuild },
				{ ui.FunBtn2_TotalCancel, FunType::TotalCancel },
				{ ui.FunBtn3_SignalReopen, FunType::SignalReopen },
				{ ui.FunBtn4_GuideBtn, FunType::GuideBtn },
				{ ui.FunBtn5_GuideClock, FunType::GuideClock },
				{ ui.FunBtn6_TotalRelieve, FunType::TotalRelieve },
				{ ui.FunBtn7_RegionRelieve, FunType::RegionRelieve },
				{ ui.FunBtn8_TotalPosition, FunType::TotalPosition },
				{ ui.FunBtn9_TotalReverse, FunType::TotalReverse },
				{ ui.FunBtn10_SingleLock, FunType::SingleLock },
				{ ui.FunBtn11_SingleUnlock, FunType::SingleUnlock },
				{ ui.FunBtn12_Blockade, FunType::Blockade },
				{ ui.FunBtn13_UnBlockade, FunType::UnBlockade },
				{ ui.FunBtn14_FunBtn, FunType::FunBtn },
				{ ui.FunBtn15_RampUnlock, FunType::RampUnlock },
				{ ui.FunBtn16_PoorRoute, FunType::PoorRoute },
				{ ui.FunBtn17_IdleConfirm, FunType::IdleConfirm },
				{ ui.FunBtn18_Lighting, FunType::Lighting },
				{ ui.FunBtn19_UnLighting, FunType::UnLighting },
			};

			onFunBtnStateReset();

			QObject::connect(ui.buttonGroup, qOverload<QAbstractButton*>(&QButtonGroup::buttonClicked), this, &StaFunBtnToolBarTKY::onButtonClicked);
			//ÃüÁîÇå³ý
			QObject::connect(ui.FunBtn21_CommandClear, &QPushButton::clicked, [&]() { emit OrderClear(); });
			//ÃüÁîÏÂ´ï
			QObject::connect(ui.FunBtn22_CommandIssued, &QPushButton::clicked, [&]() { emit OrderIssued(); });

			QObject::connect(ui.FunBtn16_PoorRoute, &QPushButton::clicked, [&]() { LeadSealDlg::LeadSealPassword(); });
		}

		StaFunBtnToolBarTKY::~StaFunBtnToolBarTKY()
		{

		}

		void StaFunBtnToolBarTKY::onFunBtnStateReset()
		{
			for (QAbstractButton* pButton : ui.buttonGroup->buttons()) {
				pButton->setEnabled(Station::MainStation()->IsAllowStaOperation());
			}
			ui.FunBtn17_IdleConfirm->setEnabled(true);
			ui.FunBtn21_CommandClear->setEnabled(Station::MainStation()->IsAllowStaOperation());
			ui.FunBtn22_CommandIssued->setEnabled(Station::MainStation()->IsAllowStaOperation());

			if (Station::MainStation()->IsAllowStaOperation()) {
				ui.FunBtn1_RouteBuild->setChecked(true);
				m_SelectFunType = FunType::RouteBuild;
			}
			else {
				for (QAbstractButton* pButton : ui.buttonGroup->buttons()) {
					pButton->setChecked(false);
				}
			}
		}
	}
}