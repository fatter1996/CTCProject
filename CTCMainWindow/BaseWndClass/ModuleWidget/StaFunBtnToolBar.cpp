#include "StaFunBtnToolBar.h"
#include "Global.h"
#include "CommonWidget/LeadSealDlg.h"
#include "CommonWidget/ModeChangeWnd.h"
#include "CommonWidget/SealTechnique.h"
#include <QTimer>

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

		void StaFunBtnToolBar::InitConnect()
		{
			connect(m_pCommandClearBtn, &QPushButton::clicked, [&]() { emit OrderClear(true); });
			connect(m_pCommandIssuedBtn, &QPushButton::clicked, [&]() { emit OrderIssued(); });
			connect(m_pAuxiliaryMenuBtn, &QPushButton::clicked, this, &StaFunBtnToolBar::onAuxiliaryMenuBtnClicked);
			connect(m_pMethodConvert, &QPushButton::clicked, this, &StaFunBtnToolBar::onMethodConvertBtnClicked);
			connect(m_pButtonGroup, qOverload<QAbstractButton*>(&QButtonGroup::buttonClicked), this, &StaFunBtnToolBar::onButtonClicked);
			onFunBtnStateReset();
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
			case FunType::GuideBtn:			//引导按钮
			case FunType::GuideClock:		//引导总锁
			case FunType::TotalRelieve:	    //总人解
			case FunType::RegionRelieve:	//区故解
			case FunType::Lighting:			//点灯
			case FunType::UnLighting:		//灭灯
			case FunType::RampUnlock:	    //坡道解锁
			case FunType::PoorRoute:		//确认空闲
			case FunType::IdleConfirm:{	//分路不良
				if (!LeadSealDlg::LeadSealPassword(KeyInputType::LeadSeal)) {
					onFunBtnStateReset();
				}
				else {
					m_nCountdown = 30;
				}
			} break;
			case FunType::CommandClear: 
			case FunType::AuxiliaryMenu: 
			case FunType::StateChange:
			case FunType::MethodConvert: 
			case FunType::CommandIssued:
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
			QMenu* pMenu = new QMenu();
			pMenu->setAttribute(Qt::WA_DeleteOnClose);
			QAction* pAction = new QAction("模式申请");
			connect(pAction, &QAction::triggered, [=]() {
				QList<Station::MainStationObject*> vecMainStation;
				vecMainStation.append(Station::MainStation());
				ModeChangeWnd* pModeChange = new ModeChangeWnd;
				pModeChange->setAttribute(Qt::WA_DeleteOnClose);
				pModeChange->InitModeChange(MODO_CHANGE, vecMainStation);
				pModeChange->exec();
			});
			pMenu->addAction(pAction);
			QAction* pAction2 = new QAction("同意模式申请");
			connect(pAction2, &QAction::triggered, [=]() {
				QList<Station::MainStationObject*> vecMainStation;
				vecMainStation.append(Station::MainStation());
				ModeChangeWnd* pModeChange = new ModeChangeWnd;
				pModeChange->setAttribute(Qt::WA_DeleteOnClose);
				pModeChange->InitModeChange(MODO_CHANGE_AGREE, vecMainStation);
				pModeChange->exec();
			});
			pMenu->addAction(pAction2);
			pMenu->exec(QCursor::pos());
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

		void StaFunBtnToolBar::InitAuxiliaryMenu(AuxiliaryMenuWnd* pAuxiliary)
		{
			pAuxiliary->AddNewAuxiliaryBtn("退出菜单", [=](AuxiliaryMenuWnd* pAuxiliary) {
				pAuxiliary->close();
			});
		}

		void StaFunBtnToolBar::TrackPowerCut(AuxiliaryMenuWnd* pAuxiliary)
		{
			m_SelectFunType = FunType::TrackPowerCut;
			pAuxiliary->close();
		}

		void StaFunBtnToolBar::SwitchDWPowerCut(AuxiliaryMenuWnd* pAuxiliary)
		{
			m_SelectFunType = FunType::SwitchDWPowerCut;
			pAuxiliary->close();
		}

		void StaFunBtnToolBar::SwitchFWPowerCut(AuxiliaryMenuWnd* pAuxiliary)
		{
			m_SelectFunType = FunType::SwitchFWPowerCut;
			pAuxiliary->close();
		}
	}
}