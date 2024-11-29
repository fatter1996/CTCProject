#include "StaFunBtnToolBar.h"
#include "Global.h"
#include "CommonWidget/LeadSealDlg.h"
#include "CommonWidget/ModeChangeWnd.h"

namespace CTCWindows {
	namespace BaseWnd {
		FunType StaFunBtnToolBar::m_SelectFunType = FunType::RouteBuild;
		OperObjType StaFunBtnToolBar::m_nOperObjType = OperObjType::Defult;

		StaFunBtnToolBar::StaFunBtnToolBar(QWidget* parent)
			: QWidget(parent)
		{
			
		}

		StaFunBtnToolBar::~StaFunBtnToolBar()
		{

		}

		void StaFunBtnToolBar::onButtonClicked(QAbstractButton* pButton)
		{
			ButtonClicked(m_mapFunBtnType[pButton]);
		}

		void StaFunBtnToolBar::ButtonClicked(FunType eSelectType)
		{
			m_SelectFunType = eSelectType;
			switch (m_SelectFunType)
			{
			case CTCWindows::FunType::GuideBtn:
			case CTCWindows::FunType::TotalCancel:
			case CTCWindows::FunType::GuideClock:
			case CTCWindows::FunType::TotalRelieve:
			case CTCWindows::FunType::RegionRelieve:
			case CTCWindows::FunType::UnLighting:
			case CTCWindows::FunType::RampUnlock:
			case CTCWindows::FunType::PoorRoute: {
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
			} break;
			default:
				break;
			}
			emit OrderClear();
		}

		void StaFunBtnToolBar::onAuxiliaryMenuBtnClicked()
		{
			AuxiliaryMenuWnd* pAuxiliary = new AuxiliaryMenuWnd;
			pAuxiliary->setAttribute(Qt::WA_DeleteOnClose);
			InitAuxiliaryMenu(pAuxiliary);
			pAuxiliary->move(QPoint(QCursor::pos().x(), QCursor::pos().y() - pAuxiliary->height()));
			pAuxiliary->exec();
		}

		void StaFunBtnToolBar::onMethodConvertBtnClicked()
		{
			ModeChangeWnd* pModeChange = new ModeChangeWnd;
			pModeChange->setAttribute(Qt::WA_DeleteOnClose);
			pModeChange->Init(MODO_CHANGE);
			pModeChange->exec();
		}

		void StaFunBtnToolBar::onFunBtnStateReset()
		{
			if (!m_pButtonGroup) {
				return;
			}

			for (QAbstractButton* pButton : m_pButtonGroup->buttons()) {
				pButton->setEnabled(Station::MainStation()->IsAllowStaOperation());
			}
			if (m_pCommandClearBtn) {
				m_pCommandClearBtn->setEnabled(Station::MainStation()->getSelectDevice().size());
			}
			if (m_pCommandIssuedBtn) {
				m_pCommandIssuedBtn->setEnabled(Station::MainStation()->IsAllowStaOperation());
			}

			if (Station::MainStation()->IsAllowStaOperation()) {
				if (m_pRouteBuildBtn) {
					m_pRouteBuildBtn->setChecked(true);
				}
				m_SelectFunType = FunType::RouteBuild;
			}
			else {
				for (QAbstractButton* pButton : m_pButtonGroup->buttons()) {
					pButton->setChecked(false);
				}
			}
		}
	}
}