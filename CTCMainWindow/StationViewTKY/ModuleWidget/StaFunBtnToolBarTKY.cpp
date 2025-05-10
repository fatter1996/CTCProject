#include "StaFunBtnToolBarTKY.h"
#include "CommonWidget/LeadSealDlg.h"
#pragma execution_character_set("utf-8")
#include "./StationObject/Device/DeviceBase.h"
#include "./StationObject/StationObject.h"
#include "./Global.h"
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

			m_pButtonGroup = ui.buttonGroup;
			m_pRouteBuildBtn = ui.FunBtn1_RouteBuild;
			m_pCommandClearBtn = ui.FunBtn21_CommandClear;
			m_pCommandIssuedBtn = ui.FunBtn22_CommandIssued;
			m_pAuxiliaryMenuBtn = ui.FunBtn20_AuxiliaryMenu;
			m_pMethodConvert = ui.FunBtn23_MethodConvert;
		}

		StaFunBtnToolBarTKY::~StaFunBtnToolBarTKY()
		{

		}

		void StaFunBtnToolBarTKY::InitAuxiliaryMenu(AuxiliaryMenuWnd* pAuxiliary)
		{
			pAuxiliary->AddNewAuxiliaryBtn("股道无电", std::bind(&StaFunBtnToolBar::TrackPowerCut, this, std::placeholders::_1));
			pAuxiliary->AddNewAuxiliaryBtn("接触网定位无电", std::bind(&StaFunBtnToolBar::SwitchDWPowerCut, this, std::placeholders::_1));
			pAuxiliary->AddNewAuxiliaryBtn("接触网反位无电", std::bind(&StaFunBtnToolBar::SwitchFWPowerCut, this, std::placeholders::_1));
			pAuxiliary->AddNewAuxiliaryBtn("破封统计", std::bind(&AuxiliaryMenuWnd::ShowSealTechnique, pAuxiliary));
			pAuxiliary->AddNewAuxiliaryBtn("接通光带", std::bind(&AuxiliaryMenuWnd::PutThrough15S, pAuxiliary));
			pAuxiliary->AddNewAuxiliaryBtn("继续接通光带30S", std::bind(&AuxiliaryMenuWnd::PutThrough30S, pAuxiliary));
			return StaFunBtnToolBar::InitAuxiliaryMenu(pAuxiliary);
		}
	}
}