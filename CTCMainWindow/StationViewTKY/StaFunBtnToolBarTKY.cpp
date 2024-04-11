#include "StaFunBtnToolBarTKY.h"
#include <QDebug>
#include <QButtonGroup>
#include <QAbstractButton>
#include "Global.h"

namespace CTCWindows {
	StaFunBtnToolBarTKY::StaFunBtnToolBarTKY(QWidget* parent)
		: StaFunBtnToolBar(parent)
	{
		ui.setupUi(this);
		m_mapFunBtnType.insert(ui.FunBtn1_RouteBuild, FunType::RouteBuild);
		m_mapFunBtnType.insert(ui.FunBtn2_TotalCancel, FunType::TotalCancel);
		m_mapFunBtnType.insert(ui.FunBtn3_SignalReopen, FunType::SignalReopen);
		m_mapFunBtnType.insert(ui.FunBtn4_GuideBtn, FunType::GuideBtn);
		m_mapFunBtnType.insert(ui.FunBtn5_GuideClock, FunType::GuideClock);
		m_mapFunBtnType.insert(ui.FunBtn6_TotalRelieve, FunType::TotalRelieve);
		m_mapFunBtnType.insert(ui.FunBtn7_RegionRelieve, FunType::RegionRelieve);
		m_mapFunBtnType.insert(ui.FunBtn8_TotalPosition, FunType::TotalPosition);
		m_mapFunBtnType.insert(ui.FunBtn9_TotalReverse, FunType::TotalReverse);
		m_mapFunBtnType.insert(ui.FunBtn10_SingleLock, FunType::SingleLock);
		m_mapFunBtnType.insert(ui.FunBtn11_SingleUnlock, FunType::SingleUnlock);
		m_mapFunBtnType.insert(ui.FunBtn12_Blockade, FunType::Blockade);
		m_mapFunBtnType.insert(ui.FunBtn13_UnBlockade, FunType::UnBlockade);
		m_mapFunBtnType.insert(ui.FunBtn14_FunBtn, FunType::FunBtn);
		m_mapFunBtnType.insert(ui.FunBtn15_RampUnlock, FunType::RampUnlock);
		m_mapFunBtnType.insert(ui.FunBtn16_PoorRoute, FunType::PoorRoute);
		m_mapFunBtnType.insert(ui.FunBtn17_IdleConfirm, FunType::IdleConfirm);
		m_mapFunBtnType.insert(ui.FunBtn18_Lighting, FunType::Lighting);
		m_mapFunBtnType.insert(ui.FunBtn19_UnLighting, FunType::UnLighting);

		FunBtnStateReset();

		connect(ui.buttonGroup, qOverload<QAbstractButton*>(&QButtonGroup::buttonClicked), [&](QAbstractButton* pButton) {
			if (ui.buttonGroup->checkedButton() == pButton) {
				m_SelectFunType = m_mapFunBtnType[pButton];
				qDebug() << pButton->objectName();
			}
		});
	}

	StaFunBtnToolBarTKY::~StaFunBtnToolBarTKY()
	{
	
	}

	void StaFunBtnToolBarTKY::FunBtnStateReset()
	{
		for (QAbstractButton* pButton : ui.buttonGroup->buttons()) {
			pButton->setEnabled(Station::StationObject::IsAllowStaOperation());
		}
		ui.FunBtn17_IdleConfirm->setEnabled(true);
		ui.FunBtn21_CommandClear->setEnabled(Station::StationObject::IsAllowStaOperation());
		ui.FunBtn22_CommandIssued->setEnabled(Station::StationObject::IsAllowStaOperation());

		if (Station::StationObject::IsAllowStaOperation()) {
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
