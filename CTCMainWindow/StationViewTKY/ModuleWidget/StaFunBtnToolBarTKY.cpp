#include "StaFunBtnToolBarTKY.h"
#include "CommonWidget/LeadSealDlg.h"

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

			QObject::connect(m_pButtonGroup, qOverload<QAbstractButton*>(&QButtonGroup::buttonClicked), this, &StaFunBtnToolBar::onButtonClicked);
			//�������
			QObject::connect(m_pCommandClearBtn, &QPushButton::clicked, [&]() { emit OrderClear(); });
			//�����´�
			QObject::connect(m_pCommandIssuedBtn, &QPushButton::clicked, [&]() { emit OrderIssued(); });

			onFunBtnStateReset();
		}

		StaFunBtnToolBarTKY::~StaFunBtnToolBarTKY()
		{

		}

		void StaFunBtnToolBarTKY::InitAuxiliaryMenu(AuxiliaryMenuWnd* pAuxiliary)
		{
			pAuxiliary->AddNewAuxiliaryBtn("�ɵ��޵�", [=]() {
				pAuxiliary->close();
			});
			pAuxiliary->AddNewAuxiliaryBtn("�Ӵ�����λ�޵�", [=]() {
				pAuxiliary->close();
			});
			pAuxiliary->AddNewAuxiliaryBtn("�Ӵ�����λ�޵�", [=]() {
				pAuxiliary->close();
			});
			pAuxiliary->AddNewAuxiliaryBtn("�Ʒ�ͳ��", [=]() {
				pAuxiliary->close();
			});
			pAuxiliary->AddNewAuxiliaryBtn("��ͨ���", [=]() {
				pAuxiliary->close();
			});
			pAuxiliary->AddNewAuxiliaryBtn("������ͨ���30S", [=]() {
				pAuxiliary->close();
			});
			pAuxiliary->AddNewAuxiliaryBtn("�˳��˵�", [=]() {
				pAuxiliary->close();
			});
		}
	}
}