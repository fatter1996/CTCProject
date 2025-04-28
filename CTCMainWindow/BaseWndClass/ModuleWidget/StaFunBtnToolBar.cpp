#include "StaFunBtnToolBar.h"
#include "Global.h"
#include "CommonWidget/LeadSealDlg.h"
#include "CommonWidget/ModeChangeWnd.h"

#pragma execution_character_set("utf-8")
namespace CTCWindows {
	namespace BaseWnd {
		int StaFunBtnToolBar::m_nCountdown = 0;
		FunType StaFunBtnToolBar::m_SelectFunType = FunType::RouteBuild;
		OperObjType StaFunBtnToolBar::m_nOperObjType = OperObjType::Defult;

		StaFunBtnToolBar::StaFunBtnToolBar(QWidget* parent)
			: QWidget(parent)
		{
			m_nTimerID_500 = startTimer(500);
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
			case CTCWindows::FunType::GuideBtn:			//引导按钮
			case CTCWindows::FunType::GuideClock:		//引导总锁
			case CTCWindows::FunType::TotalRelieve:	    //总人解
			case CTCWindows::FunType::RegionRelieve:	//区故解
			case CTCWindows::FunType::Lighting:		//点灯
			case CTCWindows::FunType::UnLighting:		//灭灯
			case CTCWindows::FunType::RampUnlock:	    //坡道解锁
			case CTCWindows::FunType::PoorRoute: {		//分路不良
				if (!CTCWindows::LeadSealDlg::LeadSealPassword(CTCWindows::KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					m_nCountdown = 30;
				}
			} break;
			case CTCWindows::FunType::CommandClear: 
			case CTCWindows::FunType::AuxiliaryMenu: 
			case CTCWindows::FunType::StateChange:
			case CTCWindows::FunType::MethodConvert: 
			case CTCWindows::FunType::CommandIssued:
				break;
			default:	
				m_nCountdown = 30;
				break;																								  
			}

			Station::MainStation()->ClearDevice();
		}

		void StaFunBtnToolBar::timerEvent(QTimerEvent* event)
		{
			if (event->timerId() == m_nTimerID_500) {
				if (m_pCommandClearBtn) {
					m_pCommandClearBtn->setEnabled(Station::MainStation()->getSelectDevice().size());
				}
				if (m_pCommandIssuedBtn) {
					m_pCommandIssuedBtn->setEnabled(Station::MainStation()->IsAllowStaOperation());
				}

				if (m_nCountdown > 0) {
					m_nCountdown--;
				}
			}
			return QWidget::timerEvent(event);
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
			m_nCountdown = 0;

			for (QAbstractButton* pButton : m_pButtonGroup->buttons()) {
				pButton->setEnabled(Station::MainStation()->IsAllowStaOperation());
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