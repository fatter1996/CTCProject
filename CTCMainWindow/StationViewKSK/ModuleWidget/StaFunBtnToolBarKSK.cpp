#include "StaFunBtnToolBarKSK.h"
#include <QButtonGroup>
#include <QAbstractButton>
#include "Global.h"
#include "CommonWidget/LeadSealDlg.h"
#pragma execution_character_set("utf-8")

namespace CTCWindows {
	namespace CASCO {
		StaFunBtnToolBarKSK::StaFunBtnToolBarKSK(QWidget* parent)
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
				{ ui.FunBtn15_PoorRoute, FunType::PoorRoute },
				{ ui.FunBtn16_IdleConfirm, FunType::IdleConfirm },
				{ ui.FunBtn22_Lighting, FunType::Lighting },
				{ ui.FunBtn23_UnLighting, FunType::UnLighting },
			};

			m_pButtonGroup = ui.buttonGroup;
			m_pRouteBuildBtn = ui.FunBtn1_RouteBuild;
			m_pCommandClearBtn = ui.FunBtn18_CommandClear;
			m_pCommandIssuedBtn = ui.FunBtn19_CommandIssued;
			m_pAuxiliaryMenuBtn = ui.FunBtn17_AuxiliaryMenu;
			m_pMethodConvert = ui.FunBtn21_MethodConvert;
		}

		StaFunBtnToolBarKSK::~StaFunBtnToolBarKSK()
		{
		}

		void StaFunBtnToolBarKSK::InitAuxiliaryMenu(AuxiliaryMenuWnd* pAuxiliary)
		{
			pAuxiliary->AddNewAuxiliaryBtn("股道无电", std::bind(&StaFunBtnToolBar::TrackPowerCut, this, std::placeholders::_1));
			pAuxiliary->AddNewAuxiliaryBtn("接触网定位无电", std::bind(&StaFunBtnToolBar::SwitchDWPowerCut, this, std::placeholders::_1));
			pAuxiliary->AddNewAuxiliaryBtn("接触网反位无电", std::bind(&StaFunBtnToolBar::SwitchFWPowerCut, this, std::placeholders::_1));
			//pAuxiliary->AddNewAuxiliaryBtn("破封统计", std::bind(&StaFunBtnToolBar::ShowSealTechnique, this, std::placeholders::_1));
			//pAuxiliary->AddNewAuxiliaryBtn("接通光带", std::bind(&StaFunBtnToolBar::PutThrough15S, this, std::placeholders::_1));
			//pAuxiliary->AddNewAuxiliaryBtn("继续接通光带30S", std::bind(&StaFunBtnToolBar::PutThrough30S, this, std::placeholders::_1));
			return StaFunBtnToolBar::InitAuxiliaryMenu(pAuxiliary);
		}
	}
}
